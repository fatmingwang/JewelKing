#include "stdafx.h"
#include "UpperScreen.h"
#include "GameApp.h"

extern int	g_iExplosionCount;

cUpperScreen::cUpperScreen()
{
	//m_pJewelsRainsMPDI = m_pJewelsRainsMPDIList->GetObject(0);

	m_pUpperUIMPDIList = cGameApp::GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	m_pUpperUIMPDI = m_pUpperUIMPDIList->GetObject(L"UpperUI");
	cEventVariable*l_pEventVariable = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"LinePlayable");
	std::wstring	l_str1 = l_pEventVariable->ConvertToString();
	m_pUpperUIPayRateHint = 0;
	//m_pUpperUIPayRateHint = m_pUpperUIMPDIList->GetObject(L"UIsb");
	m_pUpperUIMPDI->Init();
	m_pUpperUIMPDI->Update(0.1f);
	if( m_pUpperUIPayRateHint )
	{
		m_pUpperUIPayRateHint->Init();
		m_pUpperUIPayRateHint->Update(0.1f);
	}

	m_pNGMPDIList = cGameApp::GetMPDIListByFileName(NG_MPDILIST_NAME);
	m_pReelMPDI = m_pNGMPDIList->GetObject(REEL_MPDI_NAME);
	m_pReelMPDI->Init();
	m_pReelMPDI->Update(0.05f);
}

cUpperScreen::~cUpperScreen()
{
	//SAFE_DELETE(m_pUpperUIReadMeMPDI);
}

void	cUpperScreen::Update(float e_fElpaseTime)
{
	m_pUpperUIMPDI->Update(e_fElpaseTime);
	if( m_pUpperUIPayRateHint )
		m_pUpperUIPayRateHint->Update(e_fElpaseTime);
}

void	cUpperScreen::Render()
{
	m_pUpperUIMPDI->Render();
}

void	cUpperScreen::RenderReel()
{
	if( g_iExplosionCount > 0 )
	{
		if( g_iExplosionCount < 11 )
		{
			for( int i=0;i<g_iExplosionCount;++i )
			{
				m_pReelMPDI->GetObject(i)->Render();
			}
		}
	}
	else
	{
		//m_pReelMPDI->UpdateByGlobalTime(0.000001f);
		//m_pReelMPDI->RenderByGlobalTime();	
	}
}

void	cUpperScreen::RenderPayRateHint()
{
	if(m_pUpperUIPayRateHint)
		m_pUpperUIPayRateHint->Render();
}