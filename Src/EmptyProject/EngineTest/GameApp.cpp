#include "stdafx.h"
#include "GameApp.h"
#include "TestPhase.h"
//#include "2DCollisionTest.h"
UT::sTimeAndFPS g_TimeAndFPS;
UT::sTimeAndFPS g_RenderFPS;
bool	g_bMultiThread = false;
//bool	g_bMultiThread = true;


//==========ignore fatming.
bool	g_bParseFile = false;
bool	g_bMultiThreadTestUpdate = true;
bool	g_bInMultiThread = false;
bool	g_bLeaveMultiThread = false;

//bool	g_bMultiThreadTestUpdate = false;
extern void	LoadSample();
extern void	DestorySampleObject();
extern void	SampleUpdate(float e_fElpaseTime);
extern void	SampleRender();


void	MyUpdate(float e_fElpaseTime)
{
	g_TimeAndFPS.Update();
}

void	MyRender()
{
	g_RenderFPS.Update();
	glViewport(0,0,(int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y);
	glScissor(0,0,(int)cGameApp::m_svViewPortSize.x,(int)cGameApp::m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0.5,0.5,1 );
	glClearDepth(1.0f);	
	glEnable(GL_TEXTURE_2D);
	float	l_fVP[16];
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.001f);	
	if( cGameApp::m_spGlyphFontRender )
	{
		//cGameApp::m_spGlyphFontRender->RenderFont(0,50,UT::CharToWchar(g_RenderFPS.GetFPS()));
		//cGameApp::m_spGlyphFontRender->RenderFont(0,150,UT::CharToWchar(g_TimeAndFPS.GetFPS()));		
		//ATG::Input::GetInput(&ATG::Input::m_Gamepads[0]);		
		//if(ATG::Input::m_Gamepads[0].bConnected)
		//{
		//    cGameApp::m_spGlyphFontRender->RenderFont(300,300,L"Connected!");
		//}
		//else
		//    cGameApp::m_spGlyphFontRender->RenderFont(300,300,L"no Connect!");
	}
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

static void DoWorkTest(size_t _workParameter, size_t _pUri)
{
	while(g_bInMultiThread)
	{
		if( g_bParseFile == false )
		{
			if( g_bMultiThreadTestUpdate )
				MyUpdate(g_TimeAndFPS.fElpaseTime);
			else
				MyRender();
		}
	}
}

static void DoneWorkTest(size_t _workParameter, size_t _pUri)
{
	g_bLeaveMultiThread = true;
}

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
	if( g_bMultiThread )
	{
		assert( g_bInMultiThread == false );
		//setup multi thread here!
		//FUStaticFunctor2<size_t, size_t, void>* workFunctor =
		//		new FUStaticFunctor2<size_t, size_t, void>(&DoWorkTest);
		//	FUStaticFunctor2<size_t, size_t, void>* doneFunctor =
		//			new FUStaticFunctor2<size_t, size_t, void>(&DoneWorkTest);
		//g_pFeelingScene->m_pFSViewer->GetThreadPool()->ExecuteWork(workFunctor,doneFunctor,0,0);
		g_bInMultiThread = true;
	}
	else
	{
		g_bLeaveMultiThread = true;
	}

	this->m_sbSpeedControl = true;
	g_TimeAndFPS.Update();
}

cEngineTestApp::~cEngineTestApp()
{
	g_bInMultiThread = false;
	if( g_bMultiThread )
	{
		Sleep(300);
		while( !g_bLeaveMultiThread )
		{
			Sleep(300);
		}
	}
	DestorySampleObject();
	Destroy();
}

void	cEngineTestApp::Init()
{
	cGameApp::Init();
	LoadSample();

	cGamePhase*l_pGamePhase = new cGamePhase();
	l_pGamePhase->SetName(L"Test");
	bool	l_b = this->m_spPhaseManager->AddObject(l_pGamePhase);
	if( !l_b )
	{
		assert(0&&"this phase has been added!");
	}
	g_TimeAndFPS.Update();




	this->m_sTimeAndFPS.Update();
	srand(g_TimeAndFPS.uiCurrentTime);
}

//void	cEngineTestApp::Destory()
//{
//#ifdef WIN32
//	Delete2DShader(L"NoTexture");
//	Delete2DShader();
//#endif
//}

void	cEngineTestApp::Update(float e_fElpaseTime)
{
	if( g_bMultiThread&&g_bMultiThreadTestUpdate )
		return;
    cGameApp::Update(e_fElpaseTime);
	SampleUpdate(e_fElpaseTime);
	//MyUpdate(e_fElpaseTime);
}

void	cEngineTestApp::Render()
{
	if( g_bMultiThread&&!g_bMultiThreadTestUpdate )
		return;
	//MyRender();
	g_RenderFPS.Update();
	glViewport(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor( 0,0.5,0.5,1 );
	glClearDepth(1.0f);	
	glEnable(GL_TEXTURE_2D);
	float	l_fVP[16];
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER,0.001f);	
	cGameApp::Render();
	SampleRender();
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	if( cGameApp::m_spGlyphFontRender )
	{
	    WCHAR   l_str[MAX_PATH];
	    swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,20,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
#ifdef WIN32
		//ATG::Input::GetInput(&ATG::Input::m_Gamepads[0]);
		//if(ATG::Input::m_Gamepads[0].bConnected)
		//{
		//    //cGameApp::m_spGlyphFontRender->RenderFont(300,300,L"Connected!");
		//    //left trigger,FX1,FX2
		//    if(ATG::Input::m_Gamepads[0].fX1<0.f)//left
		//    {
		//    
		//    }
		//    else
		//    if(ATG::Input::m_Gamepads[0].fX1>0.f)//right
		//    {
		//    
		//    }
		//    if(ATG::Input::m_Gamepads[0].fX2<0.f)//down
		//    {
		//    
		//    }
		//    else
		//    if(ATG::Input::m_Gamepads[0].fX2>0.f)//up
		//    {
		//    
		//    }
		//    //right trigger,FY1,FY2
		//    if(ATG::Input::m_Gamepads[0].fY1<0.f)//left
		//    {
		//    
		//    }
		//    else
		//    if(ATG::Input::m_Gamepads[0].fY1>0.f)//right
		//    {
		//    
		//    }
		//    if(ATG::Input::m_Gamepads[0].fY2<0.f)//down
		//    {
		//    
		//    }
		//    else
		//    if(ATG::Input::m_Gamepads[0].fY2>0.f)//up
		//    {
		//    
		//    }		    
		//}
		//else
		//    cGameApp::m_spGlyphFontRender->RenderFont(300,300,L"no Connect!");
#endif
	}
	glDisable(GL_TEXTURE_2D);
	this->m_spPhaseManager->DebugRender();
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

//void	cEngineTestApp::Run()
//{
//	Update(g_TimeAndFPS.fElpaseTime);
//	Render();
//}

void	cEngineTestApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
}

void	cEngineTestApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
}

void	cEngineTestApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
}

void	cEngineTestApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cEngineTestApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
}