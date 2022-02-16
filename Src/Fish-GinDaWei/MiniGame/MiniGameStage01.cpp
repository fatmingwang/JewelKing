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
DESCRIPTION   : 旋風彈
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameStage01::cMiniGameStage01(void):cMiniGameBase(MINI_GAME_STAGE_01)
{
	m_nPayRate = 2; //賠率
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameStage01::~cMiniGameStage01(void)
{
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage01::Init(void)
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
void	cMiniGameStage01::Update( float e_fElpaseTime )
{
	cMiniGameBase::Update(e_fElpaseTime);
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷發射
	{
		if( m_nReslut==1 )
		{
			m_pBulletLatest[0] = ((cPlayerData*)m_pWhichPlayer)->f_Fire_MiniGame( this, -999 );
			if( m_pBulletLatest[0] ) //取得子彈
			{
				f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED );
			} //end if
		} //end if
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //雷公打出來的雷
	{
		//掃過的碰撞到的魚，要判斷會不會死
		for( int kk=0; kk<m_nCount_Fire01_Current; kk++ )
		{
			//子彈黏在魚的身上
			//m_matPlayerTonado[kk] = cMatrix44::TranslationMatrix( m_pBulletLatest[kk]->f_GetBulletMPDI()->GetPosByImageCenter() ) * m_matPlayerFace;
			m_matPlayerTonado[kk] = m_pBulletLatest[kk]->f_GetBulletMatrix();
			for( int ii=0; ii<m_pMPDI_Fire01[kk]->Count(); ++ii )
			{
				m_pMPDI_Fire01[kk]->GetObject(ii)->SetExtractTransform( &m_matPlayerTonado[kk] );
			} //next ii
			m_pMPDI_Fire01[kk]->Update(e_fElpaseTime);
		} //next kk
		//
		//掃過的碰撞到的魚，要判斷會不會死
		int l_nCount1 = m_vMonsterDiedList.size();
		int l_nGotScore = cFishApp::m_pBulletManager->f_Collide_Bullet_MiniGame( cFishApp::m_spMonsterManager, m_pBulletLatest[0], &m_vMonsterDiedList, m_pWhichPlayer );
		m_nTotalGotScore += l_nGotScore;
		int l_nCount2 = m_vMonsterDiedList.size();
		if( l_nCount2>=COUNT_MAX_FISH_EXPLODE )
		{
			l_nCount2 = COUNT_MAX_FISH_EXPLODE - 1;
		} //end if
		int l_nCountDiff = l_nCount2 - l_nCount1; //這次新增殺死了多少魚？
		if( l_nCountDiff>0 )
		{
			m_nCurrentFishExplodeIndex += l_nCountDiff;
			for( int ii=l_nCount1; ii<l_nCount2; ii++ )
			{
				m_pMPDI_FishExplode[ii]->SetTransform( (m_vMonsterDiedList)[ii]->GetTransform() );
				m_pMPDI_FishExplode[ii]->Init();
			} //next ii
		} //end if
		//
		for( int ii=0; ii<m_nCurrentFishExplodeIndex; ii++ )
		{
			m_pMPDI_FishExplode[ii]->Update(e_fElpaseTime);
		} //next ii
		//
		//魚群
		/*
		int	l_nCountMonsterAlive = cFishApp::m_spMonsterManager->f_GetCountMonsterAlive();
		cMonster** l_ppWorkingMonster = cFishApp::m_spMonsterManager->f_GetWorkingMonster();
		int l_nTotalGotScore = 0;
		for( int ii=0; ii<l_nCountMonsterAlive; ii++ )
		{
			cMonster* l_pCurrentMonster = (*(l_ppWorkingMonster + ii));
			bool l_bResult = cFishApp::m_spFishProbability->MonsterDied( l_pCurrentMonster, m_nPayRate, m_pWhichPlayer );
			if( l_bResult )
			{
			} //end if
		} //next ii
		//得分
		m_nTotalGotScore = l_nTotalGotScore;
		*/
		//
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
void	cMiniGameStage01::Render(void)
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
void	cMiniGameStage01::Render2(void)
{
	cMiniGameBase::Render2();
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷發射
	{
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //雷公打出來的雷
	{
		for( int kk=0; kk<m_nCount_Fire01_Current; kk++ )
		{
			m_pMPDI_Fire01[kk]->Render();
		} //next kk
		//
		for( int ii=0; ii<m_nCurrentFishExplodeIndex; ii++ )
		{
			m_pMPDI_FishExplode[ii]->Render();
		} //next ii
		//
		//雷公打雷最後一幕
//		m_pMPDI_WeaponFire[0]->Render();
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage01::Destroy(void)
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