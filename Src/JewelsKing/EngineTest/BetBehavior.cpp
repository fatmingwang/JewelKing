#include "stdafx.h"
#include "SlotRule.h"
#include "PlayerData.h"
#include "BetBehavior.h"
#include "SlotObject.h"
#include "GameApp.h"
#include "WinMoneyLine.h"
#include "PhaseDefine.h"
#include "GameNetwork.h"
#include "SlotTableGenerator.h"
extern void	EarningOver1000Score(int e_iMoney);
cBetBehavior::cBetBehavior(cSlotRuleData*e_pSlotRuleData,cPlayerData*e_pPlayerData)
{
	//m_pBetBehaviorResult = 0;
	m_pBetBehaviorResult = new cBetBehaviorResult();
	m_iBonusMoneyRate = 1;
	m_pNumerialImage = 0;
	m_iRound = 0;
	m_pSlotRuleData = e_pSlotRuleData;
	m_pPlayerData = e_pPlayerData;
	m_iNumLinesBet = 1;
	m_iTotalBet = 0;
	m_iWinMoney = 0;
	m_iPerLineBetMoney = 1;
#ifdef _DEBUG
	int	l_iMoney = 0;
	for( size_t i=0;i<e_pSlotRuleData->m_PerLineBetMoneyRule.size();++i )
	{
		assert(l_iMoney<e_pSlotRuleData->m_PerLineBetMoneyRule[i]);
		l_iMoney = e_pSlotRuleData->m_PerLineBetMoneyRule[i];
	}
#endif
	m_piInfoValue[eIPL_TotalBetMoneyPos] = &this->m_iTotalBet;
	m_piInfoValue[eIPL_WinMoneyPos] = &this->m_iWinMoney;
	m_piInfoValue[eIPL_NumLineBetPos] = &this->m_iNumLinesBet;
	m_piInfoValue[eIPL_PerLineBetMoneyPos] = &this->m_iPerLineBetMoney;
	m_piInfoValue[eIPL_PlayerMoneyPos] = &this->m_pPlayerData->m_iMoney;
	m_piInfoValue[eIPL_JP1] = this->m_pPlayerData->GetJP(eJPT_NETWORK);
	m_piInfoValue[eIPL_JP2] = this->m_pPlayerData->GetJP(eJPT_LOCAL);
	m_pButtonClick = 0;
	m_pButtonClick = cGameApp::m_spSoundParser->GetObject(BET_SOUND_NAME);
	m_pWinmoneyLines = 0;
}

cBetBehavior::~cBetBehavior()
{
	SAFE_DELETE(m_pBetBehaviorResult);
	SAFE_DELETE(m_pNumerialImage);
	SAFE_DELETE(m_pWinmoneyLines);
}

void	cBetBehavior::Init()
{
	int	l_iTotalSpinObjectCount = m_pSlotRuleData->m_TableCount.x*m_pSlotRuleData->m_TableCount.y;
	int	l_iLineRuleSize =  (int)this->m_pSlotRuleData->m_LineRuleList.size();
	m_iNumLinesBet = 1;
	m_iPerLineBetMoney = m_pSlotRuleData->m_PerLineBetMoneyRule[0];
	m_iTotalBet = m_iNumLinesBet*m_iPerLineBetMoney;
	if( !m_pNumerialImage )
	{
		cPuzzleImage*l_pPI = cGameApp::GetPuzzleImageByFileName(L"JewelsKing/ImageData/Num.pi");
		m_pNumerialImage = l_pPI->GetNumerialImageByName(L"but_0",L"but_9");
	}
	m_pWinmoneyLines = new cWinmoneyLines(RESULT_LINES_MPDILIST_FILE_NAME);
}

void	cBetBehavior::SetBetData(int e_iNumLineBet,int e_iPerLineBetMoney)
{
	m_iNumLinesBet = e_iNumLineBet;
	m_iPerLineBetMoney = e_iPerLineBetMoney;
	m_iTotalBet = m_iNumLinesBet*this->m_iPerLineBetMoney;
}

void	cBetBehavior::AddLineBet()
{
	if( m_pButtonClick )
		m_pButtonClick->Play(true);
	int	l_iSize =  (int)this->m_pSlotRuleData->m_LineRuleList.size();
	int	l_iNewLineBetCount = m_iNumLinesBet+1;
	if( l_iNewLineBetCount > l_iSize )
	{
		l_iNewLineBetCount = 1;
	}
	SetBetData(l_iNewLineBetCount,m_iPerLineBetMoney);
	m_pWinmoneyLines->StartLines(m_iNumLinesBet);
}

void	cBetBehavior::MinusLineBet()
{
	if( m_pButtonClick )
		m_pButtonClick->Play(true);
	int	l_iNewLineBetCount = m_iNumLinesBet-1;
	if( l_iNewLineBetCount <= 0 )
	{
		l_iNewLineBetCount = (int)this->m_pSlotRuleData->m_LineRuleList.size();
	}
	SetBetData(l_iNewLineBetCount,m_iPerLineBetMoney);
	m_pWinmoneyLines->StartLines(m_iNumLinesBet);
}

int		cBetBehavior::GetFocusBetMoneyIndexInVector()
{
	int	l_iSize = (int)this->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	for( int i=0;i<l_iSize;++i )
	{
		if( this->m_iPerLineBetMoney == this->m_pSlotRuleData->m_PerLineBetMoneyRule[i] )
			return i;
	}
	return -1;
}

void	cBetBehavior::AddBetMoney()
{
	if( m_pButtonClick )
		m_pButtonClick->Play(true);
	int	l_iNewBetMoney = 0;
	int	l_iNewIndex = GetFocusBetMoneyIndexInVector()+1;
	int	l_iSize = (int)this->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	if( l_iNewIndex >= l_iSize )
		l_iNewIndex = 0;
	l_iNewBetMoney = this->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iNewIndex];
	SetBetData(m_iNumLinesBet,l_iNewBetMoney);
}

void	cBetBehavior::MinusBetMoney()
{
	if( m_pButtonClick )
		m_pButtonClick->Play(true);
	int	l_iNewBetMoney = 0;
	int	l_iNewIndex = GetFocusBetMoneyIndexInVector()-1;
	int	l_iSize = (int)this->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	if( l_iNewIndex <0 )
	{
		l_iNewIndex = l_iSize-1;
	}
	l_iNewBetMoney = this->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iNewIndex];
	SetBetData(m_iNumLinesBet,l_iNewBetMoney);
}

void	cBetBehavior::ExchangeRateChange(int e_iCurrentPayRate,int e_iNewRate)
{
	this->m_pPlayerData->m_iMoney /= e_iCurrentPayRate;
	this->m_pPlayerData->m_iMoney *= e_iNewRate;
	cEngineTestApp::m_siExchangeRate = e_iNewRate;
}

void	cBetBehavior::BetMax()
{
	cGameApp::SoundPlay(MAX_SPIN_SOUND_NAME,true);
	int	l_iSize = (int)this->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	this->m_iNumLinesBet = (int)this->m_pSlotRuleData->m_LineRuleList.size();
	this->m_iPerLineBetMoney = this->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iSize-1];
	this->m_iTotalBet = m_iPerLineBetMoney*m_iNumLinesBet;
	m_pWinmoneyLines->StartLines(m_iNumLinesBet);
}

int		cBetBehavior::MaxBetMoney()
{
	int	l_iSize = (int)this->m_pSlotRuleData->m_PerLineBetMoneyRule.size();
	int	l_iNumLinesBet = (int)this->m_pSlotRuleData->m_LineRuleList.size();
	int	l_iPerLineBetMoney = this->m_pSlotRuleData->m_PerLineBetMoneyRule[l_iSize-1];
	int	l_iTotalBet = m_iPerLineBetMoney*m_iNumLinesBet;
	return l_iTotalBet;
}

void	cBetBehavior::Update(float e_fElpaseTime)
{

}

void	cBetBehavior::Render()
{
#ifdef _DEBUG
	m_pNumerialImage->Draw(m_iRound,400,20);
#endif
	for( int i=0;i<eIPL_MAX;++i )
	{
		//WCHAR*l_strValue = ValueToStringW(*m_piInfoValue[i]);
		//if( eIPL_WinMoneyPos == i )
		m_pNumerialImage->DrawOnCenter(*m_piInfoValue[i],(int)m_vInfoFontPos[i].x,(int)m_vInfoFontPos[i].y);
		//cGameApp::m_spGlyphFontRender->RenderFont(m_vInfoFontPos[i].x,m_vInfoFontPos[i].y,l_strValue);
	}

	this->m_SlotProbability.Render();
	//m_pNumerialImage->Draw(*m_pBetBehaviorResult->m_piTotalInMoney,100,400);
	//m_pNumerialImage->Draw(*m_pBetBehaviorResult->m_piTotalOutMoney,100,450);
	//m_pNumerialImage->Draw(*m_pBetBehaviorResult->m_piRevenue,100,500);
}


void	cBetBehavior::DebugRender()
{

}

void	cBetBehavior::MouseDown(int e_iX,int e_iY)
{

}

void	cBetBehavior::MouseUp(int e_iX,int e_iY)
{

}

void	cBetBehavior::MouseMove(int e_iX,int e_iY)
{

}
extern bool	g_bUseProbability;
void	cBetBehavior::CalculateScoreWithRound()
{
	++m_iRound;
	m_iWinMoney = 0;
	if( g_bUseProbability )
		this->GenerateProbabilitySpinResult(this->m_pBetBehaviorResult);
	else
		this->GenerateRandomSpinResult(this->m_pBetBehaviorResult);
}


void	cBetBehavior::AddWinMoney(int e_iMoney)
{
	m_iWinMoney += e_iMoney;
}

void	cBetBehavior::SetupInfoPos()
{
	cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	cMPDI*l_pMPDI = l_pMPDIList->GetObject(L"UIInfoPos");
	WCHAR*l_str[] = {
		L"TotalBetMoneyPos",
		L"WinMoneyPos",
		L"NumLineBetPos",
		L"PerLineBetMoneyPos",
		L"PlayerMoneyPos",
		L"JP1",
		L"JP2",
	};
	for( int i=0;i<eIPL_MAX;++i )
	{
		cSubMPDI*l_p = l_pMPDI->GetObject(l_str[i]);
		if( l_p )
			m_vInfoFontPos[i] = l_p->GetOriginalPointList()[0];
		//m_vInfoFontPos[i].x -= l_p->GetPointData(0)->Size.x/2.f;
		//m_vInfoFontPos[i].y -= l_p->GetPointData(0)->Size.y/2.f;
	}
}

void	cBetBehavior::UpdateSelectedLines(float e_fElpaseTime)
{
	m_pWinmoneyLines->UpdateLines(m_iNumLinesBet,e_fElpaseTime);
}
void	cBetBehavior::RenderSelectedLines()
{
	m_pWinmoneyLines->RenderLines2(m_iNumLinesBet);
}

bool	cBetBehavior::IsJPGameFilledCondition(std::vector<int>	e_ResultList)
{
	size_t	l_iSize = cEngineTestApp::m_spSlotRuleData->m_LineRuleList.size();
	if(m_iNumLinesBet != l_iSize)
	{
		return false;
	}
	const WCHAR*l_strJPName = m_pSlotRuleData->m_strSpecilaSpinObjectName.c_str();
	for( size_t i=0;i<l_iSize;++i )
	{
		bool	l_bSame = true;
		sLineRule*l_pLineRule = m_pSlotRuleData->m_LineRuleList[i];
		for(char j=0;j<l_pLineRule->cOneLineCount;++j)
		{
			int	l_iTableIndex = l_pLineRule->pcLineObjectIndex[j];
			int	l_iSpinObjectID = e_ResultList[l_iTableIndex];
			if( l_iSpinObjectID == -1 || l_iSpinObjectID>11 )
			{
				int a=0;
			}
			const WCHAR*l_strName = m_pSlotRuleData->m_SpineObjectPrizeTableList.GetObject(l_iSpinObjectID)->GetName();
			if( wcscmp(l_strName,l_strJPName) )
			{
				l_bSame = false;
				break;
			}
		}
		if( l_bSame )
			return true;
	}
	return false;
}

bool	cBetBehavior::NetWorkJPCheck(std::vector<int>*e_pSpinObjectResultList)
{
	if(IsJPGameFilledCondition(*e_pSpinObjectResultList))
	{
		if(cEngineTestApp::m_spNetwork->IsConnected())
		{
			if( cEngineTestApp::m_spNetwork->IsServer() )
			{//server could win JP?
				if( cEngineTestApp::m_spNetwork->IsICouldWINJP() )
					return true;//WIN
			}
			else
			{
				sGamePacket l_GamePacket;
				l_GamePacket.iMessageID = eGMI_TO_SERVER_I_WIN_JP;
				sSendPacket	l_Packet;
				l_Packet.iSize = sizeof(sGamePacket);
				l_Packet.pData = (char*)&l_GamePacket;

				cEngineTestApp::m_spNetwork->SendDataToServer(&l_Packet);
				l_Packet.pData = 0;
				UT::sTimeAndFPS		l_WaitNetWork;
				l_WaitNetWork.Update();
				UT::sTimeCounter	l_TimeCounter;
				l_TimeCounter.SetTargetTime(1.f);
				l_TimeCounter.Start();
				//wait network message
				while( !l_TimeCounter.bTragetTimrReached && !cEngineTestApp::m_spNetwork->m_bJPInfoGet )
				{
					l_WaitNetWork.Update();
					l_TimeCounter.Update(l_WaitNetWork.fElpaseTime);
					cEngineTestApp::m_spNetwork->Update(0.016f);
				}
				//check again...
				if(!cEngineTestApp::m_spNetwork->m_bJPInfoGet)
				{
					cEngineTestApp::m_spNetwork->Update(0.016f);
				}
				if(cEngineTestApp::m_spNetwork->m_bJPInfoGet)
				{
					if(cEngineTestApp::m_spNetwork->IsICouldWINJP())
					{//I WIN JP!!
						return true;
					}
				}
			}
		}
		//make first colum is not wild to avoid JP
		for(int i=0;i<cEngineTestApp::m_spSlotRuleData->m_TableCount.y;++i)
		{
			int	l_iOriginalIndex = (*e_pSpinObjectResultList)[cEngineTestApp::m_spSlotRuleData->m_TableCount.x*i];
			(*e_pSpinObjectResultList)[cEngineTestApp::m_spSlotRuleData->m_TableCount.x*i] = 0;
		}
	}
	return false;
}

//generate necessary random data for spin object's result
std::vector<int>	GenerateSpinResultVector(int e_iCount)
{
	std::vector<int>	l_SpinObjectResult;
	int	l_iSpinObjectMaximum = cEngineTestApp::m_spSlotRuleData->m_SpineObjectPrizeTableList.Count();
	int	l_iNumSpinObject = e_iCount;
	l_SpinObjectResult.resize(l_iNumSpinObject);

	//cheat code
	//int	l_iResult[] = {0,0,6,8,7,2,0,10,2,3,10,8,2,5,11};
	for( int i=0;i<l_iNumSpinObject;++i )
	{
		//l_SpinObjectResult[i] = l_iResult[i];
		int	l_iValue = rand()%cEngineTestApp::m_spSlotRuleData->m_SpineObjectPrizeTableList.m_iTotalProbabilityAmount;
		int	l_iIndex = cEngineTestApp::m_spSlotRuleData->m_SpineObjectPrizeTableList.GetSpinObjectIndexByProbability(l_iValue);
		l_SpinObjectResult[i] = l_iIndex;
		//cheat here
		//if( i%5 == 0 )
			//l_SpinObjectResult[i] = 10;
		//change probability data
	}
	return l_SpinObjectResult;
}

cBetBehaviorResult::sTableResult*cBetBehavior::ProcessMaxBetResultForProbabilityThenCalRealBetResultTable(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,bool*e_pbWinJP,bool e_bFreeGame)
{
	if( e_iMinScore <= 0 && e_iMaxScore <= 0 )
	{
		cBetBehaviorResult::sTableResult*l_pResult = GenerateNoWinMoneyTable(e_bFreeGame,79979);
		CalculateScoreConsecutive(l_pResult);
		return l_pResult;
	}
	//1.first maxima bet data and store original data
	//2.then calculate score and write log
	//3.then  set it back then calculate real score
	//1.
	int	l_iScore = 0;
	int	l_iOriginalNumLinesBet = this->m_iNumLinesBet;	int	l_iOriginalPerLineBetMoney = this->m_iPerLineBetMoney;	int	l_iOriginalTotalBet = m_iTotalBet;
	this->BetMax();
	bool	l_bBetMax = false;;
	if( l_iOriginalNumLinesBet == this->m_iNumLinesBet && 
		l_iOriginalPerLineBetMoney == this->m_iPerLineBetMoney &&
		l_iOriginalTotalBet == m_iTotalBet)
	{
		l_bBetMax = true;
	}
	//2.
	cBetBehaviorResult::sTableResult*l_pLoacalTableResult = GenerateMultilineWinMoneyNewTableWithMatchCount(e_iMinScore,e_iMaxScore,e_iMultiplier,&l_iScore,e_bFreeGame);
	*e_pbWinJP = IsJPGameFilledCondition(l_pLoacalTableResult->SpinObjectResultList);
	if( l_bBetMax )
		return l_pLoacalTableResult;
	//3.
	m_iNumLinesBet = l_iOriginalNumLinesBet;	m_iPerLineBetMoney = l_iOriginalPerLineBetMoney;	m_iTotalBet = l_iOriginalTotalBet;
	cBetBehaviorResult::sTableResult*l_pRealTableResult = new cBetBehaviorResult::sTableResult();
	l_pRealTableResult->SpinObjectResultList = l_pLoacalTableResult->SpinObjectResultList;
	*e_pbWinJP = IsJPGameFilledCondition(l_pLoacalTableResult->SpinObjectResultList);
	AssignLinesData(l_pRealTableResult);
	//second calculate result
	CalculateScoreConsecutive(l_pRealTableResult);
	delete l_pLoacalTableResult;
	return l_pRealTableResult;
}

cBetBehaviorResult::sTableResult*cBetBehavior::ProcessMaxBetResultForProbabilityThenCalRealBetResultTableFromOldTable(cBetBehaviorResult::sTableResult*e_pOldTable,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,bool*e_pbWinJP,bool e_bFreeGame)
{
	if( e_iMinScore <= 0 && e_iMaxScore <= 0 )
	{
		return GenerateNoWinMoneyWithOldTable(e_pOldTable,e_bFreeGame);
	}
	//1.
	int	l_iScore = 0;
	int	l_iOriginalNumLinesBet = this->m_iNumLinesBet;	int	l_iOriginalPerLineBetMoney = this->m_iPerLineBetMoney;	int	l_iOriginalTotalBet = m_iTotalBet;
	this->BetMax();
	bool	l_bBetMax = false;;
	if( l_iOriginalNumLinesBet == this->m_iNumLinesBet && 
		l_iOriginalPerLineBetMoney == this->m_iPerLineBetMoney &&
		l_iOriginalTotalBet == m_iTotalBet)
	{
		l_bBetMax = true;
	}
	//2.
	cBetBehaviorResult::sTableResult*l_pLoacalTableResult = GenerateWinMoneyWithOldTable(e_iMinScore,e_iMaxScore,e_pOldTable,e_iMultiplier,e_bFreeGame);
	*e_pbWinJP = IsJPGameFilledCondition(l_pLoacalTableResult->SpinObjectResultList);
	if( l_bBetMax )
		return l_pLoacalTableResult;
	//3.
	m_iNumLinesBet = l_iOriginalNumLinesBet;	m_iPerLineBetMoney = l_iOriginalPerLineBetMoney;	m_iTotalBet = l_iOriginalTotalBet;
	cBetBehaviorResult::sTableResult*l_pRealTableResult = new cBetBehaviorResult::sTableResult();
	l_pRealTableResult->SpinObjectResultList = l_pLoacalTableResult->SpinObjectResultList;
	AssignLinesData(l_pRealTableResult);
	//second calculate result
	CalculateScoreConsecutive(l_pRealTableResult);
	delete l_pLoacalTableResult;
	return l_pRealTableResult;
}

int	GetComboRestScore(int e_iScore)
{
	if( e_iScore <= 0 )
		return 0;
	int	l_iWantedValue = e_iScore/3+1;
	return (rand()%(l_iWantedValue))+l_iWantedValue;
}

cBetBehaviorResult::sTableResult*	cBetBehavior::GenerateSpecificTable()
{
	cBetBehaviorResult::sTableResult*l_pLoacalTableResult = new cBetBehaviorResult::sTableResult;
	l_pLoacalTableResult->SpinObjectResultList.resize(15);
	l_pLoacalTableResult->SpinObjectResultList[0] = 2;
	l_pLoacalTableResult->SpinObjectResultList[1] = 2;
	l_pLoacalTableResult->SpinObjectResultList[2] = 2;
	l_pLoacalTableResult->SpinObjectResultList[3] = 2;
	l_pLoacalTableResult->SpinObjectResultList[4] = 2;

	l_pLoacalTableResult->SpinObjectResultList[5] = 1;
	l_pLoacalTableResult->SpinObjectResultList[6] = 0;
	l_pLoacalTableResult->SpinObjectResultList[7] = 4;
	l_pLoacalTableResult->SpinObjectResultList[8] = 1;
	l_pLoacalTableResult->SpinObjectResultList[9] = 8;

	l_pLoacalTableResult->SpinObjectResultList[10] = 0;
	l_pLoacalTableResult->SpinObjectResultList[11] = 0;
	l_pLoacalTableResult->SpinObjectResultList[12] = 0;
	l_pLoacalTableResult->SpinObjectResultList[13] = 0;
	l_pLoacalTableResult->SpinObjectResultList[14] = 0;

	AssignLinesData(l_pLoacalTableResult);
	//second calculate result
	CalculateScoreConsecutive(l_pLoacalTableResult);
	return l_pLoacalTableResult;
}

void	cBetBehavior::GenerateProbabilitySpinResult(cBetBehaviorResult*e_pBetBehaviorResult)
{
	e_pBetBehaviorResult->ClearLastTableResult();
	//so filled all spin object as random
	bool	l_bWinJP = false;
	int	l_iMultiplier = this->m_iBonusMoneyRate*this->m_iPerLineBetMoney;
	m_SlotProbability.m_SlotGoodTables.GetGoodTable(e_pBetBehaviorResult);
	sWinMoneyTable*l_pWinMoneyTable = m_SlotProbability.m_SlotGoodTables.GetCurrentWinMoneyTable();
	if( l_pWinMoneyTable->GoodTableCategory == eGTC_JP_LOCAL )
	{
		if(!GenerateLocalJPTable(m_pBetBehaviorResult,l_iMultiplier))
		{

		}
		return;
	}
	if( l_pWinMoneyTable->GoodTableCategory == eGTC_JP_NETWORK )
	{
		if(!GenerateNetJPTable(m_pBetBehaviorResult,l_iMultiplier))
		{

		}
		return;
	}
	int	l_iMaxScore = l_pWinMoneyTable->iMaxMoney;
	int	l_iMinScore = l_pWinMoneyTable->iMinMoney;
	int	l_iRestMaxScore = l_iMaxScore;
	int	l_iRestMinScore = l_iMinScore;
	int	l_iMaxSpinObjectScore = this->m_pSlotRuleData->GetMaxScore(l_iMultiplier);
	if( l_iRestMinScore > l_iMaxSpinObjectScore  )
	{
		l_iRestMinScore = l_iMaxSpinObjectScore/((rand()%3)+1);
	}
	int	l_iCombo = l_pWinMoneyTable->iComboCount;
	//at least for one table.
	int	l_iCurrentIndex = 1;
	--l_iCombo;
	UT::sTimeAndFPS	l_FPS;
	cBetBehaviorResult::sTableResult*l_pTableResult = 0;
	//while( !l_pTableResult || (l_pTableResult->iWinMoney && l_iCurrentIndex < MAXIMUM_EXPLOSION_COUNT+1 && !cEngineTestApp::m_sbFreeToPlay) )
	while( !l_pTableResult || (l_pTableResult->iWinMoney && l_iCurrentIndex < MAXIMUM_EXPLOSION_COUNT+1 ) )
	{
		cBetBehaviorResult::sTableResult*l_pLoacalTableResult = 0;
		l_FPS.Update();
		int	l_iTargetScore = GetComboRestScore(l_iRestMaxScore);
		if( !l_pTableResult )
		{
			l_pLoacalTableResult = ProcessMaxBetResultForProbabilityThenCalRealBetResultTable(l_iRestMinScore,l_iTargetScore,l_iMultiplier,&l_bWinJP,l_pWinMoneyTable->GoodTableCategory==eGTC_FREE_GAME?true:false);
			if( l_bWinJP )
			{//avoid win JP
				delete l_pLoacalTableResult;
				l_pLoacalTableResult = ProcessMaxBetResultForProbabilityThenCalRealBetResultTable(0,0,1,&l_bWinJP,false);
			}
			//cheat:force to show result
			if(0)
			{
				delete l_pLoacalTableResult;
				l_pLoacalTableResult = GenerateSpecificTable();
			}
		}
		else
		{
			if( l_iCombo>0 )
				l_pLoacalTableResult = ProcessMaxBetResultForProbabilityThenCalRealBetResultTableFromOldTable(l_pTableResult,l_iRestMinScore,l_iTargetScore,l_iMultiplier,&l_bWinJP,false);
			else
				l_pLoacalTableResult = GenerateNoWinMoneyWithOldTable(l_pTableResult,false);
			++l_iCurrentIndex;
			if( l_bWinJP )
			{//avoid win JP
				delete l_pLoacalTableResult;
				l_pLoacalTableResult = GenerateNoWinMoneyWithOldTable(l_pTableResult,false);
			}
		}
		l_FPS.Update();
		//std::string	l_str = UT::ComposeMsgByFormat("1 table generate time%.3f",l_FPS.fElpaseTime);
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		//l_pLoacalTableResult = 	GenerateOnelineWinMoneyNewTable(50,1000,this->m_iBonusMoneyRate*this->m_iPerLineBetMoney,&l_iScore);
		l_pTableResult = l_pLoacalTableResult;
		e_pBetBehaviorResult->m_TableResultList.push_back(l_pLoacalTableResult);
		//lazy doing
		{
			{
				//std::string	l_str = UT::ComposeMsgByFormat("Table1:Score%d,MaxScore:%d,MinScore:%d",l_pTableResult->iWinMoney,l_iRestMaxScore,l_iRestMinScore);
				//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
			}
			l_iRestMaxScore -= l_pTableResult->iWinMoney;
			l_iRestMinScore -= l_pTableResult->iWinMoney;
			if( l_iRestMinScore<0 )
				l_iRestMinScore = 0;
			--l_iCombo;
		}
	}
	int	l_iWinMoney = m_pBetBehaviorResult->GetCurrentTablesWinMoney();
	if( l_iWinMoney < l_pWinMoneyTable->iMinMoney || l_iWinMoney >l_pWinMoneyTable->iMaxMoney )
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("Result not match!!!");
	std::string	l_str = UT::ComposeMsgByFormat("ROUND  %d,Temp:%d,Table1Result:Combo:%d,Score%d",GetRound(),m_SlotProbability.m_SlotGoodTables.GetCurrentScore(),m_pBetBehaviorResult->m_TableResultList.size(),l_iWinMoney);
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
	//for control panel record
	if( l_iWinMoney>1000 )
		EarningOver1000Score(l_iWinMoney);
}
//
void	cBetBehavior::GenerateRandomSpinResult(cBetBehaviorResult*e_pBetBehaviorResult)
{
	e_pBetBehaviorResult->ClearLastTableResult();
	//so filled all spin object as random
	int	l_iNumSpinObject = this->m_pSlotRuleData->m_TableCount.x*this->m_pSlotRuleData->m_TableCount.y;
	//at least for one table.
	cBetBehaviorResult::sTableResult*l_pTableResult = 0;
	{
		cBetBehaviorResult::sTableResult*l_pLoacalTableResult = 0;	
		//now assign spinobject as line to fill cBetBehaviorResult::sTableResult::sLine
		l_pLoacalTableResult = new cBetBehaviorResult::sTableResult;
		l_pLoacalTableResult->SpinObjectResultList = GenerateSpinResultVector(l_iNumSpinObject);
		AssignLinesData(l_pLoacalTableResult);
		//second calculate result
		CalculateScoreConsecutive(l_pLoacalTableResult);
		l_pTableResult = l_pLoacalTableResult;
		e_pBetBehaviorResult->m_TableResultList.push_back(l_pLoacalTableResult);
	}
	//win JP.
	if( NetWorkJPCheck(&l_pTableResult->SpinObjectResultList) )
		return;
	int	l_iCurrentIndex = 1;
	//check if combo is not over maximum
	//while(	l_pTableResult->iWinMoney && l_iCurrentIndex < MAXIMUM_EXPLOSION_COUNT+1 && !cEngineTestApp::m_sbFreeToPlay )
	while(	l_pTableResult->iWinMoney && l_iCurrentIndex < MAXIMUM_EXPLOSION_COUNT+1 )
	{
		//third do regenerate new spin object result
		cBetBehaviorResult::sTableResult*l_pLoacalTableResult = 0;
		l_pLoacalTableResult = GenerateSpinExplosionResultWithRandom(l_pTableResult);
		//avoid win JP
		if(IsJPGameFilledCondition(l_pLoacalTableResult->SpinObjectResultList))
		{
			size_t	l_siSize = l_pLoacalTableResult->SpinObjectResultList.size();
			//make first colum is not wild to avoid JP
			for(int i=0;i<cEngineTestApp::m_spSlotRuleData->m_TableCount.y;++i)
			{
				int	l_iOriginalIndex = l_pLoacalTableResult->SpinObjectResultList[cEngineTestApp::m_spSlotRuleData->m_TableCount.x*i];
				l_pLoacalTableResult->SpinObjectResultList[cEngineTestApp::m_spSlotRuleData->m_TableCount.x*i] = 0;
			}
		}
		AssignLinesData(l_pLoacalTableResult);
		CalculateScoreConsecutive(l_pLoacalTableResult);
		e_pBetBehaviorResult->m_TableResultList.push_back(l_pLoacalTableResult);
		l_pTableResult = l_pLoacalTableResult;
		++l_iCurrentIndex;
	}
}

cBetBehaviorResult::sTableResult*	cBetBehavior::GenerateSpinExplosionResultWithEmptySpinObject(cBetBehaviorResult::sTableResult*e_pTableResult,std::vector<int>*e_pEmptyIndexList)
{
	//find out explosion object
	int	l_iNumSpinObject = this->m_pSlotRuleData->m_TableCount.x*this->m_pSlotRuleData->m_TableCount.y;
	cBetBehaviorResult::sTableResult*l_psTableResult = new cBetBehaviorResult::sTableResult;
	std::vector<int>	l_NewSpinObjectResult;
	l_NewSpinObjectResult.resize(l_iNumSpinObject);
	//fin out last index where explosion,and count
	int				l_iTargetIndex = 0;
	//cPuzzleImage*	l_pPI = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI();
	//
	int	l_iTestValue = 0;
	int	l_iNewResultIndex = 0;
	for( int i=0;i<cEngineTestApp::m_spSlotRuleData->m_TableCount.x;++i )
	{
		int	l_iNonExplosionIndex = cEngineTestApp::m_spSlotRuleData->m_TableCount.y-1;
		int	l_iNextSpinObjectIndex = cEngineTestApp::m_spSlotRuleData->m_TableCount.x;
		int	l_iImageIndex = -1;
		for( int j=cEngineTestApp::m_spSlotRuleData->m_TableCount.y-1;j>-1;--j )
		{
			l_iTargetIndex = j*cEngineTestApp::m_spSlotRuleData->m_TableCount.x+i;
			//start index from unexplosion 
			if(!e_pTableResult->pbWinMoneySpinObject[l_iTargetIndex])
			{
				int	l_iFinalSpinObjectIndex = l_iNonExplosionIndex*l_iNextSpinObjectIndex+i;
				l_NewSpinObjectResult[l_iFinalSpinObjectIndex] = e_pTableResult->SpinObjectResultList[l_iTargetIndex];
				--l_iNonExplosionIndex;
			}
		}
		int	l_iStartIndex = i;
		for( int j=cEngineTestApp::m_spSlotRuleData->m_TableCount.y-1;j>-1;--j )
		{
			l_iTargetIndex = j*cEngineTestApp::m_spSlotRuleData->m_TableCount.x+i;
			if(e_pTableResult->pbWinMoneySpinObject[l_iTargetIndex])
			{
				++l_iTestValue;
				int	l_iFinalSpinObjectIndex = l_iStartIndex+(l_iNonExplosionIndex*l_iNextSpinObjectIndex);
				//fuck here is wonrg.
				l_NewSpinObjectResult[l_iFinalSpinObjectIndex] = -1;
				if( e_pEmptyIndexList )
					e_pEmptyIndexList->push_back(l_iFinalSpinObjectIndex);
				--l_iNonExplosionIndex;
				++l_iNewResultIndex;
			}
		}
	}
	l_psTableResult->SpinObjectResultList = l_NewSpinObjectResult;
	return l_psTableResult;
}

cBetBehaviorResult::sTableResult*	cBetBehavior::GenerateSpinExplosionResultWithRandom(cBetBehaviorResult::sTableResult*e_pTableResult)
{
	int	l_iNumSpinObject = this->m_pSlotRuleData->m_TableCount.x*this->m_pSlotRuleData->m_TableCount.y;
	int	l_iNumNewSpinObject = 0;
	for( int i=0;i<l_iNumSpinObject;++i )
	{
		if(e_pTableResult->pbWinMoneySpinObject[i])
			++l_iNumNewSpinObject;
	}
	cBetBehaviorResult::sTableResult*l_psTableResult = GenerateSpinExplosionResultWithEmptySpinObject(e_pTableResult);
	std::vector<int>	l_NeddedNewSpinObjectResult = GenerateSpinResultVector(l_iNumNewSpinObject);
	for( int i=0;i<l_iNumSpinObject;++i )
	{
		if(l_psTableResult->SpinObjectResultList[i] == -1 )
		{
			l_psTableResult->SpinObjectResultList[i] = l_NeddedNewSpinObjectResult[--l_iNumNewSpinObject];
		}
	}
	assert(l_iNumNewSpinObject == 0);
	return l_psTableResult;
}