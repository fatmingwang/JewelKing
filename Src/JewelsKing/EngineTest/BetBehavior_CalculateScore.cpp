#include "stdafx.h"
#include "SlotRule.h"
#include "PlayerData.h"
#include "BetBehavior.h"
#include "SlotObject.h"
#include "GameApp.h"
#include "WinMoneyLine.h"
#include "PhaseDefine.h"
#include "GameNetwork.h"

std::vector<cBetBehavior::sNameAndCount>	cBetBehavior::GetNameAndCountByLineIndexByConsecutive(int e_iIndex,cBetBehaviorResult::sTableResult*e_pTableResult)
{
	std::vector<sNameAndCount>	l_NameAndCountList;
	int	l_iWinMoney = 0;
	sLineRule*l_pLineRule = m_pSlotRuleData->m_LineRuleList[e_iIndex];
	int	l_iLineObjectCount = l_pLineRule->cOneLineCount;
	//do a very quick search to find outeach object's count
	for( int j=0;j<l_iLineObjectCount;++j )
	{
		int	l_iSpinObjectIndex = l_pLineRule->pcLineObjectIndex[j];
		int	l_iSpinObjectResultIndex = e_pTableResult->SpinObjectResultList[l_iSpinObjectIndex];
		//-1 to skip.
		if( l_iSpinObjectResultIndex == -1 )
			continue;
		std::wstring	l_strTargetName = this->m_pSlotRuleData->m_SpineObjectPrizeTableList.GetObject(l_iSpinObjectResultIndex)->GetName();
		int	l_iExistCount = (int)l_NameAndCountList.size();
		int	l_iSameCount = 0;
		for( int k=0;k<l_iExistCount;++k )
		{
			const WCHAR*l_str = l_NameAndCountList[k].strName.c_str();
			if(!wcscmp(l_str,l_strTargetName.c_str()))
			{
				++l_iSameCount;
				++l_NameAndCountList[k].iCount;
				l_NameAndCountList[k].WinMoneySpinObjectIndex.push_back(l_iSpinObjectIndex);
				l_NameAndCountList[k].IndexOfLine.push_back(j);
			}
		}
		if( l_iSameCount == 0 )
		{
			sNameAndCount	l_sNameAndCount;
			l_sNameAndCount.iCount = 1;
			l_sNameAndCount.strName = l_strTargetName;
			l_sNameAndCount.WinMoneySpinObjectIndex.push_back(l_iSpinObjectIndex);
			l_sNameAndCount.IndexOfLine.push_back(j);
			l_NameAndCountList.push_back(l_sNameAndCount);
		}
	}
	//ensure this line has bet the mony or skip this
	if( e_iIndex<this->m_iNumLinesBet )
	{
		//now check count and get win money
		int	l_iNameAndCountListSize  = (int)l_NameAndCountList.size();
		//do the special spingobject
		if( m_pSlotRuleData->m_strSpecilaSpinObjectName.length() )
		{
			sNameAndCount	l_SpecialSpinObjectameAndCount;
			const WCHAR*l_strSpecialSpinObject = m_pSlotRuleData->m_strSpecilaSpinObjectName.c_str();
			int	l_iNumSpecialSpinObject = 0;
			for( int j=0;j<l_iNameAndCountListSize;++j  )
			{
				if( !wcscmp(l_NameAndCountList[j].strName.c_str(),l_strSpecialSpinObject) )
				{
					l_SpecialSpinObjectameAndCount = l_NameAndCountList[j];
					l_iNumSpecialSpinObject = l_NameAndCountList[j].iCount;
					l_NameAndCountList.erase(l_NameAndCountList.begin()+j);
					l_iNameAndCountListSize  = (int)l_NameAndCountList.size();
					break;
				}
			}
			if( l_iNumSpecialSpinObject  )
			{
				for( int j=0;j<l_iNameAndCountListSize;++j  )
				{
					sNameAndCount*l_pNameAndCount = &l_NameAndCountList[j];
					l_pNameAndCount->iCount += l_iNumSpecialSpinObject;
					for(int k=0;k<(int)l_SpecialSpinObjectameAndCount.WinMoneySpinObjectIndex.size();++k)
					{
						l_pNameAndCount->WinMoneySpinObjectIndex.push_back(l_SpecialSpinObjectameAndCount.WinMoneySpinObjectIndex[k]);
						l_pNameAndCount->IndexOfLine.push_back(l_SpecialSpinObjectameAndCount.IndexOfLine[k]);
					}
					
				}
			}
		}
	}
	return l_NameAndCountList;
}

cBetBehavior::sNameAndCount*	cBetBehavior::GetMatchCountAndNameAndCountData(std::vector<sNameAndCount>*e_pNameAndCountList,int e_iIndex)
{
	sLineRule*l_pLineRule = m_pSlotRuleData->m_LineRuleList[e_iIndex];
	//now check count and get win money
	int	l_iNameAndCountListSize  = (int)e_pNameAndCountList->size();
	for( int j=0;j<l_iNameAndCountListSize;++j  )
	{
		cSpineObjectPrizeTable*l_pSpineObjectPrizeTable = this->m_pSlotRuleData->m_SpineObjectPrizeTableList.GetObject((*e_pNameAndCountList)[j].strName.c_str());
		if( !l_pSpineObjectPrizeTable )
		{
			UT::ErrorMsg((WCHAR*)(*e_pNameAndCountList)[j].strName.c_str(),L"no this object name in prize table");
		}
		sNameAndCount*l_pNameAndCount = &(*e_pNameAndCountList)[j];
		l_pNameAndCount->SortIndex();
		//spinobject count could not same as consecutive count,and the consecutive are wee need.
		int	l_iMatchCount = 1;
		int	l_iStartIndex = l_pNameAndCount->IndexOfLine[0];
		if( l_iStartIndex !=0  )
			continue;
		int	l_iWinMoneySpinObjectIndexSize = l_pNameAndCount->iCount;
		for( int k=1;k<l_iWinMoneySpinObjectIndexSize;++k )
		{
			int	l_iIndex = l_pNameAndCount->IndexOfLine[k];
			if( l_iStartIndex +1 != l_iIndex)
			{
				break;
			}
			++l_iStartIndex;
			++l_iMatchCount;
		}
		int	l_iOdds = l_pSpineObjectPrizeTable->GetOddsByCount(l_iMatchCount);
		if( l_iOdds>0 )
		{
			l_pNameAndCount->WinMoneySpinObjectIndex.clear();
			for( int k=0;k<l_iMatchCount;++k )
			{
				int	l_iSpinObjectIndex = l_pLineRule->pcLineObjectIndex[k];
				l_pNameAndCount->WinMoneySpinObjectIndex.push_back(l_iSpinObjectIndex);
			}
			l_pNameAndCount->iCount = l_iMatchCount;
			//int	l_iLineWinMoney = (int)(l_iOdds*m_iBonusMoneyRate)*m_iPerLineBetMoney;
			//std::wstring	l_OutputData = L"Line:";
			//l_OutputData += ValueToStringW(e_iIndex);
			//l_OutputData += L",Name:";
			//l_OutputData += l_pNameAndCount->strName.c_str();
			//l_OutputData += L",MatchCount";
			//l_OutputData += ValueToStringW(l_pNameAndCount->iCount);
			//l_OutputData += L",Index:";
			//for( int k=0;k<l_pNameAndCount->iCount;++k )
			//{
			//	int	l_iSpinObjectIndex = l_pNameAndCount->WinMoneySpinObjectIndex[k];
			//	l_OutputData += ValueToStringW(l_iSpinObjectIndex);
			//	l_OutputData += L",";
			//}
			//printf(UT::WcharToChar(l_OutputData.c_str()));
			//printf(UT::WcharToChar(L"\n"));
			return l_pNameAndCount;
		}
	}
	return 0;
}

void	cBetBehavior::CalculateScoreConsecutive(cBetBehaviorResult::sTableResult*e_pTableResult)
{
	int	l_iLineRuleListSize = (int)m_pSlotRuleData->m_LineRuleList.size();
	int	l_iTotalSpinObjectCount = m_pSlotRuleData->m_TableCount.x*m_pSlotRuleData->m_TableCount.y;
	if( l_iLineRuleListSize )
	{
		for( int i=0;i<l_iLineRuleListSize;++i )
		{
			std::vector<sNameAndCount>	l_NameAndCountList = GetNameAndCountByLineIndexByConsecutive(i,e_pTableResult);
			sLineRule*l_pLineRule = m_pSlotRuleData->m_LineRuleList[i];
			cBetBehaviorResult::sTableResult::sLine*l_pLine = &e_pTableResult->AllLineList[i];
			l_pLine->iIndex = i;			
			//ensure this line has bet the mony or skip this
			if( i<this->m_iNumLinesBet )
			{
				int	l_iNameAndCountListSize  = (int)l_NameAndCountList.size();
				cBetBehavior::sNameAndCount*l_pNameAndCount = GetMatchCountAndNameAndCountData(&l_NameAndCountList,i);
				if( !l_pNameAndCount )
					continue;
				int	l_iWinMoneySpinObjectID = this->m_pSlotRuleData->m_SpineObjectPrizeTableList.GetObjectIndexByName(l_pNameAndCount->strName.c_str());
				cSpineObjectPrizeTable*l_pSpineObjectPrizeTable = this->m_pSlotRuleData->m_SpineObjectPrizeTableList.GetObject(l_iWinMoneySpinObjectID);
				int	l_iOdds = l_pSpineObjectPrizeTable->GetOddsByCount(l_pNameAndCount->iCount);
				int	l_iLineWinMoney = l_iOdds*m_iBonusMoneyRate*m_iPerLineBetMoney;
				l_pLine->iWinMoney = l_iLineWinMoney;
				e_pTableResult->iWinMoney += l_iLineWinMoney;
				for( int k=0;k<l_pNameAndCount->iCount;++k )
				{
					int	l_iSpinObjectIndex = l_pLineRule->pcLineObjectIndex[k];
					l_pLine->WinMoneySpinObjectIndexList.push_back(l_iSpinObjectIndex);
					e_pTableResult->pbWinMoneySpinObject[l_iSpinObjectIndex] = true;
				}
				l_pLine->iWinMoneySpinObjectID = l_iWinMoneySpinObjectID;
				l_pLine->iMatchCount = l_pNameAndCount->iCount;
				e_pTableResult->WinMoneyLineList.push_back(*l_pLine);
				e_pTableResult->WinMoneyLineIndexList.push_back(i);
				if( l_pNameAndCount->iCount == 3 )
				{
					e_pTableResult->i3ConsecutiveCount +=1;
					e_pTableResult->i3ConsecutiveScore += l_iLineWinMoney;
				}
				else
				if( l_pNameAndCount->iCount == 4 )
				{
					e_pTableResult->i4ConsecutiveCount +=1;
					e_pTableResult->i4ConsecutiveScore += l_iLineWinMoney;
				}
				else
				if( l_pNameAndCount->iCount == 5 )
				{
					e_pTableResult->i5ConsecutiveCount += 1;
					e_pTableResult->i5ConsecutiveScore += l_iLineWinMoney;
				}
			}
			else//play havent bet after this line.
			{
				return;
			}
		}
	}
}

void	cBetBehavior::CalculateScoreByLine()
{
	//1st:find out all items on the result
	//2nd:check slot lint rule
	//3rd calculate score
	//
	int	l_iSize = (int)m_pSlotRuleData->m_LineRuleList.size();
	int	l_iTotalSpinObjectCount = m_pSlotRuleData->m_TableCount.x*m_pSlotRuleData->m_TableCount.y;
	if( l_iSize )
	{
		for( int i=0;i<l_iSize;++i )
		{
			struct	sNameAndCount
			{
				std::wstring	strName;
				int				iCount;
				std::vector<int>	WinMoneySpinObjectIndex;
			};
			std::vector<sNameAndCount>	l_NameAndCountList;
			int	l_iWinMoney = 0;
			sLineRule*l_pLineRule = m_pSlotRuleData->m_LineRuleList[i];
			int	l_iLineObjectCount = l_pLineRule->cOneLineCount;
			//do a very quick search to find outeach object's count
			for( int j=0;j<l_iLineObjectCount;++j )
			{
				int	l_iSpinObjectIndex = l_pLineRule->pcLineObjectIndex[j];
				std::wstring	l_strTargetName = GetSpinObjectNameByIndex(l_iSpinObjectIndex);
				int	l_iExistCount = (int)l_NameAndCountList.size();
				int	l_iSameCount = 0;
				for( int k=0;k<l_iExistCount;++k )
				{
					const WCHAR*l_str = l_NameAndCountList[k].strName.c_str();
					if(!wcscmp(l_str,l_strTargetName.c_str()))
					{
						++l_iSameCount;
						++l_NameAndCountList[k].iCount;
						l_NameAndCountList[k].WinMoneySpinObjectIndex.push_back(l_iSpinObjectIndex);
					}
				}
				if( l_iSameCount == 0 )
				{
					sNameAndCount	l_sNameAndCount;
					l_sNameAndCount.iCount = 1;
					l_sNameAndCount.strName = l_strTargetName;
					l_sNameAndCount.WinMoneySpinObjectIndex.push_back(l_iSpinObjectIndex);
					l_NameAndCountList.push_back(l_sNameAndCount);
				}
			}
			//ensure this line has bet the mony or skip this
			if( i<this->m_iNumLinesBet )
			{
				//now check count and get win money
				int	l_iNameAndCountListSize  = (int)l_NameAndCountList.size();

				//do the special spingobject
				if( m_pSlotRuleData->m_strSpecilaSpinObjectName.length() )
				{
					sNameAndCount	l_SpecialSpinObjectameAndCount;
					const WCHAR*l_strSpecialSpinObject = m_pSlotRuleData->m_strSpecilaSpinObjectName.c_str();
					int	l_iNumSpecialSpinObject = 0;
					for( int j=0;j<l_iNameAndCountListSize;++j  )
					{
						if( !wcscmp(l_NameAndCountList[j].strName.c_str(),l_strSpecialSpinObject) )
						{
							l_SpecialSpinObjectameAndCount = l_NameAndCountList[j];
							l_iNumSpecialSpinObject = l_NameAndCountList[j].iCount;
							l_NameAndCountList.erase(l_NameAndCountList.begin()+j);
							l_iNameAndCountListSize  = (int)l_NameAndCountList.size();
							break;
						}
					}
					if( l_iNumSpecialSpinObject  )
					{
						for( int j=0;j<l_iNameAndCountListSize;++j  )
						{
							sNameAndCount*l_pNameAndCount = &l_NameAndCountList[j];
							l_pNameAndCount->iCount += l_iNumSpecialSpinObject;
							for(int k=0;k<(int)l_SpecialSpinObjectameAndCount.WinMoneySpinObjectIndex.size();++k)
								l_pNameAndCount->WinMoneySpinObjectIndex.push_back(l_SpecialSpinObjectameAndCount.WinMoneySpinObjectIndex[k]);
							
						}
					}
				}

				for( int j=0;j<l_iNameAndCountListSize;++j  )
				{
					cSpineObjectPrizeTable*l_pSpineObjectPrizeTable = this->m_pSlotRuleData->m_SpineObjectPrizeTableList.GetObject(l_NameAndCountList[j].strName.c_str());
					if( !l_pSpineObjectPrizeTable )
					{
						UT::ErrorMsg((WCHAR*)l_NameAndCountList[j].strName.c_str(),L"no this object name in prize table");
					}

					int	l_iOdds = l_pSpineObjectPrizeTable->GetOddsByCount(l_NameAndCountList[j].iCount);
					l_iWinMoney += l_iOdds*m_iBonusMoneyRate;
				}
				//now we have know witch line index got money
				if( l_iWinMoney !=0 )
				{
					l_iWinMoney *= m_iPerLineBetMoney;
				}
			}
			else
			//play havent bet after this line.
			{
				return;
			}
		}
	}
}

void	cBetBehavior::AssignLinesData(cBetBehaviorResult::sTableResult*e_pTableResult)
{
	int	l_iNumSpinObject = e_pTableResult->SpinObjectResultList.size();
	size_t	l_iLines = cEngineTestApp::m_spSlotRuleData->m_LineRuleList.size();
	if(!e_pTableResult->pbWinMoneySpinObject)
		e_pTableResult->pbWinMoneySpinObject = new bool[l_iNumSpinObject];
	memset(e_pTableResult->pbWinMoneySpinObject,0,sizeof(bool)*l_iNumSpinObject);
	e_pTableResult->AllLineList.clear();
	e_pTableResult->AllLineList.reserve(l_iLines);
	e_pTableResult->WinMoneyLineList.clear();
	e_pTableResult->WinMoneyLineIndexList.clear();
	e_pTableResult->iWinMoney = 0;
	e_pTableResult->i3ConsecutiveCount = 0;
	e_pTableResult->i3ConsecutiveScore = 0;
	e_pTableResult->i4ConsecutiveCount = 0;
	e_pTableResult->i4ConsecutiveScore = 0;
	e_pTableResult->i5ConsecutiveCount = 0;
	e_pTableResult->i5ConsecutiveScore = 0;
	for( size_t i = 0;i<l_iLines;++i )
	{
		cBetBehaviorResult::sTableResult::sLine	l_Line;
		l_Line.iIndex = i;
		sLineRule*l_pLineRule = cEngineTestApp::m_spSlotRuleData->m_LineRuleList[i];
		char	l_cNum = l_pLineRule->cOneLineCount;
		for( char j = 0;j<l_cNum;++j )
		{
			assert(l_pLineRule->pcLineObjectIndex[j]<=l_iNumSpinObject&&"table index out of range");
			int	l_iIndex = e_pTableResult->SpinObjectResultList[l_pLineRule->pcLineObjectIndex[j]];
			l_Line.SpinObjectResultList.push_back(l_iIndex);
		}
		e_pTableResult->AllLineList.push_back(l_Line);
	}
}
