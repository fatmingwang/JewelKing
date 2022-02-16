#ifndef _SLOT_GOOD_TABLES_H_
#define _SLOT_GOOD_TABLES_H_

#include "SlotTimeStamp.h"


//<GoodTablesList>
	//<GoodTable MinMoney="500"  MaxMoney="1000"  ConditionScore="500" Type="Normal">
	//<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="900" Type="Combo" ComboCount="3" >
	//<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="FreeGame" >
	//<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="JPNetWork" >
	//<GoodTable MinMoney="900"  MaxMoney="1500"  ConditionScore="1200" Type="JPLocal" >
//</GoodTablesList>

enum	eGoodTableCategory
{
	eGTC_RANDOM,		//if random min score and max score will ignore.
	eGTC_NORMAL,		//go as normal
	eGTC_FREE_GAME,		//go as free game
	eGTC_JP_NETWORK,	//go as JP network
	eGTC_JP_LOCAL,		//go as JP local
	eGTC_MAX,
};

eGoodTableCategory	GeteGoodTableCategory(const WCHAR*e_strName);
//============================
//if no win money iComboCount is 0
//
//============================
struct	sWinMoneyTable
{
	int					iConditionScore;
	int					iMinMoney;
	int					iMaxMoney;
	eGoodTableCategory	GoodTableCategory;
	int					iComboCount;
	sWinMoneyTable();
	~sWinMoneyTable(){}
};

//ensure data is put from small to big
struct	sWinMoneyTableList
{
	sWinMoneyTableList();
	~sWinMoneyTableList(){}
	sWinMoneyTable				m_JPWinMoneyTable[eJPT_MAX];
	std::vector<sWinMoneyTable>	m_WinMoneyTableList;
	bool						IsDataVaild();
	sWinMoneyTable				*GetWinMoneyTable(int e_iScore);
	sWinMoneyTable				*GetWinMoneyTableByMoneyNeed(int e_iMaxMoney);
	//JP data is not in file I have to add it in the code
	sWinMoneyTable*				GetJPTable(eJPType e_eJPType);
};

class	cSlotTemperatureScore;
class	cSlotProbability;
class	cSlotGoodTables:public cNodeISAX
{
	friend class				cSlotProbability;
//
	void						ProcessTimeStamp(TiXmlElement*e_pTiXmlElement);
	void						ProcessScroeMultiplier(TiXmlElement*e_pTiXmlElement);
	void						ProcessGoodTablesList(TiXmlElement*e_pTiXmlElement);
	void						MoneyChangeCondition(TiXmlElement*e_pTiXmlElement);
	void						HappyAndUnHappyRate(TiXmlElement*e_pTiXmlElement);
	virtual	bool				MyParse(TiXmlElement*e_pRoot);
	//
	sWinMoneyTable*				IsJPOpen();
	//
	cSlotTemperatureScore*		m_pSlotTemperatureScore;
	sWinMoneyTableList			m_WinMoneyTableList;
	//set by sTimeStampWithScore list
	int*						m_piCurrentScore;
	sWinMoneyTable				m_CurrentWinMoneyTable;
	std::vector<sWinMoneyTable>	m_StatusScoreList;
	cSpinObjectProbability*		m_pSpineObjectPrizeTableList;
	//let player feel happy rate
	int							m_iHappyMonneyRate;
	float						m_fHappyMonneyMultiple;
	//lost too money or win too much money
	int							m_iMoneyStrangleRate;
	//because different rule,maxbet money is different,but we only has aone table file,so do divitor to get new money of tables
	int							m_iMaxBetMoney;
	//
	float						m_fSecondFreeGameProbability;
public:
	cSlotGoodTables();
	~cSlotGoodTables();
	void						Update(cBetBehaviorResult*e_pBetBehaviorResult);
	void						Render();
	void						GetGoodTable(cBetBehaviorResult*e_pBetBehaviorResult);
	sWinMoneyTableList*			GetWinMoneyTableList(){return &m_WinMoneyTableList;}
	sWinMoneyTable*				GetCurrentWinMoneyTable(){return &m_CurrentWinMoneyTable;}
	int							GetCurrentScore();
};

#endif