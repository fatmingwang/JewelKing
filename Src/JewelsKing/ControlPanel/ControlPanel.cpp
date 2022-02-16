#include "stdafx.h"
#include "ControlPanel.h"
#include "../EngineTest/Light.h"

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

void	WriteSetupChanged();

cControlPanel::cControlPanel(char*e_strPackagesFileName)
{
	m_pBG = new cBaseImage("JewelsKing/ControlPanel/BG.PNG");
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
	
	StoreOldData();
}

cControlPanel::~cControlPanel()
{
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_pEventManager);
	DELETE_VECTOR(m_IntoMenuDataList,sIntoMenuData*);
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
	cEventVariable*l_pLinePlayable = m_pEventManager->m_pEventVariableManager->GetObject(L"LinePlayable");
	cEventVariable*l_pLanguage = m_pEventManager->m_pEventVariableManager->GetObject(L"Language");
	cEventVariable*l_pProbability = m_pEventManager->m_pEventVariableManager->GetObject(L"Probability");
	cEventVariable*l_pSound = m_pEventManager->m_pEventVariableManager->GetObject(L"Sound");
	cEventVariable*l_pCoinToScore = m_pEventManager->m_pEventVariableManager->GetObject(L"CoinToScore");
	cEventVariable*l_pMoneyLimit = m_pEventManager->m_pEventVariableManager->GetObject(L"MoneyLimit");
	cEventVariable*l_pJPShareScorePercentage = m_pEventManager->m_pEventVariableManager->GetObject(L"JPShareScorePercentage");
	cEventVariable*l_pLuckyTime = m_pEventManager->m_pEventVariableManager->GetObject(L"LuckyTime");
	cEventVariable*l_pExchange_In = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_In");
	cEventVariable*l_pExchange_Out = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_Out");
	cEventVariable*l_pExchange_Ticket = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_Ticket");
	cEventVariable*l_pLeastJP = m_pEventManager->m_pEventVariableManager->GetObject(L"JPLeastMoney");


	if( !l_pLinePlayable )
	{
		UT::ErrorMsg(L"StoreOldData()",L"necessary event variable data is not exist");
		return;
	}
	m_iExchange_Ticket = *l_pExchange_Ticket->GetInt();
	m_iMoneyLimit = *l_pMoneyLimit->GetInt();
	m_fJPShareScorePercentage = *l_pJPShareScorePercentage->GetFloat();
	m_iLineBetable = *l_pLinePlayable->GetInt();
	m_strLanguage = l_pLanguage->GetWString()->c_str();
	m_strAuditPassword = l_pAuditPassword->GetWString()->c_str();
	m_strVendorPassword = l_pVendorPassword->GetWString()->c_str();
	m_iSound = *l_pSound->GetInt();
	m_fProbability = *l_pProbability->GetFloat();
	m_iLeastJP = *l_pLeastJP->GetInt();
	if(!wcscmp(l_pLuckyTime->GetWString()->c_str(),L"On"))
		m_bLuckyTime = true;
	else
		m_bLuckyTime = false;
	this->m_iExchangeIn = *l_pExchange_In->GetInt();
	this->m_iExchangeOut = -*l_pExchange_Out->GetInt();
	cGameApp::m_spSoundParser->SetVolume(m_iSound/100.f);
	m_iCoinToScore = *l_pCoinToScore->GetInt();
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
	cLight::TurnUpperLight(false);
	cLight::TurnBetPhaseLight(true);
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
				GeneratingVendorHintCode();
				GeneratingAuditHintCode();
				m_pEventPackageRunner->LoadPackage(m_IntoMenuDataList[i]->strMenuName.c_str());
				StoreOldData();
				cGameApp::m_spSoundParser->Pause(true);
				cLight::TurnLight(false);
				cLight::TurnUpperLight(true);
				return;
			}
		}
	}
}

void	cControlPanel::Render()
{
	if( m_bInControlPanelMode )
	{
		m_pBG->Render();
		m_pEventPackageRunner->Render();
	}
}

//if language or bet rule is changed.
bool	cControlPanel::IsReloadGame()
{
	cEventVariable*l_pLinePlayable = m_pEventManager->m_pEventVariableManager->GetObject(L"LinePlayable");
	cEventVariable*l_pLanguage = m_pEventManager->m_pEventVariableManager->GetObject(L"Language");
	cEventVariable*l_pVendorPassword = m_pEventManager->m_pEventVariableManager->GetObject(L"VendorPassword");
	cEventVariable*l_pAuditPassword = m_pEventManager->m_pEventVariableManager->GetObject(L"AuditPassword");
	cEventVariable*l_pSound = m_pEventManager->m_pEventVariableManager->GetObject(L"Sound");
	cEventVariable*l_pProbability = m_pEventManager->m_pEventVariableManager->GetObject(L"Probability");
	cEventVariable*l_pExchange_In = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_In");
	cEventVariable*l_pExchange_Out = m_pEventManager->m_pEventVariableManager->GetObject(L"Exchange_Out");
	cEventVariable*l_pCoinToScore = m_pEventManager->m_pEventVariableManager->GetObject(L"CoinToScore");
	cEventVariable*l_pLuckyTime = m_pEventManager->m_pEventVariableManager->GetObject(L"LuckyTime");
	cEventVariable*l_pMoneyLimit = m_pEventManager->m_pEventVariableManager->GetObject(L"MoneyLimit");
	cEventVariable*l_pJPShareScorePercentage = m_pEventManager->m_pEventVariableManager->GetObject(L"JPShareScorePercentage");
	cEventVariable*l_pLeastJP = m_pEventManager->m_pEventVariableManager->GetObject(L"JPLeastMoney");

	if( m_iMoneyLimit != *l_pMoneyLimit->GetInt() )
		m_bGameReload = true;
	if( m_fJPShareScorePercentage != *l_pJPShareScorePercentage->GetFloat() )
		m_bGameReload = true;
	if( m_fProbability != *l_pProbability->GetFloat() )
		m_bGameReload = true;
	if( m_iLeastJP != *l_pLeastJP->GetInt() )
		m_bGameReload = true;
	
	if( m_iCoinToScore != *l_pCoinToScore->GetInt() )
		m_bGameReload = true;
	if( m_iExchangeIn != *l_pExchange_In->GetInt() )
		m_bGameReload = true;
	if( m_iExchangeOut != -*l_pExchange_Out->GetInt() )
		m_bGameReload = true;
	bool	l_bLuckyTime = false;
	if(!wcscmp(l_pLuckyTime->GetWString()->c_str(),L"On"))
		l_bLuckyTime = true;
	if( m_bLuckyTime != l_bLuckyTime )
		m_bGameReload = true;
	if( m_fProbability != *l_pProbability->GetFloat() )
		m_bGameReload = true;
	if(m_iLineBetable != *l_pLinePlayable->GetInt())
		m_bGameReload = true;
	if(wcscmp(m_strLanguage.c_str(),l_pLanguage->GetWString()->c_str()))
		m_bGameReload = true;
	//m_strPassword = l_pPassword->GetWString()->c_str();


	this->StoreOldData();

	if( !m_bGameReload)
		cGameApp::m_spSoundParser->Pause(false);
	else
	{
		WriteSetupChanged();
	}
	bool	l_bWriteFile = false;
	char*	l_strFileName = m_pEventManager->GetFullpathFileName(L"VariableData.xml");
	this->m_pEventManager->m_pEventVariableManager->Export(l_strFileName);
	bool	l_bResult = m_bGameReload;
	m_bGameReload = false;
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