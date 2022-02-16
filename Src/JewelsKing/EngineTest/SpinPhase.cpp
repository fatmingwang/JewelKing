#include "stdafx.h"
#include "SpinPhase.h"
#include "SlotObject.h"
#include "GameApp.h"
#include "WinMoneyLine.h"
#include "King.h"
#include "GameMessage.h"

cSpinPhase::cSpinPhase()
{
	this->SetName(SPIN_PHASE_NAME);
	m_pConstantUI = 0;
}

cSpinPhase::~cSpinPhase()
{
	SAFE_DELETE(m_pConstantUI);
}


void	cSpinPhase::Init()
{
	if( !m_pConstantUI )
	{
		cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
		m_pConstantUI = new cMPDI(l_pMPDIList->GetObject(UPPER_UI));
		m_pConstantUI->Init();
		m_pConstantUI->Update(EPSIONAL);
		m_pConstantUI->RefreshTotalPlayTime();
	}
	if( cEngineTestApp::m_spBetBehavior->m_iBonusMoneyRate != 1 )
	{
		if(cEngineTestApp::m_spBetBehavior->m_pBetBehaviorResult->GetCurrentTablesWinMoney())
		{
			cEngineTestApp::m_spKing->SetFreeGamePayRate(cEngineTestApp::m_spBetBehavior->m_iBonusMoneyRate);
			cEngineTestApp::m_spKing->SetCurrentWorkingObject(eKS_MONEY5000);
			//cGameApp::m_spSoundParser->GetObject(FREE_GAME_EXTRA_PAY_RATE_SOUND_NAME)->Play(true);
		}
	}
	else
	{
		cEngineTestApp::m_spKing->SetCurrentWorkingObject(eKS_IDLE);
		cEngineTestApp::m_spKing->SetFreeGamePayRate(0);
	}
	//m_bCheckSpinBurronIsUp = false;
	if(!cEngineTestApp::m_sbFreeToPlay)
		cEngineTestApp::m_spKing->GetGameMessage()->SetSpin();

	for( int i=0;i<50;++i )
		m_fSpeedUp[i] = 1.f;
}

void	cSpinPhase::Update(float e_fElpaseTime)
{
	//if( !m_bCheckSpinBurronIsUp )
	//{
	//	if( !cGameApp::m_sucKeyData['S'] )
	//		m_bCheckSpinBurronIsUp  = true;
	//}
	if( cGameApp::m_sucKeyData['S'] )
		e_fElpaseTime *= 2.5f;
	//unsigned char	l_cKEyData[5] = {'C','V','B','N','M'};
	//for( int i=0;i<5;++i )
	//{
	//	if(cGameApp::m_sucKeyData[l_cKEyData[i]])
	//	{
	//		m_fSpeedUp[i] = 3.f;
	//	}
	//}

    int l_iCount = cEngineTestApp::m_spSlotLineObjectManagerList->Count();
    for(int i=0;i<l_iCount;++i)
    {
		cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(i)->Update(m_fSpeedUp[i]*e_fElpaseTime);
    }
	//cEngineTestApp::m_spSlotLineObjectManagerList->Update(e_fElpaseTime);
	//m_pConstantUI->Update(e_fElpaseTime);
	l_iCount = cEngineTestApp::m_spSlotLineObjectManagerList->Count();
	if( cEngineTestApp::m_spSlotLineObjectManagerList->IsSpinedDone() )
	{
		this->m_bSatisfiedCondition = true;
	}
}

void	cSpinPhase::Render()
{
	cEngineTestApp::m_spKing->Render();
	m_pConstantUI->Render();
	//WCHAR*l_str = ValueToStringW(cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(cEngineTestApp::m_spSlotLineObjectManagerList->Count()-1)->GetRestTime());
#ifdef _DEBUG
	WCHAR*l_str = ValueToStringW(cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(0)->GetRestTime());
	cGameApp::m_spGlyphFontRender->RenderFont(0,150,l_str);
#endif
	cEngineTestApp::m_spBetBehavior->Render();
	//if( cEngineTestApp::m_spSlotLineObjectManagerList->IsSpinedDone() )
	//{

	//}
	cEngineTestApp::m_spSlotLineObjectManagerList->Render();
}

void	cSpinPhase::DebugRender()
{

}

void	cSpinPhase::MouseDown(int e_iX,int e_iY)
{

}

void	cSpinPhase::MouseUp(int e_iX,int e_iY)
{

}

void	cSpinPhase::MouseMove(int e_iX,int e_iY)
{

}

void	cSpinPhase::KeyUp(char e_cKey)
{
	//this->m_bSatisfiedCondition = true;
}

void*	cSpinPhase::GetData()
{
	return 0;
}

const WCHAR*	cSpinPhase::GetNextPhaseName()
{
#ifdef _DEBUG
	//OutputDebugString(L"leave cSpinPhase\n");
#endif
	//return CALCULATE_SCORE_PHASE_NAME;
	//if( cEngineTestApp::m_spBetBehavior->m_WinMoneyLineIndex.size() == 0 )
	//	return BET_PHASE_NAME;
	return EXTRA_GAME_PHASE_NAME;
}