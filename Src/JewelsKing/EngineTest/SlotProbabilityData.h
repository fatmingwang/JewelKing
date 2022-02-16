#ifndef _SLOT_PROBABILITY_DATA_H_
#define _SLOT_PROBABILITY_DATA_H_

#include "PlayerData.h"

class	cBetBehavior;

class	cBetBehaviorResult
{
	int				m_iStartTotalIncome;
	int				m_iStartTotalOut;
	friend class	cBetBehavior;
	std::vector<int>		m_AllSpinObjectResultShowCount;
	//the money player has been bet for play
	//as same as cEngineTestApp::m_spBetBehavior->m_iTotalBet but if free game it's zero
	GET_SET_DEC(int	,m_iCurrentRoundBetMoney,GetCurrentRoundBetMoney,SetCurrentRoundBetMoney);
	//the money the machine has out.
	int						m_iCurrentRoundOutMoney;
	//for control panel data
	//do not delete them.
	float*					m_pfProbability;
	int*					m_piCurrentShift_TotalIncome;
	int*					m_piCurrentShift_TotalOut;
	int*					m_piCurrentShift_Revenue;

	int*					m_piTotalInMoney;
	int*					m_piTotalOutMoney;
	int*					m_piRevenue;
	int*					m_piLatestInMoney;
	int*					m_piLatestOutMoney;
	int*					m_piLatestRevenue;

	int*					m_pi3TotalConsecutiveScore;
	int*					m_pi4TotalConsecutiveScore;
	int*					m_pi5TotalConsecutiveScore;
	int*					m_pi3TotalConsecutiveCount;
	int*					m_pi4TotalConsecutiveCount;
	int*					m_pi5TotalConsecutiveCount;
	//
	int						m_iLastRoundIncome;
	int						m_iLastOutMoney;
	//vector for how many spinobject
	//mapd for consecutive count
	//it will lost while game is close or into control panel
	std::vector<std::map<int,int> >	m_WinMoneySpinObjectMatchCount;
	//each line happen times
	std::vector<int>				m_LinesWinMoneyCount;
	cBinaryFile*					m_pLogFile;
public:
	struct	sTableResult
	{
		struct sLine
		{
			//sLine(){ iWinMoney = 0; pWinMoneySpinObjectIndexList = 0; pSpinObjectResultList = 0; iWinMoneySpinObjectID = -1;iMatchCount = 1; }
			//~sLine(){ SAFE_DELETE(pWinMoneySpinObjectIndexList); SAFE_DELETE(pSpinObjectResultList); }
			//int iIndex;
			////the index for win money spin object
			//int*				pWinMoneySpinObjectIndexList;
			////data to check if win money or not
			//int*				pSpinObjectResultList;
			int					iWinMoneySpinObjectID;
			int					iMatchCount;
			sLine(){ iWinMoney = 0; }
			int iIndex;
			//the index for win money spin object
			std::vector<int>	WinMoneySpinObjectIndexList;
			//data to check if win money or not
			std::vector<int>	SpinObjectResultList;
			int					iWinMoney;
		};
		sTableResult();
		~sTableResult();
		//each spin object's result
		std::vector<int>	SpinObjectResultList;
		int					NumSpinObjectInTable(int e_iSpinObjectIndex);
		//all lines
		std::vector<sLine>	AllLineList;
		//win money lines
		std::vector<sLine>	WinMoneyLineList;
		std::vector<int>	WinMoneyLineIndexList;
		//if true this spin object full the condition of prize
		bool*				pbWinMoneySpinObject;
		//bool				pbWinMoneySpinObject[15];
		int					iWinMoney;
		//for control panel data
		int					i3ConsecutiveScore;
		int					i4ConsecutiveScore;
		int					i5ConsecutiveScore;
		int					i3ConsecutiveCount;
		int					i4ConsecutiveCount;
		int					i5ConsecutiveCount;
	};
	//result tables.
	std::vector<sTableResult*>	m_TableResultList;
	//how much is this round win?
	int						GetCurrentTablesWinMoney();
	//total bet and total score
	float					GetPayRate();
	float*					GetProbability(){return m_pfProbability;}
	int*					GetTotalInMoney(){ return m_piTotalInMoney; }
	int*					GetTotalOutMoney(){ return m_piTotalOutMoney; }
	int						GetTotalBalance(){ return *m_piRevenue; }
	int						GetCurrentBalance(){ return m_iCurrentRoundBetMoney-m_iCurrentRoundOutMoney; }
	int						GetLastRoundBalance();
	void					ClearLastTableResult();
	//for probability
	cBetBehaviorResult();
	~cBetBehaviorResult(){ClearLastTableResult(); SAFE_DELETE(m_pLogFile);}
	//assign tables result into control penel,call this at BetPhase::SetupResult
	void					AssignTableData(int e_iTotalBetMoney);
	//extra game tfor out money
	//if extra game win extra money,call this to modify refund money.
	void					AddOutMoney(int e_iOutMoney);
	void					WriteConsecutiveData();
};


//============================
//spine object's odds
//============================
class	cSpineObjectPrizeTable:public NamedTypedObject
{
	//Gavin Jones, who put 25 on Eugene, at odds of 50 to 1, has won 1,250.
	struct	sCountAndOdds
	{
		char	cCount;
		int		iOdds;
		sCountAndOdds(char	e_cCount,int e_iOdds)	{cCount = e_cCount;	iOdds = e_iOdds;}
	};
	std::vector<sCountAndOdds>	m_CountAndOddsList;
public:
	cSpineObjectPrizeTable(TiXmlElement*e_pElement);
	int		GetOddsByCount(int e_iCount);
	int		m_iProbability;
	int		GetMaxOdds();
	int		GetMinOdds();
};

//=====================
//spin object's index to be a line.
//=====================

class	cSpinObjectProbability:public cObjectListByName<cSpineObjectPrizeTable>
{
	int		m_iMinScore;
	int		m_iMaxScore;
	int		m_iMinScoreSpinObjectIndex;
	int		m_iMaxScoreSpinObjectIndex;
public:
	int	m_iTotalProbabilityAmount;
	cSpinObjectProbability();
	int	GetSpinObjectIndexByProbability(int e_iProbability);
	int	GetSpinObjectIndexByProbability();
	void	CalculateTotalProbabilityAmount();
	int		GetMaxScore(){ return m_iMaxScore; }
	int		GetMinScore(){ return m_iMinScore; }
	int		GetMinScoreSpinObjectIndex(){return m_iMinScoreSpinObjectIndex;}
	int		GetMaxScoreSpinObjectIndex(){return m_iMaxScoreSpinObjectIndex;}
};

#endif