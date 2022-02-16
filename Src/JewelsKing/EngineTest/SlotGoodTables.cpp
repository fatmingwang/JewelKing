#include "stdafx.h"
#include "SlotTemperatureScore.h"
#include "SlotGoodTables.h"
#include "GameApp.h"
#include "SlotRule.h"


sWinMoneyTable::sWinMoneyTable()
{
	GoodTableCategory = eGTC_MAX;
	iConditionScore = (int)((UINT)-1);
	iMaxMoney = iMinMoney = 0;
	//avoid max combo count.
	//while explosion was processed,combo is 1.
	//iComboCount = rand()%MAXIMUM_EXPLOSION_COUNT;
	iComboCount = 0;
}


sWinMoneyTableList::sWinMoneyTableList()
{
	m_JPWinMoneyTable[eJPT_LOCAL].GoodTableCategory = eGTC_JP_LOCAL;
	m_JPWinMoneyTable[eJPT_LOCAL].iComboCount = MAXIMUM_EXPLOSION_COUNT;
	m_JPWinMoneyTable[eJPT_NETWORK].GoodTableCategory = eGTC_JP_NETWORK;
	m_JPWinMoneyTable[eJPT_NETWORK].iComboCount = 0;
}

bool	sWinMoneyTableList::IsDataVaild()
{
	int	l_iSocre = (int)(UINT)-1;
	int	l_iSize = (int)m_WinMoneyTableList.size();
	for(int i=0;i<l_iSize;++i)
	{
		if( l_iSocre> m_WinMoneyTableList[i].iConditionScore)
		{
			return false;
		}
		l_iSocre = m_WinMoneyTableList[i].iConditionScore;
	}	
	return true;
}
sWinMoneyTable*	sWinMoneyTableList::GetWinMoneyTable(int e_iScore)
{
	int	l_iSize = (int)m_WinMoneyTableList.size();
	if( l_iSize )
	{
		for(int i=0;i<l_iSize;++i)
		{
			int	l_iScore = m_WinMoneyTableList[i].iConditionScore;
			if( l_iScore>e_iScore )
				return &m_WinMoneyTableList[i];
		}
		int	l_iTargetIndex = m_WinMoneyTableList.size()-1;
		while(m_WinMoneyTableList[l_iTargetIndex].GoodTableCategory != eGTC_NORMAL)
		{
			--l_iTargetIndex;
		}
		return &m_WinMoneyTableList[l_iTargetIndex];
	}
	return 0;
}

sWinMoneyTable*	sWinMoneyTableList::GetWinMoneyTableByMoneyNeed(int e_iMaxMoney)
{
	int	l_iCloseValue = INT_MAX;
	int	l_iSize = (int)m_WinMoneyTableList.size();
	int	l_iTargetIndex = -1;
	if( l_iSize )
	{
		for(int i=0;i<l_iSize;++i)
		{
			int	l_iMoneyOffset = abs(e_iMaxMoney-m_WinMoneyTableList[i].iMaxMoney);
			if( l_iCloseValue>l_iMoneyOffset )
			{
				l_iCloseValue = l_iMoneyOffset;
				l_iTargetIndex = i;
			}
		}
		return &m_WinMoneyTableList[l_iTargetIndex];
	}
	return 0;
}

sWinMoneyTable*	sWinMoneyTableList::GetJPTable(eJPType e_eJPType)
{
	if( e_eJPType == eJPT_NETWORK )
	{
		return &this->m_JPWinMoneyTable[eJPT_NETWORK];
	}
	else
	if( e_eJPType == eJPT_LOCAL)
	{
		return &this->m_JPWinMoneyTable[eJPT_LOCAL];
	}
	return 0;
}

eGoodTableCategory	GeteGoodTableCategory(const WCHAR*e_strName)
{
	if( !wcscmp(e_strName,L"Random") )
	{
		return eGTC_RANDOM;
	}
	else
	if( !wcscmp(e_strName,L"Normal") )
	{
		return eGTC_NORMAL;
	}
	else
	if( !wcscmp(e_strName,L"FreeGame") )
	{
		return eGTC_FREE_GAME;
	}
	else
	if( !wcscmp(e_strName,L"JPNetWork") )
	{
		return eGTC_JP_NETWORK;
	}
	else
	if( !wcscmp(e_strName,L"JPLocal") )
	{
		return eGTC_JP_LOCAL;
	}
	return 	eGTC_MAX;
}

//<Root>
//	<ScroeMultiplier BalanceMoneyStartBase="1000" NextLevelMultipler="10" ExtraPercentageBase="0.05" MaxExtractPencentage="0.5" MaxMultiplier="2" >
//
//	<MoneyChangeCondition>>
//		<Data MoneyChange="100" Score="-100" Description="Ĺ�@�ʶ����ɭԦ��@�ʤ�">
//		<Data MoneyChange="-100" Score="100" Description="��@�ʶ����ɭԥ[�@�ʤ�">
//	</MoneyChangeCondition>
//
//	<TimeStamp>
//		<ToastCoin	ElpaseTime="10"	Score="10"	MaxScore="100" 	Descrption="�h�[�S������A���ƥ[�h�֤�"  />
//		<MoneyOut	Money="100" 	Score="-100"	MaxScore="-100"	Descrption="�h�[�S�Y�����A���ƥ[�h�֤�"  />
//		<MoneyIn	Score="-100"	MaxScore="100"  		Descrption="�h�[�S�Y�����A���ƥ[�h�֤�"  />
//		<FreeGame	Score="-100"	MaxScore="-100" 		Descrption="�h�[�S�Y�����A���ƥ[�h�֤�"  />
//	</TimeStamp>
//
//	<GoodTablesList>
//		<GoodTable MinMoney="500"  MaxMoney="1000"  ConditionScore="500" Type="Normal">
//		<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="900" Type="Combo" ComboCount="3" >
//		<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="FreeGame" >
//		<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="JP" >
//		<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="JP" >
//	</GoodTablesList>
//</Root>

cSlotGoodTables::cSlotGoodTables()
{
	m_fSecondFreeGameProbability = 0.01f;
	m_fHappyMonneyMultiple = 3.f;
	m_pSlotTemperatureScore = new cSlotTemperatureScore();
	m_iHappyMonneyRate = 10;
	m_iMoneyStrangleRate = 2;
	m_piCurrentScore = &this->m_pSlotTemperatureScore->m_iCurrentScore;
}

cSlotGoodTables::~cSlotGoodTables()
{
	SAFE_DELETE(m_pSlotTemperatureScore);
}

bool	cSlotGoodTables::MyParse(TiXmlElement*e_pRoot)
{
	m_iMaxBetMoney = 125;
	TiXmlElement*l_pTiXmlElement = e_pRoot->FirstChildElement();
	const WCHAR*l_strMaxBetMoney = e_pRoot->Attribute(L"MaxBetMoney");
	if( l_strMaxBetMoney )
		m_iMaxBetMoney = _wtoi(l_strMaxBetMoney);
	const WCHAR*l_strName = l_pTiXmlElement->Value();
	while(l_pTiXmlElement)
	{
		l_strName = l_pTiXmlElement->Value();
		COMPARE_NAME("TimeStamp")
		{
			ProcessTimeStamp(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("GoodTablesList")
		{
			ProcessGoodTablesList(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("ScroeMultiplier")
		{
			ProcessScroeMultiplier(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("InitProbability")
		{
			//ProcessScroeMultiplier(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("MoneyChangeCondition")
		{
			MoneyChangeCondition(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("HappyAndUnHappyRate")
		{
			HappyAndUnHappyRate(l_pTiXmlElement);
		}
			
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
	return true;
}

void	cSlotGoodTables::ProcessTimeStamp(TiXmlElement*e_pTiXmlElement)
{
	this->m_pSlotTemperatureScore->m_SlotTimeStamp.ProcessTimeStamp(e_pTiXmlElement);
}

void	cSlotGoodTables::ProcessScroeMultiplier(TiXmlElement*e_pTiXmlElement)
{
	SAFE_DELETE(this->m_pSlotTemperatureScore->m_pScroeMultiplier);
	this->m_pSlotTemperatureScore->m_pScroeMultiplier = new sScroeMultiplier(e_pTiXmlElement);
}
//<GoodTablesList>
//	<GoodTable MinMoney="500"  MaxMoney="1000"  ConditionScore="500" Type="Normal">
//	<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="900" Type="Combo" ComboCount="3" >
//	<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="FreeGame" >
//	<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="JP" >
//	<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="JP" >
//</GoodTablesList>
void	cSlotGoodTables::ProcessGoodTablesList(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strSecondFreeGameProbability = e_pTiXmlElement->Attribute(L"SecondFreeGameProbability");
	if( l_strSecondFreeGameProbability )
		m_fSecondFreeGameProbability = (float)_wtof(l_strSecondFreeGameProbability);
	m_WinMoneyTableList.m_WinMoneyTableList.clear();
	TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	const WCHAR*ll = l_pTiXmlElement->Value();
	while( l_pTiXmlElement )
	{
		sWinMoneyTable l_WinMoneyTable;
		PARSE_ELEMENT_START(l_pTiXmlElement)
			COMPARE_NAME("MinMoney")
			{
				l_WinMoneyTable.iMinMoney = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("MaxMoney")
			{
				l_WinMoneyTable.iMaxMoney = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("ConditionScore")
			{
				l_WinMoneyTable.iConditionScore = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("Type")
			{
				l_WinMoneyTable.GoodTableCategory = GeteGoodTableCategory(l_strValue);
			}
			else
			COMPARE_NAME("ComboCount")
			{
				l_WinMoneyTable.iComboCount = VALUE_TO_INT;
			}
		PARSE_NAME_VALUE_END
		if( l_WinMoneyTable.iComboCount == 0 )
		{
			l_WinMoneyTable.iMinMoney = 0;
			l_WinMoneyTable.iMaxMoney = 0;
		}
		m_WinMoneyTableList.m_WinMoneyTableList.push_back(l_WinMoneyTable);
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
}
//<!-- HappyMonneyRate�C�@�^�X���h�־��v���ū׭p���� HappyMonneyMulplie�C�@���i�h�ּ֮ɥ����ū׭p�W�ɪ����� MoneyStrangleRate���Ĺ���Ӥj���h�־��v�|�i�h����n�ֿ�ΦhĹ --!>
//<HappyAndUnHappyRate HappyMonneyRate="10" HappyMonneyMulplie="3" MoneyStrangleRate="2" />
void	cSlotGoodTables::HappyAndUnHappyRate(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("HappyMonneyRate")
		{
			m_iHappyMonneyRate = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("MoneyStrangleRate")
		{
			m_iMoneyStrangleRate = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("HappyMonneyMulplie")
		{
			m_fHappyMonneyMultiple = VALUE_TO_FLOAT;
		}
		
	PARSE_NAME_VALUE_END
}

void	cSlotGoodTables::MoneyChangeCondition(TiXmlElement*e_pTiXmlElement)
{
	SAFE_DELETE(this->m_pSlotTemperatureScore->m_pConditionScoreDataList);
	this->m_pSlotTemperatureScore->m_pConditionScoreDataList = new sConditionScoreDataList(e_pTiXmlElement);
}

sWinMoneyTable*	cSlotGoodTables::IsJPOpen()
{
	sWinMoneyTable*l_pWinMoneyTable = 0;
	for( int i=eJPT_NETWORK;i<eJPT_MAX;++i)
	{
		if(m_pSlotTemperatureScore->m_SlotTimeStamp.IsJPOpen((eJPType)i))
		{
			return m_WinMoneyTableList.GetJPTable((eJPType)i);
		}
	}
	return l_pWinMoneyTable;
}

void	cSlotGoodTables::Update(cBetBehaviorResult*e_pBetBehaviorResult)
{
	m_pSlotTemperatureScore->Update(e_pBetBehaviorResult);
	bool	l_bJPOpen = false;
	sWinMoneyTable*l_pCurrentWinMoneyTable = this->IsJPOpen();
	if( l_pCurrentWinMoneyTable && !cEngineTestApp::m_sbFreeToPlay )
	{//give JP
		m_CurrentWinMoneyTable = *l_pCurrentWinMoneyTable;
		return;
	}
	float	l_fMultiplier = m_pSlotTemperatureScore->m_pScroeMultiplier->GetMultiplier(*e_pBetBehaviorResult->GetTotalInMoney(),*e_pBetBehaviorResult->GetTotalOutMoney());
	bool	l_bHappyHour = false;
	if(rand()%m_iHappyMonneyRate==0)
	{
		l_fMultiplier *= m_fHappyMonneyMultiple;
		l_bHappyHour = true;
	}
	m_CurrentWinMoneyTable = *m_WinMoneyTableList.GetWinMoneyTable((int)(this->m_pSlotTemperatureScore->m_iCurrentScore*l_fMultiplier));
	//
	float	l_fProbability = *e_pBetBehaviorResult->GetProbability()/100.f;
	float	l_fPayRate = e_pBetBehaviorResult->GetPayRate();
	int		l_iBalance = e_pBetBehaviorResult->GetTotalBalance();
	float	l_fPectentage = l_fProbability - l_fPayRate;
	int		l_iDivitor = 10;
	//do probability adjust but while balance is bigger adjust will slight.
	if( e_pBetBehaviorResult->GetCurrentRoundBetMoney() < m_CurrentWinMoneyTable.iMaxMoney || l_fPectentage >0.1f && (rand()%m_iMoneyStrangleRate==0) )
	{
		if( l_fPectentage < 0.f  && !l_bHappyHour )
		{//pay out money is too much adjust it.
			m_CurrentWinMoneyTable = *m_WinMoneyTableList.GetWinMoneyTableByMoneyNeed(e_pBetBehaviorResult->GetCurrentRoundBetMoney()/rand(1,5));
			m_pSlotTemperatureScore->m_iCurrentScore /= l_iDivitor;
		}
		else
		if( l_fPectentage >0.1f && e_pBetBehaviorResult->GetTotalBalance() > this->m_pSlotTemperatureScore->m_pScroeMultiplier->iBalanceMoneyStartBase*l_iDivitor )
		{//give a better money,because win too much money
			float	l_fMultiplier = e_pBetBehaviorResult->GetTotalBalance() / (float)this->m_pSlotTemperatureScore->m_pScroeMultiplier->iBalanceMoneyStartBase;
			m_CurrentWinMoneyTable = *m_WinMoneyTableList.GetWinMoneyTableByMoneyNeed((int)(m_CurrentWinMoneyTable.iMaxMoney+e_pBetBehaviorResult->GetCurrentRoundBetMoney()*(l_fMultiplier+l_fPectentage)));
			m_pSlotTemperatureScore->m_iCurrentScore = m_CurrentWinMoneyTable.iConditionScore;
		}
	}
	else//money is out too much...now win money back
	if( l_fPectentage < -0.1f  )
	{
		m_pSlotTemperatureScore->m_iCurrentScore /= l_iDivitor;
		m_CurrentWinMoneyTable.iMaxMoney /= l_iDivitor;
		m_CurrentWinMoneyTable.iMinMoney /= l_iDivitor;
	}
	if( m_CurrentWinMoneyTable.GoodTableCategory == eGTC_FREE_GAME)
	{
		m_pSlotTemperatureScore->m_iCurrentScore -= rand()%l_iDivitor;
		if(cEngineTestApp::m_sbFreeToPlay)
		{
			float	l_fValue = frand(0,1);
			if(this->m_fSecondFreeGameProbability<l_fValue)
			{
				m_CurrentWinMoneyTable.GoodTableCategory = eGTC_NORMAL;
			}
		}
	}
	if( m_pSlotTemperatureScore->m_iCurrentScore < 0 )
	{
		if(rand()%2)
			this->m_pSlotTemperatureScore->m_iCurrentScore /= l_iDivitor;
	}
	if( m_CurrentWinMoneyTable.GoodTableCategory == eGTC_JP_NETWORK || m_CurrentWinMoneyTable.GoodTableCategory == eGTC_JP_LOCAL )
	{//it should never happen,but I am afried idot add data into file.
		m_pSlotTemperatureScore->m_iCurrentScore = 0;
		m_CurrentWinMoneyTable = *m_WinMoneyTableList.GetWinMoneyTableByMoneyNeed(rand()%l_iDivitor);
	}
	int	l_iBetMoney = cEngineTestApp::m_spBetBehavior->m_iTotalBet;
	float	l_fScale = l_iBetMoney/(float)m_iMaxBetMoney;
	m_CurrentWinMoneyTable.iMaxMoney = (int)(m_CurrentWinMoneyTable.iMaxMoney*l_fScale);
	m_CurrentWinMoneyTable.iMinMoney = (int)(m_CurrentWinMoneyTable.iMinMoney*l_fScale);
	//cheat code.
	//m_CurrentWinMoneyTable.iMaxMoney = 9999;
	//m_CurrentWinMoneyTable.iComboCount = 8;
	if( cEngineTestApp::m_spstrFreeGameTest )
	{
		if( !wcscmp(L"JP1",cEngineTestApp::m_spstrFreeGameTest->c_str()) )
		{
			m_CurrentWinMoneyTable.GoodTableCategory = eGTC_JP_LOCAL;
		}
		else
		if( !wcscmp(L"JP2",cEngineTestApp::m_spstrFreeGameTest->c_str()) )
		{
			m_CurrentWinMoneyTable.GoodTableCategory = eGTC_JP_NETWORK;
		}
		else
		if( !wcscmp(L"Free",cEngineTestApp::m_spstrFreeGameTest->c_str()) )
		{
			m_CurrentWinMoneyTable.GoodTableCategory = eGTC_FREE_GAME;
		}
		*cEngineTestApp::m_spstrFreeGameTest = L"";
	}
}

void	cSlotGoodTables::Render()
{
#ifdef _DEBUG
	if( m_pSlotTemperatureScore )
		m_pSlotTemperatureScore->Render();
#endif
}

void	cSlotGoodTables::GetGoodTable(cBetBehaviorResult*e_pBetBehaviorResult)
{
	Update(e_pBetBehaviorResult);
	//m_CurrentWinMoneyTable.GoodTableCategory;
	//m_CurrentWinMoneyTable.iComboCount;
	//m_CurrentWinMoneyTable.iConditionScore;
	//m_CurrentWinMoneyTable.iMaxMoney;
	//m_CurrentWinMoneyTable.iMinMoney;
}

int	cSlotGoodTables::GetCurrentScore()
{
	return this->m_pSlotTemperatureScore->m_iCurrentScore;
}