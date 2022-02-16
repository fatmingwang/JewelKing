/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _PLAYERDATA_H_
#define _PLAYERDATA_H_

#include "../EngineTest/PlayerBehaviorBase.h"
#include "../EngineTest/MiniGameBase.h"
#include "../EngineTest/Bullet.h"
#include "../EngineTest/FishProbability.h"
#include "Coin.h"


/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cPlayerDataMiniGameCardList:public cObjectListByName<cMiniGameBase>
{
private:
public:
	cPlayerDataMiniGameCardList(void){};
	~cPlayerDataMiniGameCardList(void){};
};


/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
enum eGameStage
{
	GAME_STAGE_NORMAL = 0x00000001,
	GAME_STAGE_MINIGAME_ING,	//PRE 表示準備要進入, ING 表示進行中, ED 表示做完了
};

#define COINSTACKNUM   50
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cPlayerData:public cPlayerBehaviorBase
{
private:
	cMatrix44					m_matPlayerTransfomr;	//玩家位置和方向
	cMatrix44					m_matPlayerDirection;	//玩家方向，不含位置，只有方向
	cMatrix44					m_matTubeTransfomr;		//tube位置和方向
	cMatrix44					m_matMessageTransfomr;
	cMPDIList*					m_pPlayerMPDIList ;
	cMultiPathDynamicImage*		m_pPlayerImage ;
	cMPDIList*					m_pTubeMPDIList ;
	cFMWorkingObjectChanger<cMultiPathDynamicImage>		m_AttackList;
	cNumerialImage*				m_pLevelNumberImage;
	cNumerialImage*				m_pScoreNumberImage;

	cMultiPathDynamicImage*		m_pMessageExplode;
	cMultiPathDynamicImage*		m_pMessageInsert;
	cMultiPathDynamicImage*		m_pMessageNoMoney;
	cMultiPathDynamicImage*		m_pMessageIOError;

	int*						m_piMaxLevelNumber;
	int*						m_piAddLevelNumber;
	int*						m_piCoinToScore;
	int*						m_piMinScoreBet;
	int64*						m_pScoreLimit;

	unsigned char				m_cInput_Right;
	unsigned char				m_cInput_Left;
	cClickMouseBehavior			m_FireClick;
	unsigned char				m_cInput_Fire;
	cClickMouseBehavior			m_LevelClick;
	unsigned char				m_cInput_Level;
	cClickMouseBehavior			m_InsertClick;
	unsigned char				m_cInput_Insert;

	Vector3						m_vPlayerPos;
	Vector3						m_vMessagePos;
	Vector3						m_vTubePos;
	Vector3						m_vScorePos;
	

	float						m_fTubeTime;
	float						m_fDieChangeLevelTime;
	
	int							m_iLevel;
	int							m_iLevelCount;
	
	bool						m_bIsAttack;
	vector<int>					m_vLevelMax;
	vector<int>					m_vLevelMin;
	vector<int>					m_vLevelIndex;


	int64						m_iAutoScore;
	int							m_iAutoLevelNumber;

	UT::sTimeCounter			m_AutoFireTC;
	float						m_fAutoFireTime;
	bool						m_ChangeLevelOnce;
	bool						m_AutoAddScore;

	
	cPlayerDataMiniGameCardList*	m_pPlayerDataMiniGameCardList;
	//
	void ProcessPlayerCoin(void);
	void ProcessPlayerLevel(void);
	void ProcessPlayerAutoData(void);
	void ProcessPlayer( TiXmlElement* e_pElement );
	void ProcessPlayerData( TiXmlElement* e_pElement );
	void ProcessPlayerUI( TiXmlElement* e_pElement );
	void ProcessPlayerIO( TiXmlElement* e_pElement );
	
	//
	void Fire(void);
	void f_Fire( cBullet* e_pBullet, float e_fFireDirection );
	void UpdateFireStatusTransform();
	void LvUp(bool e_bInsert);
	void DoFire(void);

	//
public:
	int							m_iLevelNumber;
	int							m_iStackPosCount[ COINSTACKNUM ];
	int							m_NowAddPos;
	int							m_Coin_StackStartIdx;	
	int							m_StackShowMax;
	float						m_fTubeAngle; //槍管的角度
	Vector3						m_vLvPos;
	Vector3						m_vPlayerRot;
	Vector3						m_vStackScorePos[4];


	eGameStage					m_eGameStage;
	cCoinManager*				m_pCoinManager;
	cMiniGameBase*				m_pCurrentMiniGame;
	int							m_NowCoinStackNum;

	//
	cPlayerData( TiXmlElement* e_pElement );
	~cPlayerData(void);
	//
	virtual	void Init(void);
	virtual	void Update( float e_fElpaseTime );
	virtual	void Render(void);
	virtual	void Destroy(void);
	//
	virtual	void AddScore2( Vector3 Pos, cMonster* Fish, int Money );
	//
	Vector3		f_GetPlayerPosition(void){return m_vPlayerPos;}
	Vector3		f_GetTubePosition(void){return m_vTubePos;}
	Vector3		f_GetPlayerRotation(void){return m_vPlayerRot;}
	cBullet*	f_Fire_MiniGame( cMiniGameBase* e_pMiniGame, float e_fFireDirection );
	int			f_GoToMinigame( cMiniGameBase* e_pMiniGame ); 
	//
};


#endif