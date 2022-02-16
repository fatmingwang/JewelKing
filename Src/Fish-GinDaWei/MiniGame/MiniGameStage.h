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
DESCRIPTION   : �ۭ��u�A���X�s�����A�����u�A���ϼu�C
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
DESCRIPTION   : ���p�u�A�F���ۦP�����s�C
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameStage02:public cMiniGameBase
{
private:
	cMPDI*						m_pMPDI_IceBreak;								//�Q�p�q��
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
DESCRIPTION   : �N��u, �⳽�B���A�@���@����^�ӡC
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
	float						m_fSpeed_01; //�h���t��
	float						m_fSpeed_02; //�^�ӳt��
	Vector3						m_vCurrentFish;
	float						m_fTotalMovingTime;
	float						m_fTimer_IceBall_01;		//(�N+�ĴX�ӡH) -> ���h�[
	//
	static const int			COUNT_MAXIUM_ICE_FISH = 20;	//�̦h�N�u��ᦺ 20 ��
	int							m_nCurrentFishIndex;		//�`�@�n���X�� "��" �H
	//
	int							m_nCurrentIceLaserIndex;
	
	float						m_fDistanceIceLaser;
	bool						m_fIsIceLaserMoving; //�B�W�O�_�b����
	
	cMonster*					m_pMonsters[COUNT_MAXIUM_ICE_FISH];				//�Q��^�Ӫ���
	cMPDI*						m_pMPDI_IceBallOpen[COUNT_MAXIUM_ICE_FISH];		//��(�X�{)
	cMPDI*						m_pMPDI_IceBall[COUNT_MAXIUM_ICE_FISH];			//��(�ݩR)
	cMPDI*						m_pMPDI_IceBomb[COUNT_MAXIUM_ICE_FISH];			//��(�z��)
	cMPDI*						m_pMPDI_CamboDisc[COUNT_MAXIUM_ICE_FISH];		//�ĴX�ӡH
	cMPDI*						m_pMPDI_CamboDiscSingle[COUNT_MAXIUM_ICE_FISH];	//�ĴX�ӡH(�Ӧ�ƪ�)
	cMPDI*						m_pMPDI_IceBreak;								//��B�Ԧ^�Ӭۼ����ɭԤp�H�B
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
DESCRIPTION   : �p�^���u�A���Q�ݭn���h�֡C
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
DESCRIPTION   : �m�� 02
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
DESCRIPTION   : �C
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
DESCRIPTION   : �C
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cMiniGameStage32:public cMiniGameBase
{
private:
	float						m_fTimer_RunnerTotalAngle;	//���B������ɶ�
	float						m_fTimer_RunnerVelocity;	//���B����t��
	float						m_fTimer_BrakeVelocity;		//���B����٨��t��
	float						m_fTimer_BreakTime;			//�٨��n�ήɶ��P�_, ���M�[�t���檺�ɭԷ|���~�C
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
DESCRIPTION   : �C
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