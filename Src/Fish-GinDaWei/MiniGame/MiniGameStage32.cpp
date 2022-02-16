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
cMiniGameStage32::cMiniGameStage32(void):cMiniGameBase(MINI_GAME_STAGE_32)
{
	m_fTimer_RunnerTotalAngle = 0.0f;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cMiniGameStage32::~cMiniGameStage32(void)
{
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMiniGameStage32::Init(void)
{
	cMiniGameBase::Init();
	//
	m_fTimer_RunnerTotalAngle = 0.0f;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMiniGameStage32::Update( float e_fElpaseTime )
{
	cMiniGameBase::Update(e_fElpaseTime);
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING ) //獲得武器訊息動畫 (在砲台上層播放)
	{
		cMultiPathDynamicImage* l_pMPDI_WeaponGetMessage=0;
		l_pMPDI_WeaponGetMessage = m_pMPDI_WeaponGetMessage[0];
		int	l_iCount = l_pMPDI_WeaponGetMessage->Count();
		for( int ii=0; ii<l_iCount; ++ii )
		{
			l_pMPDI_WeaponGetMessage->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		//
		l_pMPDI_WeaponGetMessage->Update( e_fElpaseTime );
		if( l_pMPDI_WeaponGetMessage->IsDone()==true )
		{
			f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_IDLE ); //得到小卡等待出場
			//
			m_fTimer_RunnerTotalAngle = 0.0f;
			m_fTimer_RunnerVelocity = 100.0f;
			m_fTimer_BreakTime = 3.0f; //要幾秒停下來, 再反推回去煞車速度
			m_fTimer_BrakeVelocity = m_fTimer_RunnerVelocity/m_fTimer_BreakTime;
			//
		} //end if
		//
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //砲台消失換成雷公待機動畫 (雷公待機、發射方向指標、小卡片旋轉同時播放)
	{
		if( m_pMPDI_WeaponGetMessage[1]->IsDone()==true )
		{
			m_fTimer_RunnerTotalAngle += e_fElpaseTime*m_fTimer_RunnerVelocity;
			float l_fAngle = m_fTimer_RunnerTotalAngle;
			m_pMPDI_WeaponGetMessage[2]->GetObject(0)->SetAngle( Vector3(0.0f, 0.0f, l_fAngle) );
			//
			m_pMPDI_WeaponGetMessage[2]->Update( e_fElpaseTime );
			//f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ); //得到小卡等待出場
		}
		else
		{
			int	l_iCount = m_pMPDI_WeaponGetMessage[1]->Count();
			for( int ii=0; ii<l_iCount; ++ii )
			{
				m_pMPDI_WeaponGetMessage[1]->GetObject(ii)->SetExtractTransform( &m_matTube );
			} //next ii
			m_pMPDI_WeaponGetMessage[1]->Update( e_fElpaseTime );
			if( m_pMPDI_WeaponGetMessage[1]->IsDone()==true )
			{
				int	l_iCount = m_pMPDI_WeaponGetMessage[2]->Count();
				for( int ii=0; ii<l_iCount; ++ii )
				{
					m_pMPDI_WeaponGetMessage[2]->GetObject(ii)->SetExtractTransform( &m_matTube );
				} //next ii
			} //end if
		} //end if
		//
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷發射ing
	{
		
		if( m_fTimer_BreakTime<0.0f )
		{
			float l_fTargetAngle = (floor((m_fTimer_RunnerTotalAngle-0.1f)/30.0f) + 1) * 30.0f;
			int l_nDeltaAngle = floor(l_fTargetAngle - m_fTimer_RunnerTotalAngle) + 1;
			m_fTimer_RunnerTotalAngle = l_fTargetAngle;
		}
		else
		{
			m_fTimer_BreakTime -= e_fElpaseTime;
			m_fTimer_RunnerTotalAngle += e_fElpaseTime*m_fTimer_RunnerVelocity;
			//
			m_fTimer_RunnerVelocity -= e_fElpaseTime*m_fTimer_BrakeVelocity;
			if( m_fTimer_RunnerVelocity<20.0f )
			{
				m_fTimer_RunnerVelocity = 20.0f;
			} //end if
		} //end if

		float l_fAngle = m_fTimer_RunnerTotalAngle;

		//
		cMultiPathDynamicImage* l_pMPDI_WeaponGetMessage=0;
		l_pMPDI_WeaponGetMessage = m_pMPDI_WeaponGetMessage[2];
		int	l_iCount = l_pMPDI_WeaponGetMessage->Count();
		for( int ii=0; ii<l_iCount; ++ii )
		{
			l_pMPDI_WeaponGetMessage->GetObject(ii)->SetExtractTransform( &m_matTube );
			if( ii==0 )
			{
				l_pMPDI_WeaponGetMessage->GetObject(ii)->SetAngle( Vector3(0.0f, 0.0f, l_fAngle) );
			} //end if
		} //next ii
		//
		l_pMPDI_WeaponGetMessage->Update( e_fElpaseTime );
		//
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷動畫
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
void	cMiniGameStage32::Render(void)
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
void	cMiniGameStage32::Render2(void)
{
	cMiniGameBase::Render2();
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING )
	{
		if( m_pMPDI_WeaponGetMessage )
		{
			m_pMPDI_WeaponGetMessage[0]->Render();
		} //end if
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //雷公待命這個動作
	{
		if( m_pMPDI_WeaponGetMessage )
		{
			if( m_pMPDI_WeaponGetMessage[1]->IsDone()==true )
			{
				m_pMPDI_WeaponGetMessage[2]->Render();
			}
			else
			{
				m_pMPDI_WeaponGetMessage[1]->Render();
			} //end if
		} //end if
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷這個動作
	{
		//下排 0 度，右邊的 270 度，上邊 180 度，左邊 90 度。
		//Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		//Vector3 l_PlayerPosition = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerPosition();
		//

		if( m_pMPDI_WeaponGetMessage )
		{
			m_pMPDI_WeaponGetMessage[2]->Render();
		} //end if

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
void	cMiniGameStage32::Destroy(void)
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

