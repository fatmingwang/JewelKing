/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "MiniGameStage.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../EngineTest/FileNameDefine.h"
#include "../EngineTest/FishProbability.h"

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cMiniGameStage31::cMiniGameStage31(void):cMiniGameBase(MINI_GAME_STAGE_31)
{

}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cMiniGameStage31::~cMiniGameStage31(void)
{
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMiniGameStage31::Init(void)
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
void	cMiniGameStage31::Update( float e_fElpaseTime )
{
	cMiniGameBase::Update(e_fElpaseTime);
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //�p�����ݥ��p
	{	
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //�p�����p�ʵe
	{

	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //�p�����X�Ӫ��p
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
void	cMiniGameStage31::Render(void)
{
	cMiniGameBase::Render();

}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMiniGameStage31::Render2(void)
{
	cMiniGameBase::Render2();
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //�p�����p�o�g
	{
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //�p�����X�Ӫ��p
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
void	cMiniGameStage31::Destroy(void)
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

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */

