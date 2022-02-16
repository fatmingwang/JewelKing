#include "stdafx.h"
#include "King.h"
#include "GameApp.h"
#include "TestPhase.h"
#include "SlotRule.h"
#include "SpinPhase.h"
#include "BetPhase.h"
#include "CalculateScorePhase.h"
#include "BaseExtraGame.h"
#include "ExtraGamePhase.h"
#include "CoinDevice.h"
#include "GameNetwork.h"
extern char*				g_strSlotBetData;
extern cToastCoinDevice*	g_pToastCoinDevice;
extern cRefundingCoinDevice*g_pRefundingCoinDevice;
extern cExchangeMoneyDevice*g_pExchangeMoneyInDevice;
extern cExchangeMoneyDevice*g_pExchangeMoneyOutDevice;
extern cFrameBuffer*		g_pFrameBuffer;
//for limimt viewport
extern Vector4				g_vViewRect;
bool	g_bUseProbability = true;
void	cEngineTestApp::LoadGameSetup()
{
	int						l_iNumLineBetable = 25;
	std::wstring			l_strLanguage = L"EN";
	int						l_iProbability = 0;

	cNodeISAX				l_NodeISAX;

	if(l_NodeISAX.ParseDataIntoXMLNode("JewelsKing/GameSetup.xml"))
	{
		TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
		l_strLanguage = l_pTiXmlElement->Attribute(L"Language");
		const WCHAR*l_strBetRule = l_pTiXmlElement->Attribute(L"BetLine");
		const WCHAR*l_strProbability = l_pTiXmlElement->Attribute(L"Probability");
		const WCHAR*l_strOpenNetWork = l_pTiXmlElement->Attribute(L"OpenNetWork");
		if( !this->m_spControlPanel )
		{
			if( l_strBetRule )
				m_siNumLineBetable = _wtoi(l_strBetRule);
			else
				m_siNumLineBetable = 25;
		}
		else
		{
			l_iNumLineBetable = this->m_spControlPanel->m_iLineBetable;
			l_strLanguage = this->m_spControlPanel->m_strLanguage;				
		}
		if( l_strProbability )
			g_bUseProbability = _wtoi(l_strProbability)?true:false;
		if( l_strOpenNetWork )
			cEngineTestApp::m_sbOpenNetWork = _wtoi(l_strOpenNetWork)?true:false;
	}
	NG_MPDILIST_NAME								= L"JewelsKing/ImageData/NG.mpdi";
	ANIMATED_MOVE_SOIN_OBJECT_MPDILIST_FILE_NAME	= L"JewelsKing/ImageData/AnimatedMoveSpinObject.mpdi";
	WIN_MONEY_SPIN_OBJECT_MPDI_MPDILIST_FILE_NAME	= L"JewelsKing/ImageData/SB.mpdi";
	KING_MPDI_MPDILIST_FILE_NAME					= L"JewelsKing/ImageData/king.mpdi";
	FREE_GAME_MINING_MPDILIST_FILE_NAME				= L"JewelsKing/ImageData/freegame2.mpdi";
	FREE_GAME_JP_MPDILIST_FILE_NAME					= L"JewelsKing/ImageData/Jackpot.mpdi";
	COIN_FOUNTAIN									=  "JewelsKing/ImageData/Coin2.prt";
	JEWELS_RAIN__MPDILIST_FILE_NAME					= L"JewelsKing/ImageData/jewelrain.mpdi";
	cEventVariable*l_pEventVariable = this->m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"LinePlayable");

	switch(*l_pEventVariable->GetInt())
	{
		case 25:
			RESULT_LINES_MPDILIST_FILE_NAME	= L"JewelsKing/ImageData/line25.mpdi";
			break;
		case 15:
			RESULT_LINES_MPDILIST_FILE_NAME	= L"JewelsKing/ImageData/line15.mpdi";
			break;
		case 9:
			RESULT_LINES_MPDILIST_FILE_NAME	= L"JewelsKing/ImageData/line9.mpdi";
			break;
	}
	if( !wcscmp(L"CH",l_strLanguage.c_str()) )
	{
		FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME			= L"JewelsKing/ImageData/ImageData_CH/Freegametimes.mpdi";
		FREE_GAME_BONUS_MPDILIST_FILE_NAME				= L"JewelsKing/ImageData/ImageData_CH/FG.mpdi";
		BIG_WIN_MPDILIST_FILE_NAME						= L"JewelsKing/ImageData/ImageData_CH/BigWin.mpdi";
		switch(*l_pEventVariable->GetInt())
		{
			case 25:
				UI_MPDILIST_FILE_NAME							= L"JewelsKing/ImageData/ImageData_CH/UI25/UI.mpdi";
				break;
			case 15:
				UI_MPDILIST_FILE_NAME							= L"JewelsKing/ImageData/ImageData_CH/UI15/UI.mpdi";
				break;
			case 9:
				UI_MPDILIST_FILE_NAME							= L"JewelsKing/ImageData/ImageData_CH/UI9/UI.mpdi";
				break;
		}
	}
	else
	if( !wcscmp(L"EN",l_strLanguage.c_str()) )
	{
		FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME			= L"JewelsKing/ImageData/ImageData_EN/Freegametimes.mpdi";
		FREE_GAME_BONUS_MPDILIST_FILE_NAME				= L"JewelsKing/ImageData/ImageData_EN/FG.mpdi";
		BIG_WIN_MPDILIST_FILE_NAME						= L"JewelsKing/ImageData/ImageData_EN/BigWin.mpdi";
		switch(*l_pEventVariable->GetInt())
		{
			case 25:
				UI_MPDILIST_FILE_NAME							= L"JewelsKing/ImageData/ImageData_EN/UI25/UI.mpdi";
				break;
			case 15:
				UI_MPDILIST_FILE_NAME							= L"JewelsKing/ImageData/ImageData_EN/UI15/UI.mpdi";
				break;
			case 9:
				UI_MPDILIST_FILE_NAME							= L"JewelsKing/ImageData/ImageData_EN/UI9/UI.mpdi";
				break;
		}
	}
	m_siNumLineBetable = l_iNumLineBetable;
	std::string	l_strBetRuleFileName = "JewelsKing/BetRule_";
	l_strBetRuleFileName += ValueToString(m_siNumLineBetable);
	SAFE_DELETE(g_strSlotBetData);
	g_strSlotBetData = new char[MAX_PATH];
	sprintf(g_strSlotBetData,"%s.xml\0",l_strBetRuleFileName.c_str());
}

void	cEngineTestApp::SetupSpinData()
{
	m_spSlotLineObjectManagerList = new cSlotObjectManagerList;
	m_spSlotRuleData = new cSlotRuleData;
	m_spPlayerData = new cPlayerData();
	m_spBetBehavior = new cBetBehavior(m_spSlotRuleData,m_spPlayerData);
	m_spBetBehavior->SetupInfoPos();
	bool	l_b = m_spSlotRuleData->ParseWithMyParse(g_strSlotBetData);
	if( !l_b )
	{
		UT::ErrorMsg(g_strSlotBetData,"parse file failed");
	}
	cNodeISAX	l_NodeISAX;
	l_b = l_NodeISAX.ParseDataIntoXMLNode(SPIN_PHASE_DATA);
	if( l_b )
	{
		TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
		//const WCHAR*l_strUIMPDIFileName = l_pElement->Attribute(L"UIMPDIFileName");
		const WCHAR*l_strUIMPDIName = l_pElement->Attribute(L"UIMPDIName");

		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
		Vector4	l_vFrameRect = m_spSlotRuleData->m_vShowRect;
		cMPDI*l_pSpinObject = l_pMPDIList->GetObject(SPIN_OBJECT);
		cSubMPDI*l_pSubMPDISpinObject = l_pSpinObject->GetObject(0);//SPIN_OBJECT
		int	l_iCount = (int)l_pSubMPDISpinObject->GetOriginalPointList().size();
		int	l_iChildElementCount = l_pElement->ChildElementCount();
		assert(l_iChildElementCount == l_iCount && "daya count is not match");
		assert(l_iCount == m_spSlotRuleData->m_TableCount.x&&"line data count is not match!!BetRule.xml");
		//avoid data is not match
		TiXmlElement*l_pChildElement = l_pElement->FirstChildElement();
		cPuzzleImage*l_pPI = cGameApp::GetPuzzleImageByFileName(L"JewelsKing/ImageData/SpinObject.pi");
		for( int i=0;i<m_spSlotRuleData->m_TableCount.x;++i )
		{
			assert(!wcscmp(l_pChildElement->Value(),L"SpinData"));
			float	l_fTime = 2.f;
			float	l_fDistance = 3950.f;
			const WCHAR*l_strSpinTime = l_pChildElement->Attribute(L"Time");
			const WCHAR*l_strSpinDistance = l_pChildElement->Attribute(L"SpinDistance");
			if( l_strSpinTime )
				l_fTime = (float)_wtof(l_strSpinTime);
			if( l_strSpinDistance )
				l_fDistance = (float)_wtof(l_strSpinDistance);
			Vector3	l_vPos = Vector3::Zero;
			if( (int)l_pSubMPDISpinObject->GetOriginalPointList().size() > i )
				l_vPos = l_pSubMPDISpinObject->GetOriginalPointList()[i];
			l_vFrameRect.x = l_vPos.x;
			l_vFrameRect.y = l_vPos.y;
			cSlotObjectManager*l_pSlotLineObjectManager = new cSlotObjectManager(l_vFrameRect,l_fTime,l_fDistance,l_pPI,m_spSlotRuleData->m_TableCount.y,l_pPI->Count());
			m_spSlotLineObjectManagerList->AddObjectNeglectExist(l_pSlotLineObjectManager);
			//SlotLineData	l_SlotLineData = m_spSlotRuleData->m_SlotLineDataList[i];
			l_pChildElement = l_pChildElement->NextSiblingElement();
		}
	}
	m_spBetBehavior->Init();
}
namespace IOSM
{
	extern unsigned char g_ucOutCount;
}

void	cEngineTestApp::LoadSound(char*e_strFileName)
{
	bool	l_b = cGameApp::m_spSoundParser->Parse(e_strFileName);
	//cGameApp::m_spSoundParser->GetObject(NORMAL_BGM_SOUND_NAME)->Play(true);
	//cGameApp::m_spSoundParser->SetVolume(0.01f);//>GetObject(NORMAL_BGM_SOUND_NAME)->SetVolume(0.1f);
}

void	cEngineTestApp::LoadIOData(char*e_strFileName)
{
	if( g_pIOSMDriverInterface )
	{
		g_pIOSMDriverInterface->WriteOutputData(LIGHT_FLASH_ID,false);
		return;
	}
	if( !m_spControlPanel )
		return;
	cEventVariable*l_pLatest_ToastCoin = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ToastCoin");
	cEventVariable*l_pLatest_RefundCoin = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_RefundCoin");
	cEventVariable*l_pLatest_ExchangeIn = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ExchangeIn");
	cEventVariable*l_pLatest_ExchangeOut = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ExchangeOut");

	cEventVariable*l_pHistory_ExchangeIn = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"History_ExchangeIn");
	cEventVariable*l_pHistory_ExchangeOut = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"History_ExchangeOut");
	cEventVariable*l_pHistory_ToastCoin = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"History_ToastCoin");
	cEventVariable*l_pHistory_RefundCoin = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"History_RefundCoin");
	

	cEventVariable*l_pCurrentShift_ToastCoin   = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CurrentShift_ToastCoin");
	cEventVariable*l_pCurrentShift_RefundCoin  = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CurrentShift_RefundCoin");
	cEventVariable*l_pCurrentShift_ExchangeIn  = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CurrentShift_ExchangeIn");
	cEventVariable*l_pCurrentShift_ExchangeOut = m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CurrentShift_ExchangeOut");
	if( cGameApp::m_sbDeviceExist )
	{
		g_pIOSMDriverInterface = new cIOSMDriverInterface;
		for( int i=0;i<g_ucOutCount;++i )
			g_pIOSMDriverInterface->WriteOutputData(i,false);		
		if( g_pIOSMDriverInterface&&cGameApp::m_sbDeviceExist )
		{
			g_pIOSMDriverInterface->Init();
			bool	l_b = g_pIOSMDriverInterface->Parse(e_strFileName);
			if( l_b )
			{
				int	l_iCount = g_pIOSMDriverInterface->Count();
			}
		}
		if( cGameApp::m_sbDeviceExist )
		{
			cIOSMIOData*l_pCoinToastIOSMIOData = g_pIOSMDriverInterface->GetObject(L"CoinToast");
			cIOSMIOData*l_pMotorIOSMIOData = g_pIOSMDriverInterface->GetObject(L"Motor");
			cIOSMIOData*l_pCoinRefoundIOSMIOData = g_pIOSMDriverInterface->GetObject(L"CoinRefound");
			cIOSMIOData*l_pRefoundIOSMIOData = g_pIOSMDriverInterface->GetObject(L"Refound");

			g_pToastCoinDevice = new cToastCoinDevice(10,*l_pCoinToastIOSMIOData->GetKeyData(),l_pLatest_ToastCoin,l_pCurrentShift_ToastCoin,l_pHistory_ToastCoin);
			g_pRefundingCoinDevice = new cRefundingCoinDevice(*l_pRefoundIOSMIOData->GetKeyData(),*l_pCoinRefoundIOSMIOData->GetKeyData(),
				l_pMotorIOSMIOData,g_pToastCoinDevice,l_pLatest_RefundCoin,l_pCurrentShift_RefundCoin,l_pHistory_RefundCoin);
		}
		cIOSMIOData*l_pExchangeIn = g_pIOSMDriverInterface->GetObject(L"ExchangeIn");
		cIOSMIOData*l_pExchangeOut = g_pIOSMDriverInterface->GetObject(L"ExchangeOut");
		g_pExchangeMoneyInDevice = new cExchangeMoneyDevice(100,*l_pExchangeIn->GetKeyData(),l_pLatest_ExchangeIn,l_pCurrentShift_ExchangeIn,l_pHistory_ExchangeIn);
		g_pExchangeMoneyOutDevice = new cExchangeMoneyDevice(-100,*l_pExchangeOut->GetKeyData(),l_pLatest_ExchangeOut,l_pCurrentShift_ExchangeOut,l_pHistory_ExchangeOut);
	}
	else
	{
		if( !g_pExchangeMoneyInDevice )
		{
			g_pExchangeMoneyInDevice = new cExchangeMoneyDevice(100,'T',l_pLatest_ExchangeIn,l_pCurrentShift_ExchangeIn,l_pHistory_ExchangeIn);
			g_pExchangeMoneyOutDevice = new cExchangeMoneyDevice(-100,'Y',l_pLatest_ExchangeOut,l_pCurrentShift_ExchangeOut,l_pHistory_ExchangeOut);
			g_pToastCoinDevice = new cToastCoinDevice(10,'U',l_pLatest_ToastCoin,l_pCurrentShift_ToastCoin,l_pHistory_ToastCoin);
			g_pRefundingCoinDevice = new cRefundingCoinDevice('O','E',0,g_pToastCoinDevice,l_pLatest_RefundCoin,l_pCurrentShift_RefundCoin,l_pHistory_RefundCoin);
		}
	}
	if( g_pIOSMDriverInterface )
		g_pIOSMDriverInterface->WriteOutputData(LIGHT_FLASH_ID,false);
}

void	cEngineTestApp::LoadResolutionChanger(char*e_strFileName)
{
	cNodeISAX	l_NodeISAX;
	if(l_NodeISAX.ParseDataIntoXMLNode(e_strFileName))
	{
		TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
		const WCHAR*l_strViewRect = l_pTiXmlElement->Attribute(L"ViewRect");
		g_vViewRect = GetVector4(l_strViewRect);
		g_pFrameBuffer = new cFrameBuffer((int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y);
	}
}

void	cEngineTestApp::PhaseSetup()
{
	m_spPhaseManager->Destroy();
	cSpinPhase*l_pSpinPhase = new cSpinPhase();
	cBetPhase*l_pBetPhase = new cBetPhase();
	cCalculateScorePhase*l_pCalculateScorePhase = new cCalculateScorePhase();
	cExtraGamePhase*l_pFreeGamePhase = new cExtraGamePhase();
	//do not call init to avoid into free game and else
	//l_pFreeGamePhase->Init();
	this->m_spPhaseManager->AddObjectNeglectExist(l_pBetPhase);
	this->m_spPhaseManager->AddObjectNeglectExist(l_pSpinPhase);
	this->m_spPhaseManager->AddObjectNeglectExist(l_pFreeGamePhase);
	this->m_spPhaseManager->AddObjectNeglectExist(l_pCalculateScorePhase);
	m_spPhaseManager->SetCurrentCurrentPhase(l_pBetPhase->GetName());
}

WCHAR*g_strAdmitMenuName = L"AuditPassword";
WCHAR*g_strVendorMenuName = L"VendorPassword";


void	cEngineTestApp::LoadGame()
{
	ReleaseGame();
	//
	if( !m_spRestFreeGameTimes )
		m_spRestFreeGameTimes = new std::vector<int>;
	LoadGameSetup();
	this->m_sTimeAndFPS.Update();
	LoadSound(SOUND_FIL_ENAME);
	LoadIOData("JewelsKing/KingGemsIOData.xml");
	if( m_spGlyphFontRender )
		this->m_spGlyphFontRender->SetFontColor(Vector4(1,1,0,1));
	cEngineTestApp::m_spExtraBonusMoneyRate = new std::vector<int>;
	//================================================	
	SetupSpinData();
	m_spKing = new cKing();
	m_spKing->Init();
	m_spbForceIntoFreeGame = new bool[eFGL_MAX+1];
	memset(m_spbForceIntoFreeGame,0,sizeof(bool)*eFGL_MAX);
	//================================================
	m_OrthogonalCamera.SetResolution(Vector2(1920,1080));
	m_OrthogonalCamera.SetViewRect(Vector4(-1920,-1080,1920,1080));
	//================================================
	LoadResolutionChanger("ResolutionChanger.xml");
	//================================================
	ControlPanelSetup();
	this->m_spLogFile->WriteToFileImmediatelyWithLine("Network processed...",true);
	m_spNetwork = 0;
	m_spNetwork = new cGameNetwork();	
	this->m_spLogFile->WriteToFileImmediatelyWithLine("Network parse file...",true);
	if( m_spNetwork )
		m_spNetwork->ParseFile();
	this->m_spLogFile->WriteToFileImmediatelyWithLine("Network connecting test...",true);
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("load game OK!",true);
	//================================================
	PhaseSetup();
	this->m_sTimeAndFPS.Update();
}

void	cEngineTestApp::ControlPanelSetup()
{
	if( !m_spControlPanel )
		return;
	g_pExchangeMoneyInDevice->m_iMoney = this->m_spControlPanel->m_iExchangeIn;
	g_pExchangeMoneyOutDevice->m_iMoney = this->m_spControlPanel->m_iExchangeOut;
	g_pToastCoinDevice->m_iCoinForPlayerMoney = this->m_spControlPanel->m_iCoinToScore;
	//this->m_spControlPanel->m_bLuckyTime;
}