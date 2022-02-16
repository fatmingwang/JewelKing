/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "MiniGameStage.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../EngineTest/FishProbability.h"

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 雷霆炸彈
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameStage04::cMiniGameStage04(void):cMiniGameBase(MINI_GAME_STAGE_04)
{
	m_nPayRate = 5; //賠率
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameStage04::~cMiniGameStage04(void)
{
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage04::Init(void)
{
	cMiniGameBase::Init();
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage04::Update( float e_fElpaseTime )
{
	cMiniGameBase::Update(e_fElpaseTime);
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //雷公打雷待機
	{

	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷發射
	{

	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //雷公打出來的雷
	{

	} //end if
	//
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage04::Render(void)
{
	cMiniGameBase::Render();
	//

}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage04::Render2(void)
{
	cMiniGameBase::Render2();
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //雷公打雷待機
	{

	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷發射
	{

		
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //雷公打出來的雷
	{

	} //end if
	//
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage04::Destroy(void)
{
	cMiniGameBase::Destroy();
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cMiniGameStage04::f_SetFlag( eMiniGameFlag e_nFlag )
{
	if( e_nFlag==f_GetFlag() )
	{
		return;
	} //end if
	//
	switch( e_nFlag )
	{
	case MINI_GAME_FLAG_NONE:
	case MINI_GAME_FLAG_GOTO_THUNDER_IDLE:
	case MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING:
		m_bIsSlotDone = false;
		m_nTimesA_delay = 2;
		m_nRandNumA = 20; //20 ~ 100
		m_nTimesA = 0;
		m_bFirstTime_WeaponFire = true;
		//
		break;
	default:
		break;
	} //end switch
	//
	cMiniGameBase::f_SetFlag(e_nFlag);
}


/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */