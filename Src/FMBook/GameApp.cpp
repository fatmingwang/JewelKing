#include "stdafx.h"
#include "GameApp.h"

//for editor,event object status
cEventObject*							g_pEditorEventObject = 0;

cEventManager*							cFMBookApp::m_spEventManager = 0;
cEventPackageRunner*					cFMBookApp::m_spEventPackageRunner = 0;
cFMBookApp*								g_pFMBookApp = 0;
sFMBookEditorData*						g_pFMBookEditorData = 0;

#ifdef WIN32
//DEPEND ON WHAT WILL U DO FOR THE HWND.
//cFMBookApp::cFMBookApp(HWND e_Hwnd):cGameApp(0)
cFMBookApp::cFMBookApp(HWND e_Hwnd):cGameApp(e_Hwnd)
#else
cFMBookApp::cFMBookApp():cGameApp(Vetor2(728,1024),Vetor2(728,1024))
#endif
{
	g_pFMBookApp = this;
#ifdef WIN32
	this->m_sbSpeedControl = true;
#endif
	m_bLeave = false;
	cFMBookApp::m_spEventManager = new cEventManager();
	cFMBookApp::m_spEventManager->m_pEventFont = new cGlyphFontRender("Font");
	if( !cFMBookApp::m_spEventManager->m_pEventFont->GetFontImage() )
	{
		SAFE_DELETE(cFMBookApp::m_spEventManager->m_pEventFont);
	}
	m_spEventPackageRunner = cFMBookApp::m_spEventManager->m_pEventPackageRunner;
	g_pFMBookEditorData = new sFMBookEditorData();
}

cFMBookApp::~cFMBookApp()
{
	SAFE_DELETE(g_pFMBookEditorData);
	SAFE_DELETE(m_spEventManager);
}

void	cFMBookApp::Init()
{
	cGameApp::Init();
	cGameApp::m_sTimeAndFPS.Update();
	srand(cGameApp::m_sTimeAndFPS.uiCurrentTime);
}

void	cFMBookApp::Destroy()
{
	 m_spAnimationParser->Destroy();
	 m_spSoundParser->Destroy();
	 m_spPaticleManager->Destroy();
}

void    cFMBookApp::SwitchMode(bool e_bReader)
{

}


void	cFMBookApp::Update(float e_fElpaseTime)
{
    cGameApp::Update(e_fElpaseTime);
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->Update(e_fElpaseTime);
	}
	else
    if( m_spEventPackageRunner )
        m_spEventPackageRunner->Update(e_fElpaseTime);
}

void	cFMBookApp::Render()
{
	glViewport(0,0,(int)this->m_svViewPortSize.x,(int)m_svViewPortSize.y);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//Vector4 l_vBGColor = g_pCurrentFMPageDataList->GetBGColor();
	//glClearColor( l_vBGColor.x,l_vBGColor.y,l_vBGColor.z,l_vBGColor.w );
	glClearColor( 0,0,0,1 );
	glClearDepth(1.0f);	
	//glAlphaFunc(GL_GREATER,0.001f);	
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->Render();
	}
	else
    if( m_spEventPackageRunner )
        m_spEventPackageRunner->Render();
	glEnable(GL_TEXTURE_2D);
	float	l_fVP[16];
	glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y,l_fVP);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	cGameApp::Render();
	if( cGameApp::m_spGlyphFontRender )
	{
	    WCHAR   l_str[MAX_PATH];
	    swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	    cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
		cGameApp::m_spGlyphFontRender->RenderFont(0,150,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
	}
	this->m_spPhaseManager->DebugRender();
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->DebugRender();
	}
	glDisable2D();
#ifdef WIN32
	SwapBuffers(cGameApp::m_sHdc);
#endif
}

void	cFMBookApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
    if( this->m_spEventPackageRunner )
    {
        this->m_spEventPackageRunner->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
    }
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->MouseDown(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}

void	cFMBookApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
    if( this->m_spEventPackageRunner )
    {
		this->m_spEventPackageRunner->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
    }
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->MouseMove(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}

void	cFMBookApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
    if( this->m_spEventPackageRunner )
    {
        this->m_spEventPackageRunner->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
    }
	if( g_pEditorEventObject )
	{
		g_pEditorEventObject->MouseUp(cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
	}
}