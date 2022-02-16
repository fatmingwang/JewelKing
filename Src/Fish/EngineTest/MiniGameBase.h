/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _FISH_MINI_GAME_BASE_H_
#define _FISH_MINI_GAME_BASE_H_
//
#include "MonsterBase.h"
#include "PlayerBehaviorBase.h"
//
enum eMiniGameStage
{
	MINI_GAME_STAGE_01 = 0x00000001,				//旋風彈
	MINI_GAME_STAGE_02,								//冷凍彈
	MINI_GAME_STAGE_03,								//落雷砲
	MINI_GAME_STAGE_04,								//雷霆砲彈
	MINI_GAME_STAGE_55,								//彩金模式 02
};
//
enum eMiniGameFlag
{
	MINI_GAME_FLAG_NONE				= 0x00000001,
	MINI_GAME_FLAG_STICKY_MONSTER,					//圓盤黏著魚
	MINI_GAME_FLAG_EXPLODE_01,						//圓盤爆炸，不是每一個圓盤都會爆炸
	MINI_GAME_FLAG_EXPLODE_02,						//圓盤爆炸，這個爆炸就直接到 MINI_GAME_FLAG_GOTO_CARD_01
	
	MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING,			//獲得武器訊息動畫 (在砲台上層播放)
	MINI_GAME_FLAG_GOTO_THUNDER_IDLE,				//砲台消失換成雷公待機動畫 (雷公待機、發射方向指標、小卡片旋轉同時播放)
	MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING,			//雷公打雷發射
	MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED,			//雷公打雷發射中
	
	MINI_GAME_FLAG_GOTO_CARD_01,					//卡片排隊中

	MINI_GAME_FLAG_GOTO_FINISH_01,					//子彈飛出場外
	MINI_GAME_FLAG_GOTO_FINISH_99,					//結束 -> MINI_GAME_FLAG_NONE
	//
	MINI_GAME_MAX
};
//
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameBase:public cBaseBehavior
{
	cMatrix44	m_Camera;
private:
	cMPDI*						m_pMPDI_Plate;				//圓盤
	cMPDI*						m_pMPDI_PlateExplode;		//圓盤爆炸
	cMultiPathDynamicImage**	m_pMPDI_WeaponGetMessage;	//獲得武器提示
	
	cMPDI*						m_pMPDI_Compass;			//雷公的底座
	cMPDI*						m_pMPDI_Needle;				//圓盤方向指標
	cMPDI*						m_pMPDI_LightningBolt;		//打出雷的那一剎那
	cMPDI*						m_pMPDI_BonusShow_D;		//Bonus 跑完之後在砲台上方秀出
	static const int			MAXIUM_SCORE_NUMBER = 10;	//最多顯示十位數
	int							m_nHowManyScore;			//總得分有幾位數？
	cMPDI*						m_pMPDI_BonusShow_ScorePos_U[MAXIUM_SCORE_NUMBER];	//Bonus 跑完砲台上方旋轉的盤，出現的分數。
	cMatrix44					m_matBonusShow_ScorePos[MAXIUM_SCORE_NUMBER];
	eMiniGameStage				m_nStage;					//第幾個小遊戲
	eMiniGameFlag				m_nFlag;					//什麼旗標狀態
	//
	float						m_fTimer_Shake01;			//
	//
	//
public:
	int							m_nTotalGotScore;			//總得分
	bool						m_bAutoExplode;
	static const int			INDEX_START_ICE_LASER = 3;
	int							m_nPayRate;
	int							m_nPayRateBullet;
	//
	cMatrix44					m_matNeedle;				//
	cMatrix44					m_matTube;					//砲台的矩陣
	cMatrix44					m_matPlayerFace;			//玩家方向
	cMonster*					m_pStickyMonster;
	//
	cMPDI**						m_pMPDI_WeaponWait;			//雷公待命
	//
	int							m_nCount_Fire01_Current;	//目前雷公打出多少雷？
	int							m_nCount_Fire01;			//雷公打出多少雷？
	int							m_nCount_Fire01_CallBack;	//雷公打出的雷回報跑完了的數量
	cMPDI**						m_pMPDI_Fire01;				//雷公打出的雷特效
	cBullet**					m_pBulletLatest;			//雷公打出來的雷
	cMatrix44*					m_matPlayerTonado;			//龍捲風
	//
	static const int			COUNT_MAX_FISH_EXPLODE = 100;	//最多一百個魚爆炸動畫
	int							m_nCurrentFishExplodeIndex;
	cMPDI*						m_pMPDI_FishExplode[COUNT_MAX_FISH_EXPLODE];			//魚爆炸
	//
	int							m_nReslut;					//因為 79979 的 update 都沒有回傳值，所以得自己弄一個。
	cPlayerBehaviorBase*		m_pWhichPlayer;				//哪一砲台？
	cMPDI*						m_pMPDI_Card01;				//卡片
	//
	cMPDI**						m_pMPDI_WeaponFire;			//雷公發射
	cMPDI*						m_pMPDI_flash_light_t1;		//白光
	cMPDI*						m_pMPDI_BonusBomb;			//放在螢幕中央的大拉霸
	cMPDI*						m_pMPDI_Slot;				//放在雷公上方的小拉霸
	//
	cMPDI*						m_pMPDI_Winning_02_A;		//彩金 2 A
	cMPDI*						m_pMPDI_Winning_02_B;		//彩金 2 A
	cMPDI*						m_pMPDI_Winning_02_C;		//彩金 2 A
	//
	std::vector<cMonster*>		m_vMonsterDiedList;			//
	//
	cMiniGameBase( eMiniGameStage e_nStage );
	~cMiniGameBase(void);
	//
	void			Init(void);
	void			Update(float e_fElpaseTime);
	void			Render(void);
	virtual void	Render2(void);
	void			Destroy(void);
	//
	void			f_Fire( void );
	void			f_Fire_ed( void ); //ed 表示結束
	void			f_SetStage( eMiniGameStage e_nStage );
	eMiniGameStage	f_GetStage(void){ return m_nStage; }
	virtual void	f_SetFlag( eMiniGameFlag e_nFlag );
	eMiniGameFlag	f_GetFlag(void){ return m_nFlag; }
	//
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameManager:public cObjectListByName<cMiniGameBase>
{
private:
	static const int	COUNT_MINI_GAME_STAGE = 4;
	static const int	COUNT_PLATE = COUNT_MINI_GAME_STAGE * 10;
	int					m_nTimes_1;
	//
	
	//
public:
	//
	cMiniGameManager(void);
	~cMiniGameManager(void);
	//
	void			Init(void);
	void			Update( float e_fElpaseTime );
	void			Render(void);
	void			Render2(void);
	void			Destroy(void);
	//
	cMiniGameBase*		f_RequireSpecificMiniGame( WCHAR* e_strName, cMonster* e_pMonster );
	void				f_GoTo_MiniGame_05(void);
	bool				IsMiniGameUsing(WCHAR*e_strName);
	bool				IsMiniGameUsing();
	//
};
//


#endif