#include "stdafx.h"
#include "ProbabilityData.h"

sFishShowProbaility::sData::sData()
{
	iProbaility = 0;
	iFishIndex = -1;
	fFishSingleProbability = 1.f;
	fFishGroupMPDIProbability = 0.f;
	fFishGroupProbability = 0.f;
	iFishGroupCount.Max = 7;
	iFishGroupCount.Min = 7;
}

sFishShowProbaility::eFishGoType		sFishShowProbaility::sData::GetFishGoType()
{
	float	l_fProbabilirtSort[] = {fFishGroupMPDIProbability,fFishGroupProbability,fFishSingleProbability};
	float	l_fStartValue[3] = {fFishGroupMPDIProbability,
								fFishGroupMPDIProbability+fFishGroupProbability,
								fFishGroupMPDIProbability+fFishGroupProbability+fFishSingleProbability};
	float	l_fRandomValue = frand(0,l_fStartValue[2]);
	for( int i=0;i<3;++i )
	{
		if( l_fStartValue[i]>l_fRandomValue )
		{
			if( l_fProbabilirtSort[i] != 0 )
				return (eFishGoType)i;
		}
	}
	return eFGT_SINGLE;
}

sMiniGameData::sMiniGameBank::sMiniGameBank()
{
	bWaitingToGiveMiniGame = false;
	fNextAddProbablity = 0.01f;
	fCurrentProbability = fStartProbability = 0.01f;
	fAteMoney = 0.f;
	fTargetBank = 100000.f;
}

void	sMiniGameData::Update(float e_fElpaseTime)
{
	size_t	l_uiSize = MiniGameBankList.size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		sMiniGameData::sMiniGameBank*l_pMiniGameBank = &MiniGameBankList[i];
		//wait prior mini game over.
		//if(l_pMiniGameBank->bWaitingToGiveMiniGame)
		//	return;
		l_pMiniGameBank->Duration.Update(e_fElpaseTime);
		if(l_pMiniGameBank->Duration.bTragetTimrReached)
		{
			l_pMiniGameBank->Duration.Start();
			if(l_pMiniGameBank->IsOkToGo((int)l_pMiniGameBank->fAteMoney))
			{
				l_pMiniGameBank->bWaitingToGiveMiniGame = true;
				iIndexReadyToShowMiniGameIndex = i;
				return;
			}
		}
	}
	if( FakeMiniGameMonsterTime.bTragetTimrReached )
	{
		FakeMiniGameMonsterTime.SetTargetTime(fFakeMiniGameDuration.Rand());
	}
	FakeMiniGameMonsterTime.Update(e_fElpaseTime);
}

int		sMiniGameData::WidthDrawMoney(int e_iIndex)
{
	int	l_iMoney = (int)MiniGameBankList[e_iIndex].fAteMoney;
	this->fTotalAteMoney -= l_iMoney;
	MiniGameBankList[e_iIndex].fAteMoney = 0;
	return l_iMoney;
}

int	sMiniGameData::IndexOfMiniGameName(const WCHAR*e_strName)
{
	size_t l_uiSize = MiniGameBankList.size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		if(!wcscmp(MiniGameBankList[i].strName.c_str(),e_strName))
		{
			return i;
		}
	}
	return -1;
}


bool	sMiniGameData::sMiniGameBank::IsOkToGo(int e_fSafeBankMoney)
{
	if( e_fSafeBankMoney>=fTargetBank)
	{
		float	fProbability = frand(0,1);
		//fCurrentProbability = fStartProbability;
		if( fCurrentProbability > fProbability )
		{
			fCurrentProbability = fStartProbability;
			return true;
		}
		else
		{
			fCurrentProbability += fNextAddProbablity;
			return false;
		}
	}
	return false;
}

int		sFishShowProbaility::GetRandomFishID()
{
	assert(iTotalProbability>0);
	int	l_iValue = rand()%this->iTotalProbability;
	int	l_iSize = (int)this->m_FishShowProbabilityList.size();
	int	l_CurrentValue = 0;
	for(int i=0;i<l_iSize;++i)
	{
		l_CurrentValue += m_FishShowProbabilityList[i].iProbaility;
		if( l_CurrentValue>=l_iValue )
		{
			if( i>=17 )
			{
				int a=0;
			}
			return i;
		}
	}
	return -1;
}

//<FishProbability BaseMoney="5" Probability="0.5" SafeProbabilityOffset="0.1" HappyMultiplierProbability="1.5" UnHappyMultiplierProbability="0.1" />
void	cProbabilityData::ProcessFishProbabilityData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("BaseMoney")
		{
			m_iBaseMoney = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("Probability")
		{
			m_fBaseProbability = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("SafeProbabilityOffset")
		{
			m_fSafeProbabilityOffset = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("HappyMultiplierProbability")
		{
			m_fHappyMultiplierProbability = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("UnHappyMultiplierProbability")
		{
			m_fUnHappyMultiplierProbability = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
}
//<RevenueProbability FakeRevenue="2000" HappyRevenue="1000" HappyRevenueToWinMoneyProbability="0.03" />
void	cProbabilityData::ProcessRevenueProbabilityData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("HappyRevenue")
		{
			m_iHappyRevenue = VALUE_TO_VECTOR2*(float)(*this->m_piCoinToScore);
		}
		else
		COMPARE_NAME("HappyRevenueToWinMoneyProbability")
		{
			m_fHappyRevenueToWinMoneyProbability = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("HappyRevenueToWinMoneyLimit")
		{
			m_iHappyRevenueToWinMoneyLimit = VALUE_TO_INT*(*this->m_piCoinToScore);;
		}
		else
		COMPARE_NAME("FakeRevenue")
		{
			m_iFakeRevenue = VALUE_TO_INT*(*this->m_piCoinToScore);
		}
		else
		COMPARE_NAME("FakeRevenueCircle")
		{
			m_FakeRevenueTCRestrict = VALUE_TO_VECTOR2;
			m_FakeRevenueTC.SetTargetTime(m_FakeRevenueTCRestrict.Rand());
			m_FakeRevenueTC.Start();
		}
		else
		COMPARE_NAME("FakeRevenueFetchProbability")
		{
			m_fFakeRevenueFetchProbability = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("FakeRevenueGiveDivision")
		{
			m_iFakeRevenueGiveDivision = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("HappyTimeScale")
		{
			m_fHappyTimeScale = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
}

//<MiniGame ReservePercent="0.01" Description="if mini game is play reserve bank will resaved">
	//<MiniGame Name="Game1" BankMin="" BankMax="" ShowProbability="0.3" TimeDuring="300"  />
	//<MiniGame Name="Game1" BankMin="" BankMax="" ShowProbability="0.3" TimeDuring="300"  />
	//<MiniGame Name="Game1" BankMin="" BankMax="" ShowProbability="0.3" TimeDuring="300"  />
	//<MiniGame Name="Game1" BankMin="" BankMax="" ShowProbability="0.3" TimeDuring="300"  />
//</MiniGame>
void	cProbabilityData::ProcessMiniGameData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strReservePercent = e_pTiXmlElement->Attribute(L"ReservePercent");
	if( l_strReservePercent )
	{
		m_MiniGameData.fIncomePercentage = (float)_wtof(l_strReservePercent);
	}
	const WCHAR*l_strMiniGameFishID = e_pTiXmlElement->Attribute(L"MiniGameFishID");
	if( l_strMiniGameFishID )
	{
		m_MiniGameData.MiniGameBankList.clear();
		m_MiniGameData.MiniGameFishIDList = GetValueListByCommaDivide<int>(l_strMiniGameFishID);
	}

	const WCHAR*l_strFakeMiniGameDuration = e_pTiXmlElement->Attribute(L"FakeMiniGameDuration");
	if( l_strFakeMiniGameDuration )
	{
		m_MiniGameData.fFakeMiniGameDuration = GetVector2(l_strFakeMiniGameDuration);
	}
	
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while( e_pTiXmlElement )
	{
		sMiniGameData::sMiniGameBank	l_MiniGameBank;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Name")
			{
				l_MiniGameBank.strName = l_strValue;
			}
			else
			COMPARE_NAME("TargetBankMoney")
			{
				l_MiniGameBank.fTargetBank = VALUE_TO_FLOAT*(*this->m_piCoinToScore);
			}
			else
			COMPARE_NAME("ShowProbability")
			{
				l_MiniGameBank.fStartProbability = l_MiniGameBank.fCurrentProbability = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("TimeDuring")
			{
				l_MiniGameBank.Duration.SetTargetTime(VALUE_TO_FLOAT);
			}
			else
			COMPARE_NAME("NextAddProbablity")
			{
				l_MiniGameBank.fNextAddProbablity = VALUE_TO_FLOAT;
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		m_MiniGameData.MiniGameBankList.push_back(l_MiniGameBank);
	}
}

//<Monster ID="" Name="TurtleFish" ShowProbability="10"  />
void	cProbabilityData::ProcessFishData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strFishGroupProbability = e_pTiXmlElement->Attribute(L"FishGroupProbability");
	if( l_strFishGroupProbability )
	{
		m_fFishGroupProbability = (float)_wtof(l_strFishGroupProbability);
	}
	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	m_FishShowProbabilityList.iTotalProbability = 0;
	while( e_pTiXmlElement )
	{
		sFishShowProbaility::sData	l_sFishShowProbaility;
		if(!wcscmp(e_pTiXmlElement->Value(),L"Monster"))
		{
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("Name")
				{
					l_sFishShowProbaility.strFishName = l_strValue;
				}
				else
				COMPARE_NAME("ShowProbability")
				{
					l_sFishShowProbaility.iProbaility = VALUE_TO_INT;
					m_FishShowProbabilityList.iTotalProbability += l_sFishShowProbaility.iProbaility;
				}
				else
				COMPARE_NAME("ID")
				{
					l_sFishShowProbaility.iFishIndex = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("FishGroupProbability")
				{
					l_sFishShowProbaility.fFishGroupProbability = VALUE_TO_FLOAT;
				}
				else
				COMPARE_NAME("FishGroupCount")
				{
					l_sFishShowProbaility.iFishGroupCount = VALUE_TO_VECTOR2;
				}
				else
				COMPARE_NAME("FishGroupMPDIProbability")
				{
					l_sFishShowProbaility.fFishGroupMPDIProbability = VALUE_TO_FLOAT;
				}
				else
				COMPARE_NAME("SingleFishProbability")
				{
					l_sFishShowProbaility.fFishSingleProbability = VALUE_TO_FLOAT;
				}
			PARSE_NAME_VALUE_END
			m_FishShowProbabilityList.m_FishShowProbabilityList.push_back(l_sFishShowProbaility);
		}
		else
		if(!wcscmp(e_pTiXmlElement->Value(),L"FishGenerateFactor"))
		{
			int	l_iIndex = 0;
			sFishShowProbaility::sFishOutData	l_FishOutData;
			PARSE_ELEMENT_START(e_pTiXmlElement)
				COMPARE_NAME("PlayerCount")
				{
					l_iIndex = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("NumFishOutAtSameTime")
				{
					l_FishOutData.iOutFishCount = VALUE_TO_VECTOR2;
				}
				else
				COMPARE_NAME("TimeToGenerateMonsters")
				{
					l_FishOutData.fTimeCircle = VALUE_TO_VECTOR2;
				}
			PARSE_NAME_VALUE_END
			m_FishShowProbabilityList.m_PlayerCountWithFish[l_iIndex] = l_FishOutData;
			
		}
		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	}
}

void	cProbabilityData::ProcessBulletParobabilityData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strCheckTime = e_pTiXmlElement->Attribute(L"CheckTime");
	if( l_strCheckTime )
	{
		m_BulletFireAverage.TC.SetTargetTime((float)_wtoi(l_strCheckTime));
	}
}

bool	cProbabilityData::MyParse(TiXmlElement*e_pRoot)
{
	m_FishShowProbabilityList.iTotalProbability = 0;
	m_FishShowProbabilityList.m_FishShowProbabilityList.clear();
	e_pRoot = e_pRoot->FirstChildElement();
	const WCHAR*l_strName = 0;
	while( e_pRoot )
	{
		l_strName = e_pRoot->Value();
		COMPARE_NAME("Fish")
		{
			ProcessFishData(e_pRoot);
		}
		else
		COMPARE_NAME("FishProbability")
		{
			ProcessFishProbabilityData(e_pRoot);
		}
		else
		COMPARE_NAME("RevenueProbability")
		{
			ProcessRevenueProbabilityData(e_pRoot);
		}
		else
		COMPARE_NAME("MiniGame")
		{
			ProcessMiniGameData(e_pRoot);
		}
		else
		COMPARE_NAME("BulletParobability")
		{
			ProcessBulletParobabilityData(e_pRoot);
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	return true;
}