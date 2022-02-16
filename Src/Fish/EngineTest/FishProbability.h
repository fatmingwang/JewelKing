#ifndef _FISH_PROBABILITY_H_
#define _FISH_PROBABILITY_H_

	struct	sHittedFishData;
	class	cProbabilityData;
	class	cMonster;
	class	cPlayerBehaviorBase;
	class	cBullet;
	struct  sSchoolMasterLiSendDataWithMonster;
	struct  sSchoolMasterLiReceiveData;
	class	cLiSchoolMasterSerialIO;
	class	cFishProbability
	{
		static unsigned short				m_susCurrentID;
		void								AssignSendFishToSchollerMasterData();
		void								AssignReceivedFishFromSchollerMasterData();
		bool								m_bReceivedSingnal;
		sSchoolMasterLiSendDataWithMonster*	m_pSchoolMasterLiSendDataWithMonster;
		//for send,while send function is called,assign m_SchoolMasterLiSendDataWithMonster::SchoolMasterLiSendData to this
		sSchoolMasterLiSendDataWithMonster*	m_pSchoolMasterLiSendData;
		//for received,compare send and received data
		sSchoolMasterLiReceiveData*			m_pSchoolMasterLiReceiveData;
		cLiSchoolMasterSerialIO*			m_pLiSchoolMasterSerialIO;
	public:
		void								SetReceivedSingnal(bool e_b){ m_bReceivedSingnal = e_b;}
		sSchoolMasterLiReceiveData*			GetSchoolMasterLiReceiveData(){ return m_pSchoolMasterLiReceiveData; }
	private:
		//
		bool								m_bThisFrameMiniGameFishDied;
		GET_SET_DEC(bool,m_bPauseGenerateMonster,IsPauseGenerateMonster,SetPauseGenerateMonster);
		//fuck because while fish was hitted but still keep moving so this one is not suit for this game.
		//bool				DoProbailityWithFishProbability(sHittedFishData*e_pHittedFishData,float*e_pfAddProbability,int*e_piOutMoney);
		//generat monster
		UT::sTimeCounter					m_TimeGapToGenerateMonster;
		void								MonsterGenerate(float e_fElpaseTime);
		cMonster*							MonsterGo(int e_iMonsterID);
		//check happy revenue,if revenue is enouht to pay happy to give.
		bool								IsMoneyOutOk(int e_iMoney);
		void								MonsterToDied(sHittedFishData*e_pHittedFishData,int e_iOutMoney);
		void								MonsterToKeepGoing(sHittedFishData*e_pHittedFishData,float e_fAddProbability);
		//ensure mini game get smallest bonus
		bool								MiniGameMonsterDiedCheck(sHittedFishData*e_pHittedFishData);
		//
		void								UpdateHitMonster();
		//
		void								UpdateHitMonsterForBankFormat();
		bool								DoProbailityWithTotalRevenue(sHittedFishData*e_pHittedFishData,float*e_pfAddProbability,int*e_piOutMoney);
		bool								DoProbailityWithTotalRevenueForBankFormat(sHittedFishData*e_pHittedFishData,float*e_pfAddProbability,int*e_piOutMoney);
		bool								DoBulletPayRateCheck(int e_iBulletPayRate,sHittedFishData*e_pHittedFishData);
		bool								DoBulletPayRateCheckForBankFormat(int e_iBulletPayRate,sHittedFishData*e_pHittedFishData);
		//
		cProbabilityData*					m_pProbabilityData;
		//need to write player data every 1 second,so save player data here,not store in controller panel data.
		cBinaryFile*						m_pPlayerMoneyLogFile;
		UT::sTimeCounter					m_WritePlayerMoneyTC;
		void								WrtiePlayerMoney();
		void								OpenPlayerMoney();
	public:
		//
		cFishProbability();
		~cFishProbability();
		void								Init();
		void								Update(float e_fElpaseTime);
		void								Render();
		void								DebugRender();
		void								RevenueAdd(int e_iMoney,int e_iPlayerID);
		void								RevenueMinus(int e_iPayRate,int e_iMoney,int e_iPlayerID,bool e_bMiniGame);
		Vector3								GetRandomPos(cMonster*e_pMonster);
		Vector3								GetRandomPos(float e_fRadius);
		//
		int64								GetExtractRevenueValue();
		bool								IsPayRateLegalToWinBigMoney(int e_iPrize,int e_iPayRate);
		//for control panel
		void								ClearLatestRecord();
		//for player exchange and coin behavior.
		void								ExchangeIn(int64 e_i64Value);
		void								ExchangeOut(int64 e_i64Value);
		void								ToastCoin(int64 e_i64Value);
		void								RefundCoin(int64 e_i64Value);
		//
		//
		//			for mini game.
		//
		//			gererate mini game
		void								MonsterGenerateWithMiniGame(int e_iMonsterMonsterID,WCHAR*e_strMiniGameName,bool e_bFakeMiniGame);
		//set monster to died
		void								MiniGameMonsterDied(cMonster*e_pMonster,int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney);
		//
		cMonster*							GetConsecutiveMonsterToDied(int e_iPayRate,int e_iIndex,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney);
		//only for mini game use!!.
		bool								IsMiniGameIsMonsterDied(cMonster*e_pMonster,int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney);
		//
		void								SameTypeMonsterToDiedByProbability(int e_iPayRate,std::vector<cMonster*>*e_pStoreList,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piOutMoney);
		//
		float								AllMonsterDiedByProbability(int e_iPayRate,cPlayerBehaviorBase*e_pPlayerBehaviorBase,int*e_piGetMoney);
		//
	};

#endif