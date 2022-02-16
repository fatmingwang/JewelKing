#ifndef _BET_BEHAVIOR_H_
#define _BET_BEHAVIOR_H_

#include "SlotProbability.h"
class	cSlotGoodTables;
class	cSlotRuleData;
class	cPlayerData;
class	cCalculateScorePhase;
class	cSlotObjectManager;
class	cFreeExplosionGame;
class	cFreeJPGame;
class	cWinmoneyLines;
class	cSpinPhase;
class	cWinMoneySpinObjectMPDI;
class	cSlotGoodTables;
class	cBetBehavior
{
	struct	sNameAndCount
	{
		std::wstring	strName;
		int				iCount;
		std::vector<int>	WinMoneySpinObjectIndex;
		std::vector<int>	IndexOfLine;
		void	SortIndex()
		{
			std::list<int>	l_List;
			size_t	l_ui = IndexOfLine.size();
			for( size_t i=0;i<l_ui;++i )
			{
				l_List.push_back(IndexOfLine[i]);
			}
			l_List.sort();
			IndexOfLine.clear();
			while( l_List.size() )
			{
				IndexOfLine.push_back(l_List.front());
				l_List.pop_front();
			}
		}
	};
	friend class	cWinMoneySpinObjectMPDI;
	friend class	cWinmoneyLines;
	friend class	cFreeExplosionGame;
	friend class	cCalculateScorePhase;
	friend class	cBetPhase;
	friend class	cSpinPhase;
	friend class	cEngineTestApp;
	friend class	cFreeJPGame;
	friend class	cSlotGoodTables;
	cBasicSound*m_pButtonClick;
	//
	cNumerialImage*m_pNumerialImage;
	//===================================
	enum	eInfoPosList
	{
		eIPL_TotalBetMoneyPos = 0,
		eIPL_WinMoneyPos,
		eIPL_NumLineBetPos,
		eIPL_PerLineBetMoneyPos,
		eIPL_PlayerMoneyPos,
		eIPL_JP1,
		eIPL_JP2,
		eIPL_MAX,
	};
	Vector3		m_vInfoFontPos[eIPL_MAX];
	int*		m_piInfoValue[eIPL_MAX];
	//====================================
	//find bet able money
	int							GetFocusBetMoneyIndexInVector();
	//the bet result
	cBetBehaviorResult*			m_pBetBehaviorResult;
	//
	cSlotRuleData*				m_pSlotRuleData;
	cPlayerData*				m_pPlayerData;
	//how many lines we have bet,while betPhase this will show on the screen
	cWinmoneyLines*				m_pWinmoneyLines;
	//
	int							m_iNumLinesBet;
	int							m_iPerLineBetMoney;
	int							m_iTotalBet;
	int							m_iWinMoney;
	//how many round spined
	int							m_iRound;
	void						SetBetData(int e_iNumLineBet,int e_iPerLineBetMoney);
	//
	std::vector<sNameAndCount>	GetNameAndCountByLineIndexByConsecutive(int e_iIndex,cBetBehaviorResult::sTableResult*e_pTableResult);
	sNameAndCount*				GetMatchCountAndNameAndCountData(std::vector<sNameAndCount>*e_pNameAndCountList,int e_iIndex);
	void						CalculateScoreByLine();
	//this one could be any of spin object,and if a line of sponbojects all is it,win JP
	//dynamic to change this value so win money could be more
	//it will be set at cBetPhase::Init()
	int							m_iBonusMoneyRate;
	//cheat code
	cBetBehaviorResult::sTableResult*	GenerateSpecificTable();
	//a random result
	void						GenerateRandomSpinResult(cBetBehaviorResult*e_pBetBehaviorResult);
	void						GenerateProbabilitySpinResult(cBetBehaviorResult*e_pBetBehaviorResult);
	//
	cBetBehaviorResult::sTableResult*ProcessMaxBetResultForProbabilityThenCalRealBetResultTable(int e_iMinScore,int e_iMaxScore,int e_iMultiplier,bool*e_pbWinJP,bool e_bFreeGame);
	cBetBehaviorResult::sTableResult*ProcessMaxBetResultForProbabilityThenCalRealBetResultTableFromOldTable(cBetBehaviorResult::sTableResult*e_pOldTable,int e_iMinScore,int e_iMaxScore,int e_iMultiplier,bool*e_pbWinJP,bool e_bFreeGame);
	cSlotProbability			m_SlotProbability;
public:
	cBetBehavior(cSlotRuleData*e_pSlotRuleData,cPlayerData*e_pPlayerData);
	~cBetBehavior();
	void								BetMax();
	int									MaxBetMoney();
	void								AddLineBet();
	void								MinusLineBet();
	void								AddBetMoney();
	void								MinusBetMoney();
	void								ExchangeRateChange(int e_iCurrentPayRate,int e_iNewRate);
	//====================================
	void								Init();
	void								Update(float e_fElpaseTime);
	void								Render();

	void								UpdateSelectedLines(float e_fElpaseTime);
	void								RenderSelectedLines();

	void								DebugRender();
	void								MouseDown(int e_iX,int e_iY);
	void								MouseUp(int e_iX,int e_iY);
	void								MouseMove(int e_iX,int e_iY);
	//====================================
	//clear last round data
	void								CalculateScoreWithRound();
	//====================================
	//get from UI mpdi
	void								SetupInfoPos();
	//
	void								AddWinMoney(int e_iMoney);
	int									GetWinMoney(){return m_iWinMoney;}

	cBetBehaviorResult*					GetBetBehaviorResult(){return m_pBetBehaviorResult;}
	//assign table line data
	void								AssignLinesData(cBetBehaviorResult::sTableResult*e_pTableResult);
	//a random table generate from old table
	cBetBehaviorResult::sTableResult*	GenerateSpinExplosionResultWithRandom(cBetBehaviorResult::sTableResult*e_pTableResult);
	//a result table from old table with empty spinobject(-1),e_pEmptyIndexList for new empty indecies if need
	cBetBehaviorResult::sTableResult*	GenerateSpinExplosionResultWithEmptySpinObject(cBetBehaviorResult::sTableResult*e_pTableResult,std::vector<int>*e_pEmptyIndexList = 0);
	//calculate table result
	void								CalculateScoreConsecutive(cBetBehaviorResult::sTableResult*e_pTableResult);

	int									GetNumLinesBet(){return  m_iNumLinesBet;}
	int									GetPerLineBetMoney(){return m_iPerLineBetMoney;}
	int									GetRound(){return m_iRound;}
	//if JP is win do some internet check
	bool								NetWorkJPCheck(std::vector<int>*e_pSpinObjectResultList);
	bool								IsJPGameFilledCondition(std::vector<int>	e_ResultList);
};

#endif