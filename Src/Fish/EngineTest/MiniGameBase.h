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
	MINI_GAME_STAGE_01 = 0x00000001,				//�ۭ��u
	MINI_GAME_STAGE_02,								//�N��u
	MINI_GAME_STAGE_03,								//���p��
	MINI_GAME_STAGE_04,								//�p�^���u
	MINI_GAME_STAGE_55,								//�m���Ҧ� 02
};
//
enum eMiniGameFlag
{
	MINI_GAME_FLAG_NONE				= 0x00000001,
	MINI_GAME_FLAG_STICKY_MONSTER,					//��L�H�۳�
	MINI_GAME_FLAG_EXPLODE_01,						//��L�z���A���O�C�@�Ӷ�L���|�z��
	MINI_GAME_FLAG_EXPLODE_02,						//��L�z���A�o���z���N������ MINI_GAME_FLAG_GOTO_CARD_01
	
	MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING,			//��o�Z���T���ʵe (�b���x�W�h����)
	MINI_GAME_FLAG_GOTO_THUNDER_IDLE,				//���x���������p���ݾ��ʵe (�p���ݾ��B�o�g��V���СB�p�d������P�ɼ���)
	MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING,			//�p�����p�o�g
	MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED,			//�p�����p�o�g��
	
	MINI_GAME_FLAG_GOTO_CARD_01,					//�d���ƶ���

	MINI_GAME_FLAG_GOTO_FINISH_01,					//�l�u���X���~
	MINI_GAME_FLAG_GOTO_FINISH_99,					//���� -> MINI_GAME_FLAG_NONE
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
	cMPDI*						m_pMPDI_Plate;				//��L
	cMPDI*						m_pMPDI_PlateExplode;		//��L�z��
	cMultiPathDynamicImage**	m_pMPDI_WeaponGetMessage;	//��o�Z������
	
	cMPDI*						m_pMPDI_Compass;			//�p�������y
	cMPDI*						m_pMPDI_Needle;				//��L��V����
	cMPDI*						m_pMPDI_LightningBolt;		//���X�p�����@�b��
	cMPDI*						m_pMPDI_BonusShow_D;		//Bonus �]������b���x�W��q�X
	static const int			MAXIUM_SCORE_NUMBER = 10;	//�̦h��ܤQ���
	int							m_nHowManyScore;			//�`�o�����X��ơH
	cMPDI*						m_pMPDI_BonusShow_ScorePos_U[MAXIUM_SCORE_NUMBER];	//Bonus �]�����x�W����઺�L�A�X�{�����ơC
	cMatrix44					m_matBonusShow_ScorePos[MAXIUM_SCORE_NUMBER];
	eMiniGameStage				m_nStage;					//�ĴX�Ӥp�C��
	eMiniGameFlag				m_nFlag;					//����X�Ъ��A
	//
	float						m_fTimer_Shake01;			//
	//
	//
public:
	int							m_nTotalGotScore;			//�`�o��
	bool						m_bAutoExplode;
	static const int			INDEX_START_ICE_LASER = 3;
	int							m_nPayRate;
	int							m_nPayRateBullet;
	//
	cMatrix44					m_matNeedle;				//
	cMatrix44					m_matTube;					//���x���x�}
	cMatrix44					m_matPlayerFace;			//���a��V
	cMonster*					m_pStickyMonster;
	//
	cMPDI**						m_pMPDI_WeaponWait;			//�p���ݩR
	//
	int							m_nCount_Fire01_Current;	//�ثe�p�����X�h�ֹp�H
	int							m_nCount_Fire01;			//�p�����X�h�ֹp�H
	int							m_nCount_Fire01_CallBack;	//�p�����X���p�^���]���F���ƶq
	cMPDI**						m_pMPDI_Fire01;				//�p�����X���p�S��
	cBullet**					m_pBulletLatest;			//�p�����X�Ӫ��p
	cMatrix44*					m_matPlayerTonado;			//�s����
	//
	static const int			COUNT_MAX_FISH_EXPLODE = 100;	//�̦h�@�ʭӳ��z���ʵe
	int							m_nCurrentFishExplodeIndex;
	cMPDI*						m_pMPDI_FishExplode[COUNT_MAX_FISH_EXPLODE];			//���z��
	//
	int							m_nReslut;					//�]�� 79979 �� update ���S���^�ǭȡA�ҥH�o�ۤv�ˤ@�ӡC
	cPlayerBehaviorBase*		m_pWhichPlayer;				//���@���x�H
	cMPDI*						m_pMPDI_Card01;				//�d��
	//
	cMPDI**						m_pMPDI_WeaponFire;			//�p���o�g
	cMPDI*						m_pMPDI_flash_light_t1;		//�ե�
	cMPDI*						m_pMPDI_BonusBomb;			//��b�ù��������j���Q
	cMPDI*						m_pMPDI_Slot;				//��b�p���W�誺�p���Q
	//
	cMPDI*						m_pMPDI_Winning_02_A;		//�m�� 2 A
	cMPDI*						m_pMPDI_Winning_02_B;		//�m�� 2 A
	cMPDI*						m_pMPDI_Winning_02_C;		//�m�� 2 A
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
	void			f_Fire_ed( void ); //ed ��ܵ���
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