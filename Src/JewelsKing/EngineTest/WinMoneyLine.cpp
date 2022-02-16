#include "stdafx.h"
#include "WinMoneyLine.h"
#include "SlotObject.h"
#include "WinMoneySpinObjectMPDI.h"
#include "GameApp.h"


cWinmoneyLines::cWinmoneyLines(const WCHAR*e_strMPDIListName)
{
	for(int i =0;i<TOTAL_LINE_RESULT;++i)
	{
		m_pLineMPDI[i] = 0;
	}
	m_pResultLinesMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(e_strMPDIListName);	
	int	l_iNumLineResult = (int)cEngineTestApp::m_spSlotRuleData->m_LineRuleList.size();
	for( int i=0;i<l_iNumLineResult;++i )
	{
		int	l_iNumLineResult = cEngineTestApp::m_spSlotRuleData->m_TableCount.y;
		if(l_iNumLineResult>100)
		{
			UT::ErrorMsg(L"total result is not allow over 100 lines",L"Error");
		}
		m_pLineMPDI[i] = new cMPDI(m_pResultLinesMPDIList->GetObject(i));
		m_pLineMPDI[i]->Init();
		m_pLineMPDI[i]->SetLoop(false);
	}

	m_pWinMoneySpinObjectMPDI = new cWinMoneySpinObjectMPDI();
	cPuzzleImage*l_pPI = cGameApp::m_spImageParser->GetPuzzleImage(NUM_PI_NAME);
	m_pNumerialImage = l_pPI->GetNumerialImageByName(L"WinNum_0",L"WinNum_9");
	m_pTableResult = 0;
}
cWinmoneyLines::~cWinmoneyLines()
{
	SAFE_DELETE(m_pNumerialImage);
	SAFE_DELETE(m_pWinMoneySpinObjectMPDI);
	for(int i =0;i<TOTAL_LINE_RESULT;++i)
	{
		SAFE_DELETE(m_pLineMPDI[i]);
	}
}

void	cWinmoneyLines::Stop()
{
	int	l_iSize = (int)cEngineTestApp::m_spSlotRuleData->m_LineRuleList.size();
	for( int i=0;i<l_iSize;++i )
	{
		this->m_pLineMPDI[i]->Stop();
	}
}

void	cWinmoneyLines::Init(cBetBehaviorResult::sTableResult*e_pTableResult)
{
	Stop();
	m_pTableResult = e_pTableResult;
	m_iWinMoneyLines.clear();
	m_pWinMoneySpinObjectMPDI->Destroy();
	int	l_iSize = (int)m_pTableResult->WinMoneyLineIndexList.size();
	if( l_iSize )
	{
		if( l_iSize == 1 )
		{
			int	l_iLindexIndex = m_pTableResult->WinMoneyLineIndexList[0];
			this->m_pLineMPDI[l_iLindexIndex]->Init();
		}
		else
		{
			m_iWinMoneyLines = m_pTableResult->WinMoneyLineIndexList;
			int	l_iLindexIndex = m_iWinMoneyLines[0];
			m_pWinMoneySpinObjectMPDI->SetupWinMoneySpinObjectByLine(&m_pTableResult->WinMoneyLineList[0]);
			this->m_pLineMPDI[l_iLindexIndex]->Init();
		}
	}
}

bool	cWinmoneyLines::UpdateWinMoneyLinesByStep(float e_fElpaseTime)
{
	if(m_iWinMoneyLines.size()>0)
	{
		int	l_iLindexIndex = m_iWinMoneyLines[0];
		this->m_pLineMPDI[l_iLindexIndex]->Update(e_fElpaseTime);
		m_pWinMoneySpinObjectMPDI->Update(e_fElpaseTime);
		if( this->m_pLineMPDI[l_iLindexIndex]->IsDone() )
		{
			m_iWinMoneyLines.erase(m_iWinMoneyLines.begin());
			if(m_iWinMoneyLines.size()>0)
			{
				l_iLindexIndex = m_iWinMoneyLines[0];
				this->m_pLineMPDI[l_iLindexIndex]->Init();
				size_t	l_iWinMoneyLineIndex = 0;
				for(;l_iWinMoneyLineIndex<m_pTableResult->WinMoneyLineList.size();++l_iWinMoneyLineIndex )
				{
					if(m_pTableResult->WinMoneyLineIndexList[l_iWinMoneyLineIndex] == l_iLindexIndex)
					{
						break;
					}
				}
				//assert( l_iWinMoneyLineIndex == 79979 );
				m_pWinMoneySpinObjectMPDI->SetupWinMoneySpinObjectByLine(&m_pTableResult->WinMoneyLineList[l_iWinMoneyLineIndex]);
			}
			else
			{
				int	l_iSize = (int)m_pTableResult->WinMoneyLineIndexList.size();
				if( l_iSize )
				{
					for( int i=0;i<l_iSize;++i )
					{
						int	l_iLindexIndex = m_pTableResult->WinMoneyLineIndexList[i];
						this->m_pLineMPDI[l_iLindexIndex]->Init();
					}
				}
			}
			return true;
		}
	}
	else
	{
		Update(e_fElpaseTime);
	}
	return false;
}

void	cWinmoneyLines::Update(float e_fElpaseTime)
{
	int	l_iSize = (int)m_pTableResult->WinMoneyLineIndexList.size();
	for( int i=0;i<l_iSize;++i )
	{
		int	l_iLindexIndex = m_pTableResult->WinMoneyLineIndexList[i];
		this->m_pLineMPDI[l_iLindexIndex]->Update(e_fElpaseTime);
	}
}
void	cWinmoneyLines::Render(int e_iWinMoney)
{
	//m_pWinMoneySpinObjectMPDI->RenderByLine();
	m_pWinMoneySpinObjectMPDI->Render();
	int	l_iSize = (int)m_pTableResult->WinMoneyLineIndexList.size();
	for( int i=0;i<l_iSize;++i )
	{
		int	l_iLindexIndex = m_pTableResult->WinMoneyLineIndexList[i];
		this->m_pLineMPDI[l_iLindexIndex]->Render();
	}
	if( l_iSize && e_iWinMoney>0 )
	{
		int	l_iLindexIndex = m_pTableResult->WinMoneyLineIndexList[0];
		sLineRule*l_pLineRule = cEngineTestApp::m_spSlotRuleData->m_LineRuleList[l_iLindexIndex];
		int	l_iIndex = cEngineTestApp::m_spSlotRuleData->m_TableCount.x/2;
		l_iIndex = l_pLineRule->pcLineObjectIndex[l_iIndex];
		sLoopMovingObject*l_pLoopMovingObject = GetLoopMovingObjectByIndex(l_iIndex);
		sLoopMovingObject*l_pLoopMovingObject2 = GetLoopMovingObjectByIndex(l_iIndex+1);
		float	l_fOffset = (l_pLoopMovingObject2->vCurrentPos.x-l_pLoopMovingObject->vCurrentPos.x)/2;
		int	l_iPosX = (int)(l_fOffset+l_pLoopMovingObject->vCurrentPos.x);
		int	l_iPosY = (int)(l_fOffset/2+l_pLoopMovingObject->vCurrentPos.y);
		m_pNumerialImage->DrawOnCenter(e_iWinMoney,l_iPosX,l_iPosY);
	}
}


void	cWinmoneyLines::StartLines(int e_iToIndex)
{
	int	l_iSize = e_iToIndex;
	for( int i=0;i<l_iSize;++i )
	{
		this->m_pLineMPDI[i]->Init();
	}
}
float	cWinmoneyLines::GetCurrentProgress()
{
	return this->m_pLineMPDI[0]->GetCurrentProgress();
}

void	cWinmoneyLines::SetDone()
{
	if( m_iWinMoneyLines.size() )
	{
		m_iWinMoneyLines.clear();
		int	l_iSize = (int)m_pTableResult->WinMoneyLineIndexList.size();
		if( l_iSize )
		{
			for( int i=0;i<l_iSize;++i )
			{
				int	l_iLindexIndex = m_pTableResult->WinMoneyLineIndexList[i];
				this->m_pLineMPDI[l_iLindexIndex]->Init();
			}
			m_pWinMoneySpinObjectMPDI->Init(m_pTableResult);
		}
	}
}
//
//void	cWinmoneyLines::UpdateLines(int e_iToIndex,float e_fElpaseTime)
//{
//
//}
//
//void	cWinmoneyLines::RenderLines(int e_iToIndex)
//{
//
//}

bool	cWinmoneyLines::IsDone()
{
	//int	l_iSize = (int)cEngineTestApp::m_spBetBehavior->m_WinMoneyLineIndex.size();
	int	l_iSize = (int)m_iWinMoneyLines.size();
	if( l_iSize )
	{
		return false;
		return this->m_pLineMPDI[m_pTableResult->WinMoneyLineIndexList[0]]->IsDone();
	}
	if( m_pTableResult->WinMoneyLineIndexList.size() == 0 )
		return true;
	return this->m_pLineMPDI[m_pTableResult->WinMoneyLineIndexList[0]]->IsDone();
	//return false;
	return true;
}

void	cWinmoneyLines::ChangeShowLineTime(float e_fTime)
{
	for(int i =0;i<TOTAL_LINE_RESULT;++i)
	{
		if( m_pLineMPDI[i] )
		{
			m_pLineMPDI[i]->RearrangeTime(e_fTime);
		}
	}
}