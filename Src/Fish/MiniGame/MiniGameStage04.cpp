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
DESCRIPTION   : �p�^���u
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameStage04::cMiniGameStage04(void):cMiniGameBase(MINI_GAME_STAGE_04)
{
	m_nPayRate = 1;//5; //�߲v
	m_nScoreTmp = -999;
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
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //�p�����p�ݾ�
	{
		if( !cGameApp::m_spSoundParser->GetObject(L"24")->IsPlay() )
		{
			cGameApp::SoundPlay(L"24",true);
		} //end if
		//
		//�U�� 0 �סA�k�䪺 270 �סA�W�� 180 �סA���� 90 �סC
		Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		Vector3 l_PlayerPosition = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerPosition();
		//
		//�üƶ]�p���Q
		m_nTimesA++;
		m_nTimesA %= 4;
		if( m_nTimesA==0 )
		{
			m_nRandNumA += rand()%10; //�C���üƥ[�@�I
			if( m_nRandNumA>105 )
			{
				m_nRandNumA = rand()%10;
			}
			else if( m_nRandNumA>99 )
			{
				m_nRandNumA = 100;
			} //end if

			//�p���Q
			f_RunSmallSlot( m_nRandNumA );
			//�j���Q
			f_RunBigSlot( m_nRandNumA );
		} //end if
		//�]���o�ɭԤp���Q���|�ʡA�ҥH���Χ�s
		for( int ii=0; ii<m_pMPDI_Slot->Count(); ++ii )
		{
			m_pMPDI_Slot->GetObject(ii)->SetExtractTransform( &m_matTube );
		} //next ii
		//m_pMPDI_Slot->Update( e_fElpaseTime );
		//
		//�j���Q
		if( l_PlayerRotation.z>=90 && l_PlayerRotation.z<270 )
		{
			m_pMPDI_BonusBomb->SetAngle( Vector3( 0, 0, 180 ) );
		}
		else
		{
			m_pMPDI_BonusBomb->SetAngle( Vector3( 0, 0, 0 ) );
		} //end if
		m_pMPDI_BonusBomb->Update(e_fElpaseTime);
		//
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //�p�����p�o�g
	{
		//�U�� 0 �סA�k�䪺 270 �סA�W�� 180 �סA���� 90 �סC
		Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
		Vector3 l_PlayerPosition = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerPosition();
		//
		//�O�o�A�R�A�ܼƭn�k�s

		//
		//�����p���Q���U�ӡA�~�p�����p�o�g
		if( m_bFirstTime_WeaponFire || m_nTimesA_delay>=10 )
		{
			m_bFirstTime_WeaponFire = false;
			m_pMPDI_WeaponFire[0]->Update( e_fElpaseTime );
			m_pMPDI_flash_light_t1->Update( e_fElpaseTime );//�ե�
			//
			if( m_nTimesA_delay==10 )
			{
				if( cGameApp::m_spSoundParser->GetObject(L"6")->IsPlay()==false )
				{
					cGameApp::SoundPlay(L"6",true);
				} //end if
			} //end if
		} //end if

		//�üƶ]�p���Q
		m_nTimesA++;
		m_nTimesA %= m_nTimesA_delay;
		if( m_nTimesA_delay>10 )
		{
			//���Q����
			if( m_nScoreTmp < 0 )
			{
				int l_nScore = 0;
				float l_fBarRatio = 100.0f * cFishApp::m_spFishProbability->AllMonsterDiedByProbability( m_nPayRate*m_nPayRateBullet, m_pWhichPlayer, &l_nScore );
				m_nScoreTmp = l_nScore;
				int l_nBarRatio = int(l_fBarRatio);
				f_RunSmallSlot( l_nBarRatio );
				f_RunBigSlot( l_nBarRatio );
			} //end if
		}
		else if( m_nTimesA==0 )
		{
			if( m_bIsSlotDone )
			{
				m_nTimesA_delay ++;
			} //end if

			m_nRandNumA += rand()%30; //�C���üƥ[�@�I
			if( m_nRandNumA>105 )
			{
				m_nRandNumA = 20 + rand()%10;
			}
			else if( m_nRandNumA>99 )
			{
				m_nRandNumA = 100;
			} //end if

			f_RunSmallSlot( m_nRandNumA );

		} //end if
		//�]���o�ɭԤp���Q���|�ʡA�ҥH���Χ�s
		//
		if( m_pMPDI_Slot->IsDone()==false )
		{
			m_pMPDI_Slot->Update( e_fElpaseTime );
		}
		else
		{
			m_bIsSlotDone = true;
		} //end if
		//
		if( m_pMPDI_WeaponFire[0]->IsDone()==true && m_nScoreTmp>=0 )
		{
			if( cGameApp::m_spSoundParser->GetObject(L"24")->IsPlay() )
			{
				cGameApp::SoundPlay(L"24",false);
			} //end if

			cGameApp::SoundPlay(L"25",true);


			//�O�o�R�A�ܼƭn�k�s
			m_bIsSlotDone = false;
			m_nTimesA_delay = 3;

			//�o�� m_nTotalGotScore
			//���Q���v

			m_nTotalGotScore += m_nScoreTmp;
			m_nScoreTmp = -999;
		} //end if
		//
		if( m_nReslut==1 )
		{
			f_SetFlag( MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED );
			//���o�g
			//m_pBulletLatest[0] = ((cPlayerData*)m_pWhichPlayer)->f_Fire_MiniGame( this, -999 );
			cGameApp::SoundPlay(L"23",true);
		} //end if
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //�p�����X�Ӫ��p
	{
		int l_nCountFire01s = 0;
		for( int kk=0; kk<m_nCount_Fire01_Current; kk++ )
		{
			m_pMPDI_Fire01[kk]->Update(e_fElpaseTime);
			if( m_pMPDI_Fire01[kk]->IsDone() )
			{
				l_nCountFire01s++;
				if( l_nCountFire01s>=m_nCount_Fire01_Current )
				{
					f_SetFlag( MINI_GAME_FLAG_GOTO_FINISH_01 );
				} //end if
			} //end if
		} //next kk
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
	if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_IDLE ) //�p�����p�ݾ�
	{
		//�j���Q
		m_pMPDI_BonusBomb->Render();
		//�p���Q
		m_pMPDI_Slot->Render(); 
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ING ) //�p�����p�o�g
	{
		//�j���Q
		m_pMPDI_BonusBomb->Render(); 
		//�p���Q
		m_pMPDI_Slot->Render(); 
		//
		//�@�w�n�৹���Q�A�p���~�i�H���p
		if( m_bFirstTime_WeaponFire || m_nTimesA_delay>10 )
		{
			m_pMPDI_WeaponFire[0]->Render();//�p�����p
		} //end if
		
	}
	else if( f_GetFlag()==MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED ) //�p�����X�Ӫ��p
	{
		for( int kk=0; kk<m_nCount_Fire01_Current; kk++ )
		{
			m_pMPDI_Fire01[kk]->Render();
		} //next kk
		//
		//�p�����p�̫�@��
//		m_pMPDI_WeaponFire[0]->Render();
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
		m_nTimesA_delay = 3;
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
void cMiniGameStage04::f_RunBigSlot( int e_nNumber )
{
	//�U�� 0 �סA�k�䪺 270 �סA�W�� 180 �סA���� 90 �סC
	Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
	Vector3 l_PlayerPosition = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerPosition();
	//
	std::wstring	l_strN1;
	std::wstring	l_strN2;
	std::wstring	l_strN3;
	int l_iIndexPlayerUI;
	//
	int l_nRandNumA = e_nNumber;
	if( m_nRandNumA<60 )
	{
		l_strN1 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%dBig", l_nRandNumA%10 );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN1 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%dBig", l_nRandNumA%10 );
	}
	else
	{
		l_strN1 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%dBig", l_nRandNumA%10 );
	} //end if
//OutputDebugString( l_strN1.c_str() );
	
	l_nRandNumA /= 10;
	if( m_nRandNumA<60 )
	{
		l_strN2 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%dBig", l_nRandNumA%10 );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN2 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%dBig", l_nRandNumA%10 );
	}
	else
	{
		l_strN2 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%dBig", l_nRandNumA%10 );
	} //end if

	l_nRandNumA /= 10;
	if( m_nRandNumA<60 )
	{
		l_strN3 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%dBig", l_nRandNumA%10 );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN3 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%dBig", l_nRandNumA%10 );
	}
	else
	{
		l_strN3 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%dBig", l_nRandNumA%10 );
	} //end if
	
	if( l_PlayerRotation.z>=90 && l_PlayerRotation.z<270 )
	{
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN3.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"%" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"%" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
		
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN2.c_str() );
//OutputDebugString( l_strN2.c_str() );
//OutputDebugString( ValueToStringW( l_iIndexPlayerUI ) );
		m_pMPDI_BonusBomb->GetObject( L"numeral3" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral3" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN1.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral2" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral2" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

		std::wstring	l_strNX;
		if( m_nRandNumA<60 )
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%sBig", L"%" );
		}
		else if( m_nRandNumA<80 )
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%sBig", L"%" );
		}
		else
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%sBig", L"%" );
		} //end if
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strNX.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
	}
	else
	{
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN1.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral3" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral3" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
		
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN2.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral2" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral2" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN3.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

		std::wstring	l_strNX;
		if( m_nRandNumA<60 )
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%sBig", L"%" );
		}
		else if( m_nRandNumA<80 )
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%sBig", L"%" );
		}
		else
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%sBig", L"%" );
		} //end if
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strNX.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"%" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"%" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cMiniGameStage04::f_RunSmallSlot( int e_nNumber )
{
	//�U�� 0 �סA�k�䪺 270 �סA�W�� 180 �סA���� 90 �סC
	Vector3 l_PlayerRotation = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerRotation();
	Vector3 l_PlayerPosition = ((cPlayerData*)m_pWhichPlayer)->f_GetPlayerPosition();
	//
	int l_nRandNumA = e_nNumber;
	std::wstring	l_strN1;
	if( m_nRandNumA<60 )
	{
		l_strN1 = UT::ComposeMsgByFormat( L"G%d", l_nRandNumA%10 );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN1 = UT::ComposeMsgByFormat( L"Y%d", l_nRandNumA%10 );
	}
	else
	{
		l_strN1 = UT::ComposeMsgByFormat( L"R%d", l_nRandNumA%10 );
	} //end if
	
	l_nRandNumA /= 10;
	std::wstring	l_strN2;
	if( m_nRandNumA<60 )
	{
		l_strN2 = UT::ComposeMsgByFormat( L"G%d", l_nRandNumA%10 );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN2 = UT::ComposeMsgByFormat( L"Y%d", l_nRandNumA%10 );
	}
	else
	{
		l_strN2 = UT::ComposeMsgByFormat( L"R%d", l_nRandNumA%10 );
	} //end if

	l_nRandNumA /= 10;
	std::wstring	l_strN3;
	if( m_nRandNumA<60 )
	{
		l_strN3 = UT::ComposeMsgByFormat( L"G%d", l_nRandNumA%10 );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN3 = UT::ComposeMsgByFormat( L"Y%d", l_nRandNumA%10 );
	}
	else
	{
		l_strN3 = UT::ComposeMsgByFormat( L"R%d", l_nRandNumA%10 );
	} //end if

	int l_iIndexPlayerUI = m_pMPDI_Slot->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN1.c_str() );
	m_pMPDI_Slot->GetObject( L"1PNumeral3" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
	m_pMPDI_Slot->GetObject( L"1PNumeral3" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
	
	l_iIndexPlayerUI = m_pMPDI_Slot->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN2.c_str() );
	m_pMPDI_Slot->GetObject( L"1PNumeral2" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
	m_pMPDI_Slot->GetObject( L"1PNumeral2" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

	l_iIndexPlayerUI = m_pMPDI_Slot->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN3.c_str() );
	m_pMPDI_Slot->GetObject( L"1PNumeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
	m_pMPDI_Slot->GetObject( L"1PNumeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

	if( m_nRandNumA<60 )
	{
		l_strN3 = UT::ComposeMsgByFormat( L"G%s", "%" );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN3 = UT::ComposeMsgByFormat( L"Y%s", "%" );
	}
	else
	{
		l_strN3 = UT::ComposeMsgByFormat( L"R%s", "%" );
	} //end if
	l_iIndexPlayerUI = m_pMPDI_Slot->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN3.c_str() );
	m_pMPDI_Slot->GetObject( L"1P%" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
	m_pMPDI_Slot->GetObject( L"1P%" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

	//�j���Q
	l_nRandNumA = m_nRandNumA;
	if( m_nRandNumA<60 )
	{
		l_strN1 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%dBig", l_nRandNumA%10 );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN1 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%dBig", l_nRandNumA%10 );
	}
	else
	{
		l_strN1 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%dBig", l_nRandNumA%10 );
	} //end if
//OutputDebugString( l_strN1.c_str() );
	
	l_nRandNumA /= 10;
	if( m_nRandNumA<60 )
	{
		l_strN2 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%dBig", l_nRandNumA%10 );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN2 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%dBig", l_nRandNumA%10 );
	}
	else
	{
		l_strN2 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%dBig", l_nRandNumA%10 );
	} //end if

	l_nRandNumA /= 10;
	if( m_nRandNumA<60 )
	{
		l_strN3 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%dBig", l_nRandNumA%10 );
	}
	else if( m_nRandNumA<80 )
	{
		l_strN3 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%dBig", l_nRandNumA%10 );
	}
	else
	{
		l_strN3 = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%dBig", l_nRandNumA%10 );
	} //end if

	if( l_PlayerRotation.z>=90 && l_PlayerRotation.z<270 )
	{
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN3.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"%" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"%" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
		
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN2.c_str() );
//OutputDebugString( l_strN2.c_str() );
//OutputDebugString( ValueToStringW( l_iIndexPlayerUI ) );
		m_pMPDI_BonusBomb->GetObject( L"numeral3" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral3" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN1.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral2" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral2" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

		std::wstring	l_strNX;
		if( m_nRandNumA<60 )
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%sBig", L"%" );
		}
		else if( m_nRandNumA<80 )
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%sBig", L"%" );
		}
		else
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%sBig", L"%" );
		} //end if
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strNX.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
	}
	else
	{
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN1.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral3" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral3" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
		
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN2.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral2" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral2" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strN3.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"numeral1" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"numeral1" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;


		std::wstring	l_strNX;
		if( m_nRandNumA<60 )
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV1_%sBig", L"%" );
		}
		else if( m_nRandNumA<80 )
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV2_%sBig", L"%" );
		}
		else
		{
			l_strNX = UT::ComposeMsgByFormat( L"BonusBombNumerialLV3_%sBig", L"%" );
		} //end if
		l_iIndexPlayerUI = m_pMPDI_BonusBomb->GetPuzzleImage( L"Slot" )->GetObjectIndexByName( l_strNX.c_str() );
		m_pMPDI_BonusBomb->GetObject( L"%" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
		m_pMPDI_BonusBomb->GetObject( L"%" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;

	} //end if
}
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
