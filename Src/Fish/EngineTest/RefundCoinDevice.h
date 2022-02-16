#ifndef _FISH_IO_BASE_H_
#define _FISH_IO_BASE_H_

	class	cPlayerBehaviorBase;
	class	cRefoundCoinDevice:public NamedTypedObject
	{
		int64				m_i64RestCoinToRefund;
		//over than 5 second show no coin messae
		UT::sTimeCounter	m_RefundToCheckCoinTC;
		//UT::sTimeCounter	m_Wait8051SingnalToActive;
		bool				m_bRefundCoin;
		bool				m_bRefundCoinStart;
		cBinaryFile*		m_pBinaryFile;
		bool				m_bWorking;//bool of return	
		cPlayerBehaviorBase*m_pPlayer;//Player

		cIOSMIOPackData*	m_pInsertCoin;//玩家投幣的數量

		cIOSMIOPackData*	m_pWantRefundCountDevice;//
		cIOSMIOPackData*	m_pCurrentRefundCountDevice;//
		cIOSMIOData*		m_pResetWantRefundCountDevice;//reset refund device value as expect so device keep walking

		WCHAR				m_strReceive[MAX_PATH];
		int*				m_piCoinToScore;//幣與分的比例
		int*				m_piExchangeIn;//開分值
		cClickMouseBehavior	m_RefundKey;//
		cClickMouseBehavior	m_ExchangeInKey;//
		cClickMouseBehavior	m_ExchangeOutKey;//
		int					m_playerID;//玩家ID值
		void				ExchangeBehaviod(float e_fElpaseTime);
		void				CoinBehavior(float e_fElpaseTime);
		void				CoinRefund();
		
	public:
		cRefoundCoinDevice(
							cPlayerBehaviorBase*e_pPlayer,//player ID
							cIOSMIOPackData*e_pInsertCoin,//玩家投幣的數量
							cIOSMIOPackData*e_pHopperValue,//玩家需退幣的數量
							cIOSMIOPackData*e_pReHopperValue,//目前退幣剩餘數量
							cIOSMIOPackData*e_pTicketValue,//玩家需退彩票的數量
							cIOSMIOPackData*e_pReTicketValue,//目前退彩票剩餘數量
							cIOSMIOData*	e_pResetWantRefundCountCoinDevice,
							cIOSMIOData*	e_pResetWantRefundCountTicketDevice,
							int*e_piCoinToScore,//幣與分的比例
							unsigned char e_ucReturnKey,//退幣紐
							unsigned char e_ucOpenKey,//開分紐
							unsigned char e_ucWashKey//洗分紐
						  );
		~cRefoundCoinDevice(); 
		void			Update(float e_fElpaseTime);
		void			Render();
	};

	class	cRefoundCoinDeviceManager:public cObjectListByName<cRefoundCoinDevice>
	{		
		public:
			cRefoundCoinDeviceManager();
			~cRefoundCoinDeviceManager();

			void	Init(const char *filename);
			void	Update(float e_fElpaseTime);
			void	Render();
			void	Destroy();
	};
	


#endif