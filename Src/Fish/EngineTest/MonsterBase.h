/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _FISH_MONSTER_BASE_H_
#define _FISH_MONSTER_BASE_H_

//<Monster Type="Normal">
//	<Data Prize="" Speed="">
//	<Animation	MPDIList="xxx.mpdi" MPDI="xxxx" >
//	<Physical File="xxx.xml">
//	<AI Moving="xxx.xml">
//</Monster>
class	cPlayerBehaviorBase;
class	cBullet;
class	cBulletManager;
class	cFishProbability;
class	cFishGroup;

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
enum	eMonsterType
{
	eMT_NORMAL = 0,
	eMT_BOSS,
	eMT_MAX,
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
enum	eMonsterStatus
{
	eMS_NONE = 0,				//not on the scene
	eMS_ALIVE,					//on the scene,do AIMachine
	eMS_HITTED,					//probability give
	eMS_DYING,					//hitted play died animation
	eMS_DIED_SHOW,				//
	eMS_WAITING_FOR_CLEAN,		//
	eMS_CONTROL_BY_MINI_GAME,	//
	eMS_MAX,

};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */	
enum	eBodyType
{
	eBT_SMALLE = 0,
	eBT_MIDIUM,
	eBT_BIG,
	eBT_MAX,
};

eBodyType	GetBodyType(const WCHAR*e_str);
//please use clone to copy object
class cMonster:public cBaseBehavior
{
private:
	virtual	void				ProcessPhysicalData(TiXmlElement*e_pElement) = 0;
	GET_SET_DEC(eBodyType,m_eBodyType,GetBodyType,SetBodyType);
protected:
	friend class				cFishProbability;
	friend class				cBulletManager;
	friend class				cMonsterManager;
	//how long s it alive
	float						m_fAliveTime;
	//
	Vector4						m_vHittedHintColor;
	UT::sTimeCounter			m_HittedHintTC;
	//FishData.xml
	float						m_fProbabilityToDied;
	float						m_fRestProbabilityValueToDied;
	//while fish if update m_iTotalBiteMoney will add m_iCurrentFrameBiteMoney
	//and m_iCurrentFrameBiteMoney will be 0 .
	int							m_iCurrentFrameBiteMoney;
	int							m_iTotalBiteMoney;
	//for out of screen test
	float						m_fRadius;
	eMonsterType				m_eMonsterType;

	int*						m_piPrize;
	sMinMaxData<int>*			m_piFloatingPrize;

	void						ProcessMonsterData(TiXmlElement*e_pTiXmlElement);
	eMonsterStatus				m_eMonsterStatus;
	cPlayerBehaviorBase*		m_pHittedPlayer;
	cBullet*					m_pBullet;
	//internal 
	virtual	void				InternalInit() = 0;
	virtual	void				InternalUpdate(float e_fElpaseTime) = 0;
	int							m_iID;
	std::wstring				m_MiniGameName;
	bool						m_bMiniGameMonster;
	//if true this monster won't died
	GET_SET_DEC(bool,m_bFakeMiniGameMonster,IsFakeMiniGameMonster,SetFakeMiniGameMonster);
	float						m_fScale;
public:
	cMonster();
	~cMonster();
	cMonster(cMonster*e_pMonster);
	int							GetID(){ return m_iID; }
	void						Init();
	virtual	void				Update(float e_fElpaseTime);
	bool						IsMiniGameMonster();
	void						SetMiniGameMonster(bool e_b,WCHAR*e_strName);
	virtual	bool				IsCollide(cbtShapeCollision*e_pbtShapeCollision) = 0;
	virtual	bool				IsVisible() = 0;
	virtual	void				LeaveScene() = 0;
	virtual	void				DebugRender(){}
	virtual	void				RandomPath() = 0;
	virtual	void				SetPos(Vector3 e_vPos) = 0;
	virtual	Vector3				GetAngle() = 0;
	virtual	void				SetAngle(Vector3 e_vAngle) = 0;
	virtual	Vector3				GetPos() = 0;
	virtual	cMatrix44			GetTransform() = 0;
	virtual	void				SetPath() = 0;
	virtual	void				SetPath(cCurveWithTime* e_pCurveWithTime) = 0;
	virtual	cCurveWithTime*		GetPath() = 0;
	void						AddBiteMoney(int e_iMoney);
	eMonsterStatus				GetMonsterStatus(){ return m_eMonsterStatus; }
	virtual	void				SetMonsterStatus(eMonsterStatus e_eMonsterStatus);
	virtual	void				CollisionDataCheck() = 0;
	cPlayerBehaviorBase*		GetHittedPlayer(){return  m_pHittedPlayer;}
	cBullet*					GetBullet(){return m_pBullet;}
	int							GetPrize();
	//while monster status is wait for clear or control by mini game cannot be died
	bool						IsStatusAllowToDied();
	virtual	void				MonsterLeaveScene() = 0;

};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
struct	sHittedFishData
{
	cPlayerBehaviorBase*	pPlayer;
	cBullet*	pBullet;
	cMonster*	pMonster;
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cTurtleBoss;
class	cMonsterManager:public cObjectListByName<cMonster>,public cNodeISAX
{
	cTurtleBoss*	m_pTurtleBoss;
	//each type'sfish on the screen
	int				m_iCurrentBodyTypeCount[eBT_MAX];
	int				m_iMaxBodyTypeCount[eBT_MAX];
private:
	friend class	cBulletManager;
	friend class	cFishProbability;
	friend class	cFishGroup;
	//
	virtual	bool					MyParse(TiXmlElement*e_pRoot);	
	//from data.
	cObjectListByName< cObjectListByName<cMonster> >	m_AllFish;
	cMonster*						ProcessMonster(TiXmlElement*e_pTiXmlElement,const WCHAR*l_strTypeID);
	int								m_iNumMonsterAlive;
	int								m_iMaxMonsterAlive;
	std::map<int,int>				m_iMaxMonsterAliveWithPlayerCount;
	//just a reference
	std::map<int,int>				m_iEachMonsterReserve;
	cMonster**						m_ppWorkingMonster;
	//for hit
	int								m_iNumMonsterHitted;
	sHittedFishData*				m_pHittedFish;//amount is m_iMaxMonsterAlive
	//
	//
	cObjectListByName<cFishGroup>*	m_pAllFishGroupFile;
	cFishGroup*						m_pMultiFishGroup;
	//for row or column
	int								FishGroupGo(int e_iFishID,int e_iCount);
	//for single fish mpdi group
	int								FishGroupGo(WCHAR*e_strFishName);
	//for multi fish mpdi group
	int								FishGroupGo();
	//alive monster prize
	int								m_iTotalAliveMonsterPrize;
public:
	cMonsterManager();
	~cMonsterManager();
	static			cFAIBehaviorParser* m_spFAIBehaviorParser;
	void			MonsterLeaveScene();
	void			Init();
	void			Update(float e_fElpaseTime);
	void			Render();
	//while monster died
	void			LastRender();
	void			DebugRender();
	void			Destroy();
	cMonster*		Require(const WCHAR*e_strFishName,bool e_bForceFetch = false);
	cMonster*		Require(int e_iFishIndex,bool e_bForceFetch = false);
	int				GetSmallestPrizeFishID(int*e_piPrize);
	int				GetSmallestPrizeFishIDFromAliveFish(int*e_piPrize);
	cMonster**		f_GetWorkingMonster(void){ return m_ppWorkingMonster; }
	int				f_GetCountMonsterAlive(void){ return m_iNumMonsterAlive; }
	void			BossOut();
	cTurtleBoss*	GetBoss(){return m_pTurtleBoss;}
};

#endif