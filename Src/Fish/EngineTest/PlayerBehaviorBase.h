/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _PLAYER_BEHAVIOR_BASE_H_
#define _PLAYER_BEHAVIOR_BASE_H_


enum ePlayerStatus
{
	PLAYER_STATUS_IDLE			=0x00000001,
	PLAYER_STATUS_ATTACK,
};

class cMonster;

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
class	cPlayerBehaviorBase:public cBaseBehavior
{
private:
	
	
	//
public :
	const static int	COINPOPUP_COUNT = 10;
	float				m_fRotateSpeed;
	int					m_iPlayerID;
	int64				m_i64Score;
	int					m_iAddScoreCount;
	int					m_iCointCount;
	bool				m_bAlive;
	bool				m_bAutoPlay;
	ePlayerStatus		m_eStatus;
	bool				m_bNoCoin;
	bool				m_bReturnCoin;
	//

	//
	virtual	void	Init(void){}
	virtual	void	Update( float e_fElpaseTime ){}
	virtual	void	Render(void){}
	virtual	void	Destroy(void){}
	//
	void			AddScore( Vector3 Pos, cMonster* Fish, int Money );
	virtual	void	AddScore2( Vector3 Pos, cMonster* Fish, int Money ){};

	//
};

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cPlayerManager:public cObjectListByName<cPlayerBehaviorBase>
{
private:
	int*				m_ipPlayerCount;
	//
public:
	//
	cPlayerManager(void);
	~cPlayerManager(void);
	//
	void	Init(void);
	void	Update( float e_fElpaseTime );
	void	Render(void);
	void	Destroy(void);
	//
};

	
#endif