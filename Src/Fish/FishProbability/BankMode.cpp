#include "stdafx.h"
#include "BankMode.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/FishProbability.h"
#include "BulletData.h"
#include "ProbabilityData.h"

cBankMode::cBankMode(float e_pTargetProbability,sBulletFireAverage*e_pBulletFireAverage)
{
	m_pBulletFireAverage = e_pBulletFireAverage;
	m_TargetProbability = e_pTargetProbability;
	m_i64CurrentIn = 0;
	m_i64CurrentOut = 0;
	m_i64Revenue = 0;
	m_i64TotalBulletValue = 0;
	m_i64Aeverage = 0;
	m_iBulletCount = 0;
}

cBankMode::cBankMode(TiXmlElement*e_pTiXmlElement)
{
	m_i64TotalBulletValue = 0;
	m_i64Aeverage = 0;
	m_iBulletCount = 0;
	m_pBulletFireAverage = new sBulletFireAverage();
	m_TargetProbability = cFishApp::m_spControlPanel->m_fProbability/100.f;
	m_i64CurrentIn = 0;
	m_i64CurrentOut = 0;
	m_i64Revenue = 0;
	const WCHAR*l_strBulletDuration = e_pTiXmlElement->Attribute(L"BulletDuration");
	if( l_strBulletDuration )
	{
		m_pBulletFireAverage->TC.SetTargetTime(GetFloat(l_strBulletDuration));
	}
}

cBankMode::~cBankMode()
{
	SAFE_DELETE(m_pBulletFireAverage);
}

float	cBankMode::GetNewProbability(int e_iMoney)
{
	return (float)((double)(m_i64CurrentOut+e_iMoney)/(m_i64CurrentIn+1)); 
}

float	cBankMode::GetProbability()
{
	return (float)(double)m_i64CurrentOut/(m_i64CurrentIn+1); 
}

bool	cBankMode::IsMoneyOutOk(int e_iMoney,int e_iPayRate)
{
	float	l_fProbability = GetNewProbability(e_iMoney);
	if( l_fProbability > 0.f && l_fProbability <= m_TargetProbability)
		return true;
	//if prize is bigger than 20 check extra revenue money to give
	const int	l_iBigPrize = 10;
	if( e_iMoney/e_iPayRate >= l_iBigPrize || e_iMoney > 1500000 )
	{
		cBankRunner*l_pBankRunner = (cBankRunner*)this->GetOwner();
		//only high mode bank could fetch revenue.
		float	l_fValue = frand(0,1);
		Vector2	l_vRange = l_pBankRunner->GetTargetPercentValueRange();
		if( (l_vRange.y-l_vRange.x)/2+l_vRange.x >= l_fValue)
		{
			int64	l_i64Value = cFishApp::m_spFishProbability->GetExtractRevenueValue();
			if( l_i64Value > e_iMoney )
			{//fetch from real revenue so ignore next revenue minus.
				this->m_i64Revenue += e_iMoney;
				if( m_i64CurrentOut > e_iMoney )
					m_i64CurrentOut -= e_iMoney;
				return true;
			}
		}
	}
	return false;
}

void	cBankMode::ModeAssign(cBankMode*e_pBankMode)
{
	//m_i64CurrentIn = e_pBankMode->m_i64CurrentIn;
	//m_i64CurrentOut = e_pBankMode->m_i64CurrentOut;
	m_i64CurrentIn = 0;
	m_i64CurrentOut = 0;
	m_i64Revenue = e_pBankMode->m_i64Revenue;
	if( m_i64Revenue > 0 )
	{//store win money
		m_i64Revenue = (int64)(m_i64Revenue*m_TargetProbability);
	}
	ResetDataInit();
	//e_pBankMode
}

void	cBankMode::BulletDataUpdate(int64 e_i64Value)
{
	m_i64TotalBulletValue += e_i64Value;
	++m_iBulletCount;
	m_i64Aeverage = m_i64TotalBulletValue/m_iBulletCount;
}

void	cBankMode::AddRevenue(int64 e_i64Value)
{
	BulletDataUpdate(e_i64Value);
	this->m_pBulletFireAverage->BulletFire((int)e_i64Value);
	m_i64CurrentIn += e_i64Value;
	e_i64Value = (int64)(e_i64Value*m_TargetProbability);
	m_i64Revenue += e_i64Value;
}

void	cBankMode::Update(float e_fElpaseTime)
{
	m_pBulletFireAverage->Update(e_fElpaseTime);
}

void	cBankMode::MinusRevenue(int64 e_i64Value)
{
	m_i64CurrentOut += e_i64Value;
	m_i64Revenue -= e_i64Value;
}

cPossitiveBankMode::cPossitiveBankMode(float e_pTargetProbability,sBulletFireAverage*e_pBulletFireAverage)
:cBankMode(e_pTargetProbability,e_pBulletFireAverage)
{
	int	l_iCoinToScore = cFishApp::m_spControlPanel->m_iCoinToScore;
	m_FakeRevenue.SetupData(Vector2(10.f,1000.f),Vector2(100.f*l_iCoinToScore,10000.f*l_iCoinToScore));
}
//<PossitiveBankMode BulletDuration="6">
//	<FakeRevenue />
//</PossitiveBankMode>
cPossitiveBankMode::cPossitiveBankMode(TiXmlElement*e_pTiXmlElement):cBankMode(e_pTiXmlElement)
{
	int	l_iCoinToScore = cFishApp::m_spControlPanel->m_iCoinToScore;
	m_FakeRevenue = sFakeRevenue(e_pTiXmlElement->FirstChildElement());
}

cPossitiveBankMode::~cPossitiveBankMode()
{

}

void	cPossitiveBankMode::MinusRevenue(int64 e_i64Value)
{
	m_i64CurrentOut += e_i64Value;
	m_i64Revenue -= e_i64Value;
	this->m_FakeRevenue.FakeMoneyMinus((int)e_i64Value);
}

bool	cPossitiveBankMode::IsModeSatisfied()
{
	//if( this->m_i64Revenue > 0 && this->m_FakeRevenue.iFakeRevenue <= 0 )
	if( this->m_FakeRevenue.iFakeRevenue <= 0 )
		return true;
	return false;
}

void	cPossitiveBankMode::ResetDataInit()
{
	m_FakeRevenue.Init();
	cEventVariable*l_pCoinToScore = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CoinToScore");
	int			l_iCoinToScore = *l_pCoinToScore->GetInt();
	int64	l_iMultiplier = 1;
	//int64	l_iMultiplier = m_i64Aeverage/10/l_iCoinToScore;
	//if( l_iMultiplier < 1 )
	//	l_iMultiplier = 1;
	//else
	//if( l_iMultiplier > 10 )
	//	l_iMultiplier = 10;
	m_FakeRevenue.iFakeRevenue *= (int)l_iMultiplier;
}

bool	cPossitiveBankMode::IsMoneyOutOk(int e_iMoney,int e_iPayRate)
{
	if( this->m_FakeRevenue.iFakeRevenue>0 )
	{
		float	l_fRandValue = frand(0,1);
		if( l_fRandValue <= this->m_FakeRevenue.fFakeRevenueFetchProbability )
		{
			int	l_iPrize = e_iMoney/e_iPayRate;
			m_FakeRevenue.bCurrentRoundFakeRevenueGet = true;
			float	l_fProbability = (float)((double)(m_i64CurrentOut+e_iMoney)/(m_i64CurrentIn+this->m_FakeRevenue.GetchFakeRevenue()));
			//to avoid stack at fake revenue status.
			if( e_iPayRate*2 > this->m_FakeRevenue.iFakeRevenue && l_iPrize <= 3 )
			{
				this->m_FakeRevenue.iFakeRevenue = 0;
			}
			if( l_fProbability > 0.f && l_fProbability <= m_TargetProbability)
				return true;
			return false;
		}
		return cBankMode::IsMoneyOutOk(e_iMoney,e_iPayRate);
	}
	else
	{
		this->m_FakeRevenue.bCurrentRoundFakeRevenueGet = false;
		return cBankMode::IsMoneyOutOk(e_iMoney,e_iPayRate);
	}
	return false;
}

void	cPossitiveBankMode::Update(float e_fElpaseTime)
{
	cBankMode::Update(e_fElpaseTime);
	this->m_FakeRevenue.Update(e_fElpaseTime);
}

bool	cPossitiveBankMode::IsFakeMoneyEnoughToPay(int e_iMoney)
{
	if(m_FakeRevenue.iFakeRevenue >= e_iMoney)
		return true;
	return false;
}

void	cPossitiveBankMode::DebugRender( int e_iPosX,int e_iPosY )
{
	std::wstring	l_str = UT::ComposeMsgByFormat(L"Positive:FakeMoney:%d,Time:%.2f,Revenue:%d",this->m_FakeRevenue.iFakeRevenue,m_FakeRevenue.TC.fRestTime,(int)this->m_i64Revenue);
	cGameApp::m_spGlyphFontRender->RenderFont(e_iPosX,e_iPosY,l_str.c_str());	
}


cNegativeBankMode::cNegativeBankMode(float e_pTargetProbability,sBulletFireAverage*e_pBulletFireAverage,float e_fFetchPercentageToRevenueFromBullet)
:cBankMode(e_pTargetProbability,e_pBulletFireAverage)
{
//	m_bBigMoneyOut = false;
	m_iBigPrizeToOut = 10;
	m_i64StoredBigMoneyToOut = 0;
	m_fFetchPercentageToRevenueFromBullet = e_fFetchPercentageToRevenueFromBullet;
//	int	l_iCoinToScore = cFishApp::m_spControlPanel->m_iCoinToScore;
}
//<NegativeMode BigPrizeToOut="20" FetchPercentageToRevenueFromBullet="0.3" BulletDuration="6"/>
cNegativeBankMode::cNegativeBankMode(TiXmlElement*e_pTiXmlElement):cBankMode(e_pTiXmlElement)
{
//	m_bBigMoneyOut = false;
	m_iBigPrizeToOut = 10;
	m_i64StoredBigMoneyToOut = 0;
	m_fFetchPercentageToRevenueFromBullet = 0.3f;
	const WCHAR*l_strBigPrizeToOut = e_pTiXmlElement->Attribute(L"BigPrizeToOut");
	const WCHAR*l_strFetchPercentageToRevenueFromBullet = e_pTiXmlElement->Attribute(L"FetchPercentageToRevenueFromBullet");
	if( l_strBigPrizeToOut )
	{
		m_iBigPrizeToOut = GetInt(l_strBigPrizeToOut);
	}
	if( l_strFetchPercentageToRevenueFromBullet )
	{
		m_fFetchPercentageToRevenueFromBullet = GetFloat(l_strFetchPercentageToRevenueFromBullet);
	}
}

cNegativeBankMode::~cNegativeBankMode()
{

}

void	cNegativeBankMode::AddRevenue(int64 e_i64Value)
{
	BulletDataUpdate(e_i64Value);

	int64	l_i64FetchMoney = (int64)(e_i64Value*m_fFetchPercentageToRevenueFromBullet);
	int64	l_i64StoreRevenue = e_i64Value-(int64)(e_i64Value*m_TargetProbability);	

	m_i64StoredBigMoneyToOut += l_i64FetchMoney;

	e_i64Value -= l_i64FetchMoney;
	e_i64Value -= l_i64StoreRevenue;

	m_i64CurrentIn += e_i64Value;
	m_i64Revenue += e_i64Value;
}

bool	cNegativeBankMode::IsModeSatisfied()
{//ensure big money is out and revenue is positive
	if ( this->m_i64Revenue > 0  )//&& m_bBigMoneyOut
		return true;
	return false;
}

void	cNegativeBankMode::ResetDataInit()
{
//	m_bBigMoneyOut = false;
	if( m_i64StoredBigMoneyToOut < 0 )
		m_i64StoredBigMoneyToOut = 0;
}

bool	cNegativeBankMode::IsBigMoneyOkOkay(int e_iMoney,int e_iPayRate)
{
	if( this->m_i64StoredBigMoneyToOut >= e_iMoney )
	{
		int	l_iFishPrize = e_iMoney/e_iPayRate;
		if(l_iFishPrize >= this->m_iBigPrizeToOut && e_iPayRate>= this->m_pBulletFireAverage->iBulletFireMoneyAeverage )	
		{
//			m_bBigMoneyOut = true;
			this->m_i64Revenue += e_iMoney;
			m_i64StoredBigMoneyToOut -= e_iMoney;
			this->m_i64CurrentOut -= e_iMoney;
			return true;
		}
	}
	return false;
}

bool	cNegativeBankMode::IsMoneyOutOk(int e_iMoney,int e_iPayRate)
{
	if( IsBigMoneyOkOkay(e_iMoney,e_iPayRate) )
		return true;	
	return cBankMode::IsMoneyOutOk(e_iMoney,e_iPayRate);
}

void	cNegativeBankMode::DebugRender( int e_iPosX,int e_iPosY )
{
	std::wstring	l_str = UT::ComposeMsgByFormat(L"Negative:Revenue:%d,BigMoney:%d,BigPrice:%d",(int)this->m_i64Revenue,(int)this->m_i64StoredBigMoneyToOut,this->m_iBigPrizeToOut);
	cGameApp::m_spGlyphFontRender->RenderFont(e_iPosX,e_iPosY,l_str.c_str());	
}

cBankRunner::cBankRunner(TiXmlElement*e_pXmlElement)
{
	m_i64LeastMoney = 0;
	m_vTargetPercentValueRange = Vector2(0,1);
	m_pPossitiveBankMode = 0;
	m_pNegativeBankMode = 0;
	const WCHAR*l_strValue = e_pXmlElement->Value();
	COMPARE_VALUE("SubRepositoryBank")
	{
		const WCHAR*l_strRange = e_pXmlElement->Attribute(L"Range");
		const WCHAR*l_strLeastMoney = e_pXmlElement->Attribute(L"LeastMoney");
		if( l_strRange )
		{
			m_vTargetPercentValueRange = GetVector2(l_strRange);
		}
		if(l_strLeastMoney)
		{
			this->m_i64LeastMoney = GetInt64(l_strLeastMoney)*cFishApp::m_spControlPanel->m_iCoinToScore;
		}
		e_pXmlElement = e_pXmlElement->FirstChildElement();
		while( e_pXmlElement )
		{
			l_strValue = e_pXmlElement->Value();
			COMPARE_VALUE("PossitiveBankMode")
			{
				m_pPossitiveBankMode = new cPossitiveBankMode(e_pXmlElement);
			}
			else
			COMPARE_VALUE("NegativeMode")
			{
				m_pNegativeBankMode = new cNegativeBankMode(e_pXmlElement);
			}
			e_pXmlElement = e_pXmlElement->NextSiblingElement();
		}
	}
	assert(m_pPossitiveBankMode);
	assert(m_pNegativeBankMode);
	m_pPossitiveBankMode->SetOwner(this);
	m_pNegativeBankMode->SetOwner(this);
	this->m_pCurrentMode = m_pPossitiveBankMode;
}

cBankRunner::~cBankRunner()
{
	SAFE_DELETE(m_pPossitiveBankMode);
	SAFE_DELETE(m_pNegativeBankMode);
}

void	cBankRunner::Update(float e_fElpaseTime)
{
	m_pCurrentMode->Update(e_fElpaseTime);
	if( m_pCurrentMode->IsModeSatisfied() )
	{
		if( m_pCurrentMode == this->m_pPossitiveBankMode )
		{
			m_pCurrentMode = m_pNegativeBankMode;
			m_pCurrentMode->ModeAssign(this->m_pPossitiveBankMode);
		}
		else
		{
			m_pCurrentMode = m_pPossitiveBankMode;
			m_pCurrentMode->ModeAssign(this->m_pNegativeBankMode);		
		}
	}
}

void	cBankRunner::DebugRender( int e_iPosX,int e_iPosY )
{
	m_pCurrentMode->DebugRender(e_iPosX,e_iPosY);
}

void	cBankRunner::AddRevenue(int64 e_iValue)
{
	m_pCurrentMode->AddRevenue(e_iValue);
}

void	cBankRunner::MinusRevenue(int64 e_iValue)
{
	m_pCurrentMode->MinusRevenue(e_iValue);
}

bool	cBankRunner::IsMoneyOutOk(int e_iMoney,int e_iPayRate)
{
	return m_pCurrentMode->IsMoneyOutOk(e_iMoney,e_iPayRate);
}

bool	cBankRunner::IsFakeMoneyEnoughToPay(int e_iMoney)
{
	return m_pCurrentMode->IsFakeMoneyEnoughToPay(e_iMoney);
}

float	cBankRunner::RelativeProbability(int e_iPayRate,int e_iOneBulletHitMonsterMonut)
{
	return m_pCurrentMode->m_pBulletFireAverage->RelativeProbability(e_iPayRate,e_iOneBulletHitMonsterMonut); 
}

bool	cBankRunner::IsLeastMoneyOk(int64 e_i64Money)
{
	return e_i64Money>=m_i64LeastMoney?true:false;
}