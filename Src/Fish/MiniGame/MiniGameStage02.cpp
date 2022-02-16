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
#include "../EngineTest/FileNameDefine.h"

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 落雷彈
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameStage02::cMiniGameStage02(void):cMiniGameBase(MINI_GAME_STAGE_02)
{
	m_nPayRate = 1;//1; //賠率
	m_pMPDI_Fire_Special = 0;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameStage02::~cMiniGameStage02(void)
{
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage02::Init(void)
{
	cMiniGameBase::Init();
	//
	cMPDIList*	l_pMPDIList_Bonus=0;
	l_pMPDIList_Bonus = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BONUS_THUNDER_BOMB );
	m_pMPDI_Fire_Special = new cMPDI( l_pMPDIList_Bonus->GetObject(L"LightningBolt") );
	m_pMPDI_Fire_Special->Init();
	m_pMPDI_flash_light_t1->Init();
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage02::Update( float e_fElpaseTime )
{
	cMiniGameBase::Update(e_fElpaseTime);
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷發射
	{
		m_pMPDI_Fire_Special->Update(e_fElpaseTime);
		//
		if( m_nReslut==1 )
		{
			//
			int l_nScore = 0;
			int l_nCount1 = m_vMonsterDiedList.size();
/*
			if( m_pStickyMonster )
			{
				//總共要抓這麼多魚
				int	l_nCountMonsterAlive = cFishApp::m_spMonsterManager->f_GetCountMonsterAlive();
				//魚群
				cMonster** l_ppWorkingMonster = cFishApp::m_spMonsterManager->f_GetWorkingMonster();
				//
				m_pStickyMonster->m_iID
			}
			else
			{
				*/
				cFishApp::m_spFishProbability->SameTypeMonsterToDiedByProbability( m_nPayRate*m_nPayRateBullet, &m_vMonsterDiedList, m_pWhichPlayer, &l_nScore );
			//} //end if
			


			int l_nCount2 = m_vMonsterDiedList.size();
			if( l_nCount2>=COUNT_MAX_FISH_EXPLODE )
			{
				l_nCount2 = COUNT_MAX_FISH_EXPLODE - 1;
			} //end if
			m_nTotalGotScore += l_nScore;
			int l_nCountDiff = l_nCount2 - l_nCount1; //這次新增殺死了多少魚？
			if( l_nCountDiff>0 )
			{
				m_nCurrentFishExplodeIndex += l_nCountDiff;
				for( int ii=l_nCount1; ii<l_nCount2; ii++ )
				{
					cMatrix44 abc = (m_vMonsterDiedList)[ii]->GetTransform();
					m_pMPDI_FishExplode[ii]->SetTransform( (m_vMonsterDiedList)[ii]->GetTransform() );
					m_pMPDI_FishExplode[ii]->Init();
				} //end if
			}
			else
			{
				l_nCountDiff = l_nCountDiff;
			} //end if
			//
			m_nCount_Fire01_Current = l_nCount2;
			if( m_nCount_Fire01_Current>m_nCount_Fire01 )
			{
				m_nCount_Fire01_Current = m_nCount_Fire01;
			} //end if
			for( int kk=0; kk<m_nCount_Fire01_Current; kk++ )
			{
				float l_fAngle = 9 + 18*kk + (360 - ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation().z);
				if( m_nCount_Fire01_Current>0 )
				{
					cMonster* l_pCurrentMonster = (m_vMonsterDiedList)[kk];
					//找到魚現在相對火炮的角度
					Vector3	l_vPos_Tube = m_matTube.m[3];
					//魚的位置
					Vector3 l_vPos_Monster = l_pCurrentMonster->GetTransform().m[3];
					//方向
					float	l_fAngleZ;
					if( ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation().z==0 )
					{
						l_fAngleZ = UT::GetAngleBy2Point( l_vPos_Monster.x, l_vPos_Monster.y, l_vPos_Tube.x, l_vPos_Tube.y ) 
									+ 90;
					}
					else if( ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation().z==90 )
					{
						l_fAngleZ = UT::GetAngleBy2Point( l_vPos_Monster.x, l_vPos_Monster.y, l_vPos_Tube.x, l_vPos_Tube.y ) 
									+ 90;
					}
					else if( ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation().z==180 )
					{
						l_fAngleZ = UT::GetAngleBy2Point( l_vPos_Monster.x, l_vPos_Monster.y, l_vPos_Tube.x, l_vPos_Tube.y ) 
									+ 90;
					}
					else //if( ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation().z==270 )
					{
						l_fAngleZ = UT::GetAngleBy2Point( l_vPos_Monster.x, l_vPos_Monster.y, l_vPos_Tube.x, l_vPos_Tube.y ) 
									+ 90;
					} //end if
					l_fAngle = l_fAngleZ;
				} //end if
				m_pBulletLatest[kk] = ((cPlayerData*)m_pWhichPlayer)->f_Fire_MiniGame( this, l_fAngle );
			} //next kk

			//
			f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ); //要寫到最後，因為 SetFlag 會在 Update 一次
			//
		} //end if
		//
		for( int kk=0; kk<m_nCount_Fire01_Current; kk++ )
		{
			if( m_pBulletLatest[kk] )
			{
				//m_matPlayerTonado[kk] = m_pBulletLatest[kk]->f_GetBulletMPDI()->GetTransform();
				//for( int ii=0; ii<m_pMPDI_Fire01[kk]->Count(); ++ii )
				//{
				//	m_pMPDI_Fire01[kk]->GetObject(ii)->SetExtractTransform( &m_matPlayerTonado[kk] );
				//} //next ii
				//m_pMPDI_Fire01[kk]->SetPos( m_pBulletLatest[kk]->f_GetBulletPos() );
				//cMatrix44 m4 = m_pBulletLatest[kk]->f_GetBulletMatrix();
				//Vector3 v3 = m_pBulletLatest[kk]->f_GetBulletAngle();
				//m_pMPDI_Fire01[kk]->SetAngle( m_pBulletLatest[kk]->f_GetBulletAngle() );
				//m_pMPDI_Fire01[kk]->SetTransform( m4 );
				if( m_pBulletLatest[kk] )
				{
					Vector3 l_vPos = m_pBulletLatest[kk]->f_GetBulletPos();
					if( l_vPos.Length()>0 )
					{
						m_matPlayerTonado[kk] = m_pBulletLatest[kk]->f_GetBulletMatrix();
						//m_pMPDI_Fire01[kk]->SetPos( m_pBulletLatest[kk]->f_GetBulletPos() );
						//Vector3 l_vAng = m_pBulletLatest[kk]->f_GetBulletAngle();
						//m_pMPDI_Fire01[kk]->SetAngle( m_pBulletLatest[kk]->f_GetBulletAngle() );
						for( int pp=0; pp<m_pMPDI_Fire01[kk]->Count(); pp++ )
						{
							m_pMPDI_Fire01[kk]->SetExtraTransform( &m_matPlayerTonado[kk] );
						} //next pp
					} //end if
				} //end if
				m_pMPDI_Fire01[kk]->Update(e_fElpaseTime);
			} //end if
		} //next kk
		//
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //雷公打出來的雷
	{		for( int kk=0; kk<m_nCount_Fire01_Current; kk++ )
		{
			if( m_pBulletLatest[kk] )
			{
				//m_matPlayerTonado[kk] = m_pBulletLatest[kk]->f_GetBulletMPDI()->GetTransform();
				//for( int ii=0; ii<m_pMPDI_Fire01[kk]->Count(); ++ii )
				//{
				//	m_pMPDI_Fire01[kk]->GetObject(ii)->SetExtractTransform( &m_matPlayerTonado[kk] );
				//} //next ii
				//m_pMPDI_Fire01[kk]->SetPos( m_pBulletLatest[kk]->f_GetBulletPos() );
				//cMatrix44 m4 = m_pBulletLatest[kk]->f_GetBulletMatrix();
				//Vector3 v3 = m_pBulletLatest[kk]->f_GetBulletAngle();
				//m_pMPDI_Fire01[kk]->SetAngle( m_pBulletLatest[kk]->f_GetBulletAngle() );
				//m_pMPDI_Fire01[kk]->SetTransform( m4 );
				if( m_pBulletLatest[kk] )
				{
					Vector3 l_vPos = m_pBulletLatest[kk]->f_GetBulletPos();
					if( l_vPos.Length()>0 )
					{
						m_matPlayerTonado[kk] = m_pBulletLatest[kk]->f_GetBulletMatrix();
						//m_pMPDI_Fire01[kk]->SetPos( m_pBulletLatest[kk]->f_GetBulletPos() );
						//Vector3 l_vAng = m_pBulletLatest[kk]->f_GetBulletAngle();
						//m_pMPDI_Fire01[kk]->SetAngle( m_pBulletLatest[kk]->f_GetBulletAngle() );
						for( int pp=0; pp<m_pMPDI_Fire01[kk]->Count(); pp++ )
						{
							m_pMPDI_Fire01[kk]->SetExtraTransform( &m_matPlayerTonado[kk] );
						} //next pp
					} //end if
				} //end if
				m_pMPDI_Fire01[kk]->Update(e_fElpaseTime);
			} //end if
		} //next kk
		
		for( int ii=0; ii<m_nCurrentFishExplodeIndex; ii++ )
		{
			m_pMPDI_FishExplode[ii]->Update(e_fElpaseTime);
		} //next ii
		//
		m_pMPDI_Fire_Special->Update(e_fElpaseTime);
		//
		m_pMPDI_flash_light_t1->Update(e_fElpaseTime); //白光
		//
	} //end if
	
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage02::Render(void)
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
void	cMiniGameStage02::Render2(void)
{
	cMiniGameBase::Render2();
	//
	int l_nFlag =  f_GetFlag();
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷發射
	{
		m_pMPDI_Fire_Special->Render();
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
		m_pMPDI_WeaponFire[0]->Render();
		//
		m_pMPDI_Fire_Special->Render();
		//
		m_pMPDI_flash_light_t1->Render(); //白光
		//
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameStage02::Destroy(void)
{
	cMiniGameBase::Destroy();

	SAFE_DELETE( m_pMPDI_Fire_Special );
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cMiniGameStage02::f_SetFlag( eMiniGameFlag e_nFlag )
{
	if( e_nFlag==f_GetFlag() )
	{
		return;
	} //end if
	//
	switch( e_nFlag )
	{
	case MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING:

		for( int pp=0; pp<m_pMPDI_Fire_Special->Count(); pp++ )
		{
			m_pMPDI_Fire_Special->SetExtraTransform( &m_matTube );
		} //next pp
		m_pMPDI_Fire_Special->Init();

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