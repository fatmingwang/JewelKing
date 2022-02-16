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
#include "Light.h"
#include "GameError.h"
//en 
WCHAR*	RESULT_LINES_MPDILIST_FILE_NAME					= L"JewelsKing/ImageData_EN/ResultLines.mpdi";
WCHAR*	JEWELS_RAIN__MPDILIST_FILE_NAME					= L"JewelsKing/ImageData_EN/jewelrain.mpdi";
WCHAR*	UI_MPDILIST_FILE_NAME							= L"JewelsKing/ImageData_EN/UI.mpdi";
WCHAR*	NG_MPDILIST_NAME								= L"JewelsKing/ImageData_EN/NG.mpdi";
WCHAR*	ANIMATED_MOVE_SOIN_OBJECT_MPDILIST_FILE_NAME	= L"JewelsKing/ImageData_EN/AnimatedMoveSpinObject.mpdi";
WCHAR*	WIN_MONEY_SPIN_OBJECT_MPDI_MPDILIST_FILE_NAME	= L"JewelsKing/ImageData_EN/SB1.mpdi";
WCHAR*	KING_MPDI_MPDILIST_FILE_NAME					= L"JewelsKing/ImageData_EN/king.mpdi";
WCHAR*	FREE_GAME_MINING_MPDILIST_FILE_NAME				= L"JewelsKing/ImageData_EN/freegame2.mpdi";
WCHAR*	FREE_GAME_BONUS_MPDILIST_FILE_NAME				= L"JewelsKing/ImageData_EN/FG.mpdi";
WCHAR*	FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME			= L"JewelsKing/ImageData_EN/Freegametimes.mpdi";
WCHAR*	FREE_GAME_JP_MPDILIST_FILE_NAME					= L"JewelsKing/ImageData_EN/Jackpot.mpdi";
WCHAR*	BIG_WIN_MPDILIST_FILE_NAME						= L"JewelsKing/ImageData_EN/BigWin.mpdi";
char*	COIN_FOUNTAIN									=  "JewelsKing/ImageData_EN/Coin2.prt";

char*	g_strSlotBetData = 0;

//#include "BaseExtraGame.h"
//#include "2DCollisionTest.h"

bool	g_bPause = false;
bool	g_bRenderScreen = true;

//==========ignore fatming.
bool	g_bParseFile = false;

cSlotObjectManagerList*						cEngineTestApp::m_spSlotLineObjectManagerList = 0;
cSlotRuleData*								cEngineTestApp::m_spSlotRuleData = 0;
cBetBehavior*								cEngineTestApp::m_spBetBehavior = 0;
cPlayerData*								cEngineTestApp::m_spPlayerData = 0;
cKing*										cEngineTestApp::m_spKing = 0;
int											cEngineTestApp::m_siFreeGame = 0;
bool										cEngineTestApp::m_sbFreeToPlay = false;
bool										cEngineTestApp::m_sbOpenNetWork = false;
bool*										cEngineTestApp::m_spbForceIntoFreeGame = 0;
std::vector<int>*							cEngineTestApp::m_spExtraBonusMoneyRate = 0;
std::vector<int>*							cEngineTestApp::m_spRestFreeGameTimes = 0;
int											cEngineTestApp::m_siNumLineBetable = 25;
std::wstring*								cEngineTestApp::m_spstrFreeGameTest = 0;
eLanguage									cEngineTestApp::m_seLanguage = e_TCN;
int											cEngineTestApp::m_siExchangeRate = 1;
int											cEngineTestApp::m_siErrorMsg = ERROR_CODE_NO_ERROR;
cControlPanel*								cEngineTestApp::m_spControlPanel = 0;
cGameNetwork*								cEngineTestApp::m_spNetwork = 0;


cFrameBuffer*								g_pFrameBuffer = 0;
//for limimt viewport
Vector4										g_vViewRect;
POINT										g_TouchPoint = {0,0};
extern cImageButton*						g_pAutoSpinButton;
extern cClickMouseBehavior*					g_pExchangeRateButton;


cToastCoinDevice*g_pToastCoinDevice = 0;
cRefundingCoinDevice*g_pRefundingCoinDevice = 0;
cExchangeMoneyDevice*g_pExchangeMoneyInDevice = 0;
cExchangeMoneyDevice*g_pExchangeMoneyOutDevice = 0;
cGameMoneyLimitError*g_pGameMoneyLimitError = 0;
WCHAR	g_strLoading[MAX_PATH];

#ifdef WIN32
//DEPEND ON WHAT WILL U DO FOR THE HWND.
//cEngineTestApp::cEngineTestApp(HWND e_Hwnd):cGameApp(0)
cEngineTestApp::cEngineTestApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cEngineTestApp::cEngineTestApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	m_bLeave = false;
	m_spstrFreeGameTest = new std::wstring;
	this->m_sbSpeedControl = true;
	m_spNetwork = 0;
}

void	cEngineTestApp::ReleaseGame()
{
	this->m_spLogFile->WriteToFileImmediatelyWithLine("clean resource",true);
	SAFE_DELETE(m_spExtraBonusMoneyRate);
	SAFE_DELETE(m_spRestFreeGameTimes);
	SAFE_DELETE(m_spSlotLineObjectManagerList);
	SAFE_DELETE(m_spSlotRuleData);
	SAFE_DELETE(m_spPlayerData);
	SAFE_DELETE(m_spBetBehavior);
	SAFE_DELETE(m_spKing);
	//
	SAFE_DELETE(m_spbForceIntoFreeGame);
	//
	SAFE_DELETE(m_spNetwork);
	SAFE_DELETE(g_pFrameBuffer);
	SAFE_DELETE(g_strSlotBetData);
	this->m_spAnimationParser->Destroy();
	m_siExchangeRate = 1;
	m_siFreeGame = 0;
	m_sbFreeToPlay = false;
	m_siExchangeRate = 1;
	m_siErrorMsg = ERROR_CODE_NO_ERROR;

}

cEngineTestApp::~cEngineTestApp()
{
	SAFE_DELETE(m_spNetwork);
	SAFE_DELETE(m_spstrFreeGameTest);
	SAFE_DELETE(g_pToastCoinDevice);
	SAFE_DELETE(g_pExchangeMoneyInDevice);
	SAFE_DELETE(g_pExchangeMoneyOutDevice);
	SAFE_DELETE(g_pRefundingCoinDevice);
	SAFE_DELETE(g_pIOSMDriverInterface);
	ReleaseGame();
	SAFE_DELETE(m_spControlPanel);
}

void	cEngineTestApp::Init()
{
	this->m_spLogFile->WriteToFileImmediatelyWithLine("GameInit start",true);
	cGameApp::Init();
	this->m_spLogFile->WriteToFileImmediatelyWithLine("Control Panel data processed...",true);
	m_spControlPanel = new cControlPanel("JewelsKing/ControlPanel/EventPachages.xml");
	if( m_spControlPanel )
	{
		m_spControlPanel->AddMenuData('Q',L"AuditPassword");
		m_spControlPanel->AddMenuData('W',L"VendorPassword");
	}
	this->m_spLogFile->WriteToFileImmediatelyWithLine("Load Game processed...",true);
	LoadGame();
}

void	cEngineTestApp::Update(float e_fElpaseTime)
{
	if( m_sucKeyData['P'] )
		this->m_spLogFile->WriteToFileImmediatelyWithLine("GamePaused!!",true);
	if( g_bPause )
		e_fElpaseTime = 0.f;
	const float	l_fAllowMaxmimElpaseTime = 1/30.f;
	//make sure game working smoothly...
	if( e_fElpaseTime > l_fAllowMaxmimElpaseTime )
	{
		//e_fElpaseTime = l_fAllowMaxmimElpaseTime;
	}
	if( g_pIOSMDriverInterface )
	{
		g_pIOSMDriverInterface->Update(e_fElpaseTime);
	}
	bool	l_bControlPanel = false;
	if( m_spControlPanel )
	{
		l_bControlPanel = m_spControlPanel->IsInControlPanelMode();
		m_spControlPanel->Update(e_fElpaseTime);
	}
	if( l_bControlPanel )
	{
		if(m_spNetwork)
			m_spNetwork->CloseSocket();
		//because controls panel need to reset player money so close it
		this->m_spPlayerData->ClosePlayerMoney();
		if(!m_spControlPanel->IsInControlPanelMode())
		{
			if( m_spControlPanel->IsReloadGame() )
			{
				cEngineTestApp::m_siFreeGame = 0;
				m_sbFreeToPlay = false;
				m_siFreeGame = 0;
				LoadGame();
				g_pRefundingCoinDevice->Init();
			}
			else
			{
				//for the reason ,because controls panel need to reset player money so close it
				this->m_spPlayerData->OpenPlayerMoney();
				ControlPanelSetup();
				cLight::TurnLight(false);
				if(!wcscmp(this->m_spPhaseManager->GetCurrentPhaseName(),BET_PHASE_NAME))
				{
					cLight::TurnBetPhaseLight(true);
				}
			}
		}
	}
	else
	{
		if( g_pGameMoneyLimitError )
			g_pGameMoneyLimitError->Update(e_fElpaseTime);
		if( m_spNetwork )
			m_spNetwork->Update(e_fElpaseTime);
		this->m_spPlayerData->Update(e_fElpaseTime);
		if( !m_spKing )
			LoadGame();
		if(g_pExchangeMoneyInDevice)
		{
			g_pExchangeMoneyOutDevice->Update(e_fElpaseTime);
			g_pExchangeMoneyInDevice->Update(e_fElpaseTime);
		}
		if( g_pToastCoinDevice )
			g_pToastCoinDevice->Update(e_fElpaseTime);
		if(g_pRefundingCoinDevice)
			g_pRefundingCoinDevice->Update(e_fElpaseTime);
		if( m_spPhaseManager->GetCurrentPhaseName() )
		{
			if(wcscmp(this->m_spPhaseManager->GetCurrentPhaseName(),BET_PHASE_NAME))
			{
				if( g_pAutoSpinButton )
				{
					g_pAutoSpinButton->SingnalProcess();
					g_pAutoSpinButton->Update(e_fElpaseTime);
					if( g_pAutoSpinButton->GetMouseBehavior() != eOMB_NONE )
					{
						cBetPhase*l_pBetPhase = (cBetPhase*)this->m_spPhaseManager->GetObject(BET_PHASE_NAME);
						l_pBetPhase->SetAutoSpin( false );
						g_pAutoSpinButton->Init();
					}
				}
			}
		}
		if( m_spKing )
			m_spKing->Update(e_fElpaseTime);
		cGameApp::Update(e_fElpaseTime);
#ifdef _FUCK_
		if( this->m_spPhaseManager->GetCurrentPhaseName() )
		if(wcscmp(this->m_spPhaseManager->GetCurrentPhaseName(),EXTRA_GAME_PHASE_NAME))
		{
			if( g_pIOSMDriverInterface&&cGameApp::m_sbDeviceExist )
			{
				if( this->m_sucKeyData['S'] )
				{
					if( this->m_sucKeyData['C'] )
					{
						*m_spstrFreeGameTest = L"JP1";
					}
					else
					if( this->m_sucKeyData['V'] )
					{
						*m_spstrFreeGameTest = L"JP2";
					}
					else
					if( this->m_sucKeyData['B'] )
					{
						*m_spstrFreeGameTest = L"Free";
					}			
				}
			}
			else
			{
				if( this->m_sucKeyData['1'] )
				{
					//m_spNetwork->RemoveAllClient();
					*m_spstrFreeGameTest = L"JP1";
				}
				if( this->m_sucKeyData['2'] )
				{
					*m_spstrFreeGameTest = L"JP2";
				}
				else
				if( this->m_sucKeyData['3'] )
				{
					*m_spstrFreeGameTest = L"Free";
				}
				else
				if( this->m_sucKeyData['4'] )
				{
					//this->m_spPlayerData->m_iMoney += 1000;
				}
			}
		}
#endif
		//MyUpdate(e_fElpaseTime);
	}
}

extern POINT g_MousePosition;

void	cEngineTestApp::Render()
{
	//if( !g_bRenderScreen )
	//	return;
	glViewport(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0,0,1 );
	glClearDepth(1.0f);	
	glEnable(GL_TEXTURE_2D);
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.001f);
	//m_OrthogonalCamera.Render();
	if( m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->Render();
	}
	else
	{
		if( g_pFrameBuffer )
			g_pFrameBuffer->StartDraw();
		cGameApp::Render();
		if( !this->m_sbFreeToPlay && m_spKing )
			this->m_spKing->RenderPayRateHint();
		if( m_spSlotLineObjectManagerList )
			this->m_spSlotLineObjectManagerList->DoScissor();
		if( m_spKing )
			this->m_spKing->RenderDownScreenEffect();
		if( m_spSlotLineObjectManagerList )
			this->m_spSlotLineObjectManagerList->UnDoScissor();
		if( m_spKing )
		{
			this->m_spKing->RenderGameMessage();
			this->m_spKing->RenderExplosionCombo();
			this->m_spKing->RenderPerLineBetMoney();
		}
		const WCHAR*l_strPhaseName = this->m_spPhaseManager->GetCurrentPhaseName();
		if( l_strPhaseName&&!wcscmp(l_strPhaseName,BET_PHASE_NAME) )
		{
			cBetPhase*l_pBetPhase = (cBetPhase*)m_spPhaseManager->GetObject(BET_PHASE_NAME);
			l_pBetPhase->RenderReadMeMenu();
		}
		if( g_pExchangeRateButton )
		{
			if(wcscmp(this->m_spPhaseManager->GetCurrentPhaseName(),BET_PHASE_NAME))
				g_pExchangeRateButton->Render();
		}
		if(wcscmp(this->m_spPhaseManager->GetCurrentPhaseName(),BET_PHASE_NAME))
		{
			if( g_pAutoSpinButton )
			{
				cBetPhase*l_pBetPhase = (cBetPhase*)m_spPhaseManager->GetObject(BET_PHASE_NAME);
				g_pAutoSpinButton->Render();
				if(l_pBetPhase->IsAutoSpin() && g_pAutoSpinButton->GetMouseBehavior() == eOMB_NONE )
				{
					g_pAutoSpinButton->SetColor(Vector4::One);
					g_pAutoSpinButton->Render();
				}
				else
				{
					cSubMPDI*l_pPressedImage = dynamic_cast<cSubMPDI*>(g_pAutoSpinButton->GetPressedImage());
					l_pPressedImage->GetPointData(0)->vColor = Vector4::One;
					l_pPressedImage->Render();
				}
			}
		}
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
#ifdef _DEBUG
		std::wstring	l_strJP = L"JP:";
		l_strJP += ValueToStringW(*this->m_spPlayerData->GetJP(eJPT_NETWORK));
		//cGameApp::m_spGlyphFontRender->RenderFont(350,50,l_strJP.c_str());
		if(m_spNetwork)
		{
			if(this->m_spNetwork->IsConnected())
				cGameApp::m_spGlyphFontRender->RenderFont(350,100,L"Connected");
			else
				cGameApp::m_spGlyphFontRender->RenderFont(350,100,L"Connected failed");
		}
#endif
		if( g_pGameMoneyLimitError )
			g_pGameMoneyLimitError->Render();
		if(g_pRefundingCoinDevice)
			g_pRefundingCoinDevice->Render();
	}
#ifdef _DEBUG
		if( cGameApp::m_spGlyphFontRender )
		{
			WCHAR   l_str[MAX_PATH];
			swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
			cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
			swprintf(l_str,MAX_PATH,L"ViewPortMousePos:%d,%d",g_MousePosition.x,g_MousePosition.y);
			//cGameApp::m_spGlyphFontRender->RenderFont(200,300,l_str);
			m_spGlyphFontRender->SetScale(1.f);
			cGameApp::m_spGlyphFontRender->RenderFont(0,50,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
			swprintf(l_str,MAX_PATH,L"TouchPoint:%d,%d",g_TouchPoint.x,g_TouchPoint.y);
			//cGameApp::m_spGlyphFontRender->RenderFont(0,100,l_str);
			//cGameApp::m_spGlyphFontRender->RenderFont(200,20,ValueToStringW(this->m_siFreeGame));
		}
#endif
	if( g_pFrameBuffer )
	{
		g_pFrameBuffer->EndDraw();
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		glViewport((int)g_vViewRect.x,(int)g_vViewRect.y,(int)g_vViewRect.z,(int)g_vViewRect.w);
		POINT	l_vPos = {0,0};
		POINT	l_vSize = {(int)cGameApp::m_svGameResolution.x,(int)cGameApp::m_svGameResolution.y};
		g_pFrameBuffer->DrawBuffer(l_vPos,l_vSize);
	}

	glDisable(GL_TEXTURE_2D);
	this->m_spPhaseManager->DebugRender();
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

float	g_fOriginalViewPort = 0;
void	TouchPointConvert(int *e_piPosX,int* e_piPosY)
{
	if(g_pFrameBuffer)
	{
		g_fOriginalViewPort = cGameApp::m_svViewPortSize.x;
		if( *e_piPosX>g_vViewRect.x&&*e_piPosX<(g_vViewRect.z+g_vViewRect.x) )
		{
			float	l_fWidth = g_vViewRect.z;
			cGameApp::m_svViewPortSize.x = l_fWidth;
			*e_piPosX -= (int)g_vViewRect.x;
			g_TouchPoint.x = *e_piPosX;
			g_TouchPoint.y = *e_piPosY;
		}
		else
		{
			*e_piPosX= 0;
			*e_piPosY = 0;
		}
	}
}

void	TouchEnd()
{
	if(g_pFrameBuffer)
	{
		cGameApp::m_svViewPortSize.x = g_fOriginalViewPort;
	}
}

void	cEngineTestApp::MouseDown(int e_iPosX,int e_iPosY)
{
	TouchPointConvert(&e_iPosX,&e_iPosY);
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	TouchEnd();
	if( m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}

void	cEngineTestApp::MouseMove(int e_iPosX,int e_iPosY)
{
	TouchPointConvert(&e_iPosX,&e_iPosY);
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	TouchEnd();
	if( m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}

void	cEngineTestApp::MouseUp(int e_iPosX,int e_iPosY)
{
	TouchPointConvert(&e_iPosX,&e_iPosY);
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	TouchEnd();
	if( m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}


void	cEngineTestApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	if( e_char == 'P' )
	{
		g_bPause = !g_bPause;
	}
	if( e_char == 13 )
	{
		g_bRenderScreen = !g_bRenderScreen;
	}
	else
	{
	
	}
}

void	cEngineTestApp::KeyDown(char e_char)
{
	cGameApp::KeyDown(e_char);
}

void	cEngineTestApp::WriteDataIntoControlPanel()
{
	m_spControlPanel->SaveData();
}