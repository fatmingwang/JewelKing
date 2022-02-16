/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "GameApp.h"
#include "MiniGameBase.h"
#include "MonsterBase.h"
#include "PlayerBehaviorBase.h"
#include "Bullet.h"
#include "FishProbability.h"
#include "RefundCoinDevice.h"
#include "SceneChange.h"
#include "TestLogSetup.h"
//#include "2DCollisionTest.h"

UINT	g_uiFrame = 0;
float					cFishApp::m_sfVersion = 1.0f;
cMiniGameManager*		cFishApp::m_spMiniGameManager = 0;
cMonsterManager*		cFishApp::m_spMonsterManager = 0;
cPlayerManager*			cFishApp::m_spPlayerManager = 0;
cBulletManager*			cFishApp::m_pBulletManager = 0;
cControlPanel*			cFishApp::m_spControlPanel = 0;
cFishProbability*		cFishApp::m_spFishProbability = 0;
cSceneChange*			cFishApp::m_spSceneChange = 0;
cTestLogFile*			cFishApp::m_spTestLogFile = 0;
bool					cFishApp::m_sbSceneChange = false;
float					cFishApp::m_sfSceneChangeSpeed = 1.f;
UINT64					cFishApp::m_sui64CurrentStep = 0;
bool					cFishApp::m_sbSchoolerMasterLiProbability = false;

#ifdef WIN32
//DEPEND ON WHAT WILL U DO FOR THE HWND.
//cFishApp::cFishApp(HWND e_Hwnd):cGameApp(0)
cFishApp::cFishApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#else
cFishApp::cFishApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	//m_sbSpeedControl = true;
	cGameApp::Init();
	m_bLeave = false;
	m_pRefoundCoinDeviceManager = 0;
	if(!g_pIOSMDriverInterface)
	{
		this->m_spLogFile->WriteToFileImmediatelyWithLine(L"start init 8051 IO");
		g_pIOSMDriverInterface = new cIOSMDriverInterface();
		g_pIOSMDriverInterface->Init();
		this->m_spLogFile->WriteToFileImmediatelyWithLine(L"init 8051 IO ok");
		if(g_pIOSMDriverInterface->Parse("Fish/IO/IOData.xml"))
			this->m_spLogFile->WriteToFileImmediatelyWithLine(L"Fish/IO/IOData-file.xml ok");
		else
			this->m_spLogFile->WriteToFileImmediatelyWithLine(L"Fish/IO/IOData-file.xml falied");
	}
	if(!cGameApp::m_spSoundParser->Parse("Fish/Sound/Sound.xml"))
	{
		UT::ErrorMsg(L"Fish/Sound.xml",L"parse sound error");
	}
}

cFishApp::~cFishApp()
{
	GameDataDestroy();
	Destroy();
	SAFE_DELETE(m_spControlPanel);
	SAFE_DELETE(g_pIOSMDriverInterface);
	SAFE_DELETE(m_pRefoundCoinDeviceManager);
}

void	cFishApp::Init()
{
	m_sfSceneChangeSpeed = 1.f;
	m_sbSceneChange = false;
	cGameApp::SoundPlay(L"27",false);
	cGameApp::SoundPlay(L"28",false);
	cGameApp::SoundPlay(L"29",false);
	cGameApp::SoundPlay(L"27",true);
	cGameApp::SoundPlay(L"6",true);
	if( UT::IsFileExists("Fish/Image/Loading.png") )
	{
		UseShaderProgram();
		glViewport(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
		//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor( 0,0.5,0.5,1 );
		glClearDepth(1.0f);	
		glEnable(GL_TEXTURE_2D);
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		cBaseImage*l_pLoadingImage = new cBaseImage("Fish/Image/Loading.png");
		l_pLoadingImage->Render();
		SwapBuffers(cGameApp::m_sHdc);
		SAFE_DELETE(l_pLoadingImage);
	}
	GameDataDestroy();
	if( !m_spControlPanel )
	{
		m_spControlPanel = new cControlPanel("Fish/ControlPanel/EventPachages.xml");
		//m_spControlPanel->AddMenuData(97,L"AuditPassword");
		m_spControlPanel->AddMenuData('L',L"VendorPassword");
	}
	if( m_spControlPanel )
		cGameApp::m_spSoundParser->SetVolume(this->m_spControlPanel->m_iSound/100.f);
#define	GAME_RESOLUTION_WIDTH	1280.f
#define	GAME_RESOLUTION_HEIGHT	720.f
	//if( this->m_spControlPanel->m_iPlayerCount > 10 )
	//{
	//	this->m_svGameResolution.x = GAME_RESOLUTION_WIDTH*2;
	//	this->m_svGameResolution.y = GAME_RESOLUTION_HEIGHT;
	//}
	//else
	//{
	//	this->m_svGameResolution.x = GAME_RESOLUTION_WIDTH;
	//	this->m_svGameResolution.y = GAME_RESOLUTION_HEIGHT;	
	//}
	this->m_svGameScale.x = this->m_svGameResolution.x/GAME_RESOLUTION_WIDTH;
	this->m_svGameScale.y = this->m_svGameResolution.y/GAME_RESOLUTION_HEIGHT;
	m_spSceneChange = new cSceneChange();
	m_pBulletManager = new cBulletManager();
	m_spMiniGameManager = new cMiniGameManager();
	m_spMonsterManager = new cMonsterManager();
	m_spPlayerManager = new cPlayerManager();
	m_spFishProbability = new cFishProbability();
	m_spTestLogFile = new cTestLogFile();
	m_pRefoundCoinDeviceManager = new cRefoundCoinDeviceManager();
	//PrintMemoryInfo();
	if( m_spMiniGameManager )
		m_spMiniGameManager->Init();
	//PrintMemoryInfo();
	if( m_spMonsterManager )
		m_spMonsterManager->Init();
	//PrintMemoryInfo();
	if( m_spPlayerManager )
		m_spPlayerManager->Init();
	//PrintMemoryInfo();
	if( m_pBulletManager )
		m_pBulletManager->Init();
	//PrintMemoryInfo();
	if( m_spFishProbability )
		m_spFishProbability->Init();
	//PrintMemoryInfo();
	if( m_spSceneChange )
		m_spSceneChange->Init();
	//PrintMemoryInfo();
	if(m_pRefoundCoinDeviceManager)
		m_pRefoundCoinDeviceManager->Init(IO8051_DATA_Refound);
	//PrintMemoryInfo();
	if( m_spTestLogFile )
		m_spTestLogFile->Init();
	//PrintMemoryInfo();
	this->m_sTimeAndFPS.Update();
	g_sbCollisionRender = false;
}

void	cFishApp::GameDataDestroy()
{
	SAFE_DELETE(m_spFishProbability);
	if( m_spMiniGameManager )
	{
		m_spMiniGameManager->Destroy();
	} //end if
	SAFE_DELETE(m_spMiniGameManager);
	SAFE_DELETE(m_spMonsterManager);
	SAFE_DELETE(m_spPlayerManager);
	SAFE_DELETE(m_pBulletManager);
	SAFE_DELETE(m_pRefoundCoinDeviceManager);
	SAFE_DELETE(m_spSceneChange);
	SAFE_DELETE(m_spTestLogFile);
}

//void	cFishApp::Destroy()
//{
//#ifdef WIN32
//	Delete2DShader(L"NoTexture");
//	Delete2DShader();
//#endif
//}

enum	eDebugInfo
{
	eDI_IO = 0,
	eDI_GAMEAPP,
	eDI_SCENE_CHANGE,
	eDI_MINI_GAME,
	eDI_MONSTER,
	eDI_PLAYER,
	eDI_BULLET,
	eDI_PROBABILITY,
	eDI_REFOUND_DEVICE,
};

const int	g_iAllDebugInfo = 9;
float	g_fUpdateUsingTime[g_iAllDebugInfo];
float	g_fRenderTime[g_iAllDebugInfo];

WCHAR*g_strDebugInfo[g_iAllDebugInfo] = {L"IO",
							L"GameApp",
							L"SceneChange",
							L"MiniGameManager",
							L"MonsterManager",
							L"PlayerManager",
							L"BulletManager",
							L"FishProbability",
							L"RefoundCoinDeviceManager"};
UT::sTimeAndFPS	g_TimeStamp;

void	TimsStamp(eDebugInfo e_eDebugInfo,bool e_bUpdate,bool e_bTimeAdded = false)
{
	g_TimeStamp.Update();
	if( e_bUpdate )
	{
		cFishApp::m_sui64CurrentStep = e_eDebugInfo;
		if( e_bTimeAdded )
			g_fUpdateUsingTime[e_eDebugInfo] += g_TimeStamp.fElpaseTime;
		else
			g_fUpdateUsingTime[e_eDebugInfo] = g_TimeStamp.fElpaseTime;
		//OutputDebugString(g_strDebugInfo[e_eDebugInfo]);
		//OutputDebugString(L",");
	}
	else
	{
		cFishApp::m_sui64CurrentStep = e_eDebugInfo+100;
		if( e_bTimeAdded )
			g_fRenderTime[e_eDebugInfo] += g_TimeStamp.fElpaseTime;
		else
			g_fRenderTime[e_eDebugInfo] = g_TimeStamp.fElpaseTime;
	}
}

void	cFishApp::Update(float e_fElpaseTime)
{
	//e_fElpaseTime = 0.016f;
#ifndef _DEBUG//avoid frame skip.
	if( e_fElpaseTime>1/30.f )
		e_fElpaseTime = 1/30.f;
#endif
	g_TimeStamp.Update();
	if( g_pIOSMDriverInterface )
		g_pIOSMDriverInterface->Update(e_fElpaseTime);
	TimsStamp(eDI_IO,true);
	bool	l_bControlPanel = false;
	if( m_spControlPanel )
	{
		l_bControlPanel = m_spControlPanel->IsInControlPanelMode();
		m_spControlPanel->Update(e_fElpaseTime);
	}
	if( l_bControlPanel )
	{
		if(!m_spControlPanel->IsInControlPanelMode())
		{
			if( m_spControlPanel->IsReloadGame() )
			{
				Init();
			}
			else
			{

			}
		}
	}
	else
	{
		cGameApp::Update(e_fElpaseTime);
		if( m_spTestLogFile )
		{
			m_spTestLogFile->Update(e_fElpaseTime);
		}
		TimsStamp(eDI_GAMEAPP,true);
		if( m_spSceneChange )
			m_spSceneChange->Update(e_fElpaseTime);
		TimsStamp(eDI_SCENE_CHANGE,true);
		if( m_spMiniGameManager )
			m_spMiniGameManager->Update(e_fElpaseTime);
		TimsStamp(eDI_MINI_GAME,true);
		if( m_spMonsterManager )
			m_spMonsterManager->Update(e_fElpaseTime*m_sfSceneChangeSpeed);
		TimsStamp(eDI_MONSTER,true);
		if( m_spPlayerManager )
			m_spPlayerManager->Update(e_fElpaseTime);
		TimsStamp(eDI_PLAYER,true);
		if( m_pBulletManager )
		{
			m_pBulletManager->Update(e_fElpaseTime);
			if(m_spMonsterManager)
			{
				m_pBulletManager->Collide(m_spMonsterManager);
			}
		}
		TimsStamp(eDI_BULLET,true);
		//must update at last.
		if(m_spFishProbability)
			m_spFishProbability->Update(e_fElpaseTime);
		TimsStamp(eDI_PROBABILITY,true);
		//MyUpdate(e_fElpaseTime);
		if(m_pRefoundCoinDeviceManager)
			m_pRefoundCoinDeviceManager->Update(e_fElpaseTime);
		TimsStamp(eDI_REFOUND_DEVICE,true);
	}
	++g_uiFrame;
}

void	cFishApp::Render()
{
	glViewport(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0.5,0.5,1 );
	glClearDepth(1.0f);	
	glEnable(GL_TEXTURE_2D);
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);

	//shock camera
	//float l_fAA = frand(-2,2);
	//float l_fBB = frand(-2,2);
	//cMatrix44 l_matProjection;
	//glhOrthof2((float*)l_matProjection.m, 0, cGameApp::m_svGameResolution.x, cGameApp::m_svGameResolution.y, 0, -10000, 10000);
	//l_matProjection = l_matProjection*cMatrix44::TranslationMatrix(l_fAA,l_fBB,0);	
	//FATMING_CORE::SetupShaderViewProjectionMatrix(l_matProjection);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.001f);	
	g_TimeStamp.Update();
	if( m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->Render();
	}
	else
	{
		cGameApp::Render();
		TimsStamp(eDI_GAMEAPP,false);
		if( m_spSceneChange )
			m_spSceneChange->Render();
		TimsStamp(eDI_SCENE_CHANGE,false);
		if( m_spMiniGameManager )
			m_spMiniGameManager->Render();
		TimsStamp(eDI_MINI_GAME,false);
		if( m_spMonsterManager )
			m_spMonsterManager->Render();
		TimsStamp(eDI_MONSTER,false);
		if( m_pBulletManager )
			m_pBulletManager->Render();
		if( m_spMonsterManager )
			m_spMonsterManager->LastRender();
		TimsStamp(eDI_BULLET,false);
		if( m_spPlayerManager )
			m_spPlayerManager->Render();
		TimsStamp(eDI_PLAYER,false);
		if( m_spMiniGameManager )
			m_spMiniGameManager->Render2();
		TimsStamp(eDI_MINI_GAME,false,true);
		if(m_spFishProbability)
			m_spFishProbability->Render();
		TimsStamp(eDI_PROBABILITY,false);
		if(m_pRefoundCoinDeviceManager)
			m_pRefoundCoinDeviceManager->Render();
		TimsStamp(eDI_REFOUND_DEVICE,false);
		if(m_spSceneChange)
			m_spSceneChange->LastRender();
		TimsStamp(eDI_SCENE_CHANGE,false,true);
		glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);

		if( g_sbCollisionRender )
		{
			if(g_pIOSMDriverInterface)
			{
				//g_pIOSMDriverInterface->Render();
				TimsStamp(eDI_IO,false);
			}
			if( cGameApp::m_spGlyphFontRender )
			{
				WCHAR   l_str[MAX_PATH];
				swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
				cGameApp::m_spGlyphFontRender->RenderFont(750,0,l_str);
				cGameApp::m_spGlyphFontRender->RenderFont(1150,30,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
			}
			//if( m_spMonsterManager  )
			//	m_spMonsterManager->DebugRender();
			//if( m_pBulletManager  )
			//	m_pBulletManager->DebugRender();
			if(m_spFishProbability)
				m_spFishProbability->DebugRender();
			//if(m_spSceneChange)
			//	m_spSceneChange->DebugRender();
			//for( int i=0;i<g_iAllDebugInfo;++i )
			//{
			//	std::wstring	l_str = UT::ComposeMsgByFormat(L"%s:%.4f,%.4f",g_strDebugInfo[i],g_fUpdateUsingTime[i],g_fRenderTime[i]);
			//	cGameApp::m_spGlyphFontRender->RenderFont(150,30*i,l_str.c_str());
			//}
			//std::wstring	l_str = UT::ComposeMsgByFormat(L"Frame:%d",g_uiFrame);
			//cGameApp::m_spGlyphFontRender->RenderFont(0,650,l_str.c_str());
		}
	}
	glDisable(GL_TEXTURE_2D);
	this->m_spPhaseManager->DebugRender();
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

//void	cFishApp::Run()
//{
//	Update(g_TimeAndFPS.fElpaseTime);
//	Render();
//}

void	cFishApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
	if( m_spControlPanel&&m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		return;
	}
}

void	cFishApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
	if( m_spControlPanel&&m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		return;
	}
}

void	cFishApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
	if( m_spControlPanel&&m_spControlPanel->IsInControlPanelMode() )
	{
		m_spControlPanel->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
		return;
	}
}

void	cFishApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cFishApp::KeyUp(char e_char)
{
	//SHORT	l_ = GetAsyncKeyState('A');
	cGameApp::KeyUp(e_char);
	//std::string	l_sss = UT::ComposeMsgByFormat("KeyValue:%s,%d",&e_char,e_char);
	//std::wstring l_ss2 = UT::CharToWchar(l_sss.c_str());
	//OutputDebugString(l_ss2.c_str());
	//OutputDebugString(L"\n");
	//if( e_char == 8 )
	//{
	//	g_sbCollisionRender = !g_sbCollisionRender;
	//}
	if( cGameApp::m_sbDebugFunctionWorking )
	{
		if( e_char == 8 )
		{
			g_sbCollisionRender = !g_sbCollisionRender;
		}
		if( e_char == -36 )//107'+'
		{
			m_sfDebugValue *= 2.f;
			if( m_sfDebugValue >1073741824 )
				m_sfDebugValue  = 1073741824;
		}
		else
		if( e_char == 16 )//109'-'
		{
			m_sfDebugValue /= 2.f;	
			if( m_sfDebugValue <= 0.00000000001f )
				m_sfDebugValue = 0.00000000001f;
		}
	}
	/*if( e_char == 'W' )
	{
		static float	l_fAngle = 5.f;
		l_fAngle += 5.f;
		cBullet*l_pBullet = m_pBulletManager->Require(0,0);
		if( l_pBullet )
		{
			l_pBullet->Fire(0,Vector3(400,400,0),l_fAngle);
		}
	}*/
}