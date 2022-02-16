#include "stdafx.h"
//#include "SlotRule.h"
//#include "PlayerData.h"
//#include "BetBehavior.h"
//#include "SlotObject.h"
//#include "GameApp.h"
//cBetBehavior::cBetBehavior(cSlotRuleData*e_pSlotRuleData,cPlayerData*e_pPlayerData)
//{
//	m_iRound = 0;
//	m_pSlotRuleData = e_pSlotRuleData;
//	m_pPlayerData = e_pPlayerData;
//	m_iNumLinesBet = 0;
//	m_iPerLineBetMoney = 0;
//	m_iTotalBet = 0;
//	m_iWinMoney = 0;
//	m_iPerLineBetMoney = 1;
//#ifdef _DEBUG
//	int	l_iMoney = 0;
//	for( size_t i=0;i<e_pSlotRuleData->m_PerLineBetMoneyRule.size();++i )
//	{
//		assert(l_iMoney<e_pSlotRuleData->m_PerLineBetMoneyRule[i]);
//		l_iMoney = e_pSlotRuleData->m_PerLineBetMoneyRule[i];
//	}
//#endif
//	m_piInfoValue[eIPL_TotalBetMoneyPos] = &this->m_iTotalBet;
//	m_piInfoValue[eIPL_WinMoneyPos] = &this->m_iWinMoney;
//	m_piInfoValue[eIPL_NumLineBetPos] = &this->m_iNumLinesBet;
//	m_piInfoValue[eIPL_PerLineBetMoneyPos] = &this->m_iPerLineBetMoney;
//	m_piInfoValue[eIPL_PlayerMoneyPos] = &this->m_pPlayerData->m_iMoney;
//	m_pButtonClick = 0;
//	if(cGameApp::m_spSoundParser->AddStaticSound("JewelsKing/Sound/Button.wav"))
//	{
//		m_pButtonClick = cGameApp::m_spSoundParser->GetObject(L"Button");
//	}
//}
//cBetBehavior::~cBetBehavior()
//{
//
//}
//
//void	cBetBehavior::Init()
//{
//	m_iNumLinesBet = 1;
//	m_iPerLineBetMoney = m_pSlotRuleData->m_PerLineBetMoneyRule[0];
//	m_iTotalBet = m_iNumLinesBet*m_iPerLineBetMoney;
//}
//
//void	cBetBehavior::AddLineBet()
//{
//	if( m_pButtonClick )
//		m_pButtonClick->Play(true);
//	int	l_iSize =  (int)this->m_pSlotRuleData->m_LineRuleList.size();
//	int	l_iNewLineBetCount = m_iNumLinesBet+1;
//	if( l_iNewLineBetCount <=l_iSize )
//	{
//		int	l_iTotalBet = l_iNewLineBetCount*this->m_iPerLineBetMoney;
//		if( l_iNewLineBetCount *this->m_iPerLineBetMoney <=  this->m_pPlayerData->m_iMoney )
//		{
//			m_iNumLinesBet = l_iNewLineBetCount;
//			this->m_iTotalBet = l_iTotalBet;
//		}
//	}
//	else
//	{//play error sound
//	
//	}
//}
//
//void	cBetBehavior::MinusLineBet()
//{
//	if( m_pButtonClick )
//		m_pButtonClick->Play(true);
//	int	l_iNewLineBetCount = m_iNumLinesBet-1;
//	if( l_iNewLineBetCount >0 )
//	{
//		m_iNumLinesBet = l_iNewLineBetCount;
//		m_iTotalBet = m_iNumLinesBet*m_iPerLineBetMoney;
//	}
//	else
//	{//play error sound
//	
//	}
//	
//}
//
//int		cBetBehavior::GetFocusBetMoneyIndexInVector()
//{
//	int	l_iSize = (int)this->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
//	for( int i=0;i<l_iSize;++i )
//	{
//		if( this->m_iPerLineBetMoney == this->m_pSlotRuleData->m_PerLineBetMoneyRule[i] )
//			return i;
//	}
//	return -1;
//}
//
//void	cBetBehavior::AddBetMoney()
//{
//	if( m_pButtonClick )
//		m_pButtonClick->Play(true);
//	int	l_iNewBetMoney = 0;
//	int	l_iNewIndex = GetFocusBetMoneyIndexInVector()+1;
//	int	l_iSize = (int)this->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
//	if( l_iNewIndex == l_iSize )
//		return;
//	l_iNewBetMoney = this->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iNewIndex];
//	int	l_iTotalBetMoney = l_iNewBetMoney*this->m_iNumLinesBet;
//	if(this->m_pPlayerData->m_iMoney>=l_iTotalBetMoney)
//	{
//		this->m_iTotalBet = l_iTotalBetMoney;
//		this->m_iPerLineBetMoney = l_iNewBetMoney;
//		return;
//	}
//	else
//	{//play error message
//
//	}
//}
//
//void	cBetBehavior::MinusBetMoney()
//{
//	if( m_pButtonClick )
//		m_pButtonClick->Play(true);
//	int	l_iNewBetMoney = 0;
//	int	l_iNewIndex = GetFocusBetMoneyIndexInVector()-1;
//	int	l_iSize = (int)this->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
//	if( l_iNewIndex <0 )
//	{//play error message
//		return;
//	}
//	l_iNewBetMoney = this->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iNewIndex];
//	int	l_iTotalBetMoney = l_iNewBetMoney*this->m_iNumLinesBet;
//	this->m_iTotalBet = l_iTotalBetMoney;
//	this->m_iPerLineBetMoney = l_iNewBetMoney;
//}
//
//void	cBetBehavior::BetMax()
//{
//	if( m_pButtonClick )
//		m_pButtonClick->Play(true);
//	int	l_iSize = (int)this->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
//	this->m_iNumLinesBet = (int)this->m_pSlotRuleData->m_LineRuleList.size();
//	this->m_iPerLineBetMoney = this->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iSize-1];
//	this->m_iTotalBet = m_iPerLineBetMoney*m_iNumLinesBet;
//}
//
//void	cBetBehavior::Update(float e_fElpaseTime)
//{
//
//}
//
//void	cBetBehavior::Render()
//{
//	for( int i=0;i<eIPL_MAX;++i )
//	{
//		WCHAR*l_strValue = ValueToStringW(*m_piInfoValue[i]);
//		cGameApp::m_spGlyphFontRender->RenderFont(m_vInfoFontPos[i].x,m_vInfoFontPos[i].y,l_strValue);
//	}
//}
//
//void	cBetBehavior::DebugRender()
//{
//
//}
//
//void	cBetBehavior::MouseDown(int e_iX,int e_iY)
//{
//
//}
//
//void	cBetBehavior::MouseUp(int e_iX,int e_iY)
//{
//
//}
//
//void	cBetBehavior::MouseMove(int e_iX,int e_iY)
//{
//
//}
//
//void	cBetBehavior::CalculateScoreWithRound()
//{
//	++m_iRound;
//	m_WinMoneyLineIndex.clear();
//	m_iWinMoney = 0;
//	CalculateScore();
//}
//
//void	cBetBehavior::CalculateScore()
//{
//	m_WinMoneyLineIndex.clear();
//	//1st:find out all items on the result
//	//2nd:check slot lint rule
//	//3rd calculate score
//	//
//	int	l_iSize = (int)m_pSlotRuleData->m_LineRuleList.size();
//	if( l_iSize )
//	{
//		int	l_iCount = cEngineTestApp::m_spSlotLineObjectManagerList->Count();
//		std::vector<int>	m_EachLineStartIndex;
//		std::vector<int>	m_EachLineSpinObjectCount;
//		for( int i=0;i<l_iCount;++i )
//		{
//			cSlotObjectManager*l_pSlotObjectManager = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(i);
//			m_EachLineStartIndex.push_back(l_pSlotObjectManager->GetStopAtTopShowResultIndex());
//			m_EachLineSpinObjectCount.push_back(l_pSlotObjectManager->Count());
//		}
//		std::vector<wstring>	l_strInSearchLineObjectName;
//		int	l_iSize = (int)m_pSlotRuleData->m_LineRuleList.size();
//		for( int i=0;i<l_iSize;++i )
//		{
//			struct	sNameAndCount
//			{
//				std::wstring	strName;
//				int				iCount;
//			};
//			std::vector<sNameAndCount>	l_NameAndCountList;
//			int	l_iWinMoney = 0;
//			sLineRule*l_pLineRule = m_pSlotRuleData->m_LineRuleList[i];
//			int	l_iLineObjectCount = l_pLineRule->cOneLineCount;
//			//do a very quick search to find outeach object's count
//			for( int j=0;j<l_iLineObjectCount;++j )
//			{
//				std::wstring	l_strTargetName = GetSpinObjectNameByIndex(cEngineTestApp::m_spSlotLineObjectManagerList,l_pLineRule->pcLineObjectIndex[j]);
//				l_strInSearchLineObjectName.push_back(l_strTargetName);
//				assert(l_strInSearchLineObjectName[l_strInSearchLineObjectName.size()-1].length()>0);
//				int	l_iExistCount = (int)l_NameAndCountList.size();
//				int	l_iSameCount = 0;
//				for( int k=0;k<l_iExistCount;++k )
//				{
//					const WCHAR*l_str = l_NameAndCountList[k].strName.c_str();
//					if(!wcscmp(l_str,l_strTargetName.c_str()))
//					{
//						++l_iSameCount;
//						++l_NameAndCountList[k].iCount;
//					}
//				}
//				if( l_iSameCount == 0 )
//				{
//					sNameAndCount	l_sNameAndCount;
//					l_sNameAndCount.iCount = 1;
//					l_sNameAndCount.strName = l_strTargetName;
//					l_NameAndCountList.push_back(l_sNameAndCount);
//				}
//			}
//			//ensure this line has bet the mony or skip this
//			if( i<this->m_iNumLinesBet )
//			{
//				//now check count and get win money
//				int	l_iNameAndCountListSize  = (int)l_NameAndCountList.size();
//				for( int j=0;j<l_iNameAndCountListSize;++j  )
//				{
//					cSpineObjectPrizeTable*l_pSpineObjectPrizeTable = this->m_pSlotRuleData->m_SpineObjectPrizeTableList.GetObject(l_NameAndCountList[j].strName.c_str());
//					if( !l_pSpineObjectPrizeTable )
//					{
//						UT::ErrorMsg((WCHAR*)l_NameAndCountList[j].strName.c_str(),L"no this object name in prize table");
//					}
//					int	l_iOdds = l_pSpineObjectPrizeTable->GetOddsByCount(l_NameAndCountList[j].iCount);
//					l_iWinMoney += (int)(m_iPerLineBetMoney*l_iOdds);
//					
//				}
//				//now we have know witch line index got money
//				if( l_iWinMoney !=0 )
//				{
//					m_WinMoneyLineIndex.push_back(i);
//					l_iWinMoney *= m_iPerLineBetMoney;
//					m_iWinMoney += l_iWinMoney;
//				}
//			}
//			else
//			//play havent bet after this line.
//			{
//				return;
//			}
//		}
//	}
//}
//
//void	cBetBehavior::SetupInfoPos(cMPDI*e_pMPDI)
//{
//	WCHAR*l_str[] = {
//		L"TotalBetMoneyPos",
//		L"WinMoneyPos",
//		L"NumLineBetPos",
//		L"PerLineBetMoneyPos",
//		L"PlayerMoneyPos",
//	};
//	for( int i=0;i<eIPL_MAX;++i )
//	{
//		cSubMPDI*l_p = e_pMPDI->GetObject(l_str[i]);
//		m_vInfoFontPos[i] = l_p->GetOriginalPointList()[0];
//		e_pMPDI->RemoveObject(l_p->GetName());
//	}
//}