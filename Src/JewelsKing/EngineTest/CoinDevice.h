#ifndef _COIN_DEVICE_H_
#define _COIN_DEVICE_H_


	class	cDeviceBehaviorBase
	{
		cClickMouseBehavior	m_Singnal;
		virtual	void		SingnalOn() = 0;
	public:
		cDeviceBehaviorBase(unsigned char e_ucKey);
		virtual	void	Update(float _fElpaseTime);
		virtual	void	Render(){}
	};


	class	cExchangeMoneyDevice:public cDeviceBehaviorBase
	{
		cClickMouseBehavior	m_Singnal;
		cEventVariable*		m_pExhcangeVariable;
		cEventVariable*		m_pExhcangeVariable1;
		cEventVariable*		m_pExhcangeVariable2;
		virtual	void		SingnalOn();
	public:
		cExchangeMoneyDevice(int e_iMoney,unsigned char e_ucKey,cEventVariable*e_pExhcangeVariable,cEventVariable*e_pExhcangeVariable1,cEventVariable*e_pExhcangeVariable2);
		int				m_iMoney;
	};

	class	cRefundingCoinDevice;
	class	cToastCoinDevice:public cDeviceBehaviorBase
	{
		friend class		cRefundingCoinDevice;
		virtual	void		SingnalOn();
		cEventVariable*		m_pToastVariable;
		cEventVariable*		m_pToastVariable2;
		cEventVariable*		m_pToastVariable3;
	public:
		cToastCoinDevice(int e_iMoney,unsigned char e_ucKey,cEventVariable*e_pToastVariable,cEventVariable*e_pToastVariable2,cEventVariable*e_pToastVariable3);
		int					m_iCoinForPlayerMoney;
		//while money is toasted true,until u reset this or it will be true always
		bool				m_bToast;
	};

	#define	REFUND_MONEY_FILE_NAME "RefundMoneyFile"
	class	cRefundingCoinDevice:public cDeviceBehaviorBase
	{
		//if power lost check this file to setup player money,or ResundMoneyFile will be empty
		cBinaryFile*				m_pRefundMoneyFile;
		cEventVariable*				m_pRefuncdCoinVariable;
		cEventVariable*				m_pRefuncdCoinVariable1;
		cEventVariable*				m_pRefuncdCoinVariable2;
		cClickMouseBehavior			m_MoneyRefundSingnal;
		virtual	void				SingnalOn();
		//
		bool						m_bRefound;
		bool						m_bMoneyOutSingal;
		UT::sTimeCounter			m_iNoCoinWaitingTime;
		UT::sTimeCounter			m_iOffsetForCoinOut;
		cIOSMIOData*				m_pMotorData;
		cToastCoinDevice*			m_pToastCoinDevice;
		bool						m_bNoCoinError;
		cMPDI*						m_pMoneyNotEnoughMPDI;
	public:
		cRefundingCoinDevice(unsigned char e_ucRefundKeyData,unsigned char e_ucCoinRefundKeyData,cIOSMIOData*e_pMotorData,cToastCoinDevice*e_pToastCoinDevice,cEventVariable*e_pRefuncdCoinVariable,cEventVariable*e_pRefuncdCoinVariable1,cEventVariable*e_pRefuncdCoinVariable2);
		~cRefundingCoinDevice();
		virtual	void	Update(float _fElpaseTime);
		void	RefundCoin();
		bool	IsWorking();
		virtual	void	Render();
		//after power lost call this return -1 if file is not exist
		int		static	GetRefuncdFileMoney();
		void	Init();
	};

#endif