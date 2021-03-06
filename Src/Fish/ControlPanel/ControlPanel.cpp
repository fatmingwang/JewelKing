#include "stdafx.h"
#include "ControlPanel.h"
#include "CodeInput.h"
#include "ReportInput.h"
#include "PasswordInput.h"
#include "OldPasswordInput.h"
#include "NewPasswordInput.h"
#include "ConfirmPasswordInput.h"
#include "time.h"
//#include "../EngineTest/Light.h"

cControlPanel*g_pControlPanel;

extern unsigned int	DeCodeHintCode( unsigned int e_iValue);
extern int			GenerationHintCode( int e_iValue);
extern void			GeneratingVendorHintCode();
extern void			GeneratingAuditHintCode();
extern void			WorkerShitfChange(void*e_pData);
extern void			ClearVendorTotalRecord_CurrendBalance(void*e_pData);
extern void			ResetDefaultControlSetup(void*e_pData);
extern void			ClearRecord(void*e_pData);
extern void			SaveSetupData(void*e_pData);
extern void			AuditMenueSelection1(void*e_pData);
extern void			AuditMenueSelection2(void*e_pData);
extern void			AuditMenueSelection3(void*e_pData);
extern void			DelayTime(void*e_pData);
extern void			Encoder(void*e_pData);
extern void			Decoder(void*e_pData);
extern void			FigureCheckCode(void*e_pData);
extern void			BulletPayRateMinPoint_Check(void*e_pData);
extern void			BulletPayRateAddPoint_Check(void*e_pData);
extern void			FigureSetupCode(void*e_pData);

void	WriteSetupChanged();
struct	sIntoMenuData
{
	sIntoMenuData(unsigned char e_ucKey,const WCHAR*e_strName)
	{
		KeyToMenu.SetKeyData(e_ucKey);
		strMenuName = e_strName;
	}
	std::wstring			strMenuName;
	cClickMouseBehavior		KeyToMenu;
	void					Update(){ KeyToMenu.SingnalProcess(); }
	bool					IsSatisfied(){ return KeyToMenu.IsSatisfiedCondition(); }
	void					Init(){ KeyToMenu.Init(); }
};

cControlPanel::cControlPanel(char*e_strPackagesFileName)
{
	//m_iPlayerCount = 6;
	m_pBG = new cBaseImage("Fish/ControlPanel/Image/FishBG_BigHasScene.png");
	m_pPasswordFormBG = new cBaseImage("Fish/ControlPanel/Image/FishBG_Small.png");
	m_pPasswordFormBG->SetPos(Vector3(350,150,0));
	g_pControlPanel = this;
	m_bGameReload = false;
	m_pEventManager = new cEventManager();
	m_pEventPackageRunner = m_pEventManager->m_pEventPackageRunner;
	m_pEventManager->ParseDataIntoXMLNode(e_strPackagesFileName);
	int	l_iCount = m_pEventManager->m_pEventVariableManager->Count();
	//lost variable file load default
	if( l_iCount == 0 )
	{
		char*	l_strDefaultVariableFile = m_pEventManager->GetFullpathFileName(L"DefaultVariableData.xml");
		m_pEventManager->m_pEventVariableManager->Parse(l_strDefaultVariableFile);
	}
	m_bInControlPanelMode = false;
	m_iSound = 100;
	m_bLuckyTime = false;
	unsigned char	l_KeyDatas[5] = {'C','V','B','N','M'};
	for( int i=0;i<5;++i )
	{
		m_KeyUpEffect[i].SetKeyData(l_KeyDatas[i]);
		m_KeyUpEffect[i].Init();
	}
	m_pEventManager->AddExternFunctionName(L"WorkerShitfChange",WorkerShitfChange);
	m_pEventManager->AddExternFunctionName(L"ClearVendorTotalRecord_CurrendBalance",ClearVendorTotalRecord_CurrendBalance);	
	m_pEventManager->AddExternFunctionName(L"ResetDefaultControlSetup",ResetDefaultControlSetup);
	m_pEventManager->AddExternFunctionName(L"ClearRecord",ClearRecord);
	m_pEventManager->AddExternFunctionName(L"SaveSetupData",SaveSetupData);
	m_pEventManager->AddExternFunctionName(L"AuditMenueSelection1",AuditMenueSelection1);
	m_pEventManager->AddExternFunctionName(L"AuditMenueSelection2",AuditMenueSelection2);
	m_pEventManager->AddExternFunctionName(L"AuditMenueSelection3",AuditMenueSelection3);
	m_pEventManager->AddExternFunctionName(L"DelayTime",DelayTime);
	m_pEventManager->AddExternFunctionName(L"FigureCheckCode",FigureCheckCode);
	m_pEventManager->AddExternFunctionName(L"BulletPayRateMinPoint_Check",BulletPayRateMinPoint_Check);
	m_pEventManager->AddExternFunctionName(L"BulletPayRateAddPoint_Check",BulletPayRateAddPoint_Check);
	m_pEventManager->AddExternFunctionName(L"FigureSetupCode",FigureSetupCode);
	
	cCodeInput*l_pCodeInput = new cCodeInput();
	m_pEventManager->m_pExternalEventPackage->AddObject(l_pCodeInput);
	cReportInput*l_pReportInput = new cReportInput();
	m_pEventManager->m_pExternalEventPackage->AddObject(l_pReportInput);
	cPasswordInput*l_pPasswordInput = new cPasswordInput();
	m_pEventManager->m_pExternalEventPackage->AddObject(l_pPasswordInput);

	cOldPasswordInput*l_pOldPasswordInput = new cOldPasswordInput();
	m_pEventManager->m_pExternalEventPackage->AddObject(l_pOldPasswordInput);
	cNewPasswordInput*l_pNewPasswordInput = new cNewPasswordInput();
	m_pEventManager->m_pExternalEventPackage->AddObject(l_pNewPasswordInput);
	cConfirmPasswordInput*l_pConfirmPasswordInput = new cConfirmPasswordInput();
	m_pEventManager->m_pExternalEventPackage->AddObject(l_pConfirmPasswordInput);

	
	StoreOldData();

	GetTimeStamp();

	CheckReport();
	m_iRenderAuditMenu = 0;
	m_fReportRoutine = 0.0f;
	m_spGlyphSmallFontRender = new cGlyphFontRender(SmallFontFileName, 2000);

	
	//Cheat Hole
	CheckCheatFile();
	GetVersion();
}

cControlPanel::~cControlPanel()
{
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_pPasswordFormBG);
	SAFE_DELETE(m_pEventManager);
	DELETE_VECTOR(m_IntoMenuDataList,sIntoMenuData*);
	SAFE_DELETE(m_spGlyphSmallFontRender);
}

void	cControlPanel::SaveData()
{
	char*	l_strFileName = m_pEventManager->GetFullpathFileName(L"VariableData.xml");
	this->m_pEventManager->m_pEventVariableManager->Export(l_strFileName);
}

void	cControlPanel::StoreOldData()
{
	/*WCHAR*l_Setup[l_SetupDataCount] = {L"AuditPassword"
    L"VendorPassword",
    L"LinePlayable",
    L"Language",
    L"Probability",
    L"Sound",
    L"CoinToScore",
    L"MoneyLimit",
    L"JPShareScorePercentage",
    L"LuckyTime",
    L"Exchange_In",
    L"Exchange_Out",
	L"Exchange_Ticket"};*/

	cEventVariable*l_pAuditPassword = m_pEventManager->m_pEventVariableManager->GetObject(L"AuditPassword");
	cEventVariable*l_pVendorPassword = m_pEventManager->m_pEventVariableManager->GetObject(L"VendorPassword");
	cEventVariable*l_pLanguage = m_pEventManager->m_pEventVariableManager->GetObject(L"Language");
	cEventVariable*l_pProbability = m_pEventManager->m_pEventVariableManager->GetObject(L"Probability");
	cEventVariable*l_pSound = m_pEventManager->m_pEventVariableManager->GetObject(L"Sound");
	cEventVariable*l_pCoinToScore = m_pEventManager->m_pEventVariableManager->GetObject(L"CoinToScore");
	cEventVariable*l_pMoneyLimit = m_pEventManager->m_pEventVariableManager->GetObject(L"MoneyLimit");
	cEventVariable*l_pExchange_In = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_In");
	cEventVariable*l_pExchange_Out = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_Out");
	cEventVariable*l_pExchange_Ticket = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_Ticket");
	cEventVariable*l_pPlayerCount = m_pEventManager->m_pEventVariableManager->GetObject(L"PlayerCount");
	cEventVariable*l_pBulletPayRateLimit = m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateLimit");
	cEventVariable*l_pBulletPayRateAddPoint = m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateAddPoint");
	cEventVariable*l_pBulletPayRateMinPoint = m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateMinPoint");
	//cEventVariable*l_pEventVariable = m_pEventManager->m_pEventVariableManager->GetObject(L"PlayerCount");

	//Report Parameter
	cEventVariable*l_pReportCount = m_pEventManager->m_pEventVariableManager->GetObject(L"ReportCount");
	cEventVariable*l_pReportFlag = m_pEventManager->m_pEventVariableManager->GetObject(L"ReportFlag");
	cEventVariable*l_pReportDay = m_pEventManager->m_pEventVariableManager->GetObject(L"ReportDay");
	cEventVariable*l_pSaveTime = m_pEventManager->m_pEventVariableManager->GetObject(L"SaveTime");

	cEventVariable*l_pLatestIncome =  m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_TotalIncome");
	cEventVariable*l_pLatestPay =   m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_TotalPay");
	cEventVariable*l_pLatestRevenue =  m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_Revenue");
	cEventVariable*l_pLatestExchangeIn = m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_ExchangeIn");
	cEventVariable*l_pLatestExchangeOut = m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_ExchangeOut");
	cEventVariable*l_pLatestToastCoin =  m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_ToastCoin");
	cEventVariable*l_pLatestRefundCoin = m_pEventManager->m_pEventVariableManager->GetObject(L"Latest_RefudCoin");


	cEventVariable*l_pPriorIncome =  m_pEventManager->m_pEventVariableManager->GetObject(L"Prior_TotalIncome");
	cEventVariable*l_pPriorPay =   m_pEventManager->m_pEventVariableManager->GetObject(L"Prior_TotalPay");
	cEventVariable*l_pPriorRevenue =  m_pEventManager->m_pEventVariableManager->GetObject(L"Prior_Revenue");
	cEventVariable*l_pPriorExchangeIn = m_pEventManager->m_pEventVariableManager->GetObject(L"Prior_ExchangeIn");
	cEventVariable*l_pPriorExchangeOut = m_pEventManager->m_pEventVariableManager->GetObject(L"Prior_ExchangeOut");
	cEventVariable*l_pPriorToastCoin =  m_pEventManager->m_pEventVariableManager->GetObject(L"Prior_ToastCoin");
	cEventVariable*l_pPriorRefundCoin = m_pEventManager->m_pEventVariableManager->GetObject(L"Prior_RefudCoin");

	cEventVariable*l_pHistoryTotalIncome = m_pEventManager->m_pEventVariableManager->GetObject(L"TotalIncome");
	cEventVariable*l_pHistoryTotalPay =  m_pEventManager->m_pEventVariableManager->GetObject(L"TotalPay");
	cEventVariable*l_pHistoryRevenue =  m_pEventManager->m_pEventVariableManager->GetObject(L"TotalRevenue");
	cEventVariable*l_pHistoryExchangeIn = m_pEventManager->m_pEventVariableManager->GetObject(L"TotalExchangeIn");
	cEventVariable*l_pHistoryExchangeOut = m_pEventManager->m_pEventVariableManager->GetObject(L"TotalExchangeOut");
	cEventVariable*l_pHistoryToastCoin = m_pEventManager->m_pEventVariableManager->GetObject(L"TotalToastCoin");
	cEventVariable*l_pHistoryRefundCoin = m_pEventManager->m_pEventVariableManager->GetObject(L"TotalRefudCoin");

	cEventVariable*l_pRefundCoinOrTicket = m_pEventManager->m_pEventVariableManager->GetObject(L"RefundCoinOrTicket");

	m_pi64LatestInMoney = l_pLatestIncome->GetInt64();
	m_pi64LatestOutMoney = l_pLatestPay->GetInt64();
	m_pi64LatestRevenue = l_pLatestRevenue->GetInt64();
	m_pi64LatestExchangeIn = l_pLatestExchangeIn->GetInt64();
	m_pi64LatestExchangeOut = l_pLatestExchangeOut->GetInt64();
	m_pi64LatestCoinIn = l_pLatestToastCoin->GetInt64();
	m_pi64LatestCoinOut = l_pLatestRefundCoin->GetInt64();

	m_pi64TotalInMoney = l_pHistoryTotalIncome->GetInt64();
	m_pi64TotalOutMoney = l_pHistoryTotalPay->GetInt64();
	m_pi64TotalRevenue = l_pHistoryRevenue->GetInt64();
	m_pi64TotalExchangeIn = l_pHistoryExchangeIn->GetInt64();
	m_pi64TotalExchangeOut = l_pHistoryExchangeOut->GetInt64();
	m_pi64TotalCoinIn = l_pHistoryToastCoin->GetInt64();
	m_pi64TotalCoinOut = l_pHistoryRefundCoin->GetInt64();

	m_pi64PriorInMoney = l_pPriorIncome->GetInt64();
	m_pi64PriorOutMoney = l_pPriorPay->GetInt64();
	m_pi64PriorRevenue = l_pPriorRevenue->GetInt64();
	m_pi64PriorExchangeIn = l_pPriorExchangeIn->GetInt64();
	m_pi64PriorExchangeOut = l_pPriorExchangeOut->GetInt64();
	m_pi64PriorCoinIn = l_pPriorToastCoin->GetInt64();
	m_pi64PriorCoinOut = l_pPriorRefundCoin->GetInt64();
	//======================

	//m_iPlayerCount = *l_pEventVariable->GetInt();
	m_iBulletPayRateAddPoint = *l_pBulletPayRateAddPoint->GetInt();
	m_iBulletPayRateLimit = *l_pBulletPayRateLimit->GetInt();
	m_iBulletPayRateMinPoint = *l_pBulletPayRateMinPoint->GetInt();

	m_iExchange_Ticket = *l_pExchange_Ticket->GetInt();
	m_i64MoneyLimit = *l_pMoneyLimit->GetInt64();
	m_strLanguage = l_pLanguage->GetWString()->c_str();
	m_strAuditPassword = l_pAuditPassword->GetWString()->c_str();
	m_strVendorPassword = l_pVendorPassword->GetWString()->c_str();
	m_iSound = *l_pSound->GetInt();
	m_fProbability = *l_pProbability->GetInt();
	m_iPlayerCount = *l_pPlayerCount->GetInt();
	this->m_iExchangeIn = *l_pExchange_In->GetInt();
	this->m_iExchangeOut = -*l_pExchange_Out->GetInt();
	cGameApp::m_spSoundParser->SetVolume(m_iSound/100.f);
	m_iCoinToScore = *l_pCoinToScore->GetInt();

	//Report Varabile
	m_iReportCount = *l_pReportCount->GetInt();
	m_iReportFlag = *l_pReportFlag->GetInt();
	m_iReportDay = *l_pReportDay->GetInt();
	m_iSaveTime = *l_pSaveTime->GetInt();

	m_iRefundCoinOrTicket = *l_pRefundCoinOrTicket->GetInt(); // 0 退幣 1 退彩票
}



void	cControlPanel::CheckReport()
{
	//Report Parameter
	cEventVariable*l_pReportFlag = m_pEventManager->m_pEventVariableManager->GetObject(L"ReportFlag");
	cEventVariable*l_pDelayTimeFlag = m_pEventManager->m_pEventVariableManager->GetObject(L"DelayTimeFlag");

	//if TimeStamp != 0, Check, or GetTimeINt64ToTimeStamp;
	if(m_iTimeStamp)
	{
		int64 l_iNowTime = GetTimeForInt64();
		if(m_iTimeStamp < l_iNowTime)
		{
			m_iTimeStamp = l_iNowTime;
			int64 l_iIntervalTime = m_iTimeStamp - m_iReportTime;

			int l_iReportDay = m_iReportDay;
			if(*l_pDelayTimeFlag->GetInt() == 1)
			{
				l_iReportDay = m_iReportDay + 2;
			}
			if((l_iIntervalTime / 86400) > l_iReportDay)
			{
				m_iReportFlag = 1;
			}
			else
			{
				m_iReportFlag = 0;
			}
		}
		else
		{
			m_iReportFlag = 2; // Report Time is Error
		}
	}
	else
	{
		//Init Game
		m_iReportFlag = 0;
		m_iTimeStamp = GetTimeForInt64();
		m_iReportTime = m_iTimeStamp;
	}

	int l_iReportFlag = *l_pReportFlag->GetInt();
	if(m_iReportFlag && m_bInControlPanelMode == false) 
	{
		m_bInControlPanelMode = true;
		m_pEventPackageRunner->LoadPackage(L"VendorPassword");
	}

	l_pReportFlag->SetInt(m_iReportFlag);
	//l_pTimeStamp->SetInt64(m_iTimeStamp);
}

int64	cControlPanel::GetTimeForInt64()
{
	time_t t = time(NULL);
	return (int64)time(NULL);
}

WCHAR*	cControlPanel::GetTime(int64 e_iTime)
{
	time_t t = (time_t)e_iTime;
	tm *p;
	p = localtime( &t );
	static WCHAR l_str[MAX_PATH];
	swprintf(l_str, MAX_PATH, L"%04d/%02d/%02d %02d:%02d:%02d", p->tm_year + 1900, 
		p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	return l_str;
}

void	cControlPanel::SoundCheck()
{
	cEventVariable*l_pSound = m_pEventManager->m_pEventVariableManager->GetObject(L"Sound");
	if( *l_pSound->GetInt() != m_iSound )
	{
		m_iSound = *l_pSound->GetInt();
		cGameApp::m_spSoundParser->SetVolume(m_iSound/100.f);
	}

	for( int i=0;i<5;++i )
	{
		m_KeyUpEffect[i].SingnalProcess();
		if( m_KeyUpEffect[i].IsSatisfiedCondition() )
		{
			cGameApp::SoundPlay(L"Button_Bet",true);
			m_KeyUpEffect[i].Init();
		}
	}
}

void	cControlPanel::InternalReset()
{
	int l_iCount = (int)m_IntoMenuDataList.size();
	for( int i=0;i<l_iCount;++i ){m_IntoMenuDataList[i]->Init();}
	//cLight::TurnUpperLight(false);
	//cLight::TurnBetPhaseLight(true);
}

void	cControlPanel::AddMenuData(unsigned char e_ucKey,const WCHAR*e_strName)
{
	sIntoMenuData*l_pIntoMenuData = new sIntoMenuData(e_ucKey,e_strName);
	m_IntoMenuDataList.push_back(l_pIntoMenuData);
}

void	cControlPanel::Update(float e_fElpaseTime)
{
	if( m_bInControlPanelMode )
	{
		m_pEventPackageRunner->Update(e_fElpaseTime);
		SoundCheck();
		if( !m_pEventPackageRunner->GetCurrentEventPackage() )
		{
			m_bInControlPanelMode = false;
			InternalReset();
			CheckReport();
		}
	}
	else
	{
		int l_iCount = (int)m_IntoMenuDataList.size();
		for( int i=0;i<l_iCount;++i )
		{
			m_IntoMenuDataList[i]->Update();
			if(m_IntoMenuDataList[i]->IsSatisfied())
			{
				m_bInControlPanelMode = true;

				//Check Hole
				CheckCheatFile();

				GeneratingVendorHintCode();
				GeneratingAuditHintCode();
				m_pEventPackageRunner->LoadPackage(m_IntoMenuDataList[i]->strMenuName.c_str());
				StoreOldData();
				cGameApp::m_spSoundParser->Pause(true);
				//cLight::TurnLight(false);
				//cLight::TurnUpperLight(true);
				return;
			}
		}
	}
	m_fReportRoutine += e_fElpaseTime;
	if(m_fReportRoutine > m_iSaveTime)
	{
		m_fReportRoutine = 0;
		CheckReport();
		WriteTimeStamp();
	}
}

void	cControlPanel::Render()
{
	if( m_bInControlPanelMode )
	{
		if( m_pBG )
			m_pBG->Render();
		if(m_pEventPackageRunner->GetCurrentEventPackage())
		{
			const WCHAR *l_strName = m_pEventPackageRunner->GetCurrentEventPackage()->GetName();
			if(!wcscmp(l_strName, L"VendorPassword") || !wcscmp(l_strName, L"OldVendorPassword") || !wcscmp(l_strName, L"NewVendorPassword") || !wcscmp(l_strName, L"ConfirmVendorPassword")
				|| !wcscmp(l_strName, L"ChangePasswordOK") || !wcscmp(l_strName, L"ChangePasswordFailed")  || !wcscmp(l_strName, L"OldPasswordInputError"))
			{
				m_pPasswordFormBG->Render();
			}

			m_pEventManager->m_pEventFont->SetScale((float)1.2);
			if(!wcscmp(l_strName, L"VendorMenu") || 
				!wcscmp(l_strName, L"VendorPassword"))
			{
				m_pEventManager->m_pEventFont->RenderFont(190,60,L"设定");
			}
			else if(!wcscmp(l_strName, L"VendorMenu_ParameterAdjust_1"))
			{
				m_pEventManager->m_pEventFont->RenderFont(190,60,L"设定-参数调整");
			}
			else if(!wcscmp(l_strName, L"VendorMenu_DecodeAdjust_2") || 
					!wcscmp(l_strName, L"CodeInputPackage"))
			{
				m_pEventManager->m_pEventFont->RenderFont(190,60,L"设定-解碼调整");
			}
			else if(!wcscmp(l_strName, L"VendorMenu_Audit_3"))
			{
				m_pEventManager->m_pEventFont->RenderFont(190,60,L"设定-查账");
			}
			else if(!wcscmp(l_strName, L"VendorMenu_RenderAccount_4") || 
					!wcscmp(l_strName, L"VendorMenu_RenderAccount_4_DelayTime") || 
					!wcscmp(l_strName, L"VendorMenu_RenderAccount_4_DelayTimeOK") || 
					!wcscmp(l_strName, L"VendorMenu_RenderAccount_4_DelayTimeRefuse") || 
					!wcscmp(l_strName, L"VendorMenu_RenderAccount_4_Report") || 
					!wcscmp(l_strName, L"VendorMenu_RenderAccount_4_ReportOK") || 
					!wcscmp(l_strName, L"VendorMenu_RenderAccount_4_ReportRefuse") || 
					!wcscmp(l_strName, L"ReportInputPackage"))
			{
				m_pEventManager->m_pEventFont->RenderFont(190,60,L"设定-报账");
			}
			else if(!wcscmp(l_strName, L"VendorMenu_Setup_5") || 
					!wcscmp(l_strName, L"VendorMenu_Setup_5_IOTest") || 
					!wcscmp(l_strName, L"VendorMenu_Setup_5_ChangePassword") || 
					!wcscmp(l_strName, L"ChangePasswordOK") || 
					!wcscmp(l_strName, L"ChangePasswordFailed") || 
					!wcscmp(l_strName, L"OldVendorPassword") || 
					!wcscmp(l_strName, L"OldPasswordInputError") || 
					!wcscmp(l_strName, L"NewVendorPassword") || 
					!wcscmp(l_strName, L"ConfirmVendorPassword"))
			{
				m_pEventManager->m_pEventFont->RenderFont(190,60,L"设定-系统设置");
			}
			m_pEventManager->m_pEventFont->SetScale(1.0);
		}
		m_pEventPackageRunner->Render();

		const WCHAR *l_strName = m_pEventPackageRunner->GetCurrentEventPackage()->GetName();
		if(!wcscmp(l_strName, L"VendorMenu_Audit_3"))
		{
			RenderAuditRecord(m_iRenderAuditMenu);
		}
		else
		{

		}
		

		if(m_iReportFlag == 1)	m_pEventManager->m_pEventFont->RenderFont(890,560,L"(打码报账已逾期)");
		else if(m_iReportFlag == 2)	m_pEventManager->m_pEventFont->RenderFont(880,560,L"(打码报账時間錯誤)");

		m_spGlyphSmallFontRender->RenderFont(850,103, GetTime(GetTimeForInt64()));

		m_spGlyphSmallFontRender->RenderFont(850,60, L"GameVersion:");
		m_spGlyphSmallFontRender->RenderFont(1020,60, m_wchVersion.c_str());
	}
}

//if language or bet rule is changed.
bool	cControlPanel::IsReloadGame()
{
	//cEventVariable*l_pLinePlayable = m_pEventManager->m_pEventVariableManager->GetObject(L"LinePlayable");
	cEventVariable*l_pLanguage = m_pEventManager->m_pEventVariableManager->GetObject(L"Language");
	cEventVariable*l_pVendorPassword = m_pEventManager->m_pEventVariableManager->GetObject(L"VendorPassword");
	cEventVariable*l_pAuditPassword = m_pEventManager->m_pEventVariableManager->GetObject(L"AuditPassword");
	cEventVariable*l_pSound = m_pEventManager->m_pEventVariableManager->GetObject(L"Sound");
	cEventVariable*l_pProbability = m_pEventManager->m_pEventVariableManager->GetObject(L"Probability");
	cEventVariable*l_pExchange_In = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_In");
	cEventVariable*l_pExchange_Out = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_Out");
	cEventVariable*l_pCoinToScore = m_pEventManager->m_pEventVariableManager->GetObject(L"CoinToScore");
	cEventVariable*l_pMoneyLimit = m_pEventManager->m_pEventVariableManager->GetObject(L"MoneyLimit");
	cEventVariable*l_pRefundCoinOrTicket = m_pEventManager->m_pEventVariableManager->GetObject(L"RefundCoinOrTicket");
	cEventVariable*l_pPlayerCount = m_pEventManager->m_pEventVariableManager->GetObject(L"PlayerCount");

	cEventVariable*l_pBulletPayRateAddPoint = m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateAddPoint");
	cEventVariable*l_pBulletPayRateLimit = m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateLimit");
	cEventVariable*l_pBulletPayRateMinPoint = m_pEventManager->m_pEventVariableManager->GetObject(L"BulletPayRateMinPoint");

	if( m_iBulletPayRateLimit != *l_pBulletPayRateLimit->GetInt())
		m_bGameReload = true;
	if( m_iBulletPayRateAddPoint != *l_pBulletPayRateAddPoint->GetInt())
		m_bGameReload = true;
	if( m_iBulletPayRateMinPoint != *l_pBulletPayRateMinPoint->GetInt())
		m_bGameReload = true;

	if( m_i64MoneyLimit != *l_pMoneyLimit->GetInt64() )
		m_bGameReload = true;
	//if( m_fJPShareScorePercentage != *l_pJPShareScorePercentage->GetFloat() )
	//	m_bGameReload = true;
	if( m_fProbability != *l_pProbability->GetInt() )
		m_bGameReload = true;
	
	if( m_iCoinToScore != *l_pCoinToScore->GetInt() )
		m_bGameReload = true;
	if( m_iExchangeIn != *l_pExchange_In->GetInt() )
		m_bGameReload = true;
	if( m_iExchangeOut != -*l_pExchange_Out->GetInt() )
		m_bGameReload = true;
	bool	l_bLuckyTime = false;
	//if(!wcscmp(l_pLuckyTime->GetWString()->c_str(),L"On"))
	//	l_bLuckyTime = true;
	//if( m_bLuckyTime != l_bLuckyTime )
	//	m_bGameReload = true;
	if( m_iRefundCoinOrTicket != *l_pRefundCoinOrTicket->GetInt() )
		m_bGameReload = true;
	if(wcscmp(m_strLanguage.c_str(),l_pLanguage->GetWString()->c_str()))
		m_bGameReload = true;
	//m_strPassword = l_pPassword->GetWString()->c_str();
	if( m_iPlayerCount != *l_pPlayerCount->GetInt() )
		m_bGameReload = true;


	this->StoreOldData();

	if( !m_bGameReload)
		cGameApp::m_spSoundParser->Pause(false);
	else
	{
		//WriteSetupChanged();
	}
	bool	l_bWriteFile = false;
	char*	l_strFileName = m_pEventManager->GetFullpathFileName(L"VariableData.xml");
	this->m_pEventManager->m_pEventVariableManager->Export(l_strFileName);

	WriteTimeStamp();

	//Check Hole
	CheckCheatFile();

	m_bGameReload = true;
	bool	l_bResult = m_bGameReload;
	//m_bGameReload = false;
	return l_bResult;
}

bool	cControlPanel::IsInControlPanelMode()
{
	return m_bInControlPanelMode;
}

void	cControlPanel::MouseDown(int e_iPosX,int e_iPosY)
{
	m_pEventPackageRunner->MouseDown(e_iPosX,e_iPosY);
}

void	cControlPanel::MouseMove(int e_iPosX,int e_iPosY)
{
	m_pEventPackageRunner->MouseMove(e_iPosX,e_iPosY);
}

void	cControlPanel::MouseUp(int e_iPosX,int e_iPosY)
{
	m_pEventPackageRunner->MouseUp(e_iPosX,e_iPosY);
}

void	WriteSetupChanged()
{
	cNodeISAX	l_NodeISAX;
	TiXmlElement *l_pRoot = 0;
	TiXmlDocument l_Doc;

	
	std::string	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(SETUP_CHANGE_FILE_NAME);

	if(UT::IsFileExists(l_strFileName.c_str()))
	{//get old file
		l_NodeISAX.ParseDataIntoXMLNode(l_strFileName.c_str());
		if( l_NodeISAX.GetRootElement() )
			l_pRoot = new TiXmlElement(*l_NodeISAX.GetRootElement());
	}
	if( !l_pRoot )
	{//new file
		l_pRoot = new TiXmlElement(L"Root");
	}
	l_Doc.LinkEndChild( l_pRoot );
	TiXmlElement *l_pTime = new TiXmlElement(L"Time");
	std::wstring  l_strTime = UT::GetSystemTime(false);
	l_pTime->SetAttribute(L"Time",l_strTime.c_str());
	l_pRoot->LinkEndChild(l_pTime);
	//6xml data for a batch,so strip count over 498(6*83)
	#define	MAXIMUM_DATA_COUNT	50
	int	l_iCount = l_pRoot->ChildElementCount();
	if( l_iCount > MAXIMUM_DATA_COUNT )
	{
		int	l_iStripValue = l_iCount-MAXIMUM_DATA_COUNT;
		int	l_iIndex = 0;
		TiXmlElement*l_pTiXmlElement = l_pRoot->FirstChildElement();
		while(l_pTiXmlElement)
		{
			if( l_iIndex == l_iStripValue )
			{
				break;
			}
			TiXmlElement*l_pDeleteTiXmlElement = l_pTiXmlElement;
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			l_pRoot->RemoveChild(l_pDeleteTiXmlElement);
			++l_iIndex;
		}
	}
	l_Doc.SaveFile(l_strFileName.c_str());
}

void	cControlPanel::RenderAuditRecord(int e_iMenuIndex)
{
	if(e_iMenuIndex == 1)//遊戲記錄
	{
		WCHAR *l_strTitle[3] = {L"鱼种", L"总获得分数", L"本期获得分数"};
		WCHAR *l_strFish[17] = {L"小丑魚", L"三间火箭", L"关刀鱼", L"蓝倒吊", L"獅子鱼", 
								L"水母",L"河豚",L"海龟",L"鮟鱇鱼",L"魟鱼", L"旗鱼",
								L"黑鳍鲨鱼", L"圆齿垂头鲨", L"金龙", L"玄武（红）",L"玄武（银）", L"玄武（金）"};

		m_spGlyphSmallFontRender->RenderFont(340, 140, l_strTitle[0]);
		m_spGlyphSmallFontRender->RenderFont(420, 140, l_strTitle[1]);
		m_spGlyphSmallFontRender->RenderFont(550, 140, l_strTitle[2]);

		for(int i = 0; i < 9; i ++)
		{
			m_spGlyphSmallFontRender->RenderFont(340, 180 + 45 * i, l_strFish[i]);
			m_spGlyphSmallFontRender->RenderFont(520, 180 + 45 * i, ValueToStringW(0));
			m_spGlyphSmallFontRender->RenderFont(650, 180 + 45 * i, ValueToStringW(0));
		}
		
		m_spGlyphSmallFontRender->RenderFont(700, 140, l_strTitle[0]);
		m_spGlyphSmallFontRender->RenderFont(820, 140, l_strTitle[1]);
		m_spGlyphSmallFontRender->RenderFont(950, 140, l_strTitle[2]);

		for(int i = 0; i < 8; i ++)
		{
			m_spGlyphSmallFontRender->RenderFont(700, 180 + 45 * i, l_strFish[9 + i]);
			m_spGlyphSmallFontRender->RenderFont(915, 180 + 45 * i, ValueToStringW(0));
			m_spGlyphSmallFontRender->RenderFont(1060, 180 + 45 * i, ValueToStringW(0));
		}
	}
	else if(e_iMenuIndex == 2)
	{
		//獎項紀錄
		WCHAR *l_strTitle[2] = {L"总纪录", L"本期纪录"};
		WCHAR *l_strSubTitle[3] = {L"奖项", L"获得分数", L"获得次数"};
		WCHAR *l_strFish[4] = {L"旋风炮", L"冷冻弹", L"落雷炮", L"雷霆炸弹"};

		for(int i = 0 ; i < 2; i++)
		{
			m_spGlyphSmallFontRender->RenderFont(480 + 400 * i, 140, l_strTitle[i]);
		}

		m_spGlyphSmallFontRender->RenderFont(340, 170, l_strSubTitle[0]);
		m_spGlyphSmallFontRender->RenderFont(440, 170, l_strSubTitle[1]);
		m_spGlyphSmallFontRender->RenderFont(560, 170, l_strSubTitle[2]);
		m_spGlyphSmallFontRender->RenderFont(700, 170, l_strSubTitle[0]);
		m_spGlyphSmallFontRender->RenderFont(800, 170, l_strSubTitle[1]);
		m_spGlyphSmallFontRender->RenderFont(920, 170, l_strSubTitle[2]);

		for(int i =0 ; i < 4; i++)
		{
			m_spGlyphSmallFontRender->RenderFont(340, 220 + 40 * i, l_strFish[i]);
			m_spGlyphSmallFontRender->RenderFont(520, 220 + 40 * i,  ValueToStringW(0));
			m_spGlyphSmallFontRender->RenderFont(620, 220 + 40 * i,  ValueToStringW(0));
			m_spGlyphSmallFontRender->RenderFont(700, 220 + 40 * i, l_strFish[i]);
			m_spGlyphSmallFontRender->RenderFont(880, 220 + 40 * i,  ValueToStringW(0));
			m_spGlyphSmallFontRender->RenderFont(980, 220 + 40 * i,  ValueToStringW(0));
		}
	}
	else if(e_iMenuIndex == 3)
	{
		//營收紀錄
		//m_spGlyphSmallFontRender->RenderFont(350,150,L"Menu3");
		WCHAR *l_strTitle[3] = {L"总纪录", L"本期纪录", L"上期纪录"};
		WCHAR *l_strSubTitle[9] = {L"投币：", L"退币：", L"上分：", L"下分：", L"彩票：", L"总进分数：", L"总退分数：", L"盈余：", L"最后报账时间："};
		WCHAR *l_strSubTitle_N[9] = {L"投币：", L"退币：", L"上分：", L"下分：", L"彩票：", L"总进分数", L"总退分数", L"盈余", L"最后报账时间"};
		
		for(int i = 0 ; i < 2; i++)
		{
			m_spGlyphSmallFontRender->RenderFont(480 + 350 * i, 140, l_strTitle[i]);
		}
		m_spGlyphSmallFontRender->RenderFont(830, 430, l_strTitle[2]);
		for(int i = 0 ; i < 9; i++)
		{
			if(i >= 5)
				m_spGlyphSmallFontRender->RenderFont(460, 180 + 30 * i + 25 * (i-5), l_strSubTitle_N[i]);
			else
				m_spGlyphSmallFontRender->RenderFont(340, 180 + 30 * i, l_strSubTitle_N[i]);

			if(i < 8) 
			{
				if(i >= 5) 
					m_spGlyphSmallFontRender->RenderFont(620, 205 + 30 * i + 25 * (i-5), L"币");
				else
					m_spGlyphSmallFontRender->RenderFont(620, 180 + 30 * i, L"币");

				m_spGlyphSmallFontRender->RenderFont(700, 180 + 30 * i, l_strSubTitle[i]);
				m_spGlyphSmallFontRender->RenderFont(1000, 180 + 30 * i, L"币");
			}
		}

		m_spGlyphSmallFontRender->RenderFont(700, 460, l_strSubTitle[5]);
		m_spGlyphSmallFontRender->RenderFont(1000, 460, L"币");

		m_spGlyphSmallFontRender->RenderFont(700, 490, l_strSubTitle[6]);
		m_spGlyphSmallFontRender->RenderFont(1000, 490, L"币");

		m_spGlyphSmallFontRender->RenderFont(700, 520, l_strSubTitle[7]);
		m_spGlyphSmallFontRender->RenderFont(1000, 520, L"币");



		int64 l_iValue = *m_pi64TotalCoinIn / m_iCoinToScore;
		Vector2 l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(600 - (int)l_v2Size.x, 180, ValueToStringW(l_iValue));

		l_iValue = *m_pi64TotalCoinOut / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(600 - (int)l_v2Size.x, 210, ValueToStringW(l_iValue));

		l_iValue = *m_pi64TotalExchangeIn / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(600 - (int)l_v2Size.x, 240, ValueToStringW(l_iValue));

		l_iValue = *m_pi64TotalExchangeOut / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(600 - (int)l_v2Size.x, 270, ValueToStringW(l_iValue));

		//int l_iValue = *m_pi64TotalCoinOut / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(0));
		m_spGlyphSmallFontRender->RenderFont(600 - (int)l_v2Size.x, 300, ValueToStringW(0));

		l_iValue = *m_pi64TotalInMoney / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(530 - (int)l_v2Size.x, 355, ValueToStringW(l_iValue));

		l_iValue = *m_pi64TotalOutMoney / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(530 - (int)l_v2Size.x, 410, ValueToStringW(l_iValue));

		l_iValue = *m_pi64TotalRevenue / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(530 - (int)l_v2Size.x, 465, ValueToStringW(l_iValue));

		l_iValue = *m_pi64LatestCoinIn / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,180, ValueToStringW(l_iValue));

		l_iValue = *m_pi64LatestCoinOut / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,210, ValueToStringW(l_iValue));

		l_iValue = *m_pi64LatestExchangeIn / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,240, ValueToStringW(l_iValue));

		l_iValue = *m_pi64LatestExchangeOut / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,270, ValueToStringW(l_iValue));

		//int l_iValue = *m_pi64TotalCoinOut / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(0));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,300, ValueToStringW(0));

		l_iValue = *m_pi64LatestInMoney / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,330, ValueToStringW(l_iValue));

		l_iValue = *m_pi64LatestOutMoney / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,360, ValueToStringW(l_iValue));

		l_iValue = *m_pi64LatestRevenue / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,390, ValueToStringW(l_iValue));

		m_spGlyphSmallFontRender->RenderFont(370 ,520, ValueToStringW(GetTime(this->m_iReportTime)));


		l_iValue = *m_pi64PriorInMoney / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,460, ValueToStringW(l_iValue));

		l_iValue = *m_pi64PriorOutMoney / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,490, ValueToStringW(l_iValue));

		l_iValue = *m_pi64PriorRevenue / m_iCoinToScore;
		l_v2Size = m_spGlyphSmallFontRender->GetRenderSize(ValueToStringW(l_iValue));
		m_spGlyphSmallFontRender->RenderFont(980 - (int)l_v2Size.x,520, ValueToStringW(l_iValue));
	}
}


//ValueChanger.xml

//衝坑程式
void	cControlPanel::CheckCheatFile()
{
	char* l_strFileName = "VariableChanger.xml";

	//檔案存在且不在後台模式下，才啟動衝坑模式
	if(UT::IsFileExists(l_strFileName) && !(IsInControlPanelMode()))
	{
		ProcessVendorAskMachineStartStatusData(l_strFileName);
	}
	else
	{
		ProcessVendorInControlPanelModelStatusData(l_strFileName);
	}
}

void	cControlPanel::ProcessVendorInControlPanelModelStatusData(char*e_strFileName)
{
	//舊有資料的Parse
	char*	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(L"VariableData.xml");
	cEventVariableManager	l_EventVariableManager;
	l_EventVariableManager.Parse(l_strFileName);

	cNodeISAX l_VendorAskMachineStartStatusDataNodeISAX;
	if( l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode(e_strFileName) )
	{
		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"Data") )
				{
					const WCHAR*l_strName = l_pElement->Attribute(L"Name");
					//l_strValue = l_pElement->Attribute(L"Value");
					cEventVariable*l_pEventVariable = m_pEventManager->m_pEventVariableManager->GetObject(l_strName);
					cEventVariable*l_pEventVariable2 = l_EventVariableManager.GetObject(l_strName);
					std::wstring	l_str = l_pEventVariable2->ConvertToString();
					l_pEventVariable->SetDataByString(l_str.c_str());
					//l_pEventVariable->SetDataByString(l_EventVariableManager.GetObject(l_strName));
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		} 
	}
}

void	cControlPanel::ProcessVendorAskMachineStartStatusData(char* e_strFileName)
{
	cNodeISAX l_VendorAskMachineStartStatusDataNodeISAX;
	if( l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode(e_strFileName) )
	{
		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"Data") )
				{
					const WCHAR*l_strName = l_pElement->Attribute(L"Name");
					l_strValue = l_pElement->Attribute(L"Value");
					cEventVariable*l_pEventVariable = m_pEventManager->m_pEventVariableManager->GetObject(l_strName);
					l_pEventVariable->SetDataByString(l_strValue);
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		} 
	}
}

void	cControlPanel::GetTimeStamp()
{
	cNodeISAX l_VendorAskMachineStartStatusDataNodeISAX;
	if( l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode("TimeStamp.xml") )
	{
		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"Data") )
				{
					const WCHAR*l_strName = l_pElement->Attribute(L"Name");
					if(!wcscmp(l_strName, L"TimeStamp"))
					{
						l_strValue = l_pElement->Attribute(L"Value");
						m_iTimeStamp =  atoi(ValueToString(l_strValue));
					} 
					else if(!wcscmp(l_strName, L"ReportTime"))
					{
						l_strValue = l_pElement->Attribute(L"Value");
						m_iReportTime =  atoi(ValueToString(l_strValue));
					}
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		} 
	}
}



void	cControlPanel::WriteTimeStamp()
{
	cNodeISAX l_VendorAskMachineStartStatusDataNodeISAX;
	if( l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode("TimeStamp.xml") )
	{
		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"Data") )
				{
					const WCHAR*l_strName = l_pElement->Attribute(L"Name");
					if(!wcscmp(l_strName, L"TimeStamp"))
					{
						l_strValue = l_pElement->Attribute(L"Value");
						l_pElement->SetAttribute(L"Value", ValueToStringW(m_iTimeStamp));
					} 
					else if(!wcscmp(l_strName, L"ReportTime"))
					{
						l_strValue = l_pElement->Attribute(L"Value");
						l_pElement->SetAttribute(L"Value", ValueToStringW(m_iReportTime));
					}
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		} 
	}
	l_VendorAskMachineStartStatusDataNodeISAX.Export("TimeStamp.xml");
}


void	cControlPanel::GetVersion()
{
	cNodeISAX l_VendorAskMachineStartStatusDataNodeISAX;
	if( l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode("Version.xml") )
	{
		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"Data") )
				{
					//const WCHAR*l_strName = l_pElement->Attribute(L"Name");
					m_wchVersion = (WCHAR*)l_pElement->Attribute(L"Value");
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		} 
	}
}