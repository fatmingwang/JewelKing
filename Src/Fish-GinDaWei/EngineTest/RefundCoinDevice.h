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

		cIOSMIOPackData*	m_pInsertCoin;//���a������ƶq

		cIOSMIOPackData*	m_pWantRefundCountDevice;//
		cIOSMIOPackData*	m_pCurrentRefundCountDevice;//
		cIOSMIOData*		m_pResetWantRefundCountDevice;//reset refund device value as expect so device keep walking

		WCHAR				m_strReceive[MAX_PATH];
		int*				m_piCoinToScore;//���P�������
		int*				m_piExchangeIn;//�}����
		cClickMouseBehavior	m_RefundKey;//
		cClickMouseBehavior	m_ExchangeInKey;//
		cClickMouseBehavior	m_ExchangeOutKey;//
		int					m_playerID;//���aID��
		void				ExchangeBehaviod(float e_fElpaseTime);
		void				CoinBehavior(float e_fElpaseTime);
		void				CoinRefund();
		
	public:
		cRefoundCoinDevice(
							cPlayerBehaviorBase*e_pPlayer,//player ID
							cIOSMIOPackData*e_pInsertCoin,//���a������ƶq
							cIOSMIOPackData*e_pHopperValue,//���a�ݰh�����ƶq
							cIOSMIOPackData*e_pReHopperValue,//�ثe�h���Ѿl�ƶq
							cIOSMIOPackData*e_pTicketValue,//���a�ݰh�m�����ƶq
							cIOSMIOPackData*e_pReTicketValue,//�ثe�h�m���Ѿl�ƶq
							cIOSMIOData*	e_pResetWantRefundCountCoinDevice,
							cIOSMIOData*	e_pResetWantRefundCountTicketDevice,
							int*e_piCoinToScore,//���P�������
							unsigned char e_ucReturnKey,//�h����
							unsigned char e_ucOpenKey,//�}����
							unsigned char e_ucWashKey//�~����
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