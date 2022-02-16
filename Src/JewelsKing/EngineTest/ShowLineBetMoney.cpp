#include "stdafx.h"
#include "ShowLineBetMoney.h"
#include "GameApp.h"

cShowLineBetMoney::cShowLineBetMoney()
{
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(RESULT_LINES_MPDILIST_FILE_NAME);
	cMPDI*l_pMPDI = l_pMPDIList->GetObject(PERLINE_BET_EFFECT);
	cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(0);
	m_PathForShowPerLineBetMoney = l_pSubMPDI->GetOriginalPointList();
	m_iPerLineBetMoney = 1;
	m_pPI = cGameApp::GetPuzzleImage(NUM_PI_NAME);
	m_pImage = m_pPI->GetObject(L"line_1");
}

cShowLineBetMoney::~cShowLineBetMoney()
{

}

void	cShowLineBetMoney::Init()
{
	Update(0);
	m_iPerLineBetMoney = cEngineTestApp::m_spBetBehavior->GetPerLineBetMoney();
}

void	cShowLineBetMoney::Update(float e_fElpaseTime)
{
	if( m_iPerLineBetMoney != cEngineTestApp::m_spBetBehavior->GetPerLineBetMoney() )
	{
		m_iPerLineBetMoney = cEngineTestApp::m_spBetBehavior->GetPerLineBetMoney();
		if( m_iPerLineBetMoney>9 )
			return;
		std::wstring	l_str = L"line_";
		l_str += ValueToStringW(m_iPerLineBetMoney);
		m_pImage = m_pPI->GetObject(l_str.c_str());
	}
}

void	cShowLineBetMoney::Render()
{
	int	l_uiSize = m_PathForShowPerLineBetMoney.size();
	for( int i=0;i<l_uiSize;++i )
	{
		m_pImage->Render(m_PathForShowPerLineBetMoney[i]);
	}
	
}