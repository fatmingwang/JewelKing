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
cMiniGameStage03::cMiniGameStage03(void):cMiniGameBase(MINI_GAME_STAGE_03)
{
	m_nPayRate = 3; //賠率
	m_nFreezeStep = ENUM_STEP_FREEZE_MAX;
	m_nCurrentFishIndex = 0; // 從 0 開始
	m_nCurrentIceLaserIndex = 1; // ++ 之後會從 2 開始
	m_pCurrentMonster = 0;
	m_fSpeed_01 = 1200.0f;
	m_fDistanceIceLaser = 0.0f;
	m_fSpeed_02 = 900.0f;
	//
	for( int ii=0; ii<COUNT_MAXIUM_ICE_FISH; ii++ )
	{
		m_pMPDI_IceBallOpen[ii] = 0;
		m_pMPDI_IceBall[ii] = 0;
		m_pMPDI_IceBomb[ii] = 0;
		m_pMPDI_CamboDisc[ii] = 0;
		m_pMPDI_CamboDiscSingle[ii] = 0;
	} //next ii
	m_pMPDI_IceBreak = 0;
	//
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cMiniGameStage03::~cMiniGameStage03(void)
{
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMiniGameStage03::Init(void)
{
	cMiniGameBase::Init();
	//
/*
	cMPDIList*	l_pMPDIList_Bonus=0;
	l_pMPDIList_Bonus = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BONUS_ICE_BOMB );
	//
	for( int ii=0; ii<COUNT_MAXIUM_ICE_FISH; ii++ )
	{
		m_pMPDI_IceBallOpen[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject( L"IceBallOpen" ) );
		m_pMPDI_IceBallOpen[ii]->Init();
		m_pMPDI_IceBall[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject( L"IceBall" ) );
		m_pMPDI_IceBall[ii]->Init();
		m_pMPDI_IceBomb[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject( L"IceBomb" ) );
		m_pMPDI_IceBomb[ii]->Init();
		m_pMPDI_CamboDisc[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject( L"CamboDisc" ) );
		m_pMPDI_CamboDisc[ii]->Init();
		m_pMPDI_CamboDiscSingle[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject( L"CamboDisc2" ) );
		m_pMPDI_CamboDiscSingle[ii]->Init();
	} //next ii
	m_pMPDI_IceBreak = new cMPDI( l_pMPDIList_Bonus->GetObject( L"IceBreak" ) );
	m_pMPDI_IceBreak->Init();
*/
	//
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMiniGameStage03::Update( float e_fElpaseTime )
{
	cMiniGameBase::Update(e_fElpaseTime);
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //雷公等待打雷
	{	
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷動畫
	{
		//總共可以冰凍幾隻魚？盡量避開帶有 Mini Game 的魚
		cMonster** l_ppWorkingMonster = cFishApp::m_spMonsterManager->f_GetWorkingMonster();
		int	l_nCountMonsterAlive = cFishApp::m_spMonsterManager->f_GetCountMonsterAlive();

		//將準備要冰凍住的魚，停止不動
		for( int ii=0; ii<l_nCountMonsterAlive; ii++ )
		{
//			(*(l_ppWorkingMonster+ii))->SetMonsterStatus( eMS_CONTROL_BY_MINI_GAME );
		} //next ii


		//
		if( m_nReslut==1 ) //打雷完畢，進入發射階段
		{
			f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED );
			//
			//這裡就不發射子彈，自己抓魚。
			m_nFreezeStep = ENUM_STEP_FREEZE_START_TO_FIRE;
		} //end if
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //雷公打出來的雷
	{
		int l_nResult = f_Update_FreezeMonster( e_fElpaseTime );

		if( l_nResult==1 ) //冷凍完畢
		{
			f_SetFlag( MINI_GAME_FLAG_GOTO_FINISH_01 );
		} //en dif
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
void	cMiniGameStage03::Render(void)
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
void	cMiniGameStage03::Render2(void)
{
	cMiniGameBase::Render2();
	//
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //雷公打雷發射
	{
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //雷公打出來的雷
	{
		f_Render_FreezeMonster();

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
void	cMiniGameStage03::Destroy(void)
{
	for( int ii=0; ii<COUNT_MAXIUM_ICE_FISH; ii++ )
	{
		SAFE_DELETE( m_pMPDI_IceBallOpen[ii] );
		SAFE_DELETE( m_pMPDI_IceBall[ii] );
		SAFE_DELETE( m_pMPDI_IceBomb[ii] );
		SAFE_DELETE( m_pMPDI_CamboDisc[ii] );
		SAFE_DELETE( m_pMPDI_CamboDiscSingle[ii] );
	} //next ii
	SAFE_DELETE( m_pMPDI_IceBreak );
	//
	cMiniGameBase::Destroy();
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
int	cMiniGameStage03::f_Update_FreezeMonster( float e_fElpaseTime )
{
	//開始一隻一隻抓回來
	//總共要抓這麼多魚
	int	l_nCountMonsterAlive = cFishApp::m_spMonsterManager->f_GetCountMonsterAlive();
	//魚群
	cMonster** l_ppWorkingMonster = cFishApp::m_spMonsterManager->f_GetWorkingMonster();
	//
	switch( m_nFreezeStep )
	{
	case ENUM_STEP_FREEZE_START_TO_FIRE:
		//
		m_fTotalMovingTime = 0.0f;
		//
		//進行到第幾隻魚？
		//if( m_nCurrentFishIndex<l_nCountMonsterAlive )
		{
			//把魚抓出來
			//m_pCurrentMonster = (*(l_ppWorkingMonster+m_nCurrentFishIndex));
			if( m_nCurrentFishIndex==0 )
			{
				//float l_nPosX = -10.0f;
				//float l_nPosY = -10.0f;
				////int l_nTimes=0;
				//while( l_nTimes<1000 &&
				//	   ( l_nPosX<0 || l_nPosX>cFishApp::m_svGameResolution.x || 
				//	     l_nPosY<0 || l_nPosY>cFishApp::m_svGameResolution.y) )
				//{
				//	l_nTimes++;
					int l_nScore = 0;
					m_pCurrentMonster = cFishApp::m_spFishProbability->GetConsecutiveMonsterToDied( m_nPayRate*m_nPayRateBullet, m_nCurrentFishIndex, m_pWhichPlayer, &l_nScore );
//					m_nTotalGotScore += l_nScore;
				//	if( m_pCurrentMonster )
				//	{
				//		l_nPosX = Vector3(m_pCurrentMonster->GetTransform().m[3]).x;
				//		l_nPosY = Vector3(m_pCurrentMonster->GetTransform().m[3]).y;
				//	} //end if
				//} //end while
				
				

				m_pCurrentMonster->SetMonsterStatus( eMS_CONTROL_BY_MINI_GAME ); //冰凍住
			} //end if
			m_pMonsters[m_nCurrentFishIndex] = m_pCurrentMonster;
			//魚的位置
			Vector3 l_vPos_Monster = m_pCurrentMonster->GetTransform().m[3];


			//火砲從哪裡出發？m_nCurrentIceLaserIndex=0的時候先暫定從砲台出發，之後就是從上一個 m_nCurrentIceLaserIndex 出發
			Vector3 l_vHome;
			if( m_nCurrentFishIndex==0 )
			{
				l_vHome = m_matTube.m[3];
			}
			else
			{
				l_vHome = m_pMPDI_IceBall[m_nCurrentFishIndex-1]->GetTransform().m[3];
			} //end if
			//冰柱
			for( int kk=INDEX_START_ICE_LASER; kk<m_nCount_Fire01_Current; kk++ )
			{
				m_pMPDI_Fire01[kk]->SetPos(l_vHome);
				m_pMPDI_Fire01[kk]->Update( e_fElpaseTime );
			} //next kk
			//冰柱頭
			m_pMPDI_Fire01[0]->SetPos(l_vHome);
			m_pMPDI_Fire01[0]->Update( e_fElpaseTime );
			m_pMPDI_Fire01[1]->SetPos(l_vHome);
			m_pMPDI_Fire01[1]->Update( e_fElpaseTime );
			m_pMPDI_Fire01[2]->SetPos(l_vHome);
			m_pMPDI_Fire01[2]->Update( e_fElpaseTime );
			//
			for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
			{
				//冷字
				m_pMPDI_IceBall[ii]->Update( e_fElpaseTime );
				//第幾個？
				if( ii+1<10 )
				{
					m_pMPDI_CamboDiscSingle[ii]->Update( e_fElpaseTime );
				}
				else
				{
					m_pMPDI_CamboDisc[ii]->Update( e_fElpaseTime );
				} //end if
				
			} //next ii
			//
//			m_pMPDI_WeaponWait[0]->Init();
//			m_pMPDI_WeaponWait[0]->SetTransform( m_pMPDI_WeaponFire[0]->GetTransform() );
//			m_pMPDI_WeaponWait[0]->Update(e_fElpaseTime);

			m_nFreezeStep = ENUM_STEP_FREEZE_MOVING_FOWARD;
			//
			cGameApp::SoundPlay(L"19",true);
			//
		} //end if
		break;
	case ENUM_STEP_FREEZE_MOVING_FOWARD:
		{
		//魚的位置
		Vector3 l_vPos_Monster = m_pCurrentMonster->GetTransform().m[3];


		//火砲移動到魚
		//找到魚現在相對火炮的角度
		Vector3	l_vPos_Tube = m_matTube.m[3];
		if( m_nCurrentFishIndex==0 )
		{
			l_vPos_Tube = m_matTube.m[3];
		}
		else
		{
			l_vPos_Tube = m_pMPDI_IceBall[m_nCurrentFishIndex-1]->GetTransform().m[3];
		} //end if

	
		//如果魚在火砲的後面，要把她拉到火砲的前面，否則會看不到魚
		Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		//下排 0 度，右邊的 270 度，上邊 180 度，左邊 90 度。
		if( l_PlayerRotation.z==0 ) //下方
		{
			if( l_vPos_Monster.y>cFishApp::m_svGameResolution.y )
			{
				l_vPos_Tube = l_vPos_Tube - Vector3( 0, 40, 0 );
			} //end if
		}
		else if( l_PlayerRotation.z==90 ) //左邊
		{
			if( l_vPos_Monster.x<0 )
			{
				l_vPos_Tube = l_vPos_Tube + Vector3( 40, 0, 0 );
			} //end if
		}
		else if( l_PlayerRotation.z==180 ) //上方
		{
			if( l_vPos_Monster.y<0 )
			{
				l_vPos_Tube = l_vPos_Tube + Vector3( 0, 40, 0 );
			} //end if
		}
		else if( l_PlayerRotation.z==270 ) //右邊
		{
			if( l_vPos_Monster.x>cFishApp::m_svGameResolution.x )
			{
				l_vPos_Tube = l_vPos_Tube - Vector3( 40, 0, 0 );
			} //end if
		} //end if



		m_fTotalMovingTime += e_fElpaseTime; //移動經過了這麼多時間
		//相對位置
		Vector3 l_vPos_Length = l_vPos_Monster - l_vPos_Tube;
		float l_fLengthTimer = l_vPos_Length.Length()/m_fSpeed_01;


		//
		//火砲頭現在的位置
		Vector3	l_vPos_Head = m_pMPDI_Fire01[0]->GetTransform().m[3];
		
		//相對位置
		Vector3 l_vPos_Shift = l_vPos_Monster - l_vPos_Head;
		//必須要把 shift 正規化，不然會距離越遠跑越快
		//float l_fLengthX = l_vPos_Shift.x * l_vPos_Shift.x;
		//float l_fLengthY = l_vPos_Shift.y * l_vPos_Shift.y;
		//float l_fLengthZ = l_vPos_Shift.z * l_vPos_Shift.z;

		//判斷跑到了沒？
//		float abc = l_vPos_Shift.Length();
//		if( l_vPos_Shift.Length() > 20.0f )
		if( m_fTotalMovingTime<l_fLengthTimer )
		{
			l_vPos_Shift.NormalizeIt();
			//準備要移動多少
			Vector3	l_vMovedDistance = l_vPos_Shift * e_fElpaseTime * m_fSpeed_01;
			//移動
			//l_vPos_Head += l_vMovedDistance;

			float l_fRatio3 = m_fTotalMovingTime/l_fLengthTimer;
			if( l_fRatio3>1.0f )
			{
				l_fRatio3 = 1.0f;
			}
			else if( l_fRatio3<-1.0f )
			{
				l_fRatio3 = -1.0f;
			} //end if
			l_vPos_Head = l_vPos_Tube + l_vPos_Length * l_fRatio3;


			m_pMPDI_Fire01[0]->SetPos( l_vPos_Head ); //把火砲頭移過去
			m_pMPDI_Fire01[1]->SetPos( l_vPos_Head ); //把火砲頭移過去
			if( m_nCurrentIceLaserIndex== 0 )
			{
				m_pMPDI_Fire01[2]->SetPos( l_vPos_Head ); //把火砲頭移過去
			} //end if
			//
			//方向
			float	l_fAngleZ = 180 - UT::GetAngleBy2Point( l_vPos_Monster.x, l_vPos_Monster.y, l_vPos_Tube.x, l_vPos_Tube.y ) - 180;
			Vector3 l_vDirection = Vector3( 0, 0, l_fAngleZ );

			//float	l_fAngleZ = 180.f - UT::GetAngleBy2Point( l_vPos_Monster.x, l_vPos_Monster.y, l_vPos_Head.x, l_vPos_Head.y );
			//float l_fAngleZ = atan( l_vPos_Shift.x/l_vPos_Shift.y );
			//Vector3 l_vDirection = Vector3( 0, 0, l_fAngleZ * (180.0f/3.1415967f) - 90.0f );
			
			//火砲頭方向
			m_pMPDI_Fire01[0]->SetRotateAroundOriginlPoint(true);
			m_pMPDI_Fire01[0]->SetAngle( l_vDirection );
			m_pMPDI_Fire01[1]->SetRotateAroundOriginlPoint(true);
			m_pMPDI_Fire01[1]->SetAngle( l_vDirection );
			if( m_nCurrentIceLaserIndex== 0 )
			{
				m_pMPDI_Fire01[2]->SetAngle( l_vDirection + 180 ); //把火砲頭移過去
			} //end if
			//
			//m_matPlayerTonado[0] = cMatrix44::TranslationMatrix( l_vPos_Head ) * cMatrix44::RotationMatrix( l_vDirection );
			//for( int ii=0; ii<m_pMPDI_Fire01[0]->Count(); ++ii )
			//{
			//	m_pMPDI_Fire01[0]->GetObject(ii)->SetExtractTransform( &(m_matPlayerTonado[0]) );
			//} //next ii
			//
			//冰柱 --- BEGIN
			m_fDistanceIceLaser += l_vMovedDistance.Length();
			float l_fIceHeight2 = 19.5f;
			if( m_fDistanceIceLaser > l_fIceHeight2 )
			{
				int l_nCount2 = (int)(m_fDistanceIceLaser/l_fIceHeight2);
				for( int ii=0; ii<l_nCount2; ii++ )
				{
					m_fDistanceIceLaser -= l_fIceHeight2;
					//
					m_nCurrentIceLaserIndex++;
					if( m_nCurrentIceLaserIndex>=m_nCount_Fire01_Current )
					{
						m_nCurrentIceLaserIndex = m_nCount_Fire01_Current - 1;
					} //end if
					if( m_nCurrentIceLaserIndex>0 )
					{
						m_pMPDI_Fire01[m_nCurrentIceLaserIndex]->SetPos( Vector3(m_pMPDI_Fire01[m_nCurrentIceLaserIndex-1]->GetPos().x + sin(l_fAngleZ*3.14f/180.0f) * l_fIceHeight2, m_pMPDI_Fire01[m_nCurrentIceLaserIndex-1]->GetPos().y - cos(l_fAngleZ*3.14f/180.0f) * l_fIceHeight2, m_pMPDI_Fire01[m_nCurrentIceLaserIndex-1]->GetPos().z ) );
					}
					else
					{
						m_pMPDI_Fire01[m_nCurrentIceLaserIndex]->SetPos( l_vPos_Head );
					} //end if
//					m_pMPDI_Fire01[0]->SetPos( m_pMPDI_Fire01[m_nCurrentIceLaserIndex]->GetPos() );
					m_pMPDI_Fire01[m_nCurrentIceLaserIndex]->SetAngle( l_vDirection );
				} //next ii
			} //end if
			//冰柱 --- END
		}
		else
		{
			m_nFreezeStep = ENUM_STEP_FREEZE_ARRIVED_FISH;
			m_fDistanceIceLaser = 0; //歸零
			m_fTotalMovingTime = 0.0f;
			m_pMPDI_IceBallOpen[m_nCurrentFishIndex]->SetTransform( m_pMPDI_Fire01[0]->GetTransform() );
			Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
			m_pMPDI_IceBallOpen[m_nCurrentFishIndex]->SetAngle( l_PlayerRotation );
			//
			cGameApp::SoundPlay(L"20",true);
			//
		} //end if
		//
		//雷公待命
//		m_pMPDI_WeaponWait[0]->Update( e_fElpaseTime );
		//
		//冰柱
		for( int kk=INDEX_START_ICE_LASER; kk<m_nCount_Fire01_Current; kk++ )
		{
//			m_pMPDI_Fire01[kk]->Update( e_fElpaseTime );
		} //next kk
		//冰柱頭
//		m_pMPDI_Fire01[0]->Update( e_fElpaseTime );
//		m_pMPDI_Fire01[1]->Update( e_fElpaseTime );
//		m_pMPDI_Fire01[2]->Update( e_fElpaseTime );
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Update( e_fElpaseTime );
			//第幾個？
			if( ii+1<10 )
			{
				m_pMPDI_CamboDiscSingle[ii]->Update( e_fElpaseTime );
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Update( e_fElpaseTime );
			} //end if
			
		} //next ii
		//
		}
		break;
	case ENUM_STEP_FREEZE_ARRIVED_FISH:
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			m_pMPDI_IceBallOpen[ii]->Update( e_fElpaseTime );
		} //next ii
		if( m_pMPDI_IceBallOpen[m_nCurrentFishIndex]->IsDone() )
		{
			m_nFreezeStep = ENUM_STEP_FREEZE_WAIT_FOR_SHOWING_WORD;
			//
			m_vCurrentFish = m_pMPDI_IceBallOpen[m_nCurrentFishIndex]->GetPos();
			m_pMPDI_IceBall[m_nCurrentFishIndex]->SetTransform( m_pMPDI_IceBallOpen[m_nCurrentFishIndex]->GetTransform() );
			Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
			m_pMPDI_IceBall[m_nCurrentFishIndex]->SetAngle( l_PlayerRotation );
			//
			if(m_nCurrentFishIndex<10)
			{
				m_pMPDI_CamboDiscSingle[m_nCurrentFishIndex]->SetTransform( m_pMPDI_IceBallOpen[m_nCurrentFishIndex]->GetTransform() );
			}
			else
			{
				m_pMPDI_CamboDisc[m_nCurrentFishIndex]->SetTransform( m_pMPDI_IceBallOpen[m_nCurrentFishIndex]->GetTransform() );
			} //end if
			//
			m_fTimer_IceBall_01 = 1;  //暫定 1 秒
		} //end if
		//
		//雷公待命
//		m_pMPDI_WeaponWait[0]->Update( e_fElpaseTime );
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Update( e_fElpaseTime );
			//第幾個？
			if( ii+1<10 )
			{
				m_pMPDI_CamboDiscSingle[ii]->Update( e_fElpaseTime );
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Update( e_fElpaseTime );
			} //end if
		} //next ii
		//
		//冰柱
		for( int kk=2; kk<m_nCount_Fire01_Current; kk++ )
		{
			m_pMPDI_Fire01[kk]->Update( e_fElpaseTime );
		} //next kk
		break;
	case ENUM_STEP_FREEZE_WAIT_FOR_SHOWING_WORD:
		//雷公待命
//		m_pMPDI_WeaponWait[0]->Update( e_fElpaseTime );
		//冰柱
		for( int kk=2; kk<m_nCount_Fire01_Current; kk++ )
		{
			m_pMPDI_Fire01[kk]->Update( e_fElpaseTime );
		} //next kk
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Update( e_fElpaseTime );
			//第幾個？
			if( ii+1<10 )
			{
				m_pMPDI_CamboDiscSingle[ii]->Update( e_fElpaseTime );
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Update( e_fElpaseTime );
			} //end if
		} //next ii
		//
		m_fTimer_IceBall_01 -= e_fElpaseTime;
		if( m_fTimer_IceBall_01 < 0.0f )
		{
			m_nFreezeStep = ENUM_STEP_FREEZE_PULL_FISH_BACKWARD;
		} //end if
		//
		break;
	case ENUM_STEP_FREEZE_PULL_FISH_BACKWARD: //把魚拉回砲台
		{
		//砲台位置
		Vector3	l_vPos_Tube = m_matTube.m[3];
		if( m_nCurrentFishIndex==0 )
		{
			l_vPos_Tube = m_matTube.m[3];
		}
		else
		{
			l_vPos_Tube = m_pMPDI_IceBall[m_nCurrentFishIndex-1]->GetTransform().m[3];
		} //end if
		//
		//魚的位置
		//Vector3 l_vPos_Monster = m_pCurrentMonster->GetTransform().m[3];
		//Vector3 l_vPos_Monster = m_pCurrentMonster->GetPos();
		Vector3 l_vPos_Monster = m_vCurrentFish;
		//相對位置
		Vector3 l_vPos_Shift = l_vPos_Tube - l_vPos_Monster;
		//必須要把 shift 正規化，不然會距離越遠跑越快
		float l_fLengthX = l_vPos_Shift.x * l_vPos_Shift.x;
		float l_fLengthY = l_vPos_Shift.y * l_vPos_Shift.y;
		float l_fLengthZ = l_vPos_Shift.z * l_vPos_Shift.z;

		m_fTotalMovingTime += e_fElpaseTime; //移動經過了這麼多時間
		//相對位置
		Vector3 l_vPos_Length = l_vPos_Tube - l_vPos_Monster;
		float l_fLengthTimer = l_vPos_Length.Length()/m_fSpeed_02;



		//判斷跑到了沒？
		float l_fTimer01 = 0.1f;
		float l_fStopLength = 160.0f;
		if( m_nCurrentFishIndex==0 )
		{
			l_fTimer01 = 0.2f;
		}
		else
		{
			l_fStopLength = 110.0f;
		} //end if
		m_fIsIceLaserMoving = false;
		//if( l_vPos_Shift.Length() > l_fStopLength ) //設大一點，拉到可以擊破的地方就好
		if( (m_fTotalMovingTime+l_fTimer01)<l_fLengthTimer )
		{
			m_fIsIceLaserMoving = true; //冰柱移動，就要顯示冰柱
			float l_fLengthMax = 0.0f;
			if( l_fLengthX>l_fLengthMax )
			{
				l_fLengthMax = l_fLengthX;
			} //end if
			if( l_fLengthY>l_fLengthMax )
			{
				l_fLengthMax = l_fLengthY;
			} //end if
			if( l_fLengthZ>l_fLengthMax )
			{
				l_fLengthMax = l_fLengthZ;
			} //end if
			l_fLengthMax = pow( l_fLengthMax, 0.5f );
			Vector3 l_vPos_Normal = Vector3( l_vPos_Shift.x/l_fLengthMax, l_vPos_Shift.y/l_fLengthMax, l_vPos_Shift.z/l_fLengthMax );
			//
			float l_fKapa = 1.0f;
			//
			Vector3 l_vMove_Shift = Vector3( 
				l_vPos_Normal.x * l_fKapa, 
				l_vPos_Normal.y * l_fKapa, 
				l_vPos_Normal.z * l_fKapa );
			//移動多少
			Vector3	l_vMovedDistance = l_vMove_Shift * e_fElpaseTime * m_fSpeed_02;
			//移動
			//l_vPos_Monster += l_vMovedDistance;


			float l_fRatio3 = m_fTotalMovingTime/l_fLengthTimer;
			if( l_fRatio3>1.0f )
			{
				l_fRatio3 = 1.0f;
			}
			else if( l_fRatio3<-1.0f )
			{
				l_fRatio3 = -1.0f;
			} //end if
			Vector3 l_vPos_Head = l_vPos_Monster + l_vPos_Length * l_fRatio3;




			//魚
			m_pCurrentMonster->SetPos( l_vPos_Head );
			//冷字
			m_pMPDI_IceBall[m_nCurrentFishIndex]->SetPos( l_vPos_Head );
			//第幾個？
			if( m_nCurrentFishIndex+1<10 )
			{
				m_pMPDI_CamboDiscSingle[m_nCurrentFishIndex]->SetPos( l_vPos_Head );
			}
			else
			{
				m_pMPDI_CamboDisc[m_nCurrentFishIndex]->SetPos( l_vPos_Head );
			} //end if
			//

			//
			//冰柱 --- BEGIN
			m_fDistanceIceLaser += l_vMovedDistance.Length();
			float l_fIceHeight2 = 20.0f;
			if( m_fDistanceIceLaser > l_fIceHeight2 )
			{
				int l_nCount2 = (int)(m_fDistanceIceLaser/l_fIceHeight2);
				for( int ii=0; ii<l_nCount2; ii++ )
				{
					m_fDistanceIceLaser -= l_fIceHeight2;
					m_nCurrentIceLaserIndex--;
					if( m_nCurrentIceLaserIndex<3 )
					{
						m_nCurrentIceLaserIndex = 3;
					} //end if
					
					//把冰塊拉回來的時候碰撞的小碎冰，如果直接使用 m_nCurrentIceLaserIndex 會在冰塊正中間小碎冰，所以要有偏移。
					int l_nIndex8 = m_nCurrentIceLaserIndex - 2;
					if( l_nIndex8<0 )
					{
						l_nIndex8 = 0;
					} //end if
					m_pMPDI_IceBreak->SetTransform( m_pMPDI_Fire01[l_nIndex8]->GetTransform() );

				} //next ii
			} //end if
			//冰柱 --- END

		}
		else if( m_pMPDI_IceBreak->IsDone()==true )
		{
			m_nFreezeStep = ENUM_STEP_FREEZE_ARRIVED_HOME;
			m_fTotalMovingTime = 0.0f;
			m_pMPDI_IceBreak->Init();
		}
		else
		{
			m_pMPDI_IceBreak->Update( e_fElpaseTime );
		} //end if
		//
		//雷公待命
//		m_pMPDI_WeaponWait[0]->Update( e_fElpaseTime );
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Update( e_fElpaseTime );
			//第幾個？
			if( ii+1<10 )
			{
				m_pMPDI_CamboDiscSingle[ii]->Update( e_fElpaseTime );
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Update( e_fElpaseTime );
			} //end if
		} //next ii
		//
		//冰柱
		for( int kk=2; kk<m_nCurrentIceLaserIndex; kk++ )
		{
			m_pMPDI_Fire01[kk]->Update( e_fElpaseTime );
		} //next kk
		//冰柱頭
		m_pMPDI_Fire01[0]->Update( e_fElpaseTime );
		//
		}
		break;
	case ENUM_STEP_FREEZE_ARRIVED_HOME:
		{
		//抓回來之後，如果還有下一個，就繼續抓，如果沒有了，就敲破！ 
		//
		//雷公待命
//		m_pMPDI_WeaponWait[0]->Update( e_fElpaseTime );
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Update( e_fElpaseTime );
			//第幾個？
			if( ii+1<10 )
			{
				m_pMPDI_CamboDiscSingle[ii]->Update( e_fElpaseTime );
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Update( e_fElpaseTime );
			} //end if
		} //next ii
		//
		//冰柱
		for( int kk=2; kk<m_nCurrentIceLaserIndex; kk++ )
		{
			m_pMPDI_Fire01[kk]->Update( e_fElpaseTime );
		} //next kk
		//冰柱頭
		m_pMPDI_Fire01[0]->Update( e_fElpaseTime );
		//
		m_nCurrentFishIndex ++; //多一隻魚
		//
		int l_nScore = 0;
		m_pCurrentMonster = cFishApp::m_spFishProbability->GetConsecutiveMonsterToDied( m_nPayRate*m_nPayRateBullet, m_nCurrentFishIndex, m_pWhichPlayer, &l_nScore );
//		m_nTotalGotScore += l_nScore;
		if( m_pCurrentMonster )
		{
			//冰凍住不盹凍
			m_pCurrentMonster->SetMonsterStatus( eMS_CONTROL_BY_MINI_GAME );
		} //end if
		//
		if( m_nCurrentFishIndex>=COUNT_MAXIUM_ICE_FISH || 
			!m_pCurrentMonster )
		{
			//最多就只有 COUNT_MAXIUM_BALL 多組, 超過就停止
			m_nFreezeStep = ENUM_STEP_FREEZE_FIRE;
		}
		else
		{
			m_nFreezeStep = ENUM_STEP_FREEZE_GOTO_NEXT_FISH; //再抓下一隻
		} //end if
		//
//		m_pMPDI_WeaponFire[0]->Init(); //準備打雷
		cGameApp::SoundPlay(L"6",true);
		//
		}
		break;
	case ENUM_STEP_FREEZE_GOTO_NEXT_FISH://下一個魚
//		m_pMPDI_WeaponFire[0]->Update( e_fElpaseTime );
//		if( m_pMPDI_WeaponFire[0]->IsDone()==true )
		{
			m_nFreezeStep = ENUM_STEP_FREEZE_START_TO_FIRE;
		} //end if
		break;
	case ENUM_STEP_FREEZE_FIRE: //把冰打爆
//		m_pMPDI_WeaponFire[0]->Update( e_fElpaseTime );
//		if( m_pMPDI_WeaponFire[0]->IsDone()==true )
		{
			cGameApp::SoundPlay(L"20",false);
			m_nFreezeStep = ENUM_STEP_FREEZE_BREAK_ICE;
			for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
			{
				//冷字爆掉
				m_pMPDI_IceBomb[ii]->Init();
				m_pMPDI_IceBomb[ii]->SetTransform( m_pMPDI_IceBall[ii]->GetTransform() );
				//
				cGameApp::SoundPlay(L"21",true);
			} //next ii
		} //end if
		break;
	case ENUM_STEP_FREEZE_BREAK_ICE: //把冰給敲破
		{
//		m_pMPDI_WeaponFire[0]->Update( e_fElpaseTime );
		int l_nCountDone = 0;
		//這裡用 < 不用 <=
		for( int ii=0; ii<m_nCurrentFishIndex; ii++ )
		{
			m_pMPDI_IceBomb[ii]->Update( e_fElpaseTime );

			if( m_pMPDI_IceBomb[ii]->IsDone()==true )
			{
				int l_nScore = 0;
				cFishApp::m_spFishProbability->MonsterDied( m_pMonsters[ii], m_nPayRate*m_nPayRateBullet, m_pWhichPlayer, &l_nScore );
				m_nTotalGotScore += l_nScore;
				l_nCountDone++; // 要等全部爆完
			} //end if

		} //next ii

		if( l_nCountDone >= m_nCurrentFishIndex )
		{
			//該歸零的要歸零
			m_nFreezeStep = ENUM_STEP_FREEZE_MAX;
			m_nCurrentFishIndex = 0;
			//
			return 1; //打完魚囉
		} //end if
		}
		break;
	default:
		break;
	} //end switch
	//
	return 0;	//還沒打完魚
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMiniGameStage03::f_Render_FreezeMonster( void )
{
	switch( m_nFreezeStep )
	{
	case ENUM_STEP_FREEZE_START_TO_FIRE:
/* 更本不會執行到這個
		//冰柱
		for( int ii=INDEX_START_ICE_LASER; ii<INDEX_START_ICE_LASER + m_nCurrentIceLaserIndex; ii++ )
		{
			m_pMPDI_Fire01[ii]->Render();
		} //next ii
		//冰柱頭
		m_pMPDI_Fire01[0]->Render();
		//
		//雷公待命
		m_pMPDI_WeaponWait[0]->Render();
		//
		for( int ii=0; ii<m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Render();
			if( ii+1<10 )
			{
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", ii+1 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDiscSingle[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDiscSingle[ii]->Render();
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Render();
			} //end if
		} //next ii
*/
		//
		break;
	case ENUM_STEP_FREEZE_MOVING_FOWARD:
	
		//冰柱
		for( int ii=INDEX_START_ICE_LASER; ii<m_nCurrentIceLaserIndex; ii++ )
		{
			m_pMPDI_Fire01[ii]->Render();
		} //next ii

		//冰柱頭
		m_pMPDI_Fire01[1]->Render();
		m_pMPDI_Fire01[0]->Render();		
		//
		for( int ii=0; ii<m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Render();
			if( ii+1<10 )
			{
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", ii+1 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDiscSingle[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDiscSingle[ii]->Render();
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Render();
			} //end if
		} //next ii
		//
		//雷公打雷最後一幕
//		m_pMPDI_WeaponFire[0]->Render();
		//
		break;
	case ENUM_STEP_FREEZE_ARRIVED_FISH:
		
		//冰柱
		for( int ii=INDEX_START_ICE_LASER; ii<m_nCurrentIceLaserIndex; ii++ )
		{
			m_pMPDI_Fire01[ii]->Render();
		} //next ii
		//冰柱頭
		m_pMPDI_Fire01[1]->Render();
//		m_pMPDI_Fire01[0]->Render();	
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBallOpen[ii]->Render();
		} //next ii
		//
		for( int ii=0; ii<m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Render();
			if( ii+1<10 )
			{
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", ii+1 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDiscSingle[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDiscSingle[ii]->Render();
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Render();
			} //end if
		} //next ii
		//
		//雷公打雷最後一幕
//		m_pMPDI_WeaponFire[0]->Render();
		//
		break;
	case ENUM_STEP_FREEZE_WAIT_FOR_SHOWING_WORD:
		
		//冰柱
		for( int ii=INDEX_START_ICE_LASER; ii<m_nCurrentIceLaserIndex; ii++ )
		{
			m_pMPDI_Fire01[ii]->Render();
		} //next ii
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Render();
			if( ii+1<10 )
			{
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", ii+1 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDiscSingle[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDiscSingle[ii]->Render();
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Render();
			} //end if
		} //next ii
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Render();
			if( ii+1<10 )
			{
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", ii+1 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDiscSingle[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDiscSingle[ii]->Render();
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Render();
			} //end if
		} //next ii
		//
		//雷公打雷最後一幕
//		m_pMPDI_WeaponFire[0]->Render();
		//
		break;
	case ENUM_STEP_FREEZE_PULL_FISH_BACKWARD:
		if( m_fIsIceLaserMoving==true )
		{
			//冰柱
			for( int ii=INDEX_START_ICE_LASER; ii<m_nCurrentIceLaserIndex; ii++ )
			{
				m_pMPDI_Fire01[ii]->Render();
			} //next ii
		} //end if
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Render();
			if( ii+1<10 )
			{
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", ii+1 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDiscSingle[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDiscSingle[ii]->Render();
			}
			else
			{
				int l_nNum = ii + 1;
				//
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", l_nNum%10 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDisc[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDisc[ii]->GetObject( L"numeral2" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDisc[ii]->GetObject( L"numeral2" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				l_nNum /= 10;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", l_nNum%10 );
				l_iIndexPlayerUI = m_pMPDI_CamboDisc[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDisc[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDisc[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDisc[ii]->Render();
			} //end if
		} //next ii
		//
		//雷公打雷最後一幕
//		m_pMPDI_WeaponFire[0]->Render();
		//
		m_pMPDI_IceBreak->Render(); //把圓盤拉回來碰撞的碎冰
		//
		break;
	case ENUM_STEP_FREEZE_ARRIVED_HOME:
		//冰柱
		for( int ii=INDEX_START_ICE_LASER; ii<m_nCurrentIceLaserIndex; ii++ )
		{
			m_pMPDI_Fire01[ii]->Render();
		} //next ii
		//
		for( int ii=0; ii<=m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Render();
			if( ii+1<10 )
			{
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", ii+1 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDiscSingle[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDiscSingle[ii]->Render();
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Render();
			} //end if
		} //next ii
		//
		//雷公待命
//		m_pMPDI_WeaponWait[0]->Render();
		//
		break;
	case ENUM_STEP_FREEZE_GOTO_NEXT_FISH:
		// (這裡直接用 < 不用 <= )
		for( int ii=0; ii<m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Render();
			if( ii+1<10 )
			{
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", ii+1 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDiscSingle[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDiscSingle[ii]->Render();
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Render();
			} //end if
		} //next ii
		//
		//雷公打雷
//		m_pMPDI_WeaponFire[0]->Render();
		//
		break;
	case ENUM_STEP_FREEZE_FIRE: //把冰橇破
		// (這裡直接用 < 不用 <= )
		for( int ii=0; ii<m_nCurrentFishIndex; ii++ )
		{
			//冷字
			m_pMPDI_IceBall[ii]->Render();
			if( ii+1<10 )
			{
				std::wstring	l_strN1;
				l_strN1 = UT::ComposeMsgByFormat( L"%d", ii+1 );
				int l_iIndexPlayerUI = m_pMPDI_CamboDiscSingle[ii]->GetPuzzleImage( L"BonusIceBomb" )->GetObjectIndexByName( l_strN1.c_str() );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
				m_pMPDI_CamboDiscSingle[ii]->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
				//
				m_pMPDI_CamboDiscSingle[ii]->Render();
			}
			else
			{
				m_pMPDI_CamboDisc[ii]->Render();
			} //end if
		} //next ii
		//
		//雷公打雷
//		m_pMPDI_WeaponFire[0]->Render();
		//
		break;
	case ENUM_STEP_FREEZE_BREAK_ICE:
		//雷公打雷
//		m_pMPDI_WeaponFire[0]->Render();
		//打爆碎冰 (這裡直接用 < 不用 <= )
		for( int ii=0; ii<m_nCurrentFishIndex; ii++ )
		{
			m_pMPDI_IceBomb[ii]->Render();
		} //next ii
		break;
	default:
		break;
	} //end switch
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

