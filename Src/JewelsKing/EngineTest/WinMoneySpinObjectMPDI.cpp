#include "stdafx.h"
#include "WinMoneySpinObjectMPDI.h"
#include "SlotObject.h"
#include "GameApp.h"
#include "SlotRule.h"
#include "BetPhase.h"

#define		EACH_SPIN_OBJECT_WIN_MONEY_EFFECT_PREFIX	L""

cWinMoneySpinObjectMPDI::cWinMoneySpinObjectMPDI()
{
	m_WinMoneyMPDIList.SetFromResource(false);
	m_pbWinMoneySpinObject = 0;
}

cWinMoneySpinObjectMPDI::~cWinMoneySpinObjectMPDI()
{

}

void	cWinMoneySpinObjectMPDI::SetupWinMoneySpinObjectByLine(cBetBehaviorResult::sTableResult::sLine*e_pLine)
{
	m_WinMoneyMPDIList.Destroy();
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(WIN_MONEY_SPIN_OBJECT_MPDI_MPDILIST_FILE_NAME);
	cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(SPIN_OBJECT_PI_NAME);
	cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(0);
	float	l_fImageHalfWidth = *l_pPIUnit->GetImageShowWidth()/2.f;
	float	l_fImageHalfHeight = *l_pPIUnit->GetImageShowHeight()/2.f;
	for( size_t i=0;i<e_pLine->WinMoneySpinObjectIndexList.size();++i )
	{
		sLoopMovingObject*l_pLoopMovingObject = GetLoopMovingObjectByIndex(e_pLine->WinMoneySpinObjectIndexList[i]);
		const WCHAR*l_strPIUnitName = l_pPI->GetObject(l_pLoopMovingObject->iImageIndex)->GetName();
		std::wstring	l_strMPDIName = EACH_SPIN_OBJECT_WIN_MONEY_EFFECT_PREFIX;
		l_strMPDIName += l_strPIUnitName;
		cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strMPDIName.c_str());
		if( !l_pMPDI )
		{
			UT::ErrorMsg(L"Win money MPDI is not exist",(WCHAR*)l_strMPDIName.c_str());
		}
		else
		{
			cMPDI*l_pCloneMPDI = dynamic_cast<cMPDI*>(l_pMPDIList->GetObject(l_strMPDIName.c_str())->Clone());
			bool	l_b = m_WinMoneyMPDIList.AddObjectNeglectExist(l_pCloneMPDI);
			Vector3	l_vPos = l_pLoopMovingObject->vCurrentPos;
			l_vPos.x += l_fImageHalfWidth;
			l_vPos.y += l_fImageHalfHeight;
			l_pCloneMPDI->SetPos(l_vPos);
			if( l_b )
				l_pCloneMPDI->Init();
		}
	}
}

void	cWinMoneySpinObjectMPDI::Init(cBetBehaviorResult::sTableResult*e_pTableResult)
{
	m_WinMoneyMPDIList.Destroy();
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(WIN_MONEY_SPIN_OBJECT_MPDI_MPDILIST_FILE_NAME);
	cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(SPIN_OBJECT_PI_NAME);
	cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(0);
	float	l_fImageHalfWidth = *l_pPIUnit->GetImageShowWidth()/2.f;
	float	l_fImageHalfHeight = *l_pPIUnit->GetImageShowHeight()/2.f;
	int	l_iTotalSpinObject = cEngineTestApp::m_spSlotRuleData->m_TableCount.x*cEngineTestApp::m_spSlotRuleData->m_TableCount.y;
	for( int i=0;i<l_iTotalSpinObject;++i )
	{
		if(e_pTableResult->pbWinMoneySpinObject[i])
		{
			sLoopMovingObject*l_pLoopMovingObject = GetLoopMovingObjectByIndex(i);
			if( !l_pLoopMovingObject )
			{
				int a=0;
			}
			if( l_pLoopMovingObject->iImageIndex>=l_pPI->Count() )
			{
				int a=0;
			}
			const WCHAR*l_strPIUnitName = l_pPI->GetObject(l_pLoopMovingObject->iImageIndex)->GetName();
			std::wstring	l_strMPDIName = EACH_SPIN_OBJECT_WIN_MONEY_EFFECT_PREFIX;
			l_strMPDIName += l_strPIUnitName;
			cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strMPDIName.c_str());
			if( !l_pMPDI )
			{
				UT::ErrorMsg(L"Win money MPDI is not exist",(WCHAR*)l_strMPDIName.c_str());
			}
			else
			{
				WCHAR*l_strName = ValueToStringW(i);
				if( !m_WinMoneyMPDIList.GetObject(l_strName) )
				{
					cMPDI*l_pCloneMPDI = dynamic_cast<cMPDI*>(l_pMPDI->Clone());
					l_pCloneMPDI->SetName(l_strName);
					bool	l_b = m_WinMoneyMPDIList.AddObjectNeglectExist(l_pCloneMPDI);
					if( l_b )
					{
						l_pCloneMPDI->Init();
						Vector3	l_vPos = l_pLoopMovingObject->vCurrentPos;
						l_vPos.x += l_fImageHalfWidth;
						l_vPos.y += l_fImageHalfHeight;
						l_pCloneMPDI->SetPos(l_vPos);
					}
				}
			}
		}
	}
}

//void	cWinMoneySpinObjectMPDI::UpdateByLine(float e_fElpaseTime)
//{
//	int	l_iCount = m_WinMoneyMPDIList.Count();
//	for( int i=0;i<l_iCount;++i )
//	{
//		m_WinMoneyMPDIList[i]->Update(e_fElpaseTime);
//	}
//}

extern bool	g_bPause;

//void	cWinMoneySpinObjectMPDI::RenderByLine()
//{
//	if( m_pCurrentWorkingLineRule )
//	{
//		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(WIN_MONEY_SPIN_OBJECT_MPDI_MPDILIST_FILE_NAME);
//		cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(SPIN_OBJECT_PI_NAME);
//		int	l_iTotalSpinObject = (int)m_pCurrentWorkingLineRule->cOneLineCount;
//		cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(0);
//		float	l_fImageHalfWidth = *l_pPIUnit->GetImageShowWidth()/2.f;
//		float	l_fImageHalfHeight = *l_pPIUnit->GetImageShowHeight()/2.f;
//		for( int i=0;i<m_iMatchCount;++i )
//		{
//			int	l_iIndex = m_pCurrentWorkingLineRule->pcLineObjectIndex[i];
//			if( !m_pbWinMoneySpinObject[l_iIndex] )
//			{//no concetive
//				return;
//			}
//			sLoopMovingObject*l_pLoopMovingObject = GetLoopMovingObjectByIndex(l_iIndex);
//			Vector3	l_vPos = l_pLoopMovingObject->vCurrentPos;
//			WCHAR*l_strPIUnitName = l_pPI->GetObject(l_pLoopMovingObject->iImageIndex)->GetName();
//			std::wstring	l_strMPDIName = EACH_SPIN_OBJECT_WIN_MONEY_EFFECT_PREFIX;
//			l_strMPDIName += l_strPIUnitName;
//			cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strMPDIName.c_str());
//			if( l_pMPDI )
//			{
//				l_vPos.x += l_fImageHalfWidth;
//				l_vPos.y += l_fImageHalfHeight;
//				l_pMPDI->SetPos(l_vPos);
//				l_pMPDI->Render();
//			}
//		}
//	}
//}

void	cWinMoneySpinObjectMPDI::Update(float e_fElpaseTime)
{
	int	l_iCount = m_WinMoneyMPDIList.Count();
	for( int i=0;i<l_iCount;++i )
	{
		m_WinMoneyMPDIList[i]->Update(e_fElpaseTime);
	}
}

void	cWinMoneySpinObjectMPDI::Render()
{
	//cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(ANIMATED_MOVE_SOIN_OBJECT_MPDILIST_FILE_NAME);
	//cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(SPIN_OBJECT_PI_NAME);
	//cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(0);
	//float	l_fImageHalfWidth = *l_pPIUnit->GetImageShowWidth()/2.f;
	//float	l_fImageHalfHeight = *l_pPIUnit->GetImageShowHeight()/2.f;
	int		l_iTotalSpinObject = m_WinMoneyMPDIList.Count();
	for( int i=0;i<l_iTotalSpinObject;++i )
	{
		cMPDI*l_pMPDI = m_WinMoneyMPDIList[i];
		if( l_pMPDI )
			l_pMPDI->Render();
	}
}

bool	cWinMoneySpinObjectMPDI::IsDone()
{
	if( m_WinMoneyMPDIList.Count() == 0 )
		return true;
	return m_WinMoneyMPDIList[0]->IsDone();
}

float	cWinMoneySpinObjectMPDI::GetCurrentProgress()
{
	if(m_WinMoneyMPDIList.Count())
	{
		return m_WinMoneyMPDIList[0]->GetCurrentProgress();
	}
	return 1.f;
}