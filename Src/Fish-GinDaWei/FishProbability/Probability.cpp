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

cFishProbability::cFishProbability()
{
	m_pPlayerMoneyLogFile = 0;
	m_WritePlayerMoneyTC.SetTargetTime(cGameApp::m_sbDeviceExist?0.1f:1.f);
	m_WritePlayerMoneyTC.Start();
	m_TimeGapToGenerateMonster.SetTargetTime(0.5f);
	m_pProbabilityData = 0;
	m_bPauseGenerateMonster = false;
}

cFishProbability::~cFishProbability()
{
	SAFE_DELETE(m_pProbabilityData);
	SAFE_DELETE(m_pPlayerMoneyLogFile);
}


void	cFishProbability::Init()
{
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

//1280:720 = 17 :10,but 27 is not even so make it as 26
const int			g_iTotalDirectionOrder = 26;//17+10
const int			g_iDirectionOrder[26] = {0,1,2,3,1,3,//6
											 0,1,2,3,1,3,//6
											 0,1,2,3,1,	 //5
											 0,1,2,3,3,	 //5
											 0,1,2,3};	 //4
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


//bool		cFishProbability::HittedMonsterToDied(cMonster*e_pMonster,int e_iPayRate)
//{
//	return false;
//}
//
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
		MonsterGenerateWithMiniGame(l_iIndex,(WCHAR*)l_pMiniGameBank->strName.c_str());
		//avoid generate too much monster
		//m_TimeGapToGenerateMonster.SetTargetTime(10.f);
	}
	m_TimeGapToGenerateMonster.Update(e_fElpaseTime);
	if( !m_TimeGapToGenerateMonster.bTragetTimrReached )
	{
		return;
	}
	m_TimeGapToGenerateMonster.SetTargetTime(this->m_pProbabilityData->m_fTimeToGenerateMonsters);
	//check for new fish
	//fuck game designer
	int	l_iRest = cFishApp::m_spMonsterManager->m_iMaxMonsterAlive-cFishApp::m_spMonsterManager->m_iNumMonsterAlive;
	if( m_pProbabilityData->m_iNumMonsterOutAtSameTime > 0 && l_iRest>0 )
	{
		int	l_iNumMonster = rand()%l_iRest%this->m_pProbabilityData->m_iNumMonsterOutAtSameTime;
#define	SMALLEST_MOMSTER_AMOUNT_DIVISION	4
		if( cFishApp::m_spMonsterManager->m_iNumMonsterAlive < cFishApp::m_spMonsterManager->m_iMaxMonsterAlive/SMALLEST_MOMSTER_AMOUNT_DIVISION )
		{
			l_iNumMonster = cFishApp::m_spMonsterManager->m_iMaxMonsterAlive/SMALLEST_MOMSTER_AMOUNT_DIVISION;
		}
		int	l_iIndex = this->m_pProbabilityData->GetRandomFishID();
		float	l_fProbability = this->m_pProbabilityData->GetFishShowProbabilityList()->m_FishShowProbabilityList[l_iIndex].fFishGroupProbability;
		if( l_iNumMonster )
		{
			if( frand(0,1) < l_fProbability )
			{//fish group
				int	l_iFishCount = this->m_pProbabilityData->GetFishShowProbabilityList()->m_FishShowProbabilityList[l_iIndex].iFishGroupCount.Rand();
				cFishApp::m_spMonsterManager->FishGroupGo(l_iIndex,l_iFishCount);
				return;
			}
			else
			if( frand(0,1.f) < this->m_pProbabilityData->m_fFishGroupProbability )
			{
				cFishApp::m_spMonsterManager->FishGroupGo();
			}
			else
			{//single fish
				if( l_iRest>0&& this->m_pProbabilityData->m_iNumMonsterOutAtSameTime>0)
				{
					for(int i=0;i<l_iNumMonster;++i )
					{
						int	l_iIndex2 = this->m_pProbabilityData->GetRandomFishID();
						MonsterGo(l_iIndex2);
					}
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

bool	cFishProbability::DoProbailityWithTotalRevenue(sHittedFishData*e_pHittedFishData,float*e_pfAddProbability,int*e_piOutMoney)
{
	sHittedFishData*l_pHittedFishData = e_pHittedFishData;
	int	l_iOutMoney = l_pHittedFishData->pMonster->GetPrize()*l_pHittedFishData->pBullet->GetPayRate();
	*e_piOutMoney = l_iOutMoney;
	//check bite money and out money.
	if(l_pHittedFishData->pMonster->m_iTotalBiteMoney>=l_iOutMoney)
	{//if bite money is enough and check happy revenue,then give a random chance to give money
		float	l_fAddDiedProbability = this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney);
		if( l_iOutMoney < this->m_pProbabilityData->GetFakeRevenue() )
		{
			return true;
		}
	}

	float	l_fNewProbability = m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney);
	if( l_fNewProbability<this->m_pProbabilityData->GetTargetProbability() )
	{
		return true;
	}
	else
	{
		float	l_fAddDiedProbability = this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney);
		if( l_fAddDiedProbability > 1.f )
			l_fAddDiedProbability = 0.f;
		l_fAddDiedProbability *= frand(0,1);
		float	l_fCurrentProbability = this->m_pProbabilityData->GetCurrentProbability();
		*e_pfAddProbability = this->m_pProbabilityData->GetFishDiedHappyProbability(l_fCurrentProbability,l_fAddDiedProbability);
	}
	return false;
}

bool	cFishProbability::DoBulletPayRateCheck(int e_iBulletPayRate,sHittedFishData*e_pHittedFishData)
{	
	float	l_fValue = frand(0,1);
	if( this->m_pProbabilityData->GetReleatePayRateProbability(e_iBulletPayRate,e_pHittedFishData->pBullet->GetNumMonsterShoot()) > l_fValue )
	{
		return true;
	}
	return false;
}

//bool	cFishProbability::DoProbailityWithFishProbability(sHittedFishData*e_pHittedFishData,float*e_pfAddProbability,int*e_piOutMoney)
//{
//	sHittedFishData*l_pHittedFishData = e_pHittedFishData;
//	bool	l_bDied = false;
//	int	l_iOutMoney = l_pHittedFishData->pMonster->GetPrize()*l_pHittedFishData->pBullet->GetPayRate();
//	*e_piOutMoney = l_iOutMoney;
//	//check bite money and out money.
//	if(l_pHittedFishData->pMonster->m_iTotalBiteMoney>=l_iOutMoney)
//	{
//		l_bDied = true;
//	}
//	if( !l_bDied )
//	{//bite money is no enough,check probability
//		//float	l_fNewProbability = this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney);
//		float	l_fAddDiedProbability = this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney);
//		l_fAddDiedProbability *= frand(0,1);
//		float	l_fCurrentProbability = this->m_pProbabilityData->GetCurrentProbability();
//		l_fAddDiedProbability = this->m_pProbabilityData->GetFishDiedHappyProbability(l_fCurrentProbability,l_fAddDiedProbability);
//		float	l_fFinalValue = l_pHittedFishData->pMonster->m_fRestProbabilityValueToDied - l_fAddDiedProbability;
//		//OutputDebugString(ValueToStringW(l_fAddDiedProbability));
//		//OutputDebugString(L"\n");
//		//if( l_fAddDiedProbability >= l_pHittedFishData->pMonster->m_fProbabilityToDied || l_fFinalValue <= 0.f )
//		if( l_fFinalValue <= l_pHittedFishData->pMonster->m_fProbabilityToDied )
//		{
//			l_bDied = true;
//		}
//		else
//		{
//			*e_pfAddProbability = l_fAddDiedProbability;
//		}
//	}
//	return l_bDied;
//}

void	cFishProbability::MonsterToDied(sHittedFishData*e_pHittedFishData,int e_iOutMoney)
{
	if( e_pHittedFishData->pMonster->GetMonsterStatus() == eMS_WAITING_FOR_CLEAN )
		return;
	e_pHittedFishData->pPlayer->AddScore(e_pHittedFishData->pMonster->GetPos(),e_pHittedFishData->pMonster,e_iOutMoney);
	e_pHittedFishData->pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
	e_pHittedFishData->pMonster->m_pHittedPlayer = e_pHittedFishData->pPlayer;
	e_pHittedFishData->pMonster->m_pBullet = e_pHittedFishData->pBullet;
	this->m_pProbabilityData->RevenueMinus(e_iOutMoney,e_pHittedFishData->pPlayer->m_iPlayerID);
}

void	cFishProbability::MonsterToKeepGoing(sHittedFishData*e_pHittedFishData,float e_fAddProbability)
{
	if( e_pHittedFishData->pMonster->m_fRestProbabilityValueToDied >= e_fAddProbability )
		e_pHittedFishData->pMonster->m_fRestProbabilityValueToDied -= e_fAddProbability;
	//e_pHittedFishData->pMonster->SetMonsterStatus(eMS_ALIVE);
}

void	cFishProbability::UpdateHitMonster()
{
	for( int i=0;i<cFishApp::m_spMonsterManager->m_iNumMonsterHitted;++i )
	{
		sHittedFishData*l_pHittedFishData = &cFishApp::m_spMonsterManager->m_pHittedFish[i];
		int	l_iPayRate = l_pHittedFishData->pBullet->GetPayRate();
		bool	l_bFishDied = false;
		int		l_iOutMoney = 0;
		float	l_fAddProbability = 0.f;
		if(DoBulletPayRateCheck(l_iPayRate,l_pHittedFishData))
		{
			l_bFishDied = DoProbailityWithTotalRevenue(l_pHittedFishData,&l_fAddProbability,&l_iOutMoney);
			//bool	l_bFishDied = DoProbailityWithFishProbability(l_pHittedFishData,&l_fAddProbability,&l_iOutMoney);
			if(l_bFishDied||IsMoneyOutOk(l_iOutMoney))
			{
				l_bFishDied = true;
			}
		}
		if( l_bFishDied )
			MonsterToDied(l_pHittedFishData,l_iOutMoney);
		else
			MonsterToKeepGoing(l_pHittedFishData,l_fAddProbability);
	}
	cFishApp::m_spMonsterManager->m_iNumMonsterHitted = 0;
	if( cFishApp::m_spMonsterManager->m_pTurtleBoss && cFishApp::m_spMonsterManager->m_pTurtleBoss->IsBiteEnoughMoney())
	{
		if( cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedBullet )
		{
			if( cFishApp::m_spMonsterManager->m_pTurtleBoss->GetMonsterStatus() != eMS_WAITING_FOR_CLEAN && cFishApp::m_spMonsterManager->m_pTurtleBoss->GetMonsterStatus() != eMS_NONE )
			{
				int	l_iOutMoney = cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedBullet->GetPayRate()*cFishApp::m_spMonsterManager->m_pTurtleBoss->GetPrize();
				if(this->m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney)))
				{
					cFishApp::m_spMonsterManager->m_pTurtleBoss->SetMonsterStatus(eMS_NONE);
					this->m_pProbabilityData->RevenueMinus(l_iOutMoney,cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedPlayer->m_iPlayerID);
					cGameApp::SoundPlay(L"8",true);
					cNormalFish*l_pFish = cFishApp::m_spMonsterManager->m_pTurtleBoss->GetCurrentWorkingObject();
					cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedPlayer->AddScore(l_pFish->GetPos(),l_pFish,l_iOutMoney);
				}
			}
		}
	}
}

void	cFishProbability::Update(float e_fElpaseTime)
{
	if( cFishApp::m_spMonsterManager)
	{
		if( !cFishApp::m_sbSceneChange )
			MonsterGenerate(e_fElpaseTime);
		UpdateHitMonster();
	}

	if( m_pProbabilityData )
	{
		m_pProbabilityData->Update(e_fElpaseTime);
	} //end if

	if(cFishApp::m_spTestLogFile->IsWriteFile())
	{
		m_pProbabilityData->WrtieLog();
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

}

void	cFishProbability::DebugRender()
{
	this->m_pProbabilityData->Render();
}

void	cFishProbability::RevenueAdd(int e_iMoney,int e_iPlayerID)
{
	this->m_pProbabilityData->RevenueAdd(e_iMoney,e_iPlayerID);
}

void	cFishProbability::RevenueMinus(int e_iMoney,int e_iPlayerID)
{
	this->m_pProbabilityData->RevenueMinus(e_iMoney,e_iPlayerID);
}

bool	cFishProbability::IsMonsterDied(cMonster*e_pMonster,int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney)
{
	const int	l_iMiniGameIndex = 0;
	int	l_iMoneyCouldOut = (int)this->m_pProbabilityData->m_MiniGameData.MiniGameBankList[l_iMiniGameIndex].fAteMoney;
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
		this->m_pProbabilityData->RevenueMinus(l_iOutMoney,e_pPlayerBehaviorBase->m_iPlayerID);
		e_pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
		return true;
	}
	return false;
}

void	cFishProbability::MonsterDied(cMonster*e_pMonster,int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney)
{
	if( e_pMonster->GetMonsterStatus() == eMS_WAITING_FOR_CLEAN )
	{
		return;
	}
	int	l_iOutMoney = e_iPayRate*e_pMonster->GetPrize();
	//if( m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney)))
	{
		e_pPlayerBehaviorBase->AddScore(e_pMonster->GetPos(),e_pMonster,l_iOutMoney);
		e_pMonster->m_pHittedPlayer = e_pPlayerBehaviorBase;
		if( e_piGetMoney )
		{
			*e_piGetMoney = l_iOutMoney;
		}
		this->m_pProbabilityData->RevenueMinus(l_iOutMoney,e_pPlayerBehaviorBase->m_iPlayerID);
		e_pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
		//return true;
	}
	//return false;
}

void	cFishProbability::MonsterGenerateWithMiniGame(int e_iMonsterMonsterID,WCHAR*e_strMiniGameName)
{
	cMonster*l_pMonster = MonsterGo(e_iMonsterMonsterID);
	if( l_pMonster )
	{
		if( cFishApp::m_spMiniGameManager )
			cFishApp::m_spMiniGameManager->f_RequireSpecificMiniGame(e_strMiniGameName,l_pMonster);
	}
}
//now fuckig stupid game designer make it must died.
void	cFishProbability::SameTypeMonsterToDiedByProbability(int e_iPayRate,std::vector<cMonster*>*e_pStoreList,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piOutMoney)
{
	const int	l_iMiniGameIndex = 1;
	int	l_iMoneyCouldOut = this->m_pProbabilityData->m_MiniGameData.WidthDrawMoney(l_iMiniGameIndex);

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
				this->m_pProbabilityData->RevenueMinus(l_iOutMoney,e_pPlayerBehaviorBase->m_iPlayerID);
				l_pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
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
			this->m_pProbabilityData->RevenueMinus(l_iOutMoney,e_pPlayerBehaviorBase->m_iPlayerID);
			l_pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
			e_pStoreList->push_back(l_pMonster);
		}
		else
		{
			int a=0;
		}
	}
	delete l_piOutMoney;
}
//
float		cFishProbability::AllMonsterDiedByProbability(int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney)
{
	const int	l_iMiniGameIndex = 3;
	int	l_iMoneyCouldOut = this->m_pProbabilityData->m_MiniGameData.WidthDrawMoney(l_iMiniGameIndex);
	int	l_iOutMoney = 0;
	int	l_iPrize = 0;
	int	l_iDiedFish = 0;
	int	l_iSmallestFishID = cFishApp::m_spMonsterManager->GetSmallestPrizeFishIDFromAliveFish(&l_iPrize);
	//first find out all small prize monster
	for( int j=0;j<cFishApp::m_spMonsterManager->m_iNumMonsterAlive;++j )
	{
		cMonster*l_pMonster = cFishApp::m_spMonsterManager->m_ppWorkingMonster[j];
		if( l_pMonster->GetID() == l_iSmallestFishID )
		{//avoid fish could be died
			if( l_pMonster->IsStatusAllowToDied() )
			{
				int	l_iPayMoney = l_iPrize*e_iPayRate;
				l_iOutMoney += l_iPayMoney;
				if( *e_piGetMoney && *e_piGetMoney >0 )
				{
					//if( !m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iPayMoney)) )
					if( l_iOutMoney > l_iMoneyCouldOut )
					{
						goto EXIT;
					}
				}
				l_pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
				this->m_pProbabilityData->RevenueMinus(l_iPayMoney,e_pPlayerBehaviorBase->m_iPlayerID);
				if( e_piGetMoney )
					*e_piGetMoney += l_iPayMoney;
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
				l_iOutMoney += l_iPayMoney;
				if( l_iOutMoney > l_iMoneyCouldOut )
				//if( !m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney)))
				{
					goto EXIT;
				}
				l_pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
				if( e_piGetMoney )
					*e_piGetMoney += l_iOutMoney;
				this->m_pProbabilityData->RevenueMinus(l_iOutMoney,e_pPlayerBehaviorBase->m_iPlayerID);
				e_pPlayerBehaviorBase->AddScore(l_pMonster->GetPos(),l_pMonster,l_iOutMoney);
				l_pMonster->m_pHittedPlayer = e_pPlayerBehaviorBase;
				++l_iDiedFish;
			}
		}

	}
EXIT:

	return l_iDiedFish/(float)cFishApp::m_spMonsterManager->m_iNumMonsterAlive;
}

cMonster*	cFishProbability::GetConsecutiveMonsterToDied(int e_iPayRate,int e_iIndex,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney)
{
	const int	l_iMiniGameIndex = 2;
	int	l_iMoneyCouldOut = (int)this->m_pProbabilityData->m_MiniGameData.MiniGameBankList[l_iMiniGameIndex].fAteMoney;
	int	l_iPrize = 0;
	cMonster*l_pMonster = 0;
	int	l_iSmallestPayrateMonsterIndex = -1;
	//find a monster and check mini game bank Money
	for( int i=0;i<cFishApp::m_spMonsterManager->m_iNumMonsterAlive;++i )
	{
		l_pMonster = cFishApp::m_spMonsterManager->m_ppWorkingMonster[i];
		if( l_pMonster->IsStatusAllowToDied() )
		{
			int	l_iOutMoney = l_pMonster->GetPrize()*e_iPayRate;
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
				if( cFishApp::m_spPlayerManager->Count() )
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
}

void	cFishProbability::WrtiePlayerMoney()
{
	if(m_pPlayerMoneyLogFile)
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
	WrtiePlayerMoney();
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