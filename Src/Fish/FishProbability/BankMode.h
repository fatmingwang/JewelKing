#ifndef _BANK_MODE_H_
#define _BANK_MODE_H_

#include "FakeRevenue.h"
class	cBankRunner;
struct sBulletFireAverage;
//<cBankMode BulletDuration="6" />
class	cBankMode:public NamedTypedObject
{
	friend class		cBankRunner;
protected:
	//
	int64				m_i64TotalBulletValue;
	int64				m_i64Aeverage;
	int					m_iBulletCount;
	void				BulletDataUpdate(int64 e_iValue);
	//
	sBulletFireAverage*	m_pBulletFireAverage;
	//
	int64	m_i64CurrentIn;
	int64	m_i64CurrentOut;
	int64	m_i64Revenue;
	virtual	float	GetProbability();
	virtual	float	GetNewProbability(int e_iMoney);
	//while condition is satisifed,call ModeAssign and auto to call this.
	virtual void	ResetDataInit() = 0;
	//from control panel
	float			m_TargetProbability;
public:
	cBankMode(float e_pTargetProbability,sBulletFireAverage*e_pBulletFireAverage);
	cBankMode(TiXmlElement*e_pTiXmlElement);
	~cBankMode();
	//
	virtual	bool	IsModeSatisfied() = 0;
	//positive to negative and invert
	void			ModeAssign(cBankMode*e_pBankMode);
	virtual	void	AddRevenue(int64 e_i64Value);
	virtual	void	MinusRevenue(int64 e_i64Value);
	virtual	bool	IsMoneyOutOk(int e_iMoney,int e_iPayRate);
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	DebugRender( int e_iPosX,int e_iPosY ) = 0;
	virtual	bool	IsFakeMoneyEnoughToPay(int e_iMoney){ return false; }
};
//<PossitiveBankMode BulletDuration="6">
//	<FakeRevenue />
//</PossitiveBankMode>
class	cPossitiveBankMode:public cBankMode
{
	sFakeRevenue	m_FakeRevenue;
	virtual void	ResetDataInit();
public:
	cPossitiveBankMode(float e_pTargetProbability,sBulletFireAverage*e_pBulletFireAverage);
	cPossitiveBankMode(TiXmlElement*e_pTiXmlElement);
	~cPossitiveBankMode();
	//switch condition:fake revenue is zero and revenue is negative
	virtual	bool	IsModeSatisfied();
	virtual	void	MinusRevenue(int64 e_i64Value);
	bool			IsMoneyOutOk(int e_iMoney,int e_iPayRate);
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	DebugRender( int e_iPosX,int e_iPosY );
	virtual	bool	IsFakeMoneyEnoughToPay(int e_iMoney);
};
//<NegativeMode BigPrizeToOut="20" m_fFetchPercentageToRevenueFromBullet="0.3" BulletDuration="6"/>
class	cNegativeBankMode:public cBankMode
{
	//while revenue is positive fetch control's setup probability to balance revenue
	float			m_fFetchPercentageToRevenueFromBullet;
	//give a big money while store the revenue is processing
	int64			m_i64StoredBigMoneyToOut;
	//bool			m_bBigMoneyOut;
	int				m_iBigPrizeToOut;
	bool			IsBigMoneyOkOkay(int e_iMoney,int e_iPayRate);
	virtual void	ResetDataInit();
public:
	cNegativeBankMode(float e_pTargetProbability,sBulletFireAverage*e_pBulletFireAverage,float e_fFetchPercentageToRevenueFromBullet);
	cNegativeBankMode(TiXmlElement*e_pTiXmlElement);
	~cNegativeBankMode();
	//store income(from control panel's probability) to keep revenue as positive
	virtual	void	AddRevenue(int64 e_i64Value);
	virtual	bool	IsModeSatisfied();
	bool			IsMoneyOutOk(int e_iMoney,int e_iPayRate);
	virtual	void	DebugRender( int e_iPosX,int e_iPosY );
};
//<Root>
	//<SubRepositoryBank Range="100,70">
	//	<PossitiveBankMode >
	//		<FakeRevenue />
	//	</PossitiveBankMode>
	//	<NegativeMode BigPrizeToOut="20" m_fFetchPercentageToRevenueFromBullet="0.3" />
	//</SubRepositoryBank>
//</Root>
class	cBankRunnerManager;
class	cBankRunner:public NamedTypedObject
{
	friend class	cBankRunnerManager;
	Vector2				m_vTargetPercentValueRange;		//small 0~30,Midum 31~70,big71~100
	int64				m_i64LeastMoney;				//
	cBankMode*			m_pCurrentMode;
	cPossitiveBankMode*	m_pPossitiveBankMode;
	cNegativeBankMode*	m_pNegativeBankMode;
public:
	cBankRunner(TiXmlElement*e_pXmlElement);
	~cBankRunner();
	void	Update(float e_fElpaseTime);
	void	DebugRender( int e_iPosX,int e_iPosY );
	void	AddRevenue(int64 e_i64Value);
	void	MinusRevenue(int64 e_i64Value);
	bool	IsMoneyOutOk(int e_iMoney,int e_iPayRate);
	bool	IsFakeMoneyEnoughToPay(int e_iMoney);
	float	RelativeProbability(int e_iPayRate,int e_iOneBulletHitMonsterMonut);
	Vector2	GetTargetPercentValueRange(){return m_vTargetPercentValueRange;}
	bool	IsLeastMoneyOk(int64 e_i64Money);
};

#endif