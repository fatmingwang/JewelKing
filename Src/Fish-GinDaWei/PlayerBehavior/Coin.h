/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _COIN_H_
#define _COIN_H_

#include "../EngineTest/PlayerBehaviorBase.h"
#include "../EngineTest/GameApp.h"


enum	eCointStatus
{
	eCoinScore,
	eCoinMove,
	eCoinStack,
	eCoinNull,
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class cPlayerData;
class cCoinManager;
class cCoin:public cBaseBehavior
{
	friend class cCoinManager;
	cMPDIList*					m_pCoinMPDIList;
	cCurveWithTime				m_Curve[cPlayerBehaviorBase::COINPOPUP_COUNT];
	
	static const int			MAXIUM_SCORE_NUMBER = 10;
	cMPDIList*					m_pCoinScoreMPDIList;
	cMultiPathDynamicImage*		m_pScoreImage[MAXIUM_SCORE_NUMBER];
	int							m_iHowManyScore;
	cMatrix44					m_matScorePos[MAXIUM_SCORE_NUMBER];

	cMPDIList*					m_pCoinStackMPDIList;
	cMultiPathDynamicImage*		m_pCoinImage[cPlayerBehaviorBase::COINPOPUP_COUNT];

	cMultiPathDynamicImage*		m_pSilverCoinImage_D[cPlayerBehaviorBase::COINPOPUP_COUNT];
	cMultiPathDynamicImage*		m_pSilverCoinImage_R[cPlayerBehaviorBase::COINPOPUP_COUNT];
	cMultiPathDynamicImage*		m_pSilverCoinImage_U[cPlayerBehaviorBase::COINPOPUP_COUNT];
	cMultiPathDynamicImage*		m_pSilverCoinImage_L[cPlayerBehaviorBase::COINPOPUP_COUNT];

	cMultiPathDynamicImage*		m_pGoldCoinImage_D[cPlayerBehaviorBase::COINPOPUP_COUNT];
	cMultiPathDynamicImage*		m_pGoldCoinImage_R[cPlayerBehaviorBase::COINPOPUP_COUNT];
	cMultiPathDynamicImage*		m_pGoldCoinImage_U[cPlayerBehaviorBase::COINPOPUP_COUNT];
	cMultiPathDynamicImage*		m_pGoldCoinImage_L[cPlayerBehaviorBase::COINPOPUP_COUNT];

	cMPDI*						m_pMPDI_BigWinShow1_D;
	cMPDI*						m_pMPDI_BigWinShow1_Loop;
	cMPDI*						m_pMPDI_BigWinShow2_D;
	cMPDI*						m_pMPDI_BigWinShow2_Loop;
	cMPDI*						m_pMPDI_BonusShow_ScorePos_U[MAXIUM_SCORE_NUMBER];
	int							m_nHowManyScore;
	cMatrix44					m_matBonusShow_ScorePos[MAXIUM_SCORE_NUMBER];
								
	cPrtEmitter*				m_pCoinFountain;
	cPrtEmitter*				m_pCoinFountain_U;
	cNumerialImage*				m_pNI_BigWinShow3_Score;
	int							m_nBigWinShow3_Coin_Current;
	int							m_nTimerBigWinShow3;


	cFMWorkingObjectChanger<cMultiPathDynamicImage>		m_CoinStackList;

	cMatrix44					m_matPlayerTransfomr;

	Vector3						m_vFishPos[cPlayerBehaviorBase::COINPOPUP_COUNT];
	Vector3						m_vPlayerPos;

	float						m_fCoinAnimationTime[cPlayerBehaviorBase::COINPOPUP_COUNT];
	float						m_fCoinIdleTimer;
	float						m_fSize[cPlayerBehaviorBase::COINPOPUP_COUNT];
	cPlayerData*				m_pPlayer;

	//
public:
	eCointStatus				m_eCointStatus;
	int							m_iMoney;
	int							m_iPlayerID;
	int							m_iStackIndex;
	int							m_iCoinIndex;
	int							m_nMonsterId;
	int							m_iStackPosIndex;
	float						m_fFlyTime;

	int*						m_piPlayerCount;
	Vector3						m_vRot;
	Vector3						m_vStackPos;
	Vector3						m_vCoinStackPos[4];
	
	int							m_OriPos;
	int							m_NowAccessIdx;

	bool						m_bAccess;
	bool						m_SetOriPos;
	bool						m_bCoinMove;

	float						m_DisplayTm;
	float						m_MoveElapseTm;
	float						m_StartX;
	float						m_TargetX;
	//
	cCoin(void);
	~cCoin(void);
	//
	virtual	void	Init(void);
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render(void);
	virtual void	Destroy(void);
	//
	void			ShowCoin(cPlayerData* e_PlayerData,Vector3 e_vFish[cPlayerBehaviorBase::COINPOPUP_COUNT],int e_iMoney , int e_iStackPosIndex);
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
/*class cCoinStack:public cBaseBehavior
{


public:
	cCoinStack(void);
	~cCoinStack(void);

	virtual	void	Init(void);
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render(void);
	virtual void	Destroy(void);

	void			ShowCoinStack();
};*/

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cCoinManager:public cObjectListByName<cCoin>
{

	vector<int>		m_vMaxStackNum;
	vector<int>		m_vMinStackNum;
	vector<int>		m_vStackIndex;

	vector<int>		m_vMaxFishPrize;
	vector<int>		m_vMinFishPrize;
	vector<int>		m_vCoinIndex;

	int				m_iStackIndex;
	int				m_iCoinIndex;
	//
	void	ProcessCoin(TiXmlElement*e_pElement);
	void	ProcessCoinStack(TiXmlElement*e_pElement);
	

	//
public:
	cCoin**					m_Coin;
	const static int		COUNT_COINS=50;

	//
	cCoinManager(void);
	~cCoinManager(void);
	//
	int		StackIndex(int e_iFishMoney);
	int		CoinIndex(int e_iFishPrize);
	
	void	Init(void);
	void	Update( float e_fElpaseTime );
	void	Render(void);
	void	Destroy(void);
	//
};


#endif