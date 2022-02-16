#include "stdafx.h"
#include "../EngineTest/FishProbability.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/MonsterBase.h"
#include "../EngineTest/Bullet.h"
#include "../EngineTest/PlayerBehaviorBase.h"
#include "../EngineTest/MiniGameBase.h"
#include "ProbabilityData.h"
#include "../Monster/TurtleBoss.h"
#include "../EngineTest/TestLogSetup.h"
#include "ComportData.h"
cFishProbability::cFishProbability()
{
	m_bReceivedSingnal = false;
	m_pSchoolMasterLiSendDataWithMonster = 0;
	m_pSchoolMasterLiSendData = 0;
	m_pSchoolMasterLiReceiveData = 0;
	m_pLiSchoolMasterSerialIO = 0;
	if( cFishApp::m_sbSchoolerMasterLiProbability )
	{
		m_pSchoolMasterLiSendDataWithMonster = new sSchoolMasterLiSendDataWithMonster;
		m_pSchoolMasterLiSendData = new sSchoolMasterLiSendDataWithMonster;
		m_pSchoolMasterLiReceiveData = new sSchoolMasterLiReceiveData;
		m_pLiSchoolMasterSerialIO = new cLiSchoolMasterSerialIO();
	}
	m_bThisFrameMiniGameFishDied = false;
	m_pPlayerMoneyLogFile = 0;
	m_WritePlayerMoneyTC.SetTargetTime(cGameApp::m_sbDeviceExist?0.1f:1.f);
	m_WritePlayerMoneyTC.Start();
	m_TimeGapToGenerateMonster.SetTargetTime(0.5f);
	m_pProbabilityData = 0;
	m_bPauseGenerateMonster = false;
}

cFishProbability::~cFishProbability()
{
	SAFE_DELETE(m_pSchoolMasterLiSendDataWithMonster);
	SAFE_DELETE(m_pSchoolMasterLiSendData);
	SAFE_DELETE(m_pSchoolMasterLiReceiveData);
	SAFE_DELETE(m_pLiSchoolMasterSerialIO);
	SAFE_DELETE(m_pProbabilityData);
	SAFE_DELETE(m_pPlayerMoneyLogFile);
}


void	cFishProbability::Init()
{
	m_bThisFrameMiniGameFishDied = false;
	SAFE_DELETE(m_pProbabilityData);
	this->m_pProbabilityData = new cProbabilityData();
	if(!m_pProbabilityData->ParseWithMyParse(FISH_PROBABILITY))
	{
		UT::ErrorMsg(L"FISH_PROBABILITY",L"parse failed");
	}
	this->m_pProbabilityData->Init();
	MonsterGenerate(0.016f);
	m_TimeGapToGenerateMonster.SetTargetTime(this->m_pProbabilityData->m_fTimeToGenerateMonsters);
	m_bPauseGenerateMonster = false;
	OpenPlayerMoney();
}

bool		cFishProbability::IsPayRateLegalToWinBigMoney(int e_iPrize,int e_iPayRate)
{
	//if it is small payrate it should give a extra revenue benfit while time is reached
	static	bool	l_sbGiveForMinShot = false;
	static	float	l_sElpaseTime = 0.f;
	const float	l_Time = 1800.f;
	l_sElpaseTime += cGameApp::m_sTimeAndFPS.fElpaseTime;
	//30 minutes
	if( l_sElpaseTime >= l_Time )
	{
		l_sElpaseTime = 0.f;
		l_sbGiveForMinShot = true;
	}
	//avoid small bullet shoot big prize
	const int	l_iSmallDivide = 20;
	int	l_iMaxScore = cFishApp::m_spControlPanel->m_iBulletPayRateLimit;
	l_iMaxScore = l_iMaxScore/l_iSmallDivide;
	const int	l_iBigPrize = 100;
	if( e_iPrize >= l_iBigPrize && e_iPayRate < l_iMaxScore )
	{
		if( !l_sbGiveForMinShot )
			return false;
		l_sbGiveForMinShot = false;
	}
	return true;
}

int64		cFishProbability::GetExtractRevenueValue()
{
	float	l_fProbability = this->m_pProbabilityData->GetCurrentRealProbability();
	float	l_fOffsetProbability = m_pProbabilityData->GetTargetProbability() - l_fProbability;
	if( l_fProbability >0.f && l_fOffsetProbability > 0.01 )
	{
		const	int	l_iProbabilityToFetch = (int)(l_fOffsetProbability*100);
		if( rand()%((30/l_iProbabilityToFetch)+1) != 0 )
			return 0;
		l_fOffsetProbability *= 100;
		int64	l_iRevenueToGive = *m_pProbabilityData->m_pi64LatestRevenue/100*(int)(l_fOffsetProbability);
		return l_iRevenueToGive;
	}
	return 0;
}

//1280:720 = 17 :10,but 27 is not even so make it as 26
//const int			g_iTotalDirectionOrder = 26;//17+10
//const int			g_iDirectionOrder[26] = {0,1,2,3,1,3,//6
//											 0,1,2,3,1,3,//6
//											 0,1,2,3,1,	 //5
//											 0,1,2,3,3,	 //5
//											 0,1,2,3};	 //4

const int			g_iTotalDirectionOrder = 8;
const int			g_iDirectionOrder[8] = {2,2,2,1,0,0,0,3};	 //8

UINT				g_suiDirection = 0;

Vector3		cFishProbability::GetRandomPos(float e_fRadius)
{
	sMinMaxData<float>	l_fPosX;
	sMinMaxData<float>	l_fPosY;
	l_fPosX.Max = cGameApp::m_svGameResolution.x;		l_fPosX.Min = 0.f;
	l_fPosY.Max = cGameApp::m_svGameResolution.y;		l_fPosY.Min = 0.f;
	++g_suiDirection ;
	if( g_suiDirection >= g_iTotalDirectionOrder )
		g_suiDirection = 0;
	int	l_iDirection = g_iDirectionOrder[g_suiDirection];
	Vector3	l_vPos;
	switch( l_iDirection )
	{
		case 0://eD_LEFT
			l_vPos.x = -e_fRadius;
			l_vPos.y = l_fPosY.Rand();
			break;
		case 1://eD_UP
			l_vPos.x = l_fPosX.Rand();
			l_vPos.y = -e_fRadius;
			break;
		case 2://eD_RIGHT:
			l_vPos.x = cGameApp::m_svGameResolution.x+e_fRadius;
			l_vPos.y = l_fPosY.Rand();
			break;
		case 3://eD_DOWN:
			l_vPos.x = l_fPosX.Rand();
			l_vPos.y = cGameApp::m_svGameResolution.y+e_fRadius;
			break;
	}
	l_vPos.z = 0.f;
	return l_vPos;
}

Vector3		cFishProbability::GetRandomPos(cMonster*e_pMonster)
{
	return GetRandomPos(e_pMonster->m_fRadius);
}

cMonster*	cFishProbability::MonsterGo(int e_iMonsterID)
{
	int	l_iIndex = e_iMonsterID;
	//cMonster*l_pMonster = cFishApp::m_spMonsterManager->Require(rand()%cFishApp::m_spMonsterManager->Count());
	cMonster*l_pMonster = cFishApp::m_spMonsterManager->Require(l_iIndex);
	return l_pMonster;
}

void	cFishProbability::MonsterGenerate(float e_fElpaseTime)
{
	if( m_bPauseGenerateMonster || !m_pProbabilityData )
		return;
	int	l_iMiniGameID = this->m_pProbabilityData->FetchWaitingToShowMiniGameID();
	//l_iMiniGameID = 0;
	if( l_iMiniGameID != -1 )
	{
		//indicate what mini game out
		int	l_iIndex = this->m_pProbabilityData->GetRandomMiniGameFishID();
		sMiniGameData::sMiniGameBank*l_pMiniGameBank = &this->m_pProbabilityData->GetMiniGame()->MiniGameBankList[l_iMiniGameID];
		MonsterGenerateWithMiniGame(l_iIndex,(WCHAR*)l_pMiniGameBank->strName.c_str(),false);
		//avoid generate too much monster
		//m_TimeGapToGenerateMonster.SetTargetTime(10.f);
	}
	if(m_pProbabilityData->m_MiniGameData.FakeMiniGameMonsterTime.bTragetTimrReached)
	{
		size_t	l_iMiniGameSize = m_pProbabilityData->m_MiniGameData.MiniGameBankList.size();
		if( l_iMiniGameSize )
		{
			int	l_iValue = m_pProbabilityData->m_MiniGameData.MiniGameBankList.size()%l_iMiniGameSize;
			sMiniGameData::sMiniGameBank*l_pMiniGameBank = &this->m_pProbabilityData->GetMiniGame()->MiniGameBankList[l_iValue];
			int	l_iIndex = this->m_pProbabilityData->GetRandomMiniGameFishID();
			MonsterGenerateWithMiniGame(l_iIndex,(WCHAR*)l_pMiniGameBank->strName.c_str(),true);
		}
	}
	m_TimeGapToGenerateMonster.Update(e_fElpaseTime);
	if( !m_TimeGapToGenerateMonster.bTragetTimrReached )
	{
		return;
	}
	m_TimeGapToGenerateMonster.SetTargetTime(this->m_pProbabilityData->m_fTimeToGenerateMonsters);
	//check for new fish
	//fucking game designer
	int	l_iRest = cFishApp::m_spMonsterManager->m_iMaxMonsterAlive-cFishApp::m_spMonsterManager->m_iNumMonsterAlive;
	if( m_pProbabilityData->m_iNumMonsterOutAtSameTime > 0 && l_iRest>0 )
	{
		int	l_iNumMonster = rand()%l_iRest%this->m_pProbabilityData->m_iNumMonsterOutAtSameTime;
		#define	SMALLEST_MOMSTER_AMOUNT_DIVISION	4
		if( cFishApp::m_spMonsterManager->m_iNumMonsterAlive < cFishApp::m_spMonsterManager->m_iMaxMonsterAlive/SMALLEST_MOMSTER_AMOUNT_DIVISION )
		{
			l_iNumMonster = cFishApp::m_spMonsterManager->m_iMaxMonsterAlive/SMALLEST_MOMSTER_AMOUNT_DIVISION;
		}
		if( l_iNumMonster )
		{
			float	l_fMultiFishGroupProbability = frand(0,1);
			if( l_fMultiFishGroupProbability <= m_pProbabilityData->m_fFishGroupProbability)
			{
				cFishApp::m_spMonsterManager->FishGroupGo();
				return;
			}
			
			for(int i=0;i<l_iNumMonster;++i )
			{
				int	l_iIndex = this->m_pProbabilityData->GetRandomFishID();
				sFishShowProbaility::eFishGoType l_eFishGoType = this->m_pProbabilityData->GetFishShowProbabilityList()->m_FishShowProbabilityList[l_iIndex].GetFishGoType();
				if( l_eFishGoType == sFishShowProbaility::eFGT_GROUP )
				{//fish group
					int	l_iFishCount = this->m_pProbabilityData->GetFishShowProbabilityList()->m_FishShowProbabilityList[l_iIndex].iFishGroupCount.Rand();
					l_iNumMonster -= cFishApp::m_spMonsterManager->FishGroupGo(l_iIndex,l_iFishCount)+1;
				}
				else
				if( l_eFishGoType == sFishShowProbaility::eFGT_GROUP_MPDI )
				{
					cMonster*l_pMonster = cFishApp::m_spMonsterManager->GetObject(l_iIndex);
					if( l_pMonster )
						l_iNumMonster -= cFishApp::m_spMonsterManager->FishGroupGo((WCHAR*)l_pMonster->GetName())+1;
				}
				else
				{
					MonsterGo(l_iIndex);
				}
			}
		}
	}
}

bool	cFishProbability::IsMoneyOutOk(int e_iMoney)
{
	//ensure revenue is bigger than m_iHappyRevenue.
	if(this->m_pProbabilityData->GetLeatestRevenue()-e_iMoney>this->m_pProbabilityData->GetHappyRevenue())
	{
		float	l_fNewProbability = this->m_pProbabilityData->GetNewProbailityByAddMoney(e_iMoney);
		float	l_fTargetProbability = m_pProbabilityData->GetTargetProbability();
		//check safe probability.
		float	l_fHappyRevenueToWinMoneyProbability = m_pProbabilityData->GetHappyRevenueToWinMoneyProbability();
		if( l_fTargetProbability - l_fNewProbability > l_fHappyRevenueToWinMoneyProbability )
		{
			if(l_fHappyRevenueToWinMoneyProbability>=frand(0,1))
			{//limit money check
				if( e_iMoney <=  m_pProbabilityData->GetHappyRevenueToWinMoneyLimit())
					return true;
			}
		}
	}
	return false;
}


void	cFishProbability::MonsterToDied(sHittedFishData*e_pHittedFishData,int e_iOutMoney)
{
	if( !e_pHittedFishData->pMonster->IsStatusAllowToDied() )
		return;
	e_pHittedFishData->pPlayer->AddScore(e_pHittedFishData->pMonster->GetPos(),e_pHittedFishData->pMonster,e_iOutMoney);
	e_pHittedFishData->pMonster->SetMonsterStatus(eMS_DIED_SHOW);
	e_pHittedFishData->pMonster->m_pHittedPlayer = e_pHittedFishData->pPlayer;
	e_pHittedFishData->pMonster->m_pBullet = e_pHittedFishData->pBullet;
	this->m_pProbabilityData->RevenueMinus(e_pHittedFishData->pBullet->GetPayRate(),e_iOutMoney,e_pHittedFishData->pPlayer->m_iPlayerID,false);
}

void	cFishProbability::MonsterToKeepGoing(sHittedFishData*e_pHittedFishData,float e_fAddProbability)
{
	if( e_pHittedFishData->pMonster->m_fRestProbabilityValueToDied >= e_fAddProbability )
	{
		e_pHittedFishData->pMonster->m_fRestProbabilityValueToDied -= e_fAddProbability;
	}
	//e_pHittedFishData->pMonster->SetMonsterStatus(eMS_ALIVE);
}

bool	cFishProbability::MiniGameMonsterDiedCheck(sHittedFishData*e_pHittedFishData)
{
	if( m_bThisFrameMiniGameFishDied  )
		return false;
	if( e_pHittedFishData->pMonster->IsFakeMiniGameMonster() )
		return false;
	const int l_MiniGameMagicNumber = 10;
	if( e_pHittedFishData->pMonster->IsMiniGameMonster() && cFishApp::m_spMonsterManager->m_iNumMonsterAlive > l_MiniGameMagicNumber && 
		cFishApp::m_spMonsterManager->m_iTotalAliveMonsterPrize > l_MiniGameMagicNumber )
	{
		if(cFishApp::m_spMiniGameManager->IsMiniGameUsing())
			return false;
		int	l_iPayRate = e_pHittedFishData->pBullet->GetPayRate();
		//ensure this bullet value is bigger than aeverage
		if( l_iPayRate >= this->m_pProbabilityData->m_BulletFireAverage.iBulletFireMoneyAeverage )
		{
			int	l_iMiniGameIndex = this->m_pProbabilityData->m_MiniGameData.IndexOfMiniGameName(e_pHittedFishData->pMonster->m_MiniGameName.c_str());
			assert( l_iMiniGameIndex != -1 &&"mini game name is error" );
			if( l_iMiniGameIndex == -1 )
				return false;
			int	l_iLeastOutPrize = 10;
			if( l_iMiniGameIndex == 3 )
			{//this mini game ask least prize is 50% of screen's monster
				l_iLeastOutPrize = cFishApp::m_spMonsterManager->m_iTotalAliveMonsterPrize/2;
			}
			if( this->m_pProbabilityData->m_MiniGameData.MiniGameBankList[l_iMiniGameIndex].fAteMoney >= l_iLeastOutPrize*l_iPayRate )
			{
				m_bThisFrameMiniGameFishDied = true;
				return true;
			}
			return false;
		}
		return false;
	}
	else
	{
		return true;
	}
	return false;
}
//
void	cFishProbability::Update(float e_fElpaseTime)
{
	m_bThisFrameMiniGameFishDied = false;
	if( cFishApp::m_spMonsterManager)
	{
		if( !cFishApp::m_sbSceneChange )
			MonsterGenerate(e_fElpaseTime);
		//UpdateHitMonster();
		UpdateHitMonsterForBankFormat();
	}
	if( m_pProbabilityData )
	{
		m_pProbabilityData->Update(e_fElpaseTime);
		if(cFishApp::m_spTestLogFile->IsWriteFile())
		{
			m_pProbabilityData->WrtieLog();
		}
	}
	m_WritePlayerMoneyTC.Update(e_fElpaseTime);
	if( m_WritePlayerMoneyTC.bTragetTimrReached )
	{
		WrtiePlayerMoney();
		m_WritePlayerMoneyTC.Start();
	}
}

void	cFishProbability::Render()
{
	SerialThread::serialCtrl().DebugRender();
}

void	cFishProbability::DebugRender()
{
	if( m_pProbabilityData )
		this->m_pProbabilityData->Render();
}

void	cFishProbability::RevenueAdd(int e_iMoney,int e_iPlayerID)
{
	if( m_pProbabilityData )
		this->m_pProbabilityData->RevenueAdd(e_iMoney,e_iPlayerID);
}

void	cFishProbability::RevenueMinus(int e_iPayRate,int e_iMoney,int e_iPlayerID,bool e_bMiniGame)
{
	if( m_pProbabilityData )
	{
		this->m_pProbabilityData->RevenueMinus(e_iPayRate,e_iMoney,e_iPlayerID,e_bMiniGame);
	}
}

bool	cFishProbability::IsMiniGameIsMonsterDied(cMonster*e_pMonster,int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney)
{
	const int	l_iMiniGameIndex = 0;
	int	l_iMoneyCouldOut = (int)this->m_pProbabilityData->m_MiniGameData.MiniGameBankList[l_iMiniGameIndex].fAteMoney;
	//give a super small money if mini game banke is too small
	int	l_iMiniBoundMoney = (*this->m_pProbabilityData->m_piCoinToScore);
	if( l_iMoneyCouldOut < l_iMiniBoundMoney )
	{
		l_iMoneyCouldOut = l_iMiniBoundMoney;
	}
	//
	if( !e_pMonster->IsStatusAllowToDied() )
	{
		return false;
	}
	int	l_iOutMoney = e_iPayRate*e_pMonster->GetPrize();
	//if( m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney)))
	if( l_iMoneyCouldOut> l_iOutMoney )
	{
		this->m_pProbabilityData->m_MiniGameData.fTotalAteMoney -= l_iOutMoney;
		this->m_pProbabilityData->m_MiniGameData.MiniGameBankList[l_iMiniGameIndex].fAteMoney -= l_iOutMoney;
		e_pPlayerBehaviorBase->AddScore(e_pMonster->GetPos(),e_pMonster,l_iOutMoney);
		e_pMonster->m_pHittedPlayer = e_pPlayerBehaviorBase;
		if( e_piGetMoney )
		{
			*e_piGetMoney = l_iOutMoney;
		}
		this->m_pProbabilityData->RevenueMinus(e_iPayRate,l_iOutMoney,e_pPlayerBehaviorBase->m_iPlayerID,true);
		e_pMonster->SetMonsterStatus(eMS_DIED_SHOW);
		return true;
	}
	return false;
}

void	cFishProbability::MiniGameMonsterDied(cMonster*e_pMonster,int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney)
{
	int	l_iOutMoney = e_iPayRate*e_pMonster->GetPrize();
	//if( m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney)))
	{
		e_pPlayerBehaviorBase->AddScore(e_pMonster->GetPos(),e_pMonster,l_iOutMoney);
		e_pMonster->m_pHittedPlayer = e_pPlayerBehaviorBase;
		if( e_piGetMoney )
		{
			*e_piGetMoney = l_iOutMoney;
		}
		this->m_pProbabilityData->RevenueMinus(e_iPayRate,l_iOutMoney,e_pPlayerBehaviorBase->m_iPlayerID,true);
		e_pMonster->SetMonsterStatus(eMS_DIED_SHOW);
		//return true;
	}
	//return false;
}

void	cFishProbability::MonsterGenerateWithMiniGame(int e_iMonsterMonsterID,WCHAR*e_strMiniGameName,bool e_bFakeMiniGame)
{
	cMonster*l_pMonster = MonsterGo(e_iMonsterMonsterID);
	if( l_pMonster )
	{
		if( e_bFakeMiniGame )
			l_pMonster->SetFakeMiniGameMonster(true);
		else
			l_pMonster->SetMiniGameMonster(true,e_strMiniGameName);
		if( cFishApp::m_spMiniGameManager )
			cFishApp::m_spMiniGameManager->f_RequireSpecificMiniGame(e_strMiniGameName,l_pMonster);
	}
}
//now fuckig stupid game designer make it must died.
void	cFishProbability::SameTypeMonsterToDiedByProbability(int e_iPayRate,std::vector<cMonster*>*e_pStoreList,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piOutMoney)
{
	const int	l_iMiniGameIndex = 1;
	int	l_iMoneyCouldOut = this->m_pProbabilityData->m_MiniGameData.WidthDrawMoney(l_iMiniGameIndex);
	//give a small money if mini game banke is too small
	int	l_iMiniBoundMoney = (*this->m_pProbabilityData->m_piCoinToScore)*10;
	if( l_iMoneyCouldOut < l_iMiniBoundMoney )
	{
		l_iMoneyCouldOut = l_iMiniBoundMoney;
	}
	*e_piOutMoney = 0;
	int	l_iFishKindCount = cFishApp::m_spMonsterManager->m_AllFish.Count();
	if( l_iFishKindCount == 0 )
		return;
	int*	l_piOutMoney = new int[l_iFishKindCount];
	int	l_iSmallMoneyOut = 999999999;
	int	l_iSmallOutFishIndex = -1;
	for( int i=0;i<l_iFishKindCount;++i )
	{
		l_piOutMoney[i] = 0;
		for( int j=0;j<cFishApp::m_spMonsterManager->m_iNumMonsterAlive;++j )
		{
			cMonster*l_pMonster = cFishApp::m_spMonsterManager->m_ppWorkingMonster[j];
			if( !l_pMonster->IsStatusAllowToDied() )
				continue;
			int	l_iTypeID = l_pMonster->GetID();
			if( l_iTypeID == i )
			{
				l_piOutMoney[i] += l_pMonster->GetPrize();
			}
		}
		l_piOutMoney[i] *= e_iPayRate;
		if( l_piOutMoney[i] >0 )
		{
			if( l_iSmallMoneyOut > l_piOutMoney[i] )
			{
				l_iSmallMoneyOut = l_piOutMoney[i];
				l_iSmallOutFishIndex = i;
			}
		}
	}
	std::vector<int>			l_DiedPossibleFishID;
	for( int i=0;i<l_iFishKindCount;++i )
	{
		if( l_piOutMoney[i] < l_iMoneyCouldOut )
		{
			l_DiedPossibleFishID.push_back(i);
		}
	}
	//find out match fish
	if( l_DiedPossibleFishID.size() > 0 )
	{
		int	l_iMaxID = -1;
		int	l_iBigMoney = -1;
		for( size_t i=0;i<l_DiedPossibleFishID.size();++i )	
		{
			int	l_iMoney  =l_piOutMoney[l_DiedPossibleFishID[i]];
			if( l_iMoney > l_iBigMoney )
			{
				l_iBigMoney = l_iMoney;
				l_iMaxID = l_DiedPossibleFishID[i];
			}
		}
		l_iSmallOutFishIndex = l_iMaxID;
	}
	//smallest fish ot allow to died fish
	if( l_iSmallOutFishIndex != -1  )
	{//no fish!?
		float	l_fPercentageToDied = 1.f;
		float	l_fOffset = this->m_pProbabilityData->GetNewProbailityByAddMoney(l_piOutMoney[l_iSmallOutFishIndex])-m_pProbabilityData->GetCurrentProbability();
		if( l_fOffset < 0.f )
		{//lost too much money,give a small fish to died
			l_fPercentageToDied = frand(0.1f,0.5f);
		}
		for( int j=0;j<cFishApp::m_spMonsterManager->m_iNumMonsterAlive;++j )
		{
			cMonster*l_pMonster = cFishApp::m_spMonsterManager->m_ppWorkingMonster[j];
			int	l_iTypeID = l_pMonster->GetID();
			if( !l_pMonster->IsStatusAllowToDied() || l_iTypeID != l_iSmallOutFishIndex)
				continue;
			//bool	l_bDied = frand(0,1.f)<=l_fPercentageToDied?true:false;
			//if( l_bDied || e_pStoreList->size() == 0 )
			{
				int	l_iOutMoney = e_iPayRate*l_pMonster->GetPrize();
				e_pPlayerBehaviorBase->AddScore(l_pMonster->GetPos(),l_pMonster,l_iOutMoney);
				l_pMonster->m_pHittedPlayer = e_pPlayerBehaviorBase;
				this->m_pProbabilityData->RevenueMinus(e_iPayRate,l_iOutMoney,e_pPlayerBehaviorBase->m_iPlayerID,true);
				l_pMonster->SetMonsterStatus(eMS_DIED_SHOW);
				if( e_piOutMoney )
					*e_piOutMoney += l_iOutMoney;
				e_pStoreList->push_back(l_pMonster);
			}
		}
	}
	//no any fish force to give one
	if( e_pStoreList->size() == 0 )
	{
		int	l_iPrize = 0;
		int	l_iOutMoney = 0;
		int	l_iSmallestFishID = cFishApp::m_spMonsterManager->GetSmallestPrizeFishID(&l_iPrize);
		//give a smallest fish
		cMonster*l_pMonster = this->MonsterGo(l_iSmallestFishID);
		if(	!l_pMonster	)
		{//give a smallest fish ID from alive fishes.
			l_iSmallestFishID = cFishApp::m_spMonsterManager->GetSmallestPrizeFishIDFromAliveFish(&l_iPrize);
			l_pMonster = this->MonsterGo(l_iSmallestFishID);
			if(!l_pMonster)
			{//force to give a fish
				l_pMonster = this->MonsterGo(this->m_pProbabilityData->GetRandomMiniGameFishID());
			}
		}
		if( l_pMonster )
		{
			l_iOutMoney = l_iPrize*e_iPayRate;
			*e_piOutMoney = l_iOutMoney;
			e_pPlayerBehaviorBase->AddScore(l_pMonster->GetPos(),l_pMonster,l_iOutMoney);
			l_pMonster->m_pHittedPlayer = e_pPlayerBehaviorBase;
			this->m_pProbabilityData->RevenueMinus(e_iPayRate,l_iOutMoney,e_pPlayerBehaviorBase->m_iPlayerID,true);
			l_pMonster->SetMonsterStatus(eMS_DIED_SHOW);
			e_pStoreList->push_back(l_pMonster);
		}
		else
		{
			int a=0;
		}
	}
	delete l_piOutMoney;
#ifdef _DEBUG
	std::string	l_str = UT::ComposeMsgByFormat("SameTypeMonsterToDiedByProbability Money:%d",*e_piOutMoney);
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
#endif
}
//
float		cFishProbability::AllMonsterDiedByProbability(int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney)
{
	const int	l_iMiniGameIndex = 3;
	int	l_iMoneyCouldOut = (int)this->m_pProbabilityData->m_MiniGameData.MiniGameBankList[l_iMiniGameIndex].fAteMoney;
	//give a small money if mini game banke is too small
	int	l_iMiniBoundMoney = (*this->m_pProbabilityData->m_piCoinToScore)*10;
	if( l_iMoneyCouldOut < l_iMiniBoundMoney )
	{
		l_iMoneyCouldOut = l_iMiniBoundMoney;
	}
	int	l_iOutMoney = 0;
	int	l_iPrize = 0;
	int	l_iDiedFish = 0;
	int	l_iSmallestFishID = cFishApp::m_spMonsterManager->GetSmallestPrizeFishIDFromAliveFish(&l_iPrize);
	if(  l_iSmallestFishID != -1  )
		l_iSmallestFishID = cFishApp::m_spMonsterManager->m_ppWorkingMonster[l_iSmallestFishID]->GetID();
	//first find out all small prize monster
	for( int j=0;j<cFishApp::m_spMonsterManager->m_iNumMonsterAlive;++j )
	{
		cMonster*l_pMonster = cFishApp::m_spMonsterManager->m_ppWorkingMonster[j];
		if( l_pMonster->GetID() == l_iSmallestFishID )
		{//avoid fish could be died
			if( l_pMonster->IsStatusAllowToDied() )
			{
				int	l_iPayMoney = l_iPrize*e_iPayRate;
				//if( *e_piGetMoney && *e_piGetMoney >0 )
				{
					//if( !m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iPayMoney)) )
					if( l_iOutMoney+l_iPayMoney > l_iMoneyCouldOut )
					{
						goto EXIT;
					}
				}
				l_iOutMoney += l_iPayMoney;
				l_pMonster->SetMonsterStatus(eMS_DIED_SHOW);
				this->m_pProbabilityData->RevenueMinus(e_iPayRate,l_iPayMoney,e_pPlayerBehaviorBase->m_iPlayerID,true);
				e_pPlayerBehaviorBase->AddScore(l_pMonster->GetPos(),l_pMonster,l_iPayMoney);
				l_pMonster->m_pHittedPlayer = e_pPlayerBehaviorBase;
				++l_iDiedFish;
			}
		}
	}

	for( int j=0;j<cFishApp::m_spMonsterManager->m_iNumMonsterAlive;++j )
	{
		cMonster*l_pMonster = cFishApp::m_spMonsterManager->m_ppWorkingMonster[j];
		if( l_pMonster->IsStatusAllowToDied() )
		{
			//if(rand()%2)
			{
				l_iPrize = l_pMonster->GetPrize();
				int	l_iPayMoney = l_iPrize*e_iPayRate;
				if( l_iOutMoney+l_iPayMoney > l_iMoneyCouldOut )
				//if( !m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney)))
				{
					goto EXIT;
				}
				l_iOutMoney += l_iPayMoney;
				l_pMonster->SetMonsterStatus(eMS_DIED_SHOW);
				this->m_pProbabilityData->RevenueMinus(e_iPayRate,l_iPayMoney,e_pPlayerBehaviorBase->m_iPlayerID,true);
				e_pPlayerBehaviorBase->AddScore(l_pMonster->GetPos(),l_pMonster,l_iPayMoney);
				l_pMonster->m_pHittedPlayer = e_pPlayerBehaviorBase;
				++l_iDiedFish;
			}
		}
	}
EXIT:
	if( e_piGetMoney )
		*e_piGetMoney = l_iOutMoney;
	this->m_pProbabilityData->m_MiniGameData.MiniGameBankList[l_iMiniGameIndex].fAteMoney -= l_iOutMoney;
	this->m_pProbabilityData->m_MiniGameData.fTotalAteMoney -= l_iOutMoney;
#ifdef _DEBUG
	std::string	l_str = UT::ComposeMsgByFormat("AllMonsterDiedByProbability Money:%d",l_iOutMoney);
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
#endif
	return l_iDiedFish/(float)cFishApp::m_spMonsterManager->m_iNumMonsterAlive;
}

cMonster*	cFishProbability::GetConsecutiveMonsterToDied(int e_iPayRate,int e_iIndex,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney)
{
	const int	l_iMiniGameIndex = 2;
	int	l_iMoneyCouldOut = (int)this->m_pProbabilityData->m_MiniGameData.MiniGameBankList[l_iMiniGameIndex].fAteMoney;
	int	l_iPrize = 0;
	int	l_iSmallestPayrateMonsterIndex = -1;
	//find a monster and check mini game bank Money
	for( int i=0;i<cFishApp::m_spMonsterManager->m_iNumMonsterAlive;++i )
	{
		cMonster*l_pMonster2 = cFishApp::m_spMonsterManager->m_ppWorkingMonster[i];
		if( l_pMonster2->IsStatusAllowToDied() && l_pMonster2->IsVisible() )
		{
			int	l_iOutMoney = l_pMonster2->GetPrize()*e_iPayRate;
			if( l_iMoneyCouldOut > l_iOutMoney )
			{
				l_iSmallestPayrateMonsterIndex = i;
				break;
			}
		}
	}
	int	l_iCount = rand(3,7);
	if( l_iCount < e_iIndex  )
	{
		return 0;
	}
	cMonster*l_pMonster = 0;
	if( l_iSmallestPayrateMonsterIndex == -1 )
	{
		if( e_iIndex == 0 )
		{
			//give a smallest fish anyway
			l_iSmallestPayrateMonsterIndex = cFishApp::m_spMonsterManager->GetSmallestPrizeFishID(&l_iPrize);
			l_pMonster = this->MonsterGo(l_iSmallestPayrateMonsterIndex);
			//give a random fish,out of probability control
			if( !l_pMonster )
			{
				sMiniGameData*l_pMiniGameData = this->m_pProbabilityData->GetMiniGame();
				size_t	l_uiSize = l_pMiniGameData->MiniGameFishIDList.size();
				for( size_t i=0;i<l_uiSize;++i )
				{
					l_pMonster = this->MonsterGo(l_pMiniGameData->MiniGameFishIDList[i]);
					if( l_pMonster )
						break;
				}
			}
		}
	}
	else
	{
		l_pMonster = cFishApp::m_spMonsterManager->m_ppWorkingMonster[l_iSmallestPayrateMonsterIndex];			
	}
	if( l_pMonster )
	{
		int	l_iOutMoney = l_pMonster->GetPrize()*e_iPayRate;
		if( e_piGetMoney )
			*e_piGetMoney = l_iOutMoney;
		this->m_pProbabilityData->m_MiniGameData.MiniGameBankList[l_iMiniGameIndex].fAteMoney -= l_iOutMoney;
		this->m_pProbabilityData->m_MiniGameData.fTotalAteMoney -= l_iOutMoney;
	}
#ifdef _DEBUG
	std::string	l_str = UT::ComposeMsgByFormat("GetConsecutiveMonsterToDied Money:%d",*e_piGetMoney);
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
#endif
	return l_pMonster;
}

void	cFishProbability::OpenPlayerMoney()
{
	cBinaryFile l_BinaryFile(true);
	if(l_BinaryFile.Openfile(PLAYER_MONEY_DATA))
	{
		int	l_iSize = l_BinaryFile.GetDataFileSize();
		if( l_iSize )
		{
			int	l_iDivide = l_iSize/sizeof(int64);
			//avoid spuid guy open as text and save it.
			int	l_iSkipHedaer = l_iSize%l_iDivide;
			if( l_iDivide > 12 )//ensure total in and total out was written
			{
				byte*l_pData = (byte*)l_BinaryFile.GetDataFile(0);
				l_pData += l_iSkipHedaer;
				assert(l_iSkipHedaer == 0);
				int	l_iSkipDivide = 16;
				*this->m_pProbabilityData->m_pi64TotalInMoney = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64TotalOutMoney = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64TotalRevenue = *this->m_pProbabilityData->m_pi64TotalInMoney-*this->m_pProbabilityData->m_pi64TotalOutMoney;
				*this->m_pProbabilityData->m_pi64TotalExchangeIn = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64TotalExchangeOut = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64TotalCoinIn = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64TotalCoinOut = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);

				*this->m_pProbabilityData->m_pi64LatestInMoney = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64LatestOutMoney = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64LatestRevenue = *this->m_pProbabilityData->m_pi64LatestInMoney-*this->m_pProbabilityData->m_pi64LatestOutMoney;
				*this->m_pProbabilityData->m_pi64LatestExchangeIn = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64LatestExchangeOut = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64LatestCoinIn = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);
				*this->m_pProbabilityData->m_pi64LatestCoinOut = l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(int64);

				size_t	l_uiSize = m_pProbabilityData->m_MiniGameData.MiniGameBankList.size();

				for( size_t i=0;i<l_uiSize;++i )
				{
					double*l_pdbDouble = (double*)l_pData;
					m_pProbabilityData->m_MiniGameData.MiniGameBankList[i].fAteMoney = (double)l_BinaryFile.GetInt64(l_pData);l_pData += sizeof(double);
				}
				if( cFishApp::m_spPlayerManager&&cFishApp::m_spPlayerManager->Count() )
				{
					if(!cFishApp::m_spPlayerManager->GetObject(0)->m_bAutoPlay)
					{
						int	l_iPlayerCount = cFishApp::m_spPlayerManager->Count();
						if( l_iPlayerCount == l_iDivide-l_iSkipDivide )
						{//player count is same so give money or cler
							for( int i=0;i<l_iDivide-l_iSkipDivide;++i )
							{
								int64	l_64Value = l_BinaryFile.GetInt64(l_pData);
								cFishApp::m_spPlayerManager->GetObject(i)->m_i64Score = l_64Value;
								l_pData += sizeof(int64);
							}
						}
					}
				}
			}
		}
	}
	l_BinaryFile.CloseFile();
	SAFE_DELETE(m_pPlayerMoneyLogFile);
	m_pPlayerMoneyLogFile = new cBinaryFile();
	m_pPlayerMoneyLogFile->Writefile(PLAYER_MONEY_DATA,true,cGameApp::m_sbDeviceExist);
	WrtiePlayerMoney();
}

void	cFishProbability::WrtiePlayerMoney()
{
	if(cFishApp::m_spPlayerManager&&m_pPlayerMoneyLogFile)
	{
		m_pPlayerMoneyLogFile->SetFilePos(0);
		//6
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64TotalInMoney);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64TotalOutMoney);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64TotalExchangeIn);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64TotalExchangeOut);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64TotalCoinIn);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64TotalCoinOut);
		//6
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestInMoney);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestOutMoney);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestExchangeIn);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestExchangeOut);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestCoinIn);
		m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestCoinOut);
		//
		size_t	l_uiSize = m_pProbabilityData->m_MiniGameData.MiniGameBankList.size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			m_pPlayerMoneyLogFile->WriteToFile((int64)m_pProbabilityData->m_MiniGameData.MiniGameBankList[i].fAteMoney);
		}
		int	l_iPlayerCount = cFishApp::m_spPlayerManager->Count();
		for( int i=0;i<l_iPlayerCount;++i )
		{
			m_pPlayerMoneyLogFile->WriteToFile(cFishApp::m_spPlayerManager->GetObject(i)->m_i64Score);
		}
		m_pPlayerMoneyLogFile->Flush();
	}
}

void	cFishProbability::ClearLatestRecord()
{
	*m_pProbabilityData->m_pi64LatestInMoney = 0;
	*m_pProbabilityData->m_pi64LatestOutMoney = 0;
	*m_pProbabilityData->m_pi64LatestRevenue = 0;
	*m_pProbabilityData->m_pi64LatestExchangeIn = 0;
	*m_pProbabilityData->m_pi64LatestExchangeOut = 0;
	*m_pProbabilityData->m_pi64LatestCoinIn = 0;
	*m_pProbabilityData->m_pi64LatestCoinOut = 0;
	//first 6 is history data.
	m_pPlayerMoneyLogFile->SetFilePos(sizeof(int64)*6);
	//6
	m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestInMoney);
	m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestOutMoney);
	m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestExchangeIn);
	m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestExchangeOut);
	m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestCoinIn);
	m_pPlayerMoneyLogFile->WriteToFile(*m_pProbabilityData->m_pi64LatestCoinOut);

	m_pPlayerMoneyLogFile->Flush();
}
void	cFishProbability::ExchangeIn(int64 e_i64Value)
{
	*m_pProbabilityData->m_pi64LatestExchangeIn += e_i64Value;
	*m_pProbabilityData->m_pi64TotalExchangeIn += e_i64Value;
}
void	cFishProbability::ExchangeOut(int64 e_i64Value)
{
	*m_pProbabilityData->m_pi64TotalExchangeOut += e_i64Value;
	*m_pProbabilityData->m_pi64LatestExchangeOut += e_i64Value;
}

void	cFishProbability::ToastCoin(int64 e_i64Value)
{
	*m_pProbabilityData->m_pi64TotalCoinIn += e_i64Value;
	*m_pProbabilityData->m_pi64LatestCoinIn += e_i64Value;
}
void	cFishProbability::RefundCoin(int64 e_i64Value)
{
	*m_pProbabilityData->m_pi64LatestCoinOut += e_i64Value;
	*m_pProbabilityData->m_pi64TotalCoinOut += e_i64Value;
}