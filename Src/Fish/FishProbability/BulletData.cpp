#include "stdafx.h"
#include "BulletData.h"
#include "BankFormat.h"
#include "BankMode.h"
sBulletFireAverage::sBulletFireAverage()
{
	iSmallestBullet = INT_MAX;
	TC.SetTargetTime(30.f);
	i64TotalMoney = 0;
	iBulletCount = 1;
	iBulletFireMoneyAeverage = 5;
}

void	sBulletFireAverage::BulletFire(int64 e_iPayRate)
{
	i64TotalMoney += e_iPayRate;
	++iBulletCount;
	if( this->iSmallestBullet > i64TotalMoney )
		iSmallestBullet = (int)i64TotalMoney;
}

void	sBulletFireAverage::Update(float e_fElpaseTime)
{
	TC.Update(e_fElpaseTime);
	if( TC.bTragetTimrReached )
	{
		iBulletFireMoneyAeverage = (int)(i64TotalMoney/(iBulletCount));
		i64TotalMoney = 0;
		iBulletCount = 1;
		iSmallestBullet = INT_MAX;
		TC.Start();
	}
	else
	{
		if( iBulletFireMoneyAeverage <= 10 && i64TotalMoney != 0 )
		{
			iBulletFireMoneyAeverage = (int)(i64TotalMoney/iBulletCount);
			TC.fRestTime = 5.f;
			iSmallestBullet = INT_MAX;
		}
		else
		{
			if(TC.GetLERP()>0.5&& rand()%30 ==0)
				iBulletFireMoneyAeverage /= 2;
		}
	}
}

int		sBulletFireAverage::GetBulletFireMoneyAeverage()
{
	return iBulletFireMoneyAeverage;
}

float	sBulletFireAverage::RelativeProbability(int e_iPayRate,int e_iOneBulletHitMonsterMonut)
{
	assert(e_iPayRate>0);
	if(iBulletFireMoneyAeverage  == 0 )
		return 1.f;
	if( e_iPayRate >= iBulletFireMoneyAeverage )
	{
		return 1.f;
	}
	assert(e_iOneBulletHitMonsterMonut>=1&&"no hit any monster!?");
	//check offset but a bullet hit how many monster
	int	l_iValue = iBulletFireMoneyAeverage/e_iPayRate;
	int	l_iOffset = rand(1,l_iValue);
	float	l_fValue = 1.f/(l_iOffset+e_iOneBulletHitMonsterMonut);
	return l_fValue;
}

cBulletDataAndBankRepositorManager::cBulletDataAndBankRepositorManager()
{
	m_pSubRepositoryBankManager = new cBankRunnerManager();
	//m_BulletFireAverageVector;
	m_i64TotalBulletValue = 0;
	m_iBulletCount = 0;
	m_i64Aeverage = 0;
}

cBulletDataAndBankRepositorManager::~cBulletDataAndBankRepositorManager()
{
	SAFE_DELETE(m_pSubRepositoryBankManager);
}

void	cBulletDataAndBankRepositorManager::Init()
{
	m_pSubRepositoryBankManager->Destroy();
	if(!m_pSubRepositoryBankManager->ParseWithMyParse("Fish/Probability/BankFormatProbability.xml"))
	{
		UT::ErrorMsg(L"Fish/Probability/BankFormatProbability.xml",L"parse failed");
	}
}

int		cBulletDataAndBankRepositorManager::GetBulletAeverageFormatIndex(int e_iValue)
{
	float	l_fPercentage = (float)(e_iValue/(double)m_i64Aeverage);
	if( l_fPercentage> 1.f )
		l_fPercentage = 1.f;
	int		l_iIndex = this->m_pSubRepositoryBankManager->GetIndexByPerenctage(l_fPercentage,e_iValue);
	assert( l_iIndex != -1 && "this bank format index is not legal");
	return l_iIndex;
}
float	cBulletDataAndBankRepositorManager::GetBulletAeverageOffsetPercentage(int e_iValue)
{
	return 0.f;
}
sBulletFireAverage*cBulletDataAndBankRepositorManager::GetsBulletFireAverage(int e_iIndex)
{
	return 0;
}
//
void	cBulletDataAndBankRepositorManager::RevenueAdd(int e_iPayRate)
{
	m_i64TotalBulletValue += e_iPayRate;
	++m_iBulletCount;
	m_i64Aeverage = m_i64TotalBulletValue/m_iBulletCount;
	int		l_iIndex = GetBulletAeverageFormatIndex(e_iPayRate);
	if( l_iIndex != -1 )
	{
		cBankRunner*l_pSubRepositoryBank = m_pSubRepositoryBankManager->GetObject(l_iIndex);
		l_pSubRepositoryBank->AddRevenue(e_iPayRate);
	}
}

void	cBulletDataAndBankRepositorManager::RevenueMinus(int e_iOutMoney,int e_iPayRate)
{
	int		l_iIndex = GetBulletAeverageFormatIndex(e_iPayRate);
	if( l_iIndex != -1 )
	{
		cBankRunner*l_pSubRepositoryBank = m_pSubRepositoryBankManager->GetObject(l_iIndex);
		l_pSubRepositoryBank->MinusRevenue(e_iOutMoney);
	}
}

bool	cBulletDataAndBankRepositorManager::IsMoneyOutOk(int e_iOutMoney,int e_iPayRate)
{
	int		l_iIndex = GetBulletAeverageFormatIndex(e_iPayRate);
	if( l_iIndex != -1 )
	{
		cBankRunner*l_pSubRepositoryBank = m_pSubRepositoryBankManager->GetObject(l_iIndex);
		return l_pSubRepositoryBank->IsMoneyOutOk(e_iOutMoney,e_iPayRate);
	}
	return false;
}

void	cBulletDataAndBankRepositorManager::Update(float e_fElpaseTime)
{
	this->m_pSubRepositoryBankManager->Update(e_fElpaseTime);
}

float	cBulletDataAndBankRepositorManager::GetRelativeProbability(int e_iPayRate,int e_iOneBulletHitMonsterMonut)
{
	int		l_iIndex = GetBulletAeverageFormatIndex(e_iPayRate);
	if( l_iIndex != -1 )
	{
		return m_pSubRepositoryBankManager->GetObject(l_iIndex)->RelativeProbability(e_iPayRate,e_iOneBulletHitMonsterMonut);
	}
	return 0.f;
}

cBankRunner*	cBulletDataAndBankRepositorManager::GetSubRepositoryBankByPayRate(int e_iPayRate)
{
	int		l_iIndex = GetBulletAeverageFormatIndex(e_iPayRate);
	if( l_iIndex != -1 )
	{
		return m_pSubRepositoryBankManager->GetObject(l_iIndex);
	}
	return 0;
}

void	cBulletDataAndBankRepositorManager::DebugRender()
{
	m_pSubRepositoryBankManager->DebugRender();
}