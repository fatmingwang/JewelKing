/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "../EngineTest/MiniGameBase.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/FileNameDefine.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../EngineTest/FishProbability.h"

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameBase::cMiniGameBase( eMiniGameStage e_nStage )
{
	m_nTotalGotScore = 0;	//�o�����`�o��
	m_bAutoExplode = false;
	m_nCount_Fire01_Current = 0;
	m_nCount_Fire01 = 0;
	m_nCount_Fire01_CallBack = 0;
	m_pWhichPlayer = 0;
	m_nCurrentFishExplodeIndex = 0;
	//
	m_pMPDI_Fire01 = 0;
	m_pMPDI_Plate = 0;
	m_pMPDI_PlateExplode = 0;
	m_pMPDI_Compass = 0;
	m_pMPDI_Needle = 0;
	m_pMPDI_BonusShow_D = 0;
	//
	for( int ii=0; ii<MAXIUM_SCORE_NUMBER; ii++ )
	{
		m_pMPDI_BonusShow_ScorePos_U[ii] = 0;
	} //next ii
	m_pMPDI_BonusBomb = 0;
	m_pMPDI_Slot = 0;

	m_pMPDI_WeaponGetMessage	= new cMPDI*[1];
	for( int ii=0; ii<1; ii++ )
	{
		m_pMPDI_WeaponGetMessage[ii]=0;
	} //next ii
	m_pMPDI_WeaponWait			= new cMPDI*[1];
	for( int ii=0; ii<1; ii++ )
	{
		m_pMPDI_WeaponWait[ii]=0;
	} //next ii
	m_pMPDI_WeaponFire			= new cMPDI*[1];
	for( int ii=0; ii<1; ii++ )
	{
		m_pMPDI_WeaponFire[ii]=0;
	} //next ii
	//
	for( int ii=0; ii<COUNT_MAX_FISH_EXPLODE; ii++ )
	{
		m_pMPDI_FishExplode[ii] = 0;
	} //next ii
	//
	m_pStickyMonster = 0;
	
	m_nStage = e_nStage;
	m_nFlag = MINI_GAME_MAX;
	//
	m_pMPDI_Fire01 = 0;
	m_pBulletLatest = 0;
	m_matPlayerTonado = 0;
	m_nPayRateBullet = 1;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameBase::~cMiniGameBase(void)
{
	SAFE_DELETE( m_pMPDI_WeaponGetMessage );
	SAFE_DELETE( m_pMPDI_WeaponWait );
	SAFE_DELETE( m_pMPDI_WeaponFire );
	SAFE_DELETE( m_pMPDI_Fire01 );
	SAFE_DELETE( m_pBulletLatest );
	SAFE_DELETE( m_matPlayerTonado );
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameBase::Init(void)
{
	//�t�m�l�u --- BEGIN
	if( m_nStage==MINI_GAME_STAGE_01 )
	{
		m_nCount_Fire01 = 1;
		m_nCount_Fire01_Current = m_nCount_Fire01;
	}
	else if( m_nStage==MINI_GAME_STAGE_02 )
	{
		m_nCount_Fire01 = 10; //�@�����Q���p�X��
		m_nCount_Fire01_Current = 0;
	}
	else if( m_nStage==MINI_GAME_STAGE_03 )
	{
		m_nCount_Fire01 = 1 + 1 + 1 + 197; //�Y + �� + ��(�ϦV) + ���q
		m_nCount_Fire01_Current = m_nCount_Fire01;
	}
	else if( m_nStage==MINI_GAME_STAGE_04 )
	{
		m_nCount_Fire01 = 1;
		m_nCount_Fire01_Current = m_nCount_Fire01;
	}
	else if( m_nStage==MINI_GAME_STAGE_55 )
	{
		//�m���Ҧ�
		return;
	} //end if
	//
	m_pMPDI_Fire01				= new cMPDI*[m_nCount_Fire01]; //
	for( int ii=0; ii<m_nCount_Fire01; ii++ )
	{
		m_pMPDI_Fire01[ii]=0;
	} //next ii
	m_pBulletLatest				= new cBullet*[m_nCount_Fire01];
	for( int ii=0; ii<m_nCount_Fire01; ii++ )
	{
		m_pBulletLatest[ii]=0;
	} //next ii
	m_matPlayerTonado			= new cMatrix44[m_nCount_Fire01];
	//�t�m�l�u --- END
	//
	cMPDIList*	l_pMPDIList_Bonus=0;
	cMPDIList*	l_pMPDIList_Message=0;
	cMPDIList*  l_pMPDIList_Weapon=0;
	cMPDIList*	l_pMPDIList_BonusShow=0;
	cMPDIList*	l_pMPDIList_BonusSlot=0;
	//
	l_pMPDIList_Message = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_MESSAGE );
	l_pMPDIList_Weapon = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_WEAPON );
	l_pMPDIList_BonusShow = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BONUS_SHOW );
	l_pMPDIList_BonusSlot = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BONUS_SLOT );
	//
	m_pMPDI_Compass = new cMultiPathDynamicImage( l_pMPDIList_Weapon->GetObject(L"Compass_D") );
	m_pMPDI_Compass->Init();
	//
	m_pMPDI_Needle = new cMultiPathDynamicImage( l_pMPDIList_Weapon->GetObject(L"Needle_D") );
	m_pMPDI_Needle->Init();
	//
	m_pMPDI_LightningBolt = new cMultiPathDynamicImage( l_pMPDIList_Weapon->GetObject(L"LightningBolt") );
	m_pMPDI_LightningBolt->Init();
	//
	m_pMPDI_flash_light_t1 = new cMultiPathDynamicImage( l_pMPDIList_Weapon->GetObject(L"flash_light_t1") );
	m_pMPDI_flash_light_t1->Init();
	//
	m_pMPDI_BonusShow_D = new cMultiPathDynamicImage( l_pMPDIList_BonusShow->GetObject(L"BonusShow_D") );
	m_pMPDI_BonusShow_D->Init();
	for( int ii=0; ii<MAXIUM_SCORE_NUMBER; ii++ )
	{
		m_pMPDI_BonusShow_ScorePos_U[ii] = new cMultiPathDynamicImage( l_pMPDIList_BonusShow->GetObject(L"ScorePos_U") );
		m_pMPDI_BonusShow_ScorePos_U[ii]->Init();
	} //next ii
	//
	m_pMPDI_WeaponWait[0] = new cMultiPathDynamicImage( l_pMPDIList_Weapon->GetObject(L"WeaponWait_D") );
	m_pMPDI_WeaponWait[0]->Init();
	//
	m_pMPDI_WeaponFire[0] = new cMultiPathDynamicImage( l_pMPDIList_Weapon->GetObject(L"WeaponFire_D") );
	m_pMPDI_WeaponFire[0]->Init();
	//
	if( m_nStage==MINI_GAME_STAGE_01 )
	{
		//�ۭ��u (���y�⭿)
		//
		m_pMPDI_WeaponGetMessage[0] = new cMPDI( l_pMPDIList_Message->GetObject(L"WindWeaponGet_D") );
		m_pMPDI_WeaponGetMessage[0]->Init();
		//
		l_pMPDIList_Bonus = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BONUS_TORNADO );
		m_pMPDI_Plate = new cMPDI( l_pMPDIList_Bonus->GetObject(L"BonusShow") );
		m_pMPDI_PlateExplode = new cMPDI( l_pMPDIList_Bonus->GetObject(L"BonusShowBreak") );
		m_pMPDI_PlateExplode->Init();
		//
		for( int ii=0; ii<m_nCount_Fire01; ii++ )
		{
			m_pMPDI_Fire01[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject(L"Fire") );
			m_pMPDI_Fire01[ii]->Init();
		} //next ii
		//
		for( int ii=0; ii<COUNT_MAX_FISH_EXPLODE; ii++ )
		{
			m_pMPDI_FishExplode[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject(L"Explode") );
			m_pMPDI_FishExplode[ii]->Init();
		} //next ii
		//
	}
	else if( m_nStage==MINI_GAME_STAGE_02 )
	{
		//���p�� (�p���ۦP����)
		//
		m_pMPDI_WeaponGetMessage[0] = new cMultiPathDynamicImage( l_pMPDIList_Message->GetObject(L"LightningWeaponGet_D") );
		m_pMPDI_WeaponGetMessage[0]->Init();
		//
		l_pMPDIList_Bonus = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BONUS_THUNDER_BOMB );
		m_pMPDI_Plate = new cMPDI( l_pMPDIList_Bonus->GetObject( L"BonusShow") );
		m_pMPDI_PlateExplode = new cMPDI( l_pMPDIList_Bonus->GetObject(L"BonusShowBreak") );
		m_pMPDI_PlateExplode->Init();
		//
		for( int ii=0; ii<m_nCount_Fire01; ii++ )
		{
			m_pMPDI_Fire01[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject(L"Fire") );
			m_pMPDI_Fire01[ii]->Init();
		} //next ii
		//
		for( int ii=0; ii<COUNT_MAX_FISH_EXPLODE; ii++ )
		{
			m_pMPDI_FishExplode[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject(L"Explode") );
			m_pMPDI_FishExplode[ii]->Init();
		} //next ii
		//
	}
	else if( m_nStage==MINI_GAME_STAGE_03 )
	{
		//�N��u (�s��N��B���y�T��)
		//
		m_pMPDI_WeaponGetMessage[0] = new cMultiPathDynamicImage( l_pMPDIList_Message->GetObject(L"FreezeWeaponGet_D") );
		m_pMPDI_WeaponGetMessage[0]->Init();
		//
		l_pMPDIList_Bonus = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BONUS_ICE_BOMB );
		m_pMPDI_Plate = new cMPDI( l_pMPDIList_Bonus->GetObject(L"BonusShow") );
		m_pMPDI_PlateExplode = new cMPDI( l_pMPDIList_Bonus->GetObject(L"BonusShowBreak") );
		m_pMPDI_PlateExplode->Init();
		//
		m_pMPDI_Fire01[0] = new cMPDI( l_pMPDIList_Bonus->GetObject(L"IceBullet") );
		m_pMPDI_Fire01[0]->Init();
		m_pMPDI_Fire01[1] = new cMPDI( l_pMPDIList_Bonus->GetObject(L"LeaseHit") );
		m_pMPDI_Fire01[1]->Init();
		m_pMPDI_Fire01[2] = new cMPDI( l_pMPDIList_Bonus->GetObject(L"LeaseHit") );
		m_pMPDI_Fire01[2]->Init();
		for( int ii=INDEX_START_ICE_LASER; ii<m_nCount_Fire01; ii++ )
		{
			m_pMPDI_Fire01[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject(L"IceLaser") );
			m_pMPDI_Fire01[ii]->Init();
		} //next ii
	}
	else if( m_nStage==MINI_GAME_STAGE_04 )
	{
		//�p�^���u (���̬��u�B���y����)
		//
		m_pMPDI_WeaponGetMessage[0] = new cMultiPathDynamicImage( l_pMPDIList_Message->GetObject(L"ThunderWeaponGet_D") );
		m_pMPDI_WeaponGetMessage[0]->Init();
		//
		l_pMPDIList_Bonus = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BONUS_BOMB );
		m_pMPDI_Plate = new cMPDI( l_pMPDIList_Bonus->GetObject( L"BonusShow") );
		m_pMPDI_PlateExplode = new cMPDI( l_pMPDIList_Bonus->GetObject(L"BonusShowBreak") );
		m_pMPDI_PlateExplode->Init();
		//
		for( int ii=0; ii<m_nCount_Fire01; ii++ )
		{
			m_pMPDI_Fire01[ii] = new cMPDI( l_pMPDIList_Bonus->GetObject(L"Fire") );
			m_pMPDI_Fire01[ii]->Init();
		} //next ii
		//
		m_pMPDI_BonusBomb = new cMultiPathDynamicImage( l_pMPDIList_BonusSlot->GetObject(L"BonusBomb") );
		m_pMPDI_BonusBomb->Init();
		m_pMPDI_Slot = new cMultiPathDynamicImage( l_pMPDIList_BonusSlot->GetObject(L"Slot") );
		m_pMPDI_Slot->Init();
	} //end if
	//
	m_pMPDI_Plate->Init();
	m_pMPDI_Card01 = new cMPDI( l_pMPDIList_Bonus->GetObject(L"BonusHint_D") );
	m_pMPDI_Card01->Init();

	//
	//�]�w�X��
	f_SetFlag( MINI_GAME_FLAG_NONE );
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameBase::Update( float e_fElpaseTime )
{
	if( m_nStage==MINI_GAME_STAGE_55 ) //�m���Ҧ�
	{
		return;
	} //end if
	//
	if( m_nFlag==MINI_GAME_FLAG_STICKY_MONSTER ) //��L�H�۳�
	{
		m_pMPDI_Plate->Update(e_fElpaseTime);
		m_pMPDI_Plate->SetPos( Vector3( m_pStickyMonster->GetPos().x, m_pStickyMonster->GetPos().y, m_pStickyMonster->GetPos().z ) );
		m_pMPDI_Plate->SetAngle( m_pStickyMonster->GetAngle() );
		//
		//���۰��z��
		if( m_bAutoExplode==true )
		{
			m_pStickyMonster->SetMonsterStatus( eMS_DIED_SHOW );
		} //end if
		//
		//���[�������Q����
		//eMonsterStatus abc = m_pStickyMonster->GetMonsterStatus();
		if( m_pStickyMonster->GetMonsterStatus()==eMS_NONE )
		{
			f_SetFlag( MINI_GAME_FLAG_NONE );
		}
		else if( 
			(m_pStickyMonster->GetMonsterStatus()== eMS_DIED_SHOW ||
			m_pStickyMonster->GetMonsterStatus()== eMS_WAITING_FOR_CLEAN) &&
			m_pStickyMonster->IsVisible()==true && 
			m_nFlag==MINI_GAME_FLAG_STICKY_MONSTER )
		{
			//�o�����O�Q�֥������H
			m_nPayRateBullet = 1;
			if( m_bAutoExplode==false )
			{
				m_pWhichPlayer = m_pStickyMonster->GetHittedPlayer(); //MiniGame �����a
				if( m_pStickyMonster->GetBullet() )
				{
					m_nPayRateBullet = m_pStickyMonster->GetBullet()->GetPayRate();
				}
				else
				if( m_pWhichPlayer )
				{
					m_nPayRateBullet = ((cPlayerData*)(m_pWhichPlayer))->m_iLevelNumber;
				}
				else
				{
					m_nPayRateBullet = 10;
				}//end if
			}
			else
			{

				m_pWhichPlayer = cFishApp::m_spPlayerManager->GetObject( 1 ); //���ۤv�z���A�e�����a
				m_nPayRateBullet = ((cPlayerData*)(m_pWhichPlayer))->m_iLevelNumber;

			} //end if

			//�����D�Q�֥����� 
			if( !m_pWhichPlayer )
			{
				f_SetFlag( MINI_GAME_FLAG_NONE );
				return;
			} //end if

			
			



			//
			//�U�� 0 �סA�k�䪺 270 �סA�W�� 180 �סA���� 90 �סC
			Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
			Vector3 l_PlayerPosition = ((cPlayerData*)m_pWhichPlayer)->f_GetTubePosition();
			m_matTube = cMatrix44::TranslationMatrix(l_PlayerPosition) * cMatrix44::RotationMatrix(l_PlayerRotation);
			m_matPlayerFace = cMatrix44::RotationMatrix( l_PlayerRotation ) ;
			//
			//�p�G�o�Ӫ��a�A�{�b���b�p�C�����A�N���αq�Y�A�Ӥ@���C
			//���ަ��S���]�@���y�{�A���n�ܦ��p�d�s�ۡC
			int l_nResult = ((cPlayerData*)m_pWhichPlayer)->f_GoToMinigame( this ); //�ˬd�O�_�w�g�b���� minigame �F
			if( l_nResult==11 )
			{
				f_SetFlag( MINI_GAME_FLAG_EXPLODE_02 ); //�o���z������N�|������ MINI_GAME_FLAG_GOTO_CARD_01
			}
			else if( l_nResult==22 )
			{
				f_SetFlag( MINI_GAME_FLAG_EXPLODE_01 );
			} //end if
		}
		else if( m_pStickyMonster->GetMonsterStatus()==eMS_CONTROL_BY_MINI_GAME )
		{
			f_SetFlag( MINI_GAME_FLAG_NONE );
		} //end if
	}
	else if( m_nFlag==MINI_GAME_FLAG_EXPLODE_01 ) //��L�z���A���O�C�@�Ӷ�L���|�z��
	{
		if( m_pMPDI_PlateExplode )
		{
			m_pMPDI_PlateExplode->Update(e_fElpaseTime);
			m_pMPDI_PlateExplode->SetPos( Vector3( m_pStickyMonster->GetPos().x, m_pStickyMonster->GetPos().y, m_pStickyMonster->GetPos().z ) );
			m_pMPDI_PlateExplode->SetAngle( m_pStickyMonster->GetAngle() );

			if( m_pMPDI_PlateExplode->IsDone()==true )
			{
				f_SetFlag( MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING );
			} //end if
		}
		else
		{
			f_SetFlag( MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING );
		} //end if
	}
	else if( m_nFlag==MINI_GAME_FLAG_EXPLODE_02 ) //��L�z���A�o���z������N�|������ MINI_GAME_FLAG_GOTO_CARD_01
	{
		if( m_pMPDI_PlateExplode )
		{
			m_pMPDI_PlateExplode->Update(e_fElpaseTime);
			m_pMPDI_PlateExplode->SetPos( Vector3( m_pStickyMonster->GetPos().x, m_pStickyMonster->GetPos().y, m_pStickyMonster->GetPos().z ) );
			m_pMPDI_PlateExplode->SetAngle( m_pStickyMonster->GetAngle() );

			if( m_pMPDI_PlateExplode->IsDone()==true )
			{
				f_SetFlag( MINI_GAME_FLAG_GOTO_CARD_01 ); //�o��p�d���ݥX��
			} //end if
		}
		else
		{
			f_SetFlag( MINI_GAME_FLAG_GOTO_CARD_01 ); //�o��p�d���ݥX��
		} //end if
	}
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING ) //��o�Z���T���ʵe (�b���x�W�h����)
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
		//
		if( l_pMPDI_WeaponGetMessage->IsDone()==true )
		{
			if( m_nStage==MINI_GAME_STAGE_01 || m_nStage==MINI_GAME_STAGE_02 )
			{
				//�p���ݩR����
				cGameApp::SoundPlay(L"5",true);
				//
				f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_IDLE );
			}
			else if( m_nStage==MINI_GAME_STAGE_04 )
			{
				f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_IDLE );
			}
			else
			{
				//�p�����p����
				if(cGameApp::m_spSoundParser->GetObject(L"5")->IsPlay())
				{
					cGameApp::SoundPlay(L"5",false);
				} //end if
				cGameApp::SoundPlay(L"6",true);
				//
				f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING );
			} //end if
			//
		} //end if
	}
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //���x���������p���ݾ��ʵe (�p���ݾ��B�o�g��V���СB�p�d������P�ɼ���)
	{
		//�U�� 0 �סA�k�䪺 270 �סA�W�� 180 �סA���� 90 �סC
		Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		//�]�w���סA�u�� "�ۭ��u" �~����V��
		if( m_nStage==MINI_GAME_STAGE_01 )
		{
			m_pMPDI_LightningBolt->SetAngle(	Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ) );
			m_pMPDI_Compass->SetAngle(			Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ) );
			//m_pMPDI_Needle->SetAngle(			Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ) );
			m_matNeedle = m_matTube * cMatrix44::RotationMatrix(Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ));
			for( int ii=0; ii<m_pMPDI_Needle->Count(); ii++ )
			{
				m_pMPDI_Needle->GetObject(ii)->SetExtractTransform( &m_matNeedle );
			} //next ii
			//m_pMPDI_WeaponWait[0]->SetAngle(	Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ) );
			for( int ii=0; ii<m_pMPDI_WeaponWait[0]->Count(); ii++ )
			{
				m_pMPDI_WeaponWait[0]->GetObject(ii)->SetExtractTransform( &m_matNeedle );
			} //next ii
		} //end if
		//
		m_pMPDI_LightningBolt->Update( e_fElpaseTime ); //��L���U���p�q
		m_pMPDI_Compass->Update( e_fElpaseTime );		//��L
		m_pMPDI_Needle->Update( e_fElpaseTime );		//��L��V����
		m_pMPDI_WeaponWait[0]->Update( e_fElpaseTime );	//�p������
		//
		//���ݰʵe���񧹲�(�Q��)�p�G���a�٨S�o�g�A�N�n�ۦ�o�g�I
		if( m_pMPDI_WeaponWait[0]->IsDone()==true )
		{
			//�p�����p����
			if(cGameApp::m_spSoundParser->GetObject(L"5")->IsPlay())
			{
				cGameApp::SoundPlay(L"5",false);
			} //end if
			cGameApp::SoundPlay(L"6",true);
			//
			f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING );
		} //end if
	}
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //�p�����p�o�ging
	{
		Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		if( m_nStage==MINI_GAME_STAGE_01 )
		{
			m_pMPDI_LightningBolt->SetAngle(	Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ) );
			m_pMPDI_Compass->SetAngle(			Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ) );
			//m_pMPDI_Needle->SetAngle(			Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ) );
			m_matNeedle = m_matTube * cMatrix44::RotationMatrix(Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ));
			for( int ii=0; ii<m_pMPDI_Needle->Count(); ii++ )
			{
				m_pMPDI_Needle->GetObject(ii)->SetExtractTransform( &m_matNeedle );
			} //next ii
			//m_pMPDI_WeaponFire[0]->SetAngle(	Vector3( 0, 0, (90-l_PlayerRotation.z) - ((cPlayerData*)m_pWhichPlayer)->m_fTubeAngle ) );
			for( int ii=0; ii<m_pMPDI_WeaponFire[0]->Count(); ii++ )
			{
				m_pMPDI_WeaponFire[0]->GetObject(ii)->SetExtractTransform( &m_matNeedle );
			} //next ii
		} //end if
		//
		m_pMPDI_LightningBolt->Update( e_fElpaseTime ); //��L���U���p�q
		m_pMPDI_Compass->Update( e_fElpaseTime );		//��L
		m_pMPDI_Needle->Update( e_fElpaseTime );		//��L��V����
		//
		//�@�w�n�৹���Q�A�p���~�i�H���p
		if( m_pMPDI_Slot )
		{
		}
		else
		{
			m_pMPDI_WeaponFire[0]->Update( e_fElpaseTime );	//�p�����p
		} //end if
		//
		if( !m_pMPDI_Slot ) //�n�׶} Slot
		{
			m_pMPDI_flash_light_t1->Update( e_fElpaseTime );//�ե�
		} //end if
		//
		m_fTimer_Shake01 -= e_fElpaseTime;
		if( (m_nStage==MINI_GAME_STAGE_02 && m_fTimer_Shake01<0.0f) || 
			(m_nStage==MINI_GAME_STAGE_01 && m_pMPDI_WeaponFire[0]->IsDone()==true) ||
			(m_nStage==MINI_GAME_STAGE_03 && m_pMPDI_WeaponFire[0]->IsDone()==true) ||
			(m_nStage==MINI_GAME_STAGE_04 && m_pMPDI_WeaponFire[0]->IsDone()==true) )
//		if( m_pMPDI_WeaponFire[0]->IsDone()==true )
		{
			m_nReslut = 1; //��p���o�X��
			//
			if( m_nStage==MINI_GAME_STAGE_01 )
			{
				cGameApp::SoundPlay(L"17",true);
			}
			else if( m_nStage==MINI_GAME_STAGE_02 )
			{
				cGameApp::SoundPlay(L"18",true);
			} 
			else if( m_nStage==MINI_GAME_STAGE_03 )
			{
				cGameApp::SoundPlay(L"22",true);
			} 
			else if( m_nStage==MINI_GAME_STAGE_04 )
			{
				cGameApp::SoundPlay(L"23",true);
			} //end if
		}
		else
		{
			m_nReslut = 0;
		} //end if
	}
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //�p�����X�Ӫ��p
	{
		if( m_nCount_Fire01_Current<=0 )
		{
			f_Fire_ed();
		} //end if
		//�g�b�U���O��
		if( m_fTimer_Shake01>0.0f )
		{
			m_fTimer_Shake01 -= e_fElpaseTime;
			if( m_fTimer_Shake01<0.0f )
			{
				m_fTimer_Shake01 = 0.0f;
			} //end if
		} //end if
		//
	}
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_FINISH_01 ) //Bonus Show
	{
		Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		//
		//Bonus Show --- BEGIN
		for( int ii=0; ii<m_pMPDI_BonusShow_D->Count(); ++ii )
		{
			m_pMPDI_BonusShow_D->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		m_pMPDI_BonusShow_D->Update( e_fElpaseTime ); //�q�b���x�W��
		//Bonus Show --- END
		//
		//Bonus Show ���� --- BEGIN
		//���Ʀ��X��ơH
		int l_nHowManyNum = 0;
		int l_nTotalGotScore = m_nTotalGotScore;
		while( l_nTotalGotScore>0 )
		{
			l_nHowManyNum++;
			l_nTotalGotScore /= 10;
		} //end if
		//if( m_nTotalGotScore==0 )
		//{
		//	l_nHowManyNum = 1;
		//} //end if
		if( l_nHowManyNum>MAXIUM_SCORE_NUMBER )
		{
			l_nHowManyNum = MAXIUM_SCORE_NUMBER;
		} //end if
		m_nHowManyScore = l_nHowManyNum;
		l_nTotalGotScore = m_nTotalGotScore;
		for( int ii=0; ii<m_nHowManyScore; ii++ )
		{
			//
			int l_nCurrentNum = l_nTotalGotScore%10;
			l_nTotalGotScore /= 10;
			std::wstring	l_strN1;
			l_strN1 = UT::ComposeMsgByFormat( L"BigWinNumerial%d", l_nCurrentNum );
			int l_iIndexPlayerUI = m_pMPDI_BonusShow_ScorePos_U[ii]->GetPuzzleImage( L"BigWin" )->GetObjectIndexByName( l_strN1.c_str() );
			m_pMPDI_BonusShow_ScorePos_U[ii]->GetObject( L"ScorePos" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
			m_pMPDI_BonusShow_ScorePos_U[ii]->GetObject( L"ScorePos" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
//			m_pMPDI_BonusShow_ScorePos_U[ii]->SetAngle( Vector3( 0, 0, l_PlayerRotation.z ) );
			m_pMPDI_BonusShow_ScorePos_U[ii]->SetAngle( Vector3( 0, 0, 180 ) );
			//
			for( int kk=0; kk<m_pMPDI_BonusShow_ScorePos_U[ii]->Count(); ++kk )
			{
				m_matBonusShow_ScorePos[ii] = 
					m_matTube * 
					cMatrix44::TranslationMatrix( Vector3(0 - (float(ii)-(float(m_nHowManyScore)/2.0f)+0.5f)*35.0f, 0.0f, 0.0f) );
				m_pMPDI_BonusShow_ScorePos_U[ii]->GetObject(kk)->SetExtractTransform( &m_matBonusShow_ScorePos[ii] );
			} //next ii
			//
			m_pMPDI_BonusShow_ScorePos_U[ii]->Update( e_fElpaseTime );
		} //next ii
		//Bonus Show ���� --- END
		//
		if( m_pMPDI_BonusShow_D->IsDone()==true )
		{
			m_nTotalGotScore = 0; // �����k�s
			//
			f_SetFlag( MINI_GAME_FLAG_GOTO_FINISH_99 ); //�p�C������
		} //end if
	} //end if
	
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameBase::Render(void)
{
	if( m_nStage==MINI_GAME_STAGE_55 ) //�m���Ҧ�
	{
		return;
	} //end if
	//
	if( m_nFlag==MINI_GAME_FLAG_STICKY_MONSTER )
	{
		m_pMPDI_Plate->Render();
	}//end if

}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameBase::Render2(void)
{
	if( m_nStage==MINI_GAME_STAGE_55 ) //�m���Ҧ�
	{
		return;
	} //end if
	//
	if( m_nFlag==MINI_GAME_FLAG_EXPLODE_01 )
	{
		if( m_pMPDI_PlateExplode )
		{
			m_pMPDI_PlateExplode->Render();
		} //end if
	}
	else if( m_nFlag==MINI_GAME_FLAG_EXPLODE_02 )
	{
		if( m_pMPDI_PlateExplode )
		{
			m_pMPDI_PlateExplode->Render();
		} //end if
	} //end if
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING )
	{
		//�U�� 0 �סA�k�䪺 270 �סA�W�� 180 �סA���� 90 �סC
		Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		Vector3 l_PlayerPosition = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerPosition();
		//
		m_pMPDI_WeaponGetMessage[0]->Render();
	}
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //�p���ݩR�o�Ӱʧ@
	{
		//�U�� 0 �סA�k�䪺 270 �סA�W�� 180 �סA���� 90 �סC
		Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		Vector3 l_PlayerPosition = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerPosition();
		//
		if( m_nStage==MINI_GAME_STAGE_01 || m_nStage==MINI_GAME_STAGE_02 || m_nStage==MINI_GAME_STAGE_03 )
		{
			m_pMPDI_LightningBolt->Render(); //�p
			
			if( m_nStage==MINI_GAME_STAGE_01 )
			{
				m_pMPDI_Compass->Render(); //��ջ\
				m_pMPDI_Needle->Render(); //��ջ\����V����
			} //end if

			m_pMPDI_WeaponWait[0]->Render(); //�p���ݩR
		} //end if
		//
	}
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //�p�����p�o�Ӱʧ@
	{
		//�U�� 0 �סA�k�䪺 270 �סA�W�� 180 �סA���� 90 �סC
		Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		Vector3 l_PlayerPosition = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerPosition();
		//
		if( !m_pMPDI_Slot ) //�n�׶} Slot
		{
			m_pMPDI_flash_light_t1->Render(); //�ե�
		} //end if
		//
		if( m_nStage==MINI_GAME_STAGE_01 || m_nStage==MINI_GAME_STAGE_02 || m_nStage==MINI_GAME_STAGE_03 )
		{
			m_pMPDI_LightningBolt->Render(); //�p

			if( m_nStage==MINI_GAME_STAGE_01 )
			{
				m_pMPDI_Compass->Render(); //��ջ\
				m_pMPDI_Needle->Render(); //��ջ\����V����
			} //end if

			m_pMPDI_WeaponFire[0]->Render();//�p�����p
		} //end if
		//
	}
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //�p�����X�Ӫ��p
	{
		//
		//�g�b�U���O
		//
		if( m_fTimer_Shake01>0.0f )
		{
		 float l_sfAA = frand(-5,2);
		 float l_sfBB = frand(-2,5);
		 glEnable2D(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
		 cMatrix44 l_matProjection;
		 glhOrthof2((float*)l_matProjection.m, 0, cGameApp::m_svGameResolution.x, cGameApp::m_svGameResolution.y, 0, -10000, 10000);
		 l_matProjection = l_matProjection*cMatrix44::TranslationMatrix(l_sfAA,l_sfBB,0);
		 FATMING_CORE::SetupShaderViewProjectionMatrix(l_matProjection);
		} //end if
		//
	}
	else if( m_nFlag==MINI_GAME_FLAG_GOTO_FINISH_01 )
	{
		m_pMPDI_BonusShow_D->Render(); //�q�b���x�W��
		for( int ii=0; ii<m_nHowManyScore; ii++ )
		{
			m_pMPDI_BonusShow_ScorePos_U[ii]->Render();
		} //next ii
	} //end if

}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameBase::Destroy(void)
{
	m_pMPDI_Plate->Destroy();
	SAFE_DELETE( m_pMPDI_Plate );

	for( int ii=0; ii<m_nCount_Fire01; ii++ )
	{
		m_pMPDI_Fire01[ii]->Destroy();
		SAFE_DELETE( m_pMPDI_Fire01[ii] );
	} //next ii

	m_pMPDI_Card01->Destroy();
	SAFE_DELETE( m_pMPDI_Card01 );

	m_pMPDI_Compass->Destroy();
	SAFE_DELETE( m_pMPDI_Compass );

	m_pMPDI_Needle->Destroy();
	SAFE_DELETE( m_pMPDI_Needle );

	m_pMPDI_LightningBolt->Destroy();
	SAFE_DELETE( m_pMPDI_LightningBolt );

	m_pMPDI_flash_light_t1->Destroy();
	SAFE_DELETE( m_pMPDI_flash_light_t1 );

	m_pMPDI_BonusShow_D->Destroy();
	SAFE_DELETE( m_pMPDI_BonusShow_D );
	for( int ii=0; ii<MAXIUM_SCORE_NUMBER; ii++ )
	{
		m_pMPDI_BonusShow_ScorePos_U[ii]->Destroy();
		SAFE_DELETE( m_pMPDI_BonusShow_ScorePos_U[ii] );
	} //next ii

	if( m_pMPDI_BonusBomb )
	{
		m_pMPDI_BonusBomb->Destroy();
	} //end if
	SAFE_DELETE( m_pMPDI_BonusBomb );

	if( m_pMPDI_Slot )
	{
		m_pMPDI_Slot->Destroy();
	} //end if
	SAFE_DELETE( m_pMPDI_Slot );

	if( m_pMPDI_PlateExplode )
	{
		m_pMPDI_PlateExplode->Destroy();
	} //end if
	SAFE_DELETE( m_pMPDI_PlateExplode );

	for( int ii=0; ii<1; ii++ ) //�|�Ӥ�V
	{
		m_pMPDI_WeaponGetMessage[ii]->Destroy();
		SAFE_DELETE( m_pMPDI_WeaponGetMessage[ii] );

		m_pMPDI_WeaponWait[ii]->Destroy();
		SAFE_DELETE( m_pMPDI_WeaponWait[ii] );

		m_pMPDI_WeaponFire[ii]->Destroy();
		SAFE_DELETE( m_pMPDI_WeaponFire[ii] );
	} //next ii

	for( int ii=0; ii<COUNT_MAX_FISH_EXPLODE; ii++ )
	{
		if( m_pMPDI_FishExplode[ii] )
		{
			m_pMPDI_FishExplode[ii]->Destroy();
		} //end if
		SAFE_DELETE( m_pMPDI_FishExplode[ii] );
	} //next ii
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameBase::f_Fire( void )
{
	if( m_nFlag==MINI_GAME_FLAG_GOTO_THUNDER_IDLE )
	{
		//�p�����p����
		if(cGameApp::m_spSoundParser->GetObject(L"5")->IsPlay())
		{
			cGameApp::SoundPlay(L"5",false);
		} //end if
		//
		if( m_nStage==MINI_GAME_STAGE_04 )
		{
			cGameApp::SoundPlay(L"25",true);
		}
		else
		{
			cGameApp::SoundPlay(L"6",true);
		} //end if
		//
		f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING );

	} //end if
}


/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameBase::f_Fire_ed( void ) //ed ��ܵ���
{
	//�o�g����
	m_nCount_Fire01_CallBack++;
	if( m_nCount_Fire01_CallBack>=m_nCount_Fire01_Current )
	{
		int l_nCount1 = m_vMonsterDiedList.size();
		//
		m_nCount_Fire01_CallBack = 0;
		f_SetFlag( MINI_GAME_FLAG_GOTO_FINISH_01 );
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cMiniGameBase::f_SetStage( eMiniGameStage e_nStage )
{
	if( e_nStage==m_nStage )
	{
		return;
	} //end if
	//
	//
	m_nStage = e_nStage;
	return;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cMiniGameBase::f_SetFlag( eMiniGameFlag e_nFlag )
{
	if( e_nFlag==m_nFlag )
	{
		return;
	} //end if
	//
	switch( e_nFlag )
	{
	case MINI_GAME_FLAG_NONE:
		m_pWhichPlayer = 0; //�L�H����
		if( m_pMPDI_PlateExplode )
		{
			m_pMPDI_PlateExplode->Init();
		} //end if
		m_pMPDI_WeaponGetMessage[0]->Init();
		m_pMPDI_WeaponFire[0]->Init();
		m_pMPDI_BonusShow_D->Init();
		m_pMPDI_flash_light_t1->Init();
		//
		m_vMonsterDiedList.clear();
		m_nCurrentFishExplodeIndex = 0;
		//
		break;
	case MINI_GAME_FLAG_STICKY_MONSTER:
		if( m_pMPDI_PlateExplode )
		{
			m_pMPDI_PlateExplode->Init();
		} //end if
		break;
	case MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING:
		m_pMPDI_WeaponGetMessage[0]->Init();
		cGameApp::SoundPlay(L"16",true);
		break;
	case MINI_GAME_FLAG_GOTO_THUNDER_IDLE:
		{
		//�p�G���p���Q�A�����p���Q���b�̫e�� --- BEGIN
		if( m_pMPDI_Slot )
		{
			m_pMPDI_Slot->Update(0.01f); //�p���Q
		} //end if
		//�p�G���p���Q�A�����p���Q���b�̫e�� --- END
		//
		//�p���}���U����L���U���p��
		for( int ii=0; ii<m_pMPDI_LightningBolt->Count(); ++ii )
		{
			m_pMPDI_LightningBolt->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		//�p���}���U����L
		for( int ii=0; ii<m_pMPDI_Compass->Count(); ++ii )
		{
			m_pMPDI_Compass->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		//�p����V����
		for( int ii=0; ii<m_pMPDI_Needle->Count(); ++ii )
		{
			m_pMPDI_Needle->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		//�p������
		cMPDI *l_pMPDI_WeaponWait = m_pMPDI_WeaponWait[0];
		int	l_iCount = l_pMPDI_WeaponWait->Count();
		for( int ii=0; ii<l_iCount; ++ii )
		{
			l_pMPDI_WeaponWait->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		m_pMPDI_WeaponWait[0]->Init();
		//
		m_pMPDI_LightningBolt->SetAngle(	Vector3( 0, 0, 0 ) );
		m_pMPDI_Compass->SetAngle(			Vector3( 0, 0, 0 ) );
		m_pMPDI_Needle->SetAngle(			Vector3( 0, 0, 0 ) );
		m_pMPDI_WeaponFire[0]->SetAngle(	Vector3( 0, 0, 0 ) );
		}
		//
		break;
	case MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING:
		{
		//�p���}���U����L���U���p��
		for( int ii=0; ii<m_pMPDI_LightningBolt->Count(); ++ii )
		{
			m_pMPDI_LightningBolt->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		//�p���}���U����L
		for( int ii=0; ii<m_pMPDI_Compass->Count(); ++ii )
		{
			m_pMPDI_Compass->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		//�p����V����
		for( int ii=0; ii<m_pMPDI_Needle->Count(); ++ii )
		{
			m_pMPDI_Needle->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		//�p�����p
		cMPDI *l_pMPDI_WeaponFire = m_pMPDI_WeaponFire[0];
		int	l_iCount = l_pMPDI_WeaponFire->Count();
		for( int ii=0; ii<l_iCount; ++ii )
		{
			l_pMPDI_WeaponFire->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		//
		m_pMPDI_flash_light_t1->Init(); //�ե�
		//
		m_vMonsterDiedList.clear();
		m_nCurrentFishExplodeIndex = 0;
		m_fTimer_Shake01 = 0.4f;
		}
		//
		break;
	case MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED:
		m_fTimer_Shake01 = 2.0f;
		m_pMPDI_Fire01[0]->Init();
		break;
	case MINI_GAME_FLAG_GOTO_FINISH_01:
		break;
	case MINI_GAME_FLAG_GOTO_FINISH_99:
		m_pMPDI_BonusShow_D->Init();
		break;
	default:
		break;
	} //end if
	//
	m_nFlag = e_nFlag;
	Update(0.0001f); //�]�w�� Flag ����A���W�n�b Update �@���ARender �~�|�ǡC
	//
	return;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */

