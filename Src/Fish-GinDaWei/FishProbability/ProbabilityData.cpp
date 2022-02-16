#include "stdafx.h"
#include "ProbabilityData.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/PlayerBehaviorBase.h"
#include "../../Core/GamePlayUT/Event/AllEventInclude.h"
#include "../ControlPanel/AllControlPanelInclude.h"
#include "../../Core/GamePlayUT/DataCurve/DataCurve.h"


cProbabilityData::cProbabilityData()
{
	m_fMachinRunTime = 0.f;
	m_fHappyTimeScale = 2.f;
	m_fFakeRevenueFetchProbability = 0.25f;
	m_iFakeRevenueGiveDivision = 4;
	m_fFishGroupProbability = 0.1f;
	m_fHappyMultiplierProbability = 1.f;
	m_fSafeProbabilityOffset = 0.1f;
	m_fHappyRevenueToWinMoneyProbability = 0.001f;
	if( !cFishApp::m_spControlPanel )
	{
		UT::ErrorMsg(L"no control panel",L"warning");
		return;
	}

	cEventVariable*l_pLatestIncome =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_TotalIncome");
	cEventVariable*l_pLatestPay =			cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_TotalPay");
	cEventVariable*l_pLatestRevenue =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_Revenue");
	cEventVariable*l_pLatestExchangeIn =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ExchangeIn");
	cEventVariable*l_pLatestExchangeOut =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ExchangeOut");
	cEventVariable*l_pLatestToastCoin =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_ToastCoin");
	cEventVariable*l_pLatestRefundCoin =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Latest_RefudCoin");


	cEventVariable*l_pHistoryTotalIncome =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"TotalIncome");
	cEventVariable*l_pHistoryTotalPay =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"TotalPay");
	cEventVariable*l_pHistoryRevenue =		cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"TotalRevenue");
	cEventVariable*l_pHistoryExchangeIn =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"TotalExchangeIn");
	cEventVariable*l_pHistoryExchangeOut =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"TotalExchangeOut");
	cEventVariable*l_pHistoryToastCoin =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"TotalToastCoin");
	cEventVariable*l_pHistoryRefundCoin =	cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"TotalRefudCoin");



	cEventVariable*l_pProbability = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Probability");
	cEventVariable*l_pCoinToScore = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CoinToScore");
	cEventVariable*l_pHappyTime = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"HappyTime");
	m_piHappyTime = l_pHappyTime->GetInt();


	m_piTargetProbability = l_pProbability->GetInt();

	m_pi64LatestInMoney = l_pLatestIncome->GetInt64();
	m_pi64LatestOutMoney = l_pLatestPay->GetInt64();
	m_pi64LatestRevenue = l_pLatestRevenue->GetInt64();
	m_pi64LatestExchangeIn = l_pLatestExchangeIn->GetInt64();
	m_pi64LatestExchangeOut = l_pLatestExchangeOut->GetInt64();
	m_pi64LatestCoinIn = l_pLatestToastCoin->GetInt64();
	m_pi64LatestCoinOut = l_pLatestRefundCoin->GetInt64();

	m_pi64TotalInMoney = l_pHistoryTotalIncome->GetInt64();
	m_pi64TotalOutMoney = l_pHistoryTotalPay->GetInt64();
	m_pi64TotalRevenue = l_pHistoryRevenue->GetInt64();
	m_pi64TotalExchangeIn = l_pHistoryExchangeIn->GetInt64();
	m_pi64TotalExchangeOut = l_pHistoryExchangeOut->GetInt64();
	m_pi64TotalCoinIn = l_pHistoryToastCoin->GetInt64();
	m_pi64TotalCoinOut = l_pHistoryRefundCoin->GetInt64();


	m_iStartTotalIncome = *m_pi64TotalInMoney;
	m_iStartTotalOut = *m_pi64TotalOutMoney;

	m_piCoinToScore = l_pCoinToScore->GetInt();

	m_iFakeRevenue = 2000*(*m_piCoinToScore);
	m_iHappyRevenue = m_iFakeRevenue;
	m_iHappyRevenueToWinMoneyLimit = 1000*(*m_piCoinToScore);
	m_pStaticsLogFile = 0;
	m_pi64PlayerRevenueLog = 0;
	m_iBaseMoney = 5;
	m_fBaseProbability = 0.5f;
	m_FakeRevenueTCRestrict = Vector2(30,60);
}

cProbabilityData::~cProbabilityData()
{
	SAFE_DELETE(m_pStaticsLogFile);
	SAFE_DELETE(m_pi64PlayerRevenueLog);
}

void	cProbabilityData::Init()
{
	SAFE_DELETE(m_pStaticsLogFile);
	SAFE_DELETE(m_pi64PlayerRevenueLog);
	m_pi64PlayerRevenueLog = new int64[cFishApp::m_spControlPanel->m_iPlayerCount];
	memset(m_pi64PlayerRevenueLog,0,sizeof(int64)*cFishApp::m_spControlPanel->m_iPlayerCount);
	m_pStaticsLogFile = new cBinaryFile();
	std::wstring	l_strSystemTime = L"Log/FishProbability";
	l_strSystemTime += UT::GetSystemTimeForFile(true);
	l_strSystemTime += L".txt";
	m_pStaticsLogFile->Writefile((char*)UT::WcharToChar(l_strSystemTime.c_str()),true,false);







}


void	cProbabilityData::RevenuChange()
{
	*m_pi64LatestRevenue =			*m_pi64LatestInMoney-*m_pi64LatestOutMoney;
	*m_pi64TotalRevenue =				*m_pi64TotalInMoney-*m_pi64TotalOutMoney;
}

void	cProbabilityData::RevenueMinus(int e_iMoney,int e_iPlayerID)
{
	*m_pi64LatestOutMoney			+=		e_iMoney;
	*m_pi64TotalOutMoney			+=		e_iMoney;
	m_iFakeRevenue					-=		e_iMoney;
	if( m_iFakeRevenue<0 )
		m_iFakeRevenue = 0;
	RevenuChange();

	this->m_pi64PlayerRevenueLog[e_iPlayerID] += e_iMoney;
}

void	cProbabilityData::RevenueAdd(int e_iMoney,int e_iPlayerID)
{
	this->m_BulletFireAverage.BulletFire(e_iMoney);
	*m_pi64LatestInMoney				+=	e_iMoney;
	*m_pi64TotalInMoney					+=	e_iMoney;

	m_MiniGameData.fTotalAteMoney = 0;
	for( size_t i=0;i<m_MiniGameData.MiniGameBankList.size();++i )
	{
		m_MiniGameData.MiniGameBankList[i].fAteMoney += (m_MiniGameData.fIncomePercentage*e_iMoney);
		m_MiniGameData.fTotalAteMoney += m_MiniGameData.MiniGameBankList[i].fAteMoney;
	}
	RevenuChange();
	this->m_pi64PlayerRevenueLog[e_iPlayerID] -= e_iMoney;
}

//float	cProbabilityData::GetOutMoneyProbability(int e_iOutMoney)
//{
//	if( e_iOutMoney == 0 )
//		return 0.f;
//	return m_iBaseMoney/(float)e_iOutMoney*m_fBaseProbability;
//}

float	cProbabilityData::GetReleatePayRateProbability(int e_iPayRate,int e_iOneBulletHitMonsterMonut)
{
	return m_BulletFireAverage.RelativeProbability(e_iPayRate,e_iOneBulletHitMonsterMonut);
}

float	cProbabilityData::GetTargetProbability()
{
	return *m_piTargetProbability/100.f;
}

int64		cProbabilityData::GetLeatestInMoney()
{
	int64	l_i64Value = 0;
	if( m_iFakeRevenue<=0 )
	{
		l_i64Value = (int64)(*m_pi64LatestInMoney-m_MiniGameData.fTotalAteMoney);
	}
	else
	{
		float	l_fValue = frand(0,1);
		int	l_FakeMoneyFetch = 0;
		float	l_fHappyScale = 1.f;
		if( this->IsHappyTime() )
			l_fHappyScale = this->m_fHappyTimeScale;
		l_fValue *= l_fHappyScale;
		if( l_fValue>=m_fFakeRevenueFetchProbability  )
		{
			l_FakeMoneyFetch = rand(0,m_iFakeRevenue/rand(1,m_iFakeRevenueGiveDivision));
			l_FakeMoneyFetch = (int)(l_fHappyScale*l_FakeMoneyFetch);
			if( l_FakeMoneyFetch > m_iFakeRevenue )
				l_FakeMoneyFetch = m_iFakeRevenue;
		}
		else
			l_FakeMoneyFetch = 0;
		l_i64Value = (int64)(*m_pi64LatestInMoney-m_MiniGameData.fTotalAteMoney+l_FakeMoneyFetch);
	}
	if( l_i64Value < 0 )
		l_i64Value = 0;
	//avoid total in is zero
	return l_i64Value+1;
}

float	cProbabilityData::GetCurrentRealProbability()
{
	if( *m_pi64LatestInMoney != 0 )
		return (float)((double)(*m_pi64LatestOutMoney)/(*m_pi64LatestInMoney));
	return 0.f;
}

float	cProbabilityData::GetCurrentProbability()
{
	int64	l_iTotalInMoney = GetLeatestInMoney();
	return (float)((double)*m_pi64LatestOutMoney/l_iTotalInMoney);
}


float	cProbabilityData::GetNewProbailityByAddMoney(int e_iOutMoney)
{
	int64	l_iNewTotalOutMoney = *this->m_pi64LatestOutMoney+e_iOutMoney;
	return (float)((double)l_iNewTotalOutMoney/(double)GetLeatestInMoney());
}

bool	cProbabilityData::IsProbailitySafeToOut(float e_fProbaility)
{
	if(e_fProbaility<this->GetTargetProbability())
	{
		return true;
	}
	return false;
	//{
	//	from new bite money get died probaility offset;
	//	add bite money and add a died probaility offset.
	//}
}
//haha.....-_-
float	cProbabilityData::GetFishDiedHappyProbability(float e_fCurrentProbability,float e_fDiedProbability)
{
	float	l_fOffset = this->GetTargetProbability()-e_fCurrentProbability;
	if( l_fOffset >= m_fSafeProbabilityOffset )
	{
		return e_fDiedProbability*this->m_fHappyMultiplierProbability;
	}
	else
	if( l_fOffset <= -m_fSafeProbabilityOffset )
	{
		return e_fDiedProbability*this->m_fUnHappyMultiplierProbability;
	}
	return e_fDiedProbability;
}

int		cProbabilityData::GetRandomFishID()
{
	return m_FishShowProbabilityList.GetRandomFishID();
}

int		cProbabilityData::GetRandomMiniGameFishID()
{
	assert(m_MiniGameData.MiniGameFishIDList.size());
	int	l_iValue = rand()%m_MiniGameData.MiniGameFishIDList.size();
	return m_MiniGameData.MiniGameFishIDList[l_iValue];
}


void	cProbabilityData::FakeRevenueUpdate(float e_fElpaseTime)
{
	m_FakeRevenueTC.Update(e_fElpaseTime);
	if( m_FakeRevenueTC.bTragetTimrReached )
	{
		if( m_iFakeRevenue <= 0 )
		{
			//hard to give fake revenue,but safe for probability.
			//*this->m_pi64TotalRevenue > this->m_iHappyRevenue 
			if(this->m_iFakeRevenue == 0 && IsProbailitySafeToOut(this->GetCurrentProbability()))
			{
				m_iFakeRevenue = m_iHappyRevenue;
			}
			if(this->IsHappyTime())
				m_FakeRevenueTC.SetTargetTime(m_FakeRevenueTCRestrict.Rand()/this->m_fHappyTimeScale);
			else
				m_FakeRevenueTC.SetTargetTime(m_FakeRevenueTCRestrict.Rand());
		}
		m_FakeRevenueTC.Start();
	}
}

void	cProbabilityData::Update(float e_fElpaseTime)
{
	m_fMachinRunTime += e_fElpaseTime;
	sFishShowProbaility::sFishOutData l_FishOutData = this->m_FishShowProbabilityList.m_PlayerCountWithFish[cFishApp::m_spControlPanel->m_iPlayerCount];
	this->m_iNumMonsterOutAtSameTime = l_FishOutData.iOutFishCount.Rand();
	this->m_fTimeToGenerateMonsters = l_FishOutData.fTimeCircle.Rand();
	m_BulletFireAverage.Update(e_fElpaseTime);
	FakeRevenueUpdate(e_fElpaseTime);
	if( this->m_MiniGameData.iIndexReadyToShowMiniGameIndex != -1 )
		return;
	m_MiniGameData.Update(e_fElpaseTime);
}

void	cProbabilityData::Render()
{
	std::wstring	l_str = UT::ComposeMsgByFormat(L"TotalIn:%I64d\nTotalOut:%I64d\nTotalRevenue:%I64d",*this->m_pi64TotalInMoney,*m_pi64TotalOutMoney,*m_pi64TotalRevenue);
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,0,1,1));
	cGameApp::m_spGlyphFontRender->RenderFont(0,300,l_str.c_str());

	l_str = UT::ComposeMsgByFormat(L"LatestIn:%I64d\nLatestOut:%I64d\nLatestRevenue:%I64d",*m_pi64LatestInMoney,*m_pi64LatestOutMoney,*m_pi64LatestRevenue);
	cGameApp::m_spGlyphFontRender->RenderFont(0,420,l_str.c_str());

	l_str = UT::ComposeMsgByFormat(L"FakeRevenue:%d",m_iFakeRevenue);
	cGameApp::m_spGlyphFontRender->RenderFont(0,520,l_str.c_str());
	l_str = UT::ComposeMsgByFormat(L"RestTime:%.2f",m_FakeRevenueTC.fRestTime);
	cGameApp::m_spGlyphFontRender->RenderFont(0,570,l_str.c_str());	
	l_str = UT::ComposeMsgByFormat(L"RunningTime:%.1f",m_fMachinRunTime);
	cGameApp::m_spGlyphFontRender->RenderFont(0,680,l_str.c_str());
	cGameApp::m_spGlyphFontRender->SetFontColor(Vector4(1,1,1,1));

}

int		cProbabilityData::FetchWaitingToShowMiniGameID()
{
	int	l_iIndex = this->m_MiniGameData.iIndexReadyToShowMiniGameIndex;
	if( l_iIndex != -1 )
	{
		this->m_MiniGameData.iIndexReadyToShowMiniGameIndex = -1;
		//this->m_MiniGameData.fTotalAteMoney -= this->m_MiniGameData.MiniGameBankList[l_iIndex].fAteMoney;
		//this->m_MiniGameData.MiniGameBankList[l_iIndex].fAteMoney = 0;
		this->m_MiniGameData.MiniGameBankList[l_iIndex].bWaitingToGiveMiniGame = false;
	}
	return l_iIndex;
}

//int	cProbabilityData::GetPayRateRelativeToCoinToScore(int e_iMoney)
//{
//	return e_iMoney/(*this->m_piCoinToScore);
//}

bool cProbabilityData::IsHappyTime()
{
	return *m_piHappyTime==1?true:false;
}

void	cProbabilityData::WrtieLog()
{
	int	l_iCount = cFishApp::m_spControlPanel->m_iPlayerCount;
	int64	l_i64TotalRevene = 0;
	for( int i=0;i<l_iCount;++i )
	{
		l_i64TotalRevene += this->m_pi64PlayerRevenueLog[i];
		//revenue
		this->m_pStaticsLogFile->WriteToFile(this->m_pi64PlayerRevenueLog[i]);
	}
	for( int i=0;i<l_iCount;++i )
	{
		//money
		this->m_pStaticsLogFile->WriteToFile(cFishApp::m_spPlayerManager->GetObject(i)->m_i64Score);
	}
	memset(m_pi64PlayerRevenueLog,0,sizeof(int64)*l_iCount);
	//total in 
	//this->m_pStaticsLogFile->WriteToFile(*m_piTotalInMoney);
	//total out
	//this->m_pStaticsLogFile->WriteToFile(*m_piTotalOutMoney);

	float	l_fCurrentProbability = GetCurrentRealProbability();
	this->m_pStaticsLogFile->WriteToFile(l_i64TotalRevene);
	this->m_pStaticsLogFile->WriteToFile(l_fCurrentProbability);
	//OutputDebugString(ValueToStringW(l_i64TotalRevene));
	//OutputDebugString(L",");
	//OutputDebugString(ValueToStringW(l_fCurrentProbability));
	//OutputDebugString(L"\n");
	m_pStaticsLogFile->Flush();
}