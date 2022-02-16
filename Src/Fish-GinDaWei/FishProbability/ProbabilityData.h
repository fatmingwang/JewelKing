#ifndef _FISH_PROBABILITY_DATA_H_
#define _FISH_PROBABILITY_DATA_H_

//<Monster ID="0" Name="AnglerFish" ShowProbability="300"  />
namespace FATMING_CORE
{
	class	cDataAndCurveListWithXMlSechma;
};
struct	sFishShowProbaility
{
	struct	sData
	{
		int						iProbaility;
		int						iFishIndex;
		float					fFishGroupProbability;
		UT::sMinMaxData<int>	iFishGroupCount;
		std::wstring	strFishName;
		sData();
	};
	int							iTotalProbability;
	int							GetRandomFishID();
	std::vector<sData>			m_FishShowProbabilityList;

	struct	sFishOutData
	{
		//how many fish out at same time
		UT::sMinMaxData<float>	fTimeCircle;
		UT::sMinMaxData<int>	iOutFishCount;
	};
	std::map<int,sFishOutData>	m_PlayerCountWithFish;
};


//<MiniGame ReservePercent="0.01" Description="if mini game is play reserve bank will resaved">
	//<MiniGame Name="Game1" BankMin="" BankMax="" ShowProbability="0.3" TimeDuring="300"  />
	//<MiniGame Name="Game1" BankMin="" BankMax="" ShowProbability="0.3" TimeDuring="300"  />
	//<MiniGame Name="Game1" BankMin="" BankMax="" ShowProbability="0.3" TimeDuring="300"  />
	//<MiniGame Name="Game1" BankMin="" BankMax="" ShowProbability="0.3" TimeDuring="300"  />
//</MiniGame>

//each while player shoot a bullet,it will store a small part of income to iCurrentAteMoney
//every time machine reboot,it will set to zero,and restore until next time
//while mini game's condition is filled,it will clear to zero.
struct	sMiniGameData
{
	//alow to attach mini game
	std::vector<int>			MiniGameFishIDList;
	int		iIndexReadyToShowMiniGameIndex;
	//to save money.
	float	fIncomePercentage;
	double	fTotalAteMoney;
	struct sMiniGameBank
	{
		sMiniGameBank();
		double					fAteMoney;
		float					fTargetBank;
		float					fStartProbability;
		float					fCurrentProbability;
		float					fNextAddProbablity;
		UT::sTimeCounter		Duration;
		std::wstring			strName;
		//if waiting to give mini game call mini game while fish hitted.
		bool					bWaitingToGiveMiniGame;
		bool					IsOkToGo(int e_fSafeBankMoney);
	};
	std::vector<sMiniGameBank>	MiniGameBankList;
	sMiniGameData()
	{
		fIncomePercentage = 0.01f;
		fTotalAteMoney = 0;
		iIndexReadyToShowMiniGameIndex = -1;
	}
	void	Update(float e_fElpaseTime);
	int		WidthDrawMoney(int e_iIndex);
};

struct	sBinaryFileStruct
{
	int		iNumPlayer;
	int*	piPlayerMoney;
	int		iMoneyIn;
	int		iMoneyOut;
};

//if payrate is smaller than aeverage, Probability should recalculate by a bullet shot how many fish and divide by fish's prize.
//ex"one bullet shoot 3 fishes,each fish's prize is
//2,5,10,then first fish's probability is 1/3/2,second's is 1/3/2,thirs's is 1/3/10
struct	sBulletFireAverage
{
	sBulletFireAverage();
	UT::sTimeCounter	TC;
	int					iTotalMoney;
	int					iBulletCount;
	int					iBulletFireMoneyAeverage;
	void				BulletFire(int e_iPayRate);
	void				Update(float e_fElpaseTime);
	int					GetBulletFireMoneyAeverage();
	float				RelativeProbability(int e_iPayRate,int e_iOneBulletHitMonsterMonut);
};


class	cFishProbability;
class	cProbabilityData:public cNodeISAX
{
	float					m_fMachinRunTime;
	friend class			cFishProbability;
	//probability check
	sBulletFireAverage		m_BulletFireAverage;
	sFishShowProbaility		m_FishShowProbabilityList;
	sMiniGameData			m_MiniGameData;
	//
	void					ProcessFishProbabilityData(TiXmlElement*e_pTiXmlElement);
	void					ProcessRevenueProbabilityData(TiXmlElement*e_pTiXmlElement);
	void					ProcessFishData(TiXmlElement*e_pTiXmlElement);
	void					ProcessMiniGameData(TiXmlElement*e_pTiXmlElement);
	void					ProcessBulletParobabilityData(TiXmlElement*e_pTiXmlElement);
	virtual	bool			MyParse(TiXmlElement*e_pRoot);
	//if safe probability offset(target-current),happy to give double fish dead probability
	float					m_fSafeProbabilityOffset;
	//if above is satifsied new probability will plus this value
	float					m_fHappyMultiplierProbability;
	//as invert as happy,for fish probability
	float					m_fUnHappyMultiplierProbability;
	int						m_iBaseMoney;
	float					m_fBaseProbability;
	//for dynamic tempture check,but now it doesn't work
	int64					m_iStartTotalIncome;
	int64					m_iStartTotalOut;
	//for control panel data
	//do not delete them.
	int*					m_piTargetProbability;

	int64*					m_pi64TotalInMoney;
	int64*					m_pi64TotalOutMoney;
	int64*					m_pi64TotalRevenue;
	int64*					m_pi64TotalExchangeIn;
	int64*					m_pi64TotalExchangeOut;
	int64*					m_pi64TotalCoinIn;
	int64*					m_pi64TotalCoinOut;

	int64*					m_pi64LatestInMoney;
	int64*					m_pi64LatestOutMoney;
	int64*					m_pi64LatestRevenue;
	int64*					m_pi64LatestExchangeIn;
	int64*					m_pi64LatestExchangeOut;
	int64*					m_pi64LatestCoinIn;
	int64*					m_pi64LatestCoinOut;
	//
	int*					m_piHappyTime;
	float					m_fHappyTimeScale;
	//while time is hitted give it a fake revenue
	UT::sMinMaxData<float>	m_FakeRevenueTCRestrict;
	UT::sTimeCounter		m_FakeRevenueTC;
	int						m_iFakeRevenue;
	int						m_iHappyRevenue;
	float					m_fFakeRevenueFetchProbability;
	int						m_iFakeRevenueGiveDivision;
	//while probability is not easy to win money,it still give a small chane to win money
	float					m_fHappyRevenueToWinMoneyProbability;
	int						m_iHappyRevenueToWinMoneyLimit;
	void					FakeRevenueUpdate(float e_fElpaseTime);
	//one coin for xx score
	int*					m_piCoinToScore;
	//6 for total,6 for latest else for player money.
	cBinaryFile*			m_pStaticsLogFile;
	//last two is all player's revenue,and last one is probability
	int64					*m_pi64PlayerRevenueLog;
	//check latest revenue
	void					RevenuChange();
	//
	float					GetCurrentRealProbability();
public:
	cProbabilityData();
	~cProbabilityData();
	void					Init();
	//total bet and total score
	float					GetCurrentProbability();
	float					GetTargetProbability();
	int64					GetLeatestInMoney();
	int64					GetLeatestOutMoney(){ return *m_pi64LatestOutMoney; }
	int64					GetLeatestRevenue(){ return *m_pi64LatestRevenue; }
	void					RevenueAdd(int e_iMoney,int e_iPlayerID);
	void					RevenueMinus(int e_iMoney,int e_iPlayerID);
	//void					AddOutMoney(int e_iMoney);
	//for fish hitted probability
	//float					GetOutMoneyProbability(int e_iOutMoney);
	float					GetReleatePayRateProbability(int e_iPayRate,int e_iOneBulletHitMonsterMonut);
	float					GetNewProbailityByAddMoney(int e_iOutMoney);
	bool					IsProbailitySafeToOut(float e_fProbaility);
	//for fish died probability,but now I am not judge by this,it only offer for fish status change.
	float					GetFishDiedHappyProbability(float e_fCurrentProbability,float e_fDiedProbability);
	int						GetRandomFishID();
	int						GetRandomMiniGameFishID();
	sMiniGameData*			GetMiniGame(){return &m_MiniGameData;}
	sFishShowProbaility*	GetFishShowProbabilityList(){return &m_FishShowProbabilityList;}
	//for probability
	float					m_fFishGroupProbability;
	//
	//how many fish out at same time
	int						m_iNumMonsterOutAtSameTime;
	//
	float					m_fTimeToGenerateMonsters;
	//
	void					Update(float e_fElpaseTime);
	void					Render();
	//while mini game is waiting to show value is not -1,and it will auto clear safe bank data
	int						FetchWaitingToShowMiniGameID();
	//
	//int						GetPayRateRelativeToCoinToScore(int e_iMoney);
	//
	bool					IsHappyTime();
	//
	int						GetFakeRevenue(){return m_iFakeRevenue;}
	int						GetHappyRevenue(){return m_iHappyRevenue;}
	float					GetHappyRevenueToWinMoneyProbability(){return m_fHappyRevenueToWinMoneyProbability;}
	int						GetHappyRevenueToWinMoneyLimit(){return m_iHappyRevenueToWinMoneyLimit;}
	void					WrtieLog();
};

#endif