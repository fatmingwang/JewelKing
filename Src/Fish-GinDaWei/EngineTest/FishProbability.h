#ifndef _FISH_PROBABILITY_H_
#define _FISH_PROBABILITY_H_

	struct	sHittedFishData;
	class	cProbabilityData;
	class	cMonster;
	class	cPlayerBehaviorBase;
	class	cFishProbability
	{
		GET_SET_DEC(bool,m_bPauseGenerateMonster,IsPauseGenerateMonster,SetPauseGenerateMonster);
		//fuck because while fish was hitted but still keep moving so this one is not suit for this game.
		//bool				DoProbailityWithFishProbability(sHittedFishData*e_pHittedFishData,float*e_pfAddProbability,int*e_piOutMoney);
		//
		//generat monster
		UT::sTimeCounter		m_TimeGapToGenerateMonster;
		void					MonsterGenerate(float e_fElpaseTime);
		cMonster*				MonsterGo(int e_iMonsterID);
		//check happy revenue,if revenue is enouht to pay happy to give.
		bool					IsMoneyOutOk(int e_iMoney);
		void					MonsterToDied(sHittedFishData*e_pHittedFishData,int e_iOutMoney);
		void					MonsterToKeepGoing(sHittedFishData*e_pHittedFishData,float e_fAddProbability);
		void					UpdateHitMonster();
		bool					DoProbailityWithTotalRevenue(sHittedFishData*e_pHittedFishData,float*e_pfAddProbability,int*e_piOutMoney);
		bool					DoBulletPayRateCheck(int e_iBulletPayRate,sHittedFishData*e_pHittedFishData);
		//
		cProbabilityData*		m_pProbabilityData;
		//need to write player data every 1 second,so save player data here,not store in controller panel data.
		cBinaryFile*			m_pPlayerMoneyLogFile;
		UT::sTimeCounter		m_WritePlayerMoneyTC;
		void					WrtiePlayerMoney();
		void					OpenPlayerMoney();
	public:
		//
		cFishProbability();
		~cFishProbability();
		void		Init();
		void		Update(float e_fElpaseTime);
		void		Render();
		void		DebugRender();
		void		RevenueAdd(int e_iMoney,int e_iPlayerID);
		void		RevenueMinus(int e_iMoney,int e_iPlayerID);
		Vector3		GetRandomPos(cMonster*e_pMonster);
		Vector3		GetRandomPos(float e_fRadius);
		//for control panel
		void		ClearLatestRecord();
		//for player exchange and coin behavior.
		void		ExchangeIn(int64 e_i64Value);
		void		ExchangeOut(int64 e_i64Value);
		void		ToastCoin(int64 e_i64Value);
		void		RefundCoin(int64 e_i64Value);
		//
		//
		//			for mini game.
		//
		//			gererate mini game
		void		MonsterGenerateWithMiniGame(int e_iMonsterMonsterID,WCHAR*e_strMiniGameName);
		//
		bool		IsMonsterDied(cMonster*e_pMonster,int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney);
		//set monster to died
		void		MonsterDied(cMonster*e_pMonster,int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney);
		//
		cMonster*	GetConsecutiveMonsterToDied(int e_iPayRate,int e_iIndex,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney);
		//return true monster died and call player's add score.
		//bool		HittedMonsterToDied(cMonster*e_pMonster,int e_iPayRate);
		//
		void		SameTypeMonsterToDiedByProbability(int e_iPayRate,std::vector<cMonster*>*e_pStoreList,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piOutMoney);
		//
		float		AllMonsterDiedByProbability(int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney);
		//
	};

#endif