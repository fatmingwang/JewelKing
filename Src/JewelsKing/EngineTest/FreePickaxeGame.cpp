#include "stdafx.h"
#include "FreePickaxeGame.h"
#include "GameApp.h"
#include "SlotObject.h"

const wchar_t*         cFreePickaxeGame::TypeID( L"cFreePickaxeGame" );

cFreePickaxeGame::cFreePickaxeGame(const WCHAR*e_strConditionName,int e_iConditionCount)
:cBaseExtraGame(e_strConditionName,e_iConditionCount)
{
	m_eFreeGameList = eFGL_PICKAXE;
	m_pMPDIList = 0;
}

cFreePickaxeGame::~cFreePickaxeGame()
{
}

bool	cFreePickaxeGame::InternalConditionCheck()
{
	int	l_iCount = cEngineTestApp::m_spSlotLineObjectManagerList->GetNameCount(this->m_strConditionName.c_str());
	//it's already on free game2
	if( cEngineTestApp::m_sbFreeToPlay && l_iCount>0 )
		return true;
	if(this->m_iConditionCount<=l_iCount)
		return true;
	return false;
}

void	cFreePickaxeGame::LoadResource()
{
	if( !m_pMPDIList )
	{
		m_pMPDIList = cGameApp::GetMPDIListByFileName(ANIMATED_MOVE_SOIN_OBJECT_MPDILIST_FILE_NAME);
		m_pPickaxeMPDI = m_pMPDIList->GetObject(FREE_GAME_PICKAXE_MPDI_NAME);
	}
	m_pPickaxeMPDI->Init();
}

void	cFreePickaxeGame::Init()
{
	cBaseExtraGame::Init();
}

int	cFreePickaxeGame::FreeGameCountToAdd()
{
	int	l_iCount = cEngineTestApp::m_spSlotLineObjectManagerList->GetNameCount(this->m_strConditionName.c_str());
	if( !cEngineTestApp::m_sbFreeToPlay)
	{
		l_iCount = l_iCount+1-this->m_iConditionCount;
	}
	return l_iCount;
}

void	cFreePickaxeGame::Update(float e_fElpaseTime)
{
	m_pPickaxeMPDI->Update(e_fElpaseTime);
	if( m_pPickaxeMPDI->IsDone() )
	{
		//cEngineTestApp::m_siFreeGame += FreeGameCountToAdd();
		m_bDone = true;
	}
}

void	cFreePickaxeGame::Render()
{
	cEngineTestApp::m_spSlotLineObjectManagerList->Render();
	cPuzzleImageUnit*l_pPIUnit = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI()->GetObject(0);
	Vector2	l_vSize((float)*l_pPIUnit->GetImageShowWidth(),(float)*l_pPIUnit->GetImageShowHeight());
	for(int i=0;i<cEngineTestApp::m_spSlotRuleData->m_TableCount.x;++i)
	{
		cSlotObjectManager*l_pSlotObjectManager = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(i);
		for(int j=0;j<cEngineTestApp::m_spSlotRuleData->m_TableCount.y;++j)
		{
			if(!wcscmp(l_pSlotObjectManager->GetNameByResultIndex(j),m_strConditionName.c_str()))
			{

				sLoopMovingObject*l_pLoopMovingObject = l_pSlotObjectManager->GetLoopMovingObject(j);
				Vector3	l_vPos = l_pLoopMovingObject->vCurrentPos;
				l_vPos.x += l_vSize.x/2.f;
				l_vPos.y += l_vSize.y/2.f;
				m_pPickaxeMPDI->SetPos(l_vPos);
				m_pPickaxeMPDI->Render();
			}
		}
	}
	int	l_iCount = FreeGameCountToAdd();
	std::wstring	l_str = ValueToStringW(cEngineTestApp::m_siFreeGame);
	l_str += L" + ";
	l_str += ValueToStringW(l_iCount);
	cGameApp::m_spGlyphFontRender->RenderFont(200,200,(WCHAR*)l_str.c_str());
}