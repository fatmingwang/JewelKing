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
#include "BankMode.h"
#include "BankFormat.h"
#include "ComportData.h"
unsigned short		cFishProbability::m_susCurrentID = 0;

void	cFishProbability::AssignSendFishToSchollerMasterData()
{
	unsigned	char	l_uc = MAX_FISH_DIED_AT_ONE_UPLOAD;
	int	l_iRestFishCouldDied = l_uc-m_pSchoolMasterLiSendDataWithMonster->SchoolMasterLiSendData.cNumFishHitted;
	for( int i=0;i<cFishApp::m_spMonsterManager->m_iNumMonsterHitted;++i )
	{
		sHittedFishData*l_pHittedFishData = &cFishApp::m_spMonsterManager->m_pHittedFish[i];
		if( m_pSchoolMasterLiSendDataWithMonster->SchoolMasterLiSendData.cNumFishHitted < l_uc )
		{
			sSchoolMasterLiSendData::sHittedFish	l_sHittedFish;
			l_sHittedFish.ucBulletValue = l_pHittedFishData->pBullet->GetPayRate();
			l_sHittedFish.ucPlayerID = l_pHittedFishData->pPlayer->m_iPlayerID;
			l_sHittedFish.uiFishOutMoney = l_pHittedFishData->pMonster->GetPrize();
			l_sHittedFish.usID = m_susCurrentID;

			m_pSchoolMasterLiSendDataWithMonster->SchoolMasterLiSendData.HittedFish[m_pSchoolMasterLiSendDataWithMonster->SchoolMasterLiSendData.cNumFishHitted] = l_sHittedFish;
			m_pSchoolMasterLiSendDataWithMonster->CompareMonster.push_back(l_pHittedFishData->pMonster);
			m_pSchoolMasterLiSendDataWithMonster->CompareBullet.push_back(l_pHittedFishData->pBullet);
			m_pSchoolMasterLiSendDataWithMonster->ComparePlayer.push_back(l_pHittedFishData->pPlayer);
			++m_pSchoolMasterLiSendDataWithMonster->SchoolMasterLiSendData.cNumFishHitted;
			++m_susCurrentID;
			if( m_susCurrentID > 9999 )
				m_susCurrentID = 0;
		}
		else
		{
			MonsterToKeepGoing(l_pHittedFishData,0.05f);
		}
	}

	if( this->m_pLiSchoolMasterSerialIO->IsSafeToDoSingnal() )
	{
		if( m_pSchoolMasterLiSendDataWithMonster->SchoolMasterLiSendData.cNumFishHitted != 0 )
		{
			unsigned char	l_cData[MAX_SEND_BUFFER];
			memset(l_cData,0,MAX_SEND_BUFFER);
			int	l_iCurrentIndex = 0;
			l_cData[l_iCurrentIndex] = 0x55;
			l_iCurrentIndex += 1;
			l_cData[1] = m_pSchoolMasterLiSendDataWithMonster->SchoolMasterLiSendData.cNumFishHitted;
			l_iCurrentIndex += 1;
			//memcpy(&l_cData[2],m_pSchoolMasterLiSendData->SchoolMasterLiSendData.HittedFish,sizeof(sSchoolMasterLiSendData::sHittedFish)*l_cData[1]);
			//int	l_iSkipSize = sizeof(sSchoolMasterLiSendData::sHittedFish)*l_cData[1];
			m_pSchoolMasterLiSendData->Copy(m_pSchoolMasterLiSendDataWithMonster);
			m_pSchoolMasterLiSendDataWithMonster->Clear();

			m_pSchoolMasterLiSendData->SchoolMasterLiSendData.cChecksum = 55;
			m_pSchoolMasterLiSendData->SchoolMasterLiSendData.cChecksum += m_pSchoolMasterLiSendData->SchoolMasterLiSendData.cNumFishHitted;
			for( int i=0;i<m_pSchoolMasterLiSendData->SchoolMasterLiSendData.cNumFishHitted;++i )
			{
				sSchoolMasterLiSendData::sHittedFish*l_pHittedFish = &m_pSchoolMasterLiSendData->SchoolMasterLiSendData.HittedFish[i];
				short*l_pID = (short*)&l_cData[l_iCurrentIndex];
				*l_pID = l_pHittedFish->usID;;
				//memcpy(&l_cData[l_iCurrentIndex],&l_pHittedFish->usID,sizeof(short));
				l_iCurrentIndex += 2;
				l_cData[l_iCurrentIndex] = l_pHittedFish->ucPlayerID;
				l_iCurrentIndex += 1;
				short*l_pBuletValue = (short*)&l_cData[l_iCurrentIndex];
				//memcpy(&l_cData[l_iCurrentIndex],&l_pHittedFish->ucBulletValue,sizeof(short));
				*l_pBuletValue = l_pHittedFish->ucBulletValue;
				l_iCurrentIndex += 2;
				int*l_pScore = (int*)&l_cData[l_iCurrentIndex];
				*l_pScore = l_pHittedFish->uiFishOutMoney;
				//memcpy(&l_cData[l_iCurrentIndex],&l_pHittedFish->uiFishOutMoney,sizeof(int));
				l_iCurrentIndex += 4;
			}
			l_cData[l_iCurrentIndex] = 0;
			++l_iCurrentIndex;
			for( int i=0;i<l_iCurrentIndex-1;++i )
			{
				l_cData[l_iCurrentIndex-1] += l_cData[i];
			}
			m_pSchoolMasterLiSendData->SchoolMasterLiSendData.cChecksum += l_cData[l_iCurrentIndex-1];
			m_pLiSchoolMasterSerialIO->Write((char*)&l_cData,l_iCurrentIndex);
		}
		m_pLiSchoolMasterSerialIO->m_serialProcess->SetSafeToDoSingnal(false);
	}
	if( m_bReceivedSingnal )
	{
		AssignReceivedFishFromSchollerMasterData();
	}
}

void	cFishProbability::AssignReceivedFishFromSchollerMasterData()
{
	//if( this->m_pSchoolMasterLiSendDataWithMonster->SchoolMasterLiSendData.cNumFishHitted != m_pSchoolMasterLiReceiveData->cNumFishHitted )
	//{
	//	UT::ErrorMsg(L"Received and sent Data Count is not match!",L"please check comport data");
	//}
	//else
	{
		this->m_bReceivedSingnal = false;
		while( m_pSchoolMasterLiReceiveData->cNumFishHitted != 0 )
		{
			cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a1");
			--m_pSchoolMasterLiReceiveData->cNumFishHitted;
			int	l_iCurrentIndex = m_pSchoolMasterLiReceiveData->cNumFishHitted;
			cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a2");
			sSchoolMasterLiReceiveData::sHittedFish	l_ReceivedHittedFish = m_pSchoolMasterLiReceiveData->HittedFish[l_iCurrentIndex];
			cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a3");
			sSchoolMasterLiSendData::sHittedFish	l_SentHittedFish = m_pSchoolMasterLiSendData->SchoolMasterLiSendData.HittedFish[l_iCurrentIndex];
			char	l_CheckSum = 0;
			//if( l_ReceivedHittedFish.usID == l_SentHittedFish.usID )
			{
				cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a4");
				sHittedFishData	l_sHittedFishData;
				l_sHittedFishData.pMonster = m_pSchoolMasterLiSendData->CompareMonster[l_iCurrentIndex];
				cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a5");
				l_sHittedFishData.pBullet = m_pSchoolMasterLiSendData->CompareBullet[l_iCurrentIndex];
				cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a6");
				l_sHittedFishData.pPlayer = m_pSchoolMasterLiSendData->ComparePlayer[l_iCurrentIndex];
				cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a7");
				if(l_ReceivedHittedFish.uiFishOutMoney>0)
				{
					cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a8");
					MonsterToDied(&l_sHittedFishData,l_ReceivedHittedFish.uiFishOutMoney);
				}
				else
				{
					cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a9");
					MonsterToKeepGoing(&l_sHittedFishData,0.05f);
				}
				cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a10");
			}
			//else
			//{
			//	UT::ErrorMsg(L"Received and sent Data Count is not match!",L"please check comport data");
			//	break;
			//}
		}
	}
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a11");
	m_pSchoolMasterLiSendData->Clear();
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("a12");
}

void	cFishProbability::UpdateHitMonsterForBankFormat()
{
	if(cFishApp::m_sbSchoolerMasterLiProbability)
	{
		AssignSendFishToSchollerMasterData();
		return;
	}
	int	l_iConsective = 0;
	for( int i=0;i<cFishApp::m_spMonsterManager->m_iNumMonsterHitted;++i )
	{
		sHittedFishData*l_pHittedFishData = &cFishApp::m_spMonsterManager->m_pHittedFish[i];
		if(!IsPayRateLegalToWinBigMoney(l_pHittedFishData->pMonster->GetPrize(),l_pHittedFishData->pBullet->GetPayRate()))
			continue;
		int	l_iAveragePayRate = this->m_pProbabilityData->m_BulletFireAverage.iBulletFireMoneyAeverage;
		int	l_iMonsterPrize = l_pHittedFishData->pMonster->GetPrize();
		bool	l_bFishDied = false;
		int		l_iOutMoney = 0;
		float	l_fAddProbability = 0.f;
		//check average bullet and monster ate money.
		this->m_pProbabilityData->m_bCurrentRoundFakeRevenueGet = false;
		const int	l_iMaxConsectiveCount = 3;
		if( l_iConsective < l_iMaxConsectiveCount )
		{
			if(DoBulletPayRateCheckForBankFormat(l_pHittedFishData->pBullet->GetPayRate(),l_pHittedFishData) || l_iMonsterPrize*l_iAveragePayRate <= l_pHittedFishData->pMonster->m_iTotalBiteMoney )
			{
				if( MiniGameMonsterDiedCheck(l_pHittedFishData) )
					l_bFishDied = DoProbailityWithTotalRevenueForBankFormat(l_pHittedFishData,&l_fAddProbability,&l_iOutMoney);
			}
		}
		if( l_bFishDied )
		{
			MonsterToDied(l_pHittedFishData,l_iOutMoney);
			++l_iConsective;
		}
		else
		{
			l_iConsective = 0;
			MonsterToKeepGoing(l_pHittedFishData,l_fAddProbability);
		}
	}
	cFishApp::m_spMonsterManager->m_iNumMonsterHitted = 0;
	if(cFishApp::m_spMonsterManager->m_pTurtleBoss&&cFishApp::m_spMonsterManager->m_pTurtleBoss->IsBiteEnoughMoney())
	{
		if( cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedBullet )
		{
			if( cFishApp::m_spMonsterManager->m_pTurtleBoss->GetMonsterStatus() != eMS_WAITING_FOR_CLEAN && 
				cFishApp::m_spMonsterManager->m_pTurtleBoss->GetMonsterStatus() != eMS_NONE &&
				cFishApp::m_spMonsterManager->m_pTurtleBoss->GetMonsterStatus() != eMS_DIED_SHOW	)
			{
				int	l_iOutMoney = cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedBullet->GetPayRate()*cFishApp::m_spMonsterManager->m_pTurtleBoss->GetPrize();
				if(this->m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney)))
				{
					cFishApp::m_spMonsterManager->m_pTurtleBoss->SetMonsterStatus(eMS_NONE);
					this->m_pProbabilityData->RevenueMinus(cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedBullet->GetPayRate(),l_iOutMoney,cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedPlayer->m_iPlayerID,false);
					cGameApp::SoundPlay(L"8",true);
					cNormalFish*l_pFish = cFishApp::m_spMonsterManager->m_pTurtleBoss->GetCurrentWorkingObject();
					cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedPlayer->AddScore(l_pFish->GetPos(),l_pFish,l_iOutMoney);
				}
			}
		}
	}
}
//
void	cFishProbability::UpdateHitMonster()
{
	if(cFishApp::m_sbSchoolerMasterLiProbability)
	{
		AssignSendFishToSchollerMasterData();
		return;
	}
	for( int i=0;i<cFishApp::m_spMonsterManager->m_iNumMonsterHitted;++i )
	{
		sHittedFishData*l_pHittedFishData = &cFishApp::m_spMonsterManager->m_pHittedFish[i];
		int	l_iAveragePayRate = this->m_pProbabilityData->m_BulletFireAverage.iBulletFireMoneyAeverage;
		int	l_iMonsterPrize = l_pHittedFishData->pMonster->GetPrize();
		bool	l_bFishDied = false;
		int		l_iOutMoney = 0;
		float	l_fAddProbability = 0.f;
		//check average bullet and monster ate money.
		this->m_pProbabilityData->m_bCurrentRoundFakeRevenueGet = false;
		if(DoBulletPayRateCheck(l_pHittedFishData->pBullet->GetPayRate(),l_pHittedFishData) || l_iMonsterPrize*l_iAveragePayRate <= l_pHittedFishData->pMonster->m_iTotalBiteMoney )
		{
			if(MiniGameMonsterDiedCheck(l_pHittedFishData))
			{
				l_bFishDied = DoProbailityWithTotalRevenue(l_pHittedFishData,&l_fAddProbability,&l_iOutMoney);
				//bool	l_bFishDied = DoProbailityWithFishProbability(l_pHittedFishData,&l_fAddProbability,&l_iOutMoney);
				if(!l_bFishDied&&IsMoneyOutOk(l_iOutMoney))
				{
					l_bFishDied = true;
				}
			}
		}
		if( l_bFishDied )
			MonsterToDied(l_pHittedFishData,l_iOutMoney);
		else
			MonsterToKeepGoing(l_pHittedFishData,l_fAddProbability);
	}
	cFishApp::m_spMonsterManager->m_iNumMonsterHitted = 0;
	if(cFishApp::m_spMonsterManager->m_pTurtleBoss&&cFishApp::m_spMonsterManager->m_pTurtleBoss->IsBiteEnoughMoney())
	{
		if( cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedBullet )
		{
			if( cFishApp::m_spMonsterManager->m_pTurtleBoss->GetMonsterStatus() != eMS_WAITING_FOR_CLEAN && 
				cFishApp::m_spMonsterManager->m_pTurtleBoss->GetMonsterStatus() != eMS_NONE &&
				cFishApp::m_spMonsterManager->m_pTurtleBoss->GetMonsterStatus() != eMS_DIED_SHOW	)
			{
				int	l_iOutMoney = cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedBullet->GetPayRate()*cFishApp::m_spMonsterManager->m_pTurtleBoss->GetPrize();
				if(this->m_pProbabilityData->IsProbailitySafeToOut(this->m_pProbabilityData->GetNewProbailityByAddMoney(l_iOutMoney)))
				{
					cFishApp::m_spMonsterManager->m_pTurtleBoss->SetMonsterStatus(eMS_NONE);
					this->m_pProbabilityData->RevenueMinus(cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedBullet->GetPayRate(),l_iOutMoney,cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedPlayer->m_iPlayerID,false);
					cGameApp::SoundPlay(L"8",true);
					cNormalFish*l_pFish = cFishApp::m_spMonsterManager->m_pTurtleBoss->GetCurrentWorkingObject();
					cFishApp::m_spMonsterManager->m_pTurtleBoss->m_pHittedPlayer->AddScore(l_pFish->GetPos(),l_pFish,l_iOutMoney);
				}
			}
		}
	}
}

bool	cFishProbability::DoProbailityWithTotalRevenueForBankFormat(sHittedFishData*e_pHittedFishData,float*e_pfAddProbability,int*e_piOutMoney)
{
	sHittedFishData*l_pHittedFishData = e_pHittedFishData;
	int	l_iOutMoney = l_pHittedFishData->pMonster->GetPrize()*l_pHittedFishData->pBullet->GetPayRate();
	int	l_iPayRate = l_pHittedFishData->pBullet->GetPayRate();
	*e_piOutMoney = l_iOutMoney;
	//check bite money and out money.
	cBankRunner*l_pSubRepositoryBank = m_pProbabilityData->m_BulletData.GetSubRepositoryBankByPayRate(l_iPayRate);
	if(l_pHittedFishData->pMonster->m_iTotalBiteMoney>=l_iOutMoney)
	{
		//if bite money is enough and check happy revenue,then give a random chance to give money
		if(l_pSubRepositoryBank->IsFakeMoneyEnoughToPay(l_iOutMoney))
		{
			return true;
		}
	}
	
	if( l_pSubRepositoryBank->IsMoneyOutOk(l_iOutMoney,l_iPayRate) )
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

bool	cFishProbability::DoBulletPayRateCheckForBankFormat(int e_iBulletPayRate,sHittedFishData*e_pHittedFishData)
{
	float	l_fValue = frand(0,1);
	float	l_fValue2 = frand(0,1);
	int		l_iPrize = e_pHittedFishData->pMonster->GetPrize();
	float	l_fPrizeProbability = 1.f/rand(1,l_iPrize)*(e_pHittedFishData->pMonster->m_fProbabilityToDied);
	if( this->m_pProbabilityData->m_iFakeRevenue > 0 )
	{
		int	l_iValue = m_pProbabilityData->m_iFakeRevenue/4000;
		l_fPrizeProbability *= l_iValue;
	}
	//if( l_fValue2<=l_fPrizeProbabilitythis->m_pProbabilityData->GetReleatePayRateProbability(e_iBulletPayRate,e_pHittedFishData->pBullet->GetNumMonsterShoot()) > l_fValue )
	if( l_fValue2<=l_fPrizeProbability)
	{
		if(this->m_pProbabilityData->m_BulletData.GetRelativeProbability(e_iBulletPayRate,e_pHittedFishData->pBullet->GetNumMonsterShoot() ))
			return true;
	}
	return false;
}

bool	cFishProbability::DoBulletPayRateCheck(int e_iBulletPayRate,sHittedFishData*e_pHittedFishData)
{
	float	l_fValue2 = frand(0,1);
	//fatming
	//float	l_fValue = frand(0,1);
	//int		l_iPrize = e_pHittedFishData->pMonster->GetPrize();
	//float	l_fPrizeProbability = 1.f/rand(1,l_iPrize)*(e_pHittedFishData->pMonster->m_fProbabilityToDied);
	//if( this->m_pProbabilityData->m_iFakeRevenue > 0 )
	//{
	//	int	l_iValue = m_pProbabilityData->m_iFakeRevenue/4000;
	//	l_fPrizeProbability *= l_iValue;
	//}
	//if( l_fValue2<=l_fPrizeProbability)
	//{
	//	if(this->m_pProbabilityData->GetReleatePayRateProbability(e_iBulletPayRate,e_pHittedFishData->pBullet->GetNumMonsterShoot()) > l_fValue ))
	//		return true;
	//}
	//return false;
	//william
	int		l_iPrize = e_pHittedFishData->pMonster->GetPrize();
	float	l_fPrizeProbability = 1.f/l_iPrize;
	l_fPrizeProbability *= ((e_iBulletPayRate/(*this->m_pProbabilityData->m_piBulletLimit)+1.f)/2.f);
	if( l_fValue2<=l_fPrizeProbability)
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
