#ifndef _KING_EFFECT_H_
#define _KING_EFFECT_H_

	struct	sFreeGameData
	{
		sFreeGameData();
		~sFreeGameData();
		int						iTimeForPickupGems;
		//while free game merge below 3 MPDI to whoe result
		cMPDI*					pPrizeMPDI;
		cMPDI*					pPrizeAwardMPDI;
		cMPDI*					pPrizeAward_NumerialMPDI;
		void					RestIntoFreeGameTimes();
		//ignore this
		void					SetupRestFreeSpinTimes(int e_iPrize);
		void					Update(float e_fElpaseTime);
		void					Render();
	};

	enum	eKingStatus
	{
		eKS_IDLE = 0,
		eKS_FREE_GAME,
		//eKS_FREE_GAME_WALK_AWAY,
		eKS_MONEY1,
		eKS_MONEY501,
		eKS_MONEY1001,//5
		eKS_MONEY5000,
		//eKS_FREE_GAME_BACK,
		eKS_MAX
	};
	//enum eJewelStatus
	//{
	//	eJS_1 = 0,
	//	eJS_501,
	//	eJS_1001,
	//	eJS_5000,
	//	eJS_MAX
	//};
	class	cUpperScreen;
	class	cDownScreenEffect;
	class	cGameMessage;
	class	cKing:public cStatusObjectChanger(cMPDI)
	{
		sFreeGameData*			m_pFreeGameData;
		//
		cBasicSound*			m_pKingSound[eKS_MAX];
		//
		cMPDI*					m_pNumberForFreeGamePayRate;
		//
		cPrtEmitter*			m_pDimond;
		//
		cUpperScreen*			m_pUpperScreen;
		cDownScreenEffect*		m_pDownScreenEffect;
		std::vector<int>		m_iMoneyToChangeStatus;
		std::vector<int>		m_iMoneyToChangeJewelStatus;
		GET_SET_DEC(bool,m_bRenderDownScreenBG,IsRenderDownScreenBG,SetRenderDownScreenBG);
		GET_SET_DEC(bool,m_bRenderUpperScreenBG,IsRenderUpperScreenBG,SetRenderUpperScreenBG);
		GET_SET_DEC(bool,m_bRenderPayRateHint,IsRenderPayRateHint,SetRenderPayRateHint);
		GET_SET_DEC(bool,m_bRenderKing,IsRenderKing,SetRenderKing);
	public:
		cKing();
		~cKing();
		sFreeGameData*	GetFreeGameData(){return m_pFreeGameData;}
		virtual	bool    SetCurrentWorkingObject(int e_iIndex,bool e_bRestart = true);
		virtual	void	Init();
		virtual	void	InitStatus();
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		void			RenderPayRateHint();
		void			RenderUpperScreen();
		void			RenderExplosionCombo();
		void			RenderDownScreenEffect();
		void			RenderPerLineBetMoney();
		void			RenderGameMessage();
		void			ChangeStatusByMoney(int e_iMoney,bool e_bShowBigWin);
		void			SetFreeGamePayRate(int e_iPayRate);
		cGameMessage*	GetGameMessage();
		bool			IsBigWinDone();
	};

#endif