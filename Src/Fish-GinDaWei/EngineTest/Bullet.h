/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _FISH_BULLET_H_
#define _FISH_BULLET_H_

#include "PlayerBehaviorBase.h"
#include "../EngineTest/MiniGameBase.h"

class	cPlayer;
class	cPlayerBullet;
class	cBulletManager;
class	cMonsterManager;
class	cBullet;

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
class	cWeb
{

	std::vector<cMPDI*>				m_WebList;
	std::vector<cbtShapeCollision*>	m_btShapeCollisionList;
public:
	cWeb(std::vector<cMPDI*>*e_pMPDIList,std::vector<cbtShapeCollision*>*e_pbtShapeCollisionList);
	~cWeb();
	void		CastNetFishing(cBullet*e_pBullet);
	void		Update(float e_fElpaseTime);
	void		Render();
	void		DebugRender();
	bool		IsDone();
};

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
enum	eBulletStatus
{
	eBS_MOVING = 0,
	eBS_HITTED,
	eBS_EXPLOSION,
	eBS_EXPLOSION_DONE,
	eBS_IDLE,
	eBS_DONE,
	eBS_MAX,
};

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
class	cBullet:public cBaseBehavior
{
private:
	cMatrix44				m_matMatrix;
	float					m_fAngle;
	Vector3					m_vPos;

	friend class			cPlayerBullet;
	friend class			cBulletManager;
	friend class			cWeb;
	//web
	bool					m_bDoCollided;
	cMPDI*					m_pWebMPDI;
	cbtShapeCollision*		m_pWebCllision;
	void					CastNetFishing(void);
	void					WebUpdate(float e_fElpaseTime);
	//bullet
	eBulletStatus			m_eBulletStatus;
	cbtShapeCollision*		m_pBulletCollision;
	cPlayerBehaviorBase*	m_pPlayer;
	cMPDI*					m_pBulletMPDI;
	Vector3					m_vDirection;
	float					m_fSpeed;
	void					SetDirection( Vector3 e_vDirection );
	void					SetDirection( float e_fAngle );
	void					BulletUpdate( float e_fElpaseTime );
	void					SetTransform();
	//bullet level,useless just for future function
	int						m_iPowerLevel;
	//how many will be paid
	int						m_iPayRate;
	int						m_iNumMonsterShoot;
	//
	void					f_Update_Bullet_MiniGame( float e_fElpaseTime );
	void					f_Render_Bullet_MiniGame(void);
	bool					f_Collide_Bullet_MiniGame( cbtShapeCollision*e_pbtShapeCollision );
	//
public:
	cMiniGameBase*			m_pMiniGame;
	//
	cBullet( cMPDI* e_pBulletMPDI, cbtShapeCollision* e_pBulletCollision, float e_fSpeed, cMPDI* e_pWeb, cbtShapeCollision* e_pWebCollision );
	~cBullet(void);
	//
	virtual	void	Init(void);
	virtual	void	Update( float e_fElpaseTime );
	virtual	void	Render(void);
	virtual	void	DebugRender(void);
	virtual	void	Destroy(void);
	//
	void			Fire(cPlayerBehaviorBase*e_pPlayer,Vector3 e_vStartPos,Vector3 e_vDirection);
	void			Fire(cPlayerBehaviorBase*e_pPlayer,Vector3 e_vStartPos,float e_fAngle);
	virtual	bool	Collide(cbtShapeCollision*e_pbtShapeCollision);
	int				GetPayRate(){return m_iPayRate;}
	cMPDI*			f_GetBulletMPDI(void){return m_pBulletMPDI;}
	eBulletStatus	f_GetBulletStatus(void){return m_eBulletStatus;}
	Vector3			f_GetBulletDirection(void){return m_vDirection;}
	float			f_GetBulletSpeed(void){return m_fSpeed;}
	cMatrix44		f_GetBulletMatrix(void){ return m_matMatrix; }
	Vector3			f_GetBulletPos(void){ return m_vPos; }
	Vector3			f_GetBulletAngle(void){ return Vector3(0,0,m_fAngle); }
	int				GetNumMonsterShoot(){ return m_iNumMonsterShoot; };

};

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
//implement PlayerBuller
class	cPlayerBullet:public NamedTypedObject
{
	friend class	cBulletManager;
public:
	struct	sBulletData
	{
		cbtShapeCollision*	m_pBulletCollision;
		cMPDI*				m_pBulletMPDI;
		float				m_fSpeed;
		cbtShapeCollision*	m_pbtWebCollision;
		cMPDI*				m_pWebMPDI;
	};
private:
	void									AddBulletData(sBulletData*e_pBulletData);
	std::vector<sBulletData*>				m_BulletDataList;
	//level,count.
	std::vector<std::vector<cBullet*>* >	m_AllBullet;
	cBullet*								Require(int e_iLevel);
	void									ProcessBullet(int e_iAmount);
public:
	cPlayerBullet();
	~cPlayerBullet();
	//void									Fire(cPlayer*e_pPlayer,Vector3 e_vStartPos,Vector3 e_vDirection,int e_iLevel);
	//virtual	void							Update(float e_fElpaseTime);
	//virtual	void							Render();
	//virtual	void							Init();
};

//<Root EachPlayerLimitCount="50">
//	<CollisionFile FileName="" />
//	<PlayerBullet MPDIList="00.mpdi"  >
//		<LV1 Collision="" MPDI="" Speed="100"/>
//	</PlayerBullet>
//	<Web MPDIList="00.mpdi" ExlposionMPDI="Explosion">
//		<LV1 Name=""/>
//	</Web>
//</Root>

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
class cBulletManager:public cObjectListByName<cPlayerBullet>,public cNodeISAX
{
private:
	friend class cBullet;
	c2DImageCollisionData		m_2DImageCollisionData;
	void						ProcessCollisionFile(TiXmlElement*e_pRoot);
	void						ProcessPlayerBullet(TiXmlElement*e_pRoot);
	void						ProcessWeb(TiXmlElement*e_pRoot);
	virtual	bool				MyParse(TiXmlElement*e_pRoot);
	int							m_iNumWorking;
	//for update only
	cBullet**					m_ppUpdateBulletData;
	//each play has its limit bullet count
	int							m_iEachPlayerLimitCount;
	//
public:
	static bool					m_sbCollideSkip;
	cBulletManager();
	~cBulletManager();
	//
	cObjectListByName<cbtConcaveShapeList>	m_BulletCollisionData;
	cObjectListByName<cbtConcaveShapeList>	m_WebCollisionData;
	//
	//each play has its own bullet limit.
	cBullet*					Require( int e_iPlayerIndex, int e_iLevel, int e_iPayRate );
	void						Init();
	void						Update(float e_fElpaseTime);
	void						Render();
	void						DebugRender();
	void						Destroy();
	void						Collide( cMonsterManager* e_pMonsterManager );
	//
	int							f_Collide_Bullet_MiniGame( cMonsterManager* e_pMonsterManager, cBullet* e_pBullet, std::vector<cMonster*> *e_pMonsterList, cPlayerBehaviorBase*e_pPlayerBehaviorBase );
};

#endif