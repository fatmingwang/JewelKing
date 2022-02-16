#include "stdafx.h"
#include "FreeJP.h"
#include "GameApp.h"
#include "SlotObject.h"
#include "King.h"
#include "WinMoneyLine.h"
#include "GameNetwork.h"
#include "JPEffect.h"
const wchar_t*         cFreeJPGame::TypeID( L"cFreeJPGame" );

cFreeJPGame::cFreeJPGame(const WCHAR*e_strConditionName,int e_iConditionCount,int e_iJPMoney,cMPDI*e_pConstantUI)
:cBaseExtraGame(e_strConditionName,e_iConditionCount)
{
	m_pConstantUI = e_pConstantUI;
	m_iJPMoney = e_iJPMoney;
	m_eFreeGameList = eFGL_WHOLE_TARGET_SAME;
	m_pWinmoneyLines = 0;
	m_pJPEffect = 0;
}

cFreeJPGame::~cFreeJPGame()
{
	SAFE_DELETE(m_pJPEffect);
	SAFE_DELETE(m_pWinmoneyLines);
}

bool	cFreeJPGame::InternalConditionCheck()
{
	int	l_iSize = (int)cEngineTestApp::m_spSlotRuleData->m_LineRuleList.size();
	if( l_iSize )
	{
		//int	l_iCount = cEngineTestApp::m_spSlotLineObjectManagerList->Count();
		std::vector<wstring>	l_strInSearchLineObjectName;
		//int	l_iSize = (int)cEngineTestApp::->m_LineRuleList.size();
		int	l_iConditionImageIndex = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI()->GetObjectIndexByName(this->m_strConditionName.c_str());
		for( int i=0;i<l_iSize;++i )
		{
			sLineRule*l_pLineRule = cEngineTestApp::m_spSlotRuleData->m_LineRuleList[i];
			char*l_pcLineIndex = l_pLineRule->pcLineObjectIndex;
			int	l_iCount = l_pLineRule->cOneLineCount;
			bool	l_bSame = true;
			for( int i=0;i<l_iCount;++i )
			{
				if( GetLoopMovingObjectByIndex(l_pcLineIndex[i])->iImageIndex != l_iConditionImageIndex )
				{
					l_bSame = false;
				}
			}
			if( l_bSame )
			{
				return true;
			}
		}
	}
	return false;
}

void	cFreeJPGame::Destroy()
{
#ifdef _DEBUG
//	OutputDebugString(L"leave cFreeJPGame\n");
#endif
}

void	cFreeJPGame::LoadResource()
{
	if( !m_pJPEffect )
	{
		m_pJPEffect = new cJPEffect(1);
	}
	m_pJPEffect->Init(*cEngineTestApp::m_spPlayerData->GetJP(eJPT_NETWORK));
	//m_pJPEffect->Init(99999);
	cEngineTestApp::m_spKing->ChangeStatusByMoney(m_iJPMoney,false);
	cEngineTestApp::m_spKing->SetRenderPayRateHint(false);
	cBetBehaviorResult::sTableResult*l_pTableResult = cEngineTestApp::m_spBetBehavior->m_pBetBehaviorResult->m_TableResultList[0];
	m_pWinmoneyLines->Init(l_pTableResult);
	m_pWinmoneyLines->SetDone();
}

void	cFreeJPGame::Init()
{
	cBaseExtraGame::Init();
	if( !m_pWinmoneyLines )
	m_pWinmoneyLines = new cWinmoneyLines(RESULT_LINES_MPDILIST_FILE_NAME);
	int*l_pJP = cEngineTestApp::m_spPlayerData->GetJP(eJPT_NETWORK);
	m_iJPMoney = *l_pJP;
	m_iStartJPWinMoney = cEngineTestApp::m_spBetBehavior->m_pBetBehaviorResult->GetCurrentTablesWinMoney();
	m_iFinalJPWinMoney = *cEngineTestApp::m_spPlayerData->GetJP(eJPT_NETWORK)+m_iStartJPWinMoney;
}

void	cFreeJPGame::Update(float e_fElpaseTime)
{
	if( !m_pWinmoneyLines->IsDone() )
	{
		m_pWinmoneyLines->UpdateWinMoneyLinesByStep(e_fElpaseTime);
	}
	else
	{
		if( !m_pJPEffect->IsDone() )
		{
			m_pJPEffect->Update(e_fElpaseTime);
			float	l_fProgress = m_pJPEffect->GetCurrentProgress();
			if( l_fProgress >1.f )
				l_fProgress = 1.f;
			cEngineTestApp::m_spBetBehavior->m_iWinMoney = UT::LERP(m_iStartJPWinMoney,m_iFinalJPWinMoney,l_fProgress);
			int*l_pJP = cEngineTestApp::m_spPlayerData->GetJP(eJPT_NETWORK);
			*l_pJP = (int)((1-l_fProgress)*m_iJPMoney);
			m_pJPEffect->SetNumerial(*l_pJP);
		}
		if( m_pJPEffect->IsDone() )
		{
			m_bDone = true;
			cEngineTestApp::m_spBetBehavior->m_iWinMoney = m_iFinalJPWinMoney;

			cEngineTestApp::m_spKing->SetRenderPayRateHint(true);
			sGamePacket	l_GamePacket;
			l_GamePacket.iMessageID = eGMI_TO_SERVER_FLUSH_JP;
			cEngineTestApp::m_spNetwork->SendData(&l_GamePacket);

			cEventVariable*l_pGameRecord_JP = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"GameRecord_NetWorkJP");
			cEventVariable*l_pGamePrize_JP = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"GamePrize_NetWorkJP");

			l_pGameRecord_JP->SetInt(*l_pGameRecord_JP->GetInt()+1);
			l_pGamePrize_JP->SetInt(*l_pGamePrize_JP->GetInt()+m_iJPMoney);
			//assign refund money
			cEngineTestApp::m_spBetBehavior->m_pBetBehaviorResult->AddOutMoney(*cEngineTestApp::m_spPlayerData->GetJP(eJPT_NETWORK));
			cEngineTestApp::m_spPlayerData->SetJP(eJPT_NETWORK,0);
			int*l_piCount = cEngineTestApp::m_spPlayerData->GetJPOpenCount(eJPT_NETWORK);
			*l_piCount += 1;
			cEngineTestApp::m_spLogFile->WriteToFileImmediatelyWithLine("NETWORK	JP OPEN!!!!!");
			cEngineTestApp::m_sfDebugValue = 1.f;
		}
	}
}

void	cFreeJPGame::Render()
{
	cEngineTestApp::m_spKing->RenderUpperScreen();
	cEngineTestApp::m_spKing->Render();
	m_pConstantUI->Render();
	cEngineTestApp::m_spSlotLineObjectManagerList->Render();
	m_pWinmoneyLines->Render(0);
	if( m_pWinmoneyLines->IsDone() )
		m_pJPEffect->Render();
}