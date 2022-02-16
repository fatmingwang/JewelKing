#include "stdafx.h"
#include "SlotProbabilityData.h"
#include "GameApp.h"
#include "SlotRule.h"
//from control panel
extern void	EarningOver1000Score(int e_iMoney);

cBetBehaviorResult::cBetBehaviorResult()
{
	m_iLastRoundIncome = 0;
	m_iLastOutMoney = 0;
	cEventVariable*l_pCurrentShift_TotalIncome = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CurrentShift_TotalIncome");
	cEventVariable*l_pCurrentShift_TotalPay = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CurrentShift_TotalPay");
	cEventVariable*l_pCurrentShift_Revenue = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CurrentShift_Revenue");


	cEventVariable*l_pLatestIncome = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_TotalIncome");
	cEventVariable*l_pLatestPay = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_TotalPay");
	cEventVariable*l_pLatestRevenue = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_Revenue");

	cEventVariable*l_pEventVariable3 = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"GameRecord_Combo3");
	cEventVariable*l_pEventVariable4 = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"GameRecord_Combo4");
	cEventVariable*l_pEventVariable5 = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"GameRecord_Combo5");
	cEventVariable*l_pGamePrize_Combo3 = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"GamePrize_Combo3");
	cEventVariable*l_pGamePrize_Combo4 = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"GamePrize_Combo4");
	cEventVariable*l_pGamePrize_Combo5 = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"GamePrize_Combo5");
	cEventVariable*l_p1 = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"History_TotalIncome");
	cEventVariable*l_p2 = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"History_TotalPay");
	cEventVariable*l_p3 = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"History_Revenue");
	cEventVariable*l_pProbability = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Probability");

	
	m_pfProbability = l_pProbability->GetFloat();
	m_piCurrentShift_TotalIncome = l_pCurrentShift_TotalIncome->GetInt();
	m_piCurrentShift_TotalOut = l_pCurrentShift_TotalPay->GetInt();
	m_piCurrentShift_Revenue = l_pCurrentShift_Revenue->GetInt();

	m_piLatestInMoney = l_pLatestIncome->GetInt();
	m_piLatestOutMoney = l_pLatestPay->GetInt();
	m_piLatestRevenue = l_pLatestRevenue->GetInt();


	m_pi3TotalConsecutiveCount = l_pEventVariable3->GetInt();
	m_pi4TotalConsecutiveCount = l_pEventVariable4->GetInt();
	m_pi5TotalConsecutiveCount = l_pEventVariable5->GetInt();
	m_pi3TotalConsecutiveScore = l_pGamePrize_Combo3->GetInt();
	m_pi4TotalConsecutiveScore = l_pGamePrize_Combo4->GetInt();
	m_pi5TotalConsecutiveScore = l_pGamePrize_Combo5->GetInt();
	m_piTotalInMoney = l_p1->GetInt();
	m_piTotalOutMoney = l_p2->GetInt();
	m_piRevenue = l_p3->GetInt();
	m_iStartTotalIncome = *m_piTotalInMoney;
	m_iStartTotalOut = *m_piTotalOutMoney;

	m_iCurrentRoundBetMoney = 0;
	m_iCurrentRoundOutMoney = 0;
	m_pLogFile = new cBinaryFile();
	std::wstring	l_strSystemTime = L"Log/log_";
	l_strSystemTime += UT::GetSystemTimeForFile(true);
	l_strSystemTime += L"_Consecutive.txt";
	m_pLogFile->Writefile(UT::WcharToChar(l_strSystemTime.c_str()),false,false);
}

float	cBetBehaviorResult::GetPayRate()
{
	return (float)*m_piTotalOutMoney/(*m_piTotalInMoney); 
}

cBetBehaviorResult::sTableResult::sTableResult()
{
	pbWinMoneySpinObject = 0;
	iWinMoney = 0;
	i3ConsecutiveScore = 0;
	i4ConsecutiveScore = 0;
	i5ConsecutiveScore = 0;
	i3ConsecutiveCount = 0;
	i4ConsecutiveCount = 0;
	i5ConsecutiveCount = 0;
}

cBetBehaviorResult::sTableResult::~sTableResult()
{
	SAFE_DELETE(pbWinMoneySpinObject);
}

int	cBetBehaviorResult::sTableResult::NumSpinObjectInTable(int e_iSpinObjectIndex)
{
	int	l_iSize = (int)SpinObjectResultList.size();
	int	l_iNum = 0;
	for( int i =0;i<l_iSize;++i )
	{
		if( SpinObjectResultList[i] == e_iSpinObjectIndex )
			++l_iNum;
	}
	return l_iNum;
}

int	cBetBehaviorResult::GetCurrentTablesWinMoney()
{
	size_t	l_uiSize = m_TableResultList.size();
	int	l_iWinMoney = 0;
	for( size_t i=0;i<l_uiSize;++i )
	{
		l_iWinMoney += m_TableResultList[i]->iWinMoney;
	}
	return l_iWinMoney;
}

void	cBetBehaviorResult::ClearLastTableResult()
{
	if( m_AllSpinObjectResultShowCount.size() == 0 )
	{
		if( cEngineTestApp::m_spSlotRuleData )
		{
			int	l_iSpinObjectMaximum = cEngineTestApp::m_spSlotRuleData->m_SpineObjectPrizeTableList.Count();
			m_AllSpinObjectResultShowCount.resize(l_iSpinObjectMaximum);
			if( m_WinMoneySpinObjectMatchCount.size() == 0 )
			{
				for( int i=0;i<l_iSpinObjectMaximum;++i )
				{
					std::map<int,int>l_EmptyCosecutiveCount;
					for( int j=0;j<cEngineTestApp::m_spSlotRuleData->m_TableCount.x;++j )
					{
						l_EmptyCosecutiveCount[j] = 0;
					}
					m_WinMoneySpinObjectMatchCount.push_back(l_EmptyCosecutiveCount);
				}
			}
			m_LinesWinMoneyCount.resize(cEngineTestApp::m_spSlotRuleData->m_LineRuleList.size());
		}
	}
	DELETE_VECTOR(m_TableResultList,sTableResult*);
}

void	cBetBehaviorResult::AssignTableData(int e_iTotalBetMoney)
{
	m_iCurrentRoundBetMoney = e_iTotalBetMoney;
	size_t	l_uiSize = this->m_TableResultList.size();
	//int	l_Test[5] = {0,0,0,0,0};
	for( size_t i=0;i<l_uiSize;++i )
	{
		sTableResult*l_pTableResult = m_TableResultList[i];
		size_t	l_uiSpinObjectSize = l_pTableResult->SpinObjectResultList.size();
		for( size_t j=0;j<l_uiSpinObjectSize;++j )
		{
			int	l_iIndex = l_pTableResult->SpinObjectResultList[j];
			m_AllSpinObjectResultShowCount[l_iIndex] = m_AllSpinObjectResultShowCount[l_iIndex]+1;
		}
		for(size_t j = 0;j<l_pTableResult->WinMoneyLineIndexList.size();++j)
		{
			++m_LinesWinMoneyCount[l_pTableResult->WinMoneyLineIndexList[j]];
		}

		*m_pi3TotalConsecutiveScore += l_pTableResult->i3ConsecutiveScore;
		*m_pi4TotalConsecutiveScore += l_pTableResult->i4ConsecutiveScore;
		*m_pi5TotalConsecutiveScore += l_pTableResult->i5ConsecutiveScore;
		*m_pi3TotalConsecutiveCount += l_pTableResult->i3ConsecutiveCount;
		*m_pi4TotalConsecutiveCount += l_pTableResult->i4ConsecutiveCount;
		*m_pi5TotalConsecutiveCount += l_pTableResult->i5ConsecutiveCount;
		//l_Test[2] += l_pTableResult->i3ConsecutiveCount;
		//l_Test[3] += l_pTableResult->i4ConsecutiveCount;
		//l_Test[4] += l_pTableResult->i5ConsecutiveCount;
		m_iCurrentRoundOutMoney += l_pTableResult->iWinMoney;
		size_t	l_uiWinLineSize = l_pTableResult->WinMoneyLineList.size();
		for( size_t j=0;j<l_uiWinLineSize;++j )
		{
			int	l_iMatchCount = l_pTableResult->WinMoneyLineList[j].iMatchCount;
			int	l_iWinMoneySpinObjectID = l_pTableResult->WinMoneyLineList[j].iWinMoneySpinObjectID;
			//-1 for 0 start
			++m_WinMoneySpinObjectMatchCount[l_iWinMoneySpinObjectID][l_iMatchCount-1];
		}
	}
	int	l_iRevenue = m_iCurrentRoundBetMoney-m_iCurrentRoundOutMoney;

	*m_piCurrentShift_TotalIncome +=	m_iCurrentRoundBetMoney;
	*m_piCurrentShift_TotalOut +=		m_iCurrentRoundOutMoney;
	*m_piCurrentShift_Revenue +=		l_iRevenue;

	*m_piLatestInMoney +=				m_iCurrentRoundBetMoney;
	*m_piLatestOutMoney +=				m_iCurrentRoundOutMoney;
	*m_piLatestRevenue +=				l_iRevenue;

	*m_piTotalInMoney +=				m_iCurrentRoundBetMoney;
	*m_piTotalOutMoney +=				m_iCurrentRoundOutMoney;
	*m_piRevenue +=						l_iRevenue;
	WriteConsecutiveData();
	m_iCurrentRoundOutMoney = 0;
}

void	cBetBehaviorResult::AddOutMoney(int e_iOutMoney)
{
	*m_piCurrentShift_TotalOut +=		e_iOutMoney;
	*m_piCurrentShift_Revenue -=		e_iOutMoney;

	*m_piLatestOutMoney +=				e_iOutMoney;
	*m_piLatestRevenue -=				e_iOutMoney;

	*m_piTotalOutMoney +=				e_iOutMoney;
	*m_piRevenue -=						e_iOutMoney;
	if( e_iOutMoney>1000 )
		EarningOver1000Score(e_iOutMoney);
}

void	cBetBehaviorResult::WriteConsecutiveData()
{
	int	l_iNumLine = 0;
	std::string	l_str = "\n";
	for( size_t i=0;i<m_WinMoneySpinObjectMatchCount.size();++i )
	{
		int	l_iSpinObjectID = i;
		std::map<int,int>l_EmptyCosecutiveCount;
		l_str += "ID:";
		l_str += ValueToString(l_iSpinObjectID);
		l_str += "\t";
		for( int j=0;j<cEngineTestApp::m_spSlotRuleData->m_TableCount.x;++j )
		{
			int	l_iConsecutive = j+1;
			int	l_iCount = m_WinMoneySpinObjectMatchCount[i][j];
			l_str += "Combo";
			l_str += ValueToString(l_iConsecutive);
			l_str += ":";
			l_str += ValueToString(l_iCount);
			l_str += "\t\t";
		}
		l_str += "\n";
		++l_iNumLine;
	}
	l_str += "-----------------------------------\n";
	for(size_t j = 0;j<m_LinesWinMoneyCount.size();++j)
	{
		l_str += "Line";
		l_str += ValueToString(j);
		l_str += "\tCount:";
		l_str += ValueToString(m_LinesWinMoneyCount[j]);
		l_str += "\n";
		++l_iNumLine;
	}
	l_str += "PastRound:";
	l_str += ValueToString(cEngineTestApp::m_spBetBehavior->GetRound());
	l_str += "\n";
	l_str += "StartInCome:";
	l_str += ValueToString(m_iStartTotalIncome);
	l_str += "\n";
	l_str += "StartPay:";
	l_str += ValueToString(m_iStartTotalOut);
	l_str += "\n";
	l_str += "TotalInCome:";
	l_str += ValueToString(*m_piTotalInMoney);
	l_str += "\n";
	l_str += "TotalPay:";
	l_str += ValueToString(*m_piTotalOutMoney);
	l_str += "\n";
	l_str += "Revenue:";
	l_str += ValueToString(*m_piRevenue);
	l_str += "\n";	


	m_pLogFile->SetFilePos(0);
	//+1for empty line
	for( int i=0;i<l_iNumLine+2;++i )
	{
		m_pLogFile->WriteToFileImmediatelyWithLine("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t",false);
	}
	m_pLogFile->SetFilePos(0);
	m_pLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
}
//<SpinObjectData Name="1" >
//	<Prize Count="2" Odds="1" />
//	<Prize Count="3" Odds="2" />
//	<Prize Count="4" Odds="3" />
//	<Prize Count="5" Odds="4" />
//</SpinObjectData>
cSpineObjectPrizeTable::cSpineObjectPrizeTable(TiXmlElement*e_pElement)
{
	m_iProbability = 100;
	assert(!wcscmp(e_pElement->Value(),L"SpinObjectData"));
	const WCHAR*l_strName = e_pElement->Attribute(L"Name");
	const WCHAR*l_strProbability = e_pElement->Attribute(L"Probability");
	if( l_strProbability )
	{
		m_iProbability = _wtoi(l_strProbability);
	}
	this->SetName(l_strName);
	TiXmlElement*l_pElement = e_pElement->FirstChildElement();
	while(l_pElement)
	{
		const WCHAR*l_strOdds = l_pElement->Attribute(L"Odds");
		const WCHAR*l_strCount = l_pElement->Attribute(L"Count");
		int	l_iCount = _wtoi(l_strCount);
#ifdef _DEBUG
		for( size_t i=0;i<m_CountAndOddsList.size();++i )
		{
			if( l_iCount == m_CountAndOddsList[i].cCount )
				assert(0&&"Prize Table Count exists!!");
		}
#endif
		sCountAndOdds	l_CountAndOdds(l_iCount,_wtoi(l_strOdds));
		m_CountAndOddsList.push_back(l_CountAndOdds);
		l_pElement = l_pElement->NextSiblingElement();
	}
}

int	cSpineObjectPrizeTable::GetOddsByCount(int e_iCount)
{
	int	l_iSize = (int)m_CountAndOddsList.size();
	for( int i=0;i<l_iSize;++i )
	{
		if(m_CountAndOddsList[i].cCount == e_iCount)
			return m_CountAndOddsList[i].iOdds;
	}
	return 0;
}

int	cSpineObjectPrizeTable::GetMaxOdds()
{
	int	l_iSize = (int)m_CountAndOddsList.size();
	int	l_iOdds = 0;
	for( int i=0;i<l_iSize;++i )
	{
		if(m_CountAndOddsList[i].iOdds > l_iOdds)
			l_iOdds = m_CountAndOddsList[i].iOdds;
	}
	return l_iOdds;
}

int	cSpineObjectPrizeTable::GetMinOdds()
{
	int	l_iSize = (int)m_CountAndOddsList.size();
	int	l_iOdds = INT_MAX;
	for( int i=0;i<l_iSize;++i )
	{
		if(m_CountAndOddsList[i].iOdds < l_iOdds)
			l_iOdds = m_CountAndOddsList[i].iOdds;
	}
	return l_iOdds;
}

void	cSpinObjectProbability::CalculateTotalProbabilityAmount()
{
	m_iTotalProbabilityAmount = 0;
	int	l_iCount = Count();
	m_iMaxScore = -1;
	m_iMinScore = INT_MAX;
	for( int i=0;i<l_iCount;++i )
	{
		m_iTotalProbabilityAmount += this->m_ObjectList[i]->m_iProbability;
		int	l_iOdds = this->m_ObjectList[i]->GetMaxOdds();
		if( m_iMaxScore<l_iOdds )
		{
			m_iMaxScore = l_iOdds;
			m_iMaxScoreSpinObjectIndex = i;
		}
		l_iOdds = this->m_ObjectList[i]->GetMinOdds();
		if( m_iMinScore>l_iOdds&& l_iOdds !=0 )
		{
			m_iMinScore = l_iOdds;
			m_iMinScoreSpinObjectIndex = i;
		}
	}
}

cSpinObjectProbability::cSpinObjectProbability()
{
	m_iMinScore = 0;
	m_iMaxScore = 0;
	m_iMinScoreSpinObjectIndex = -1;
	m_iMaxScoreSpinObjectIndex = -1;
	m_iTotalProbabilityAmount = 0;
}


int	cSpinObjectProbability::GetSpinObjectIndexByProbability()
{
	int	l_iProbability = rand()%m_iTotalProbabilityAmount;
	return GetSpinObjectIndexByProbability(l_iProbability);
}

int		cSpinObjectProbability::GetSpinObjectIndexByProbability(int e_iProbability)
{
	int	l_iCount = Count();
	int	l_iProbability = 0;
	for( int i=0;i<l_iCount;++i )
	{
		l_iProbability += this->m_ObjectList[i]->m_iProbability;
		if( l_iProbability >= e_iProbability )
		{
			return i;
		}
	}
	return 0;
}