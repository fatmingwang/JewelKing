/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _TURTLE_BOSS_H_
#define _TURTLE_BOSS_H_
#include "MultiCollisionFish.h"


//<Root>
//	<Boss LeaestBiteMoney="10000" AliveTime="100,300" />
//	<Status Name="Moving" MonsterName="GoldKenmu" Probability="10" />
//	<Status Name="Defence" MonsterName="GoldKenmuDefence" Probability="100" />
//	<Status Name="RandomlyRotation" MonsterName="GoldKenmuRandomlyRotation"Probability="200"  />
//</Root>

enum	eTurtleBossStatus
{
	eTBS_MOVING = 0,
	eTBS_DEFENCE,
	eTBS_RANDOMLY_ROTATION,
	eTBS_MAX
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
struct	sProbabilityAeverage
{
	sProbabilityAeverage();
	int					iTotalProbability;
	std::vector<int>	TotalProbabilityList;
	int					GetRandomIndex();
};

class	cBulletManager;
class	cBullet;
class	cPlayerBehaviorBase;
class	cFishProbability;
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cTurtleBoss:public ISAXCallback,public cFMWorkingObjectChanger<cNormalFish>
{
	friend class	cBulletManager;
	friend class	cFishProbability;
	eTurtleBossStatus			m_eTurtleBossStatus;
	sProbabilityAeverage		m_ProbabilityAeverage;
	eMonsterStatus				m_eMonsterStatus;
	virtual	void				HandleElementData(TiXmlElement*e_pTiXmlElement);
	void						ProcessStatusData(TiXmlElement*e_pTiXmlElement);
	void						ProcessBossData(TiXmlElement*e_pTiXmlElement);
	int							m_iLeastBiteMoney;
	int							m_iCurrentLeastBiteMoney;
	UT::sMinMaxData<float>		m_LeastBiteMoneyRestrict;
	UT::sTimeCounter			m_TimeToLeave;
	UT::sMinMaxData<float>		m_TimeRestrict;
	void						MonsterLeaveCheck(float e_fElpaseTime);
	//
	void						ChangeStatus();
	bool						m_bSpecialMoving;
	//
	cBullet*					m_pHittedBullet;
	cPlayerBehaviorBase*		m_pHittedPlayer;
public:
	cTurtleBoss();
	~cTurtleBoss();
	void						Init();
	virtual	void				Update(float e_fElpaseTime);
	virtual	void				Render();
	virtual	bool				IsCollide(cbtShapeCollision*e_pbtShapeCollision);
	virtual	bool				IsVisible();
	virtual	void				DebugRender();
	eMonsterStatus				GetMonsterStatus(){ return m_eMonsterStatus; }
	virtual	void				SetMonsterStatus(eMonsterStatus e_eMonsterStatus);
	bool						IsBiteEnoughMoney();
	int							GetPrize();
};

#endif