#include "stdafx.h"
#include "CalculateScorePhase.h"
#include "SlotObject.h"
#include "King.h"
#include "GameApp.h"
extern int	g_iExplosionCount;
cCalculateScorePhase::cCalculateScorePhase()
{
	m_iShowMPDIMoney = 2000;
	m_pWinMoneyOver2000MPDI = 0;
	m_pConstantUI = 0;
	m_IdleTimeForShowHowManyMoneyWin.SetTargetTime(1.f);
	this->SetName(CALCULATE_SCORE_PHASE_NAME);
}

cCalculateScorePhase::~cCalculateScorePhase()
{
	SAFE_DELETE(m_pConstantUI);
}

void	cCalculateScorePhase::Init()
{
	if( !m_pConstantUI )
	{
		cNodeISAX	l_NodeISAX;
		bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(CALCULATE_SCORE_PHASE_DATA);
		if( l_b )
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			const WCHAR*l_strUIMPDIFileName = l_pElement->Attribute(L"UIMPDIFileName");
			const WCHAR*l_strResultLinesMPDIFileName = l_pElement->Attribute(L"ResultLinesMPDIFileName");
			const WCHAR*l_strUIMPDIName = l_pElement->Attribute(L"UIMPDIName");
			const WCHAR*l_strShowTime = l_pElement->Attribute(L"ShowTime");
			const WCHAR*l_strWinOver200MPDI = l_pElement->Attribute(L"WinOver200MPDI");
			const WCHAR*l_strShowMPDIMoney = l_pElement->Attribute(L"ShowMPDIMoney");
			const WCHAR*l_strShowWinMoneyIdleTime = l_pElement->Attribute(L"ShowWinMoneyIdleTime");
			if( l_strUIMPDIFileName && !m_pConstantUI )
			{
				//cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(l_strUIMPDIFileName);
				cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
				//m_pConstantUI = new cMPDI(l_pMPDIList->GetObject(l_strUIMPDIName));
				m_pConstantUI = new cMPDI(l_pMPDIList->GetObject(UPPER_UI));
				m_pConstantUI->Init();
				m_pConstantUI->Update(EPSIONAL);
				m_iShowMPDIMoney = _wtoi(l_strShowMPDIMoney);
				//m_pWinMoneyOver2000MPDI = l_pMPDIList->GetObject(l_strWinOver200MPDI);
			}
			int	l_iNumLineResult = cEngineTestApp::m_spSlotRuleData->m_TableCount.y;
			if(l_iNumLineResult>100)
			{
				UT::ErrorMsg(L"total result is not allow over 100 lines",L"Error");
			}
			float	l_fTimeForWho = (float)_wtof(l_strShowTime);
			m_TimeForScoreToChangeUserMoney.SetTargetTime(l_fTimeForWho);
			if( l_strShowWinMoneyIdleTime )
			{
				float	l_fTime = (float)_wtof(l_strShowWinMoneyIdleTime);
				m_IdleTimeForShowHowManyMoneyWin.SetTargetTime(l_fTime);
			}
		}
	}
	m_iPlayerOriginalMoney = cEngineTestApp::m_spPlayerData->m_iMoney;
	m_TimeForScoreToChangeUserMoney.Start();
	//cEngineTestApp::m_spBetBehavior->CalculateScoreWithRound();
	m_iWinMoney = cEngineTestApp::m_spBetBehavior->m_iWinMoney;
	m_iPlayerMoney = cEngineTestApp::m_spPlayerData->m_iMoney;
	m_IdleTimeForShowHowManyMoneyWin.Start();
	cEngineTestApp::m_spSlotLineObjectManagerList->Init();
	if( m_iWinMoney>0 )
	{//play win omney sound
	
	}
	else
	{
		this->m_bSatisfiedCondition = true;
	}
	cEngineTestApp::WriteDataIntoControlPanel();
}

void	cCalculateScorePhase::Update(float e_fElpaseTime)
{
	if(m_IdleTimeForShowHowManyMoneyWin.bTragetTimrReached)
	{
		//m_pConstantUI->Update(e_fElpaseTime);
		m_TimeForScoreToChangeUserMoney.Update(e_fElpaseTime);
		if( m_TimeForScoreToChangeUserMoney.bTragetTimrReached )
		{
			cEngineTestApp::m_spBetBehavior->m_iWinMoney = 0;
			cEngineTestApp::m_spBetBehavior->m_pPlayerData->m_iMoney = m_iPlayerOriginalMoney+m_iWinMoney;
			g_iExplosionCount = 0;
			this->m_bSatisfiedCondition = true;
		}
		else
		if( m_iWinMoney >0 )
		{
			int	l_iMoney = (int)((1-m_TimeForScoreToChangeUserMoney.GetLERP())*m_iWinMoney);
			cEngineTestApp::m_spBetBehavior->m_iWinMoney = l_iMoney;
			cEngineTestApp::m_spBetBehavior->m_pPlayerData->m_iMoney = m_iPlayerMoney+(m_iWinMoney-cEngineTestApp::m_spBetBehavior->m_iWinMoney);
			if( m_pWinMoneyOver2000MPDI )
				m_pWinMoneyOver2000MPDI->Update(e_fElpaseTime);
		}
	}
	else
	{
		m_IdleTimeForShowHowManyMoneyWin.Update(e_fElpaseTime);
		if(m_IdleTimeForShowHowManyMoneyWin.bTragetTimrReached)
		{
			if( m_pWinMoneyOver2000MPDI )
				m_pWinMoneyOver2000MPDI->Init();
		}
	}
}

void	cCalculateScorePhase::Render()
{
	cEngineTestApp::m_spKing->Render();
	m_pConstantUI->Render();
	cEngineTestApp::m_spBetBehavior->Render();
	cEngineTestApp::m_spSlotLineObjectManagerList->Render();
	if(m_IdleTimeForShowHowManyMoneyWin.bTragetTimrReached)
	{
		if( this->m_iWinMoney >m_iShowMPDIMoney )
		{
			if( m_pWinMoneyOver2000MPDI )
				m_pWinMoneyOver2000MPDI->Render();
		}
	}
	//just skip it 
#ifdef _DEBUG
	WCHAR*l_str = ValueToStringW(m_TimeForScoreToChangeUserMoney.fRestTime);
	cGameApp::m_spGlyphFontRender->RenderFont(0,150,l_str);
#endif
}

void	cCalculateScorePhase::DebugRender()
{

}

void	cCalculateScorePhase::MouseDown(int e_iX,int e_iY)
{

}

void	cCalculateScorePhase::MouseUp(int e_iX,int e_iY)
{

}

void	cCalculateScorePhase::MouseMove(int e_iX,int e_iY)
{

}

void	cCalculateScorePhase::KeyUp(char e_cKey)
{
	//this->m_bSatisfiedCondition = true;
}

const WCHAR*	cCalculateScorePhase::GetNextPhaseName()
{
#ifdef _DEBUG
	//OutputDebugString(L"leave cCalculateScorePhase\n");
#endif
	return BET_PHASE_NAME;
}