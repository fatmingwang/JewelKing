/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _FISH_MINI_GAME_STAGE_H_
#define _FISH_MINI_GAME_STAGE_H_

#include "../EngineTest/MiniGameBase.h"
#include "../EngineTest/MonsterBase.h"
//
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 旋風彈，打出龍捲風，走直線，不反彈。
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameStage01:public cMiniGameBase
{
private:
public:
	cMiniGameStage01(void);
	~cMiniGameStage01(void);
	//
	void			Init(void);
	void			Update(float e_fElpaseTime);
	void			Render(void);
	void			Render2(void);
	void			Destroy(void);
	//
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 落雷彈，轟炸相同的魚群。
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameStage02:public cMiniGameBase
{
private:
	cMPDI*						m_pMPDI_IceBreak;								//被雷電到
	cMPDI*						m_pMPDI_Fire_Special;							//
	//
public:
	cMiniGameStage02(void);
	~cMiniGameStage02(void);
	//
	void			Init(void);
	void			Update(float e_fElpaseTime);
	void			Render(void);
	void			Render2(void);
	void			Destroy(void);
	//
	void			f_SetFlag( eMiniGameFlag e_nFlag );
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 冷凍彈, 把魚冰凍住，一隻一隻抓回來。
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
enum eFreezeStep
{
	ENUM_STEP_FREEZE_START_TO_FIRE = 0,
	ENUM_STEP_FREEZE_MOVING_FOWARD,
	ENUM_STEP_FREEZE_ARRIVED_FISH,
	ENUM_STEP_FREEZE_WAIT_FOR_SHOWING_WORD,
	ENUM_STEP_FREEZE_PULL_FISH_BACKWARD,
	ENUM_STEP_FREEZE_ARRIVED_HOME,
	ENUM_STEP_FREEZE_GOTO_NEXT_FISH,
	ENUM_STEP_FREEZE_FIRE,
	ENUM_STEP_FREEZE_BREAK_ICE,
	ENUM_STEP_FREEZE_MAX
};
class	cMiniGameStage03:public cMiniGameBase
{
private:
	eFreezeStep					m_nFreezeStep;
	cMonster*					m_pCurrentMonster;
	float						m_fSpeed_01; //去的速度
	float						m_fSpeed_02; //回來速度
	Vector3						m_vCurrentFish;
	float						m_fTotalMovingTime;
	float						m_fTimer_IceBall_01;		//(冷+第幾個？) -> 停多久
	//
	static const int			COUNT_MAXIUM_ICE_FISH = 20;	//最多就只能凍死 20 隻
	int							m_nCurrentFishIndex;		//總共要有幾組 "凍" ？
	//
	int							m_nCurrentIceLaserIndex;
	
	float						m_fDistanceIceLaser;
	bool						m_fIsIceLaserMoving; //冰柱是否在移動
	
	cMonster*					m_pMonsters[COUNT_MAXIUM_ICE_FISH];				//被抓回來的魚
	cMPDI*						m_pMPDI_IceBallOpen[COUNT_MAXIUM_ICE_FISH];		//凍(出現)
	cMPDI*						m_pMPDI_IceBall[COUNT_MAXIUM_ICE_FISH];			//凍(待命)
	cMPDI*						m_pMPDI_IceBomb[COUNT_MAXIUM_ICE_FISH];			//凍(爆炸)
	cMPDI*						m_pMPDI_CamboDisc[COUNT_MAXIUM_ICE_FISH];		//第幾個？
	cMPDI*						m_pMPDI_CamboDiscSingle[COUNT_MAXIUM_ICE_FISH];	//第幾個？(個位數的)
	cMPDI*						m_pMPDI_IceBreak;								//把冰拉回來相撞的時候小碎冰
	//
	int				f_Update_FreezeMonster( float e_fElpaseTime );
	void			f_Render_FreezeMonster( void );
	//
public:
	//
	cMiniGameStage03(void);
	~cMiniGameStage03(void);
	//
	void			Init(void);
	void			Update(float e_fElpaseTime);
	void			Render(void);
	void			Render2(void);
	void			Destroy(void);
	//
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 雷霆炸彈，拉霸看要死多少。
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameStage04:public cMiniGameBase
{
private:
	bool			m_bIsSlotDone;
	int				m_nTimesA_delay;
	int				m_nRandNumA;
	int				m_nTimesA;
	bool			m_bFirstTime_WeaponFire;
	//
public:
	cMiniGameStage04(void);
	~cMiniGameStage04(void);
	//
	void			Init(void);
	void			Update(float e_fElpaseTime);
	void			Render(void);
	void			Render2(void);
	void			Destroy(void);
	void			f_SetFlag( eMiniGameFlag e_nFlag );
	//
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 彩金 02
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameStage05:public cMiniGameBase
{
private:
public:
	cMiniGameStage05(void);
	~cMiniGameStage05(void);
	//
	void			Init(void);
	void			Update(float e_fElpaseTime);
	void			Render(void);
	void			Render2(void);
	void			Destroy(void);
	//
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 。
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameStage31:public cMiniGameBase
{
private:
public:
	cMiniGameStage31(void);
	~cMiniGameStage31(void);
	//
	void			Init(void);
	void			Update(float e_fElpaseTime);
	void			Render(void);
	void			Render2(void);
	void			Destroy(void);
	//
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 。
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameStage32:public cMiniGameBase
{
private:
	float						m_fTimer_RunnerTotalAngle;	//幸運轉輪的時間
	float						m_fTimer_RunnerVelocity;	//幸運轉輪速度
	float						m_fTimer_BrakeVelocity;		//幸運轉輪煞車速度
	float						m_fTimer_BreakTime;			//煞車要用時間判斷, 不然加速執行的時候會有誤。
	//
public:
	cMiniGameStage32(void);
	~cMiniGameStage32(void);
	//
	void			Init(void);
	void			Update(float e_fElpaseTime);
	void			Render(void);
	void			Render2(void);
	void			Destroy(void);
	//
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 。
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameStage33:public cMiniGameBase
{
private:
public:
	cMiniGameStage33(void);
	~cMiniGameStage33(void);
	//
	void			Init(void);
	void			Update(float e_fElpaseTime);
	void			Render(void);
	void			Render2(void);
	void			Destroy(void);
	//
};


#endif