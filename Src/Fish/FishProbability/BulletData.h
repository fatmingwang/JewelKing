#ifndef _BULLET_DATA_H_
#define _BULLET_DATA_H_

//struct	sBinaryFileStruct
//{
//	int		iNumPlayer;
//	int*	piPlayerMoney;
//	int		iMoneyIn;
//	int		iMoneyOut;
//};

//if payrate is smaller than aeverage, Probability should recalculate by a bullet shot how many fish and divide by fish's prize.
//ex"one bullet shoot 3 fishes,each fish's prize is
//2,5,10,then first fish's probability is 1/3/2,second's is 1/3/2,thirs's is 1/3/10
struct	sBulletFireAverage
{
	sBulletFireAverage();
	UT::sTimeCounter	TC;
	int					iSmallestBullet;
	int64				i64TotalMoney;
	int					iBulletCount;
	int					iBulletFireMoneyAeverage;
	void				BulletFire(int64 e_iPayRate);
	void				Update(float e_fElpaseTime);
	int					GetBulletFireMoneyAeverage();
	float				RelativeProbability(int e_iPayRate,int e_iOneBulletHitMonsterMonut);
};

class	cBankRunnerManager;
class	cBankRunner;
class	cBulletDataAndBankRepositorManager
{
	cBankRunnerManager*	m_pSubRepositoryBankManager;
	int64						m_i64TotalBulletValue;
	int64						m_i64Aeverage;
	int							m_iBulletCount;
	int							GetBulletAeverageFormatIndex(int e_iValue);
public:
	cBulletDataAndBankRepositorManager();
	~cBulletDataAndBankRepositorManager();
	float	GetBulletAeverageOffsetPercentage(int e_iValue);
	sBulletFireAverage*GetsBulletFireAverage(int e_iIndex);
	//
	void				Init();
	void				RevenueAdd(int e_iPayRate);
	void				RevenueMinus(int e_iOutMoney,int e_iPayRate);
	bool				IsMoneyOutOk(int e_iOutMoney,int e_iPayRate);
	void				Update(float e_fElpaseTime);
	float				GetRelativeProbability(int e_iPayRate,int e_iOneBulletHitMonsterMonut);
	cBankRunner*		GetSubRepositoryBankByPayRate(int e_iPayRate);
	void				DebugRender();
};

#endif