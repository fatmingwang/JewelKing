/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "Coin.h"
#include "PlayerData.h"
#include "../../Core/GamePlayUT/Event/AllEventInclude.h"
#include "../ControlPanel/AllControlPanelInclude.h"
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cCoin::cCoin()
{
	m_fFlyTime = 0.0f;
	m_iStackPosIndex = -1;
	m_iHowManyScore = 0;
	m_nHowManyScore = 0;
	m_nMonsterId = -1;
	m_pCoinMPDIList = 0;
	m_iPlayerID = -1;
	m_piPlayerCount = &cFishApp::m_spControlPanel->m_iPlayerCount;
	m_fCoinIdleTimer = 0;
	m_pCoinStackMPDIList = 0;
	m_iStackIndex = 0 ;
	for(int i = 0; i < cPlayerBehaviorBase::COINPOPUP_COUNT; ++i)
	{
		m_fCoinAnimationTime[i] = 0.0f;
		m_pCoinImage[i] = 0;
		m_pSilverCoinImage_D[i] = 0;
		m_pSilverCoinImage_R[i] = 0;
		m_pSilverCoinImage_U[i] = 0;
		m_pSilverCoinImage_L[i] = 0;

		m_pGoldCoinImage_D[i] = 0;
		m_pGoldCoinImage_R[i] = 0;
		m_pGoldCoinImage_U[i] = 0;
		m_pGoldCoinImage_L[i] = 0;

	} //next i
	//
	for( int ii=0; ii<MAXIUM_SCORE_NUMBER; ii++ )
	{
		m_pMPDI_BonusShow_ScorePos_U[ii] = 0;
	} //next ii
	m_pMPDI_BigWinShow1_D = 0;
	m_pMPDI_BigWinShow1_Loop = 0;
	m_pMPDI_BigWinShow2_D = 0;
	m_pMPDI_BigWinShow2_Loop = 0;
	m_pNI_BigWinShow3_Score = 0;
	m_pCoinFountain = 0;

	m_OriPos = -1;
	m_NowAccessIdx = 0;

	m_bAccess   = false;
	m_SetOriPos = true;
	m_bCoinMove = false;

	m_StartX  = 0.f;
	m_TargetX = 0.f;

	m_DisplayTm    = COINSTACKANITM;
	m_MoveElapseTm = 0.f;

	for(int i = 0; i < MAXIUM_SCORE_NUMBER; ++i)
	{
		m_pScoreImage[i] = 0;
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cCoin::~cCoin(void)
{
	Destroy();
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cCoin::ShowCoin(cPlayerData* e_PlayerData,Vector3 e_vFish[cPlayerBehaviorBase::COINPOPUP_COUNT],int e_iMoney, int e_iStackPosIndex)
{
	m_nTimerBigWinShow3 = 0;
	m_nBigWinShow3_Coin_Current = 0;
	m_iMoney = e_iMoney;
	for(int i = 0; i < cPlayerBehaviorBase::COINPOPUP_COUNT; ++i)
	{
		m_vFishPos[i]=e_vFish[i];
	}
	m_vPlayerPos=e_PlayerData->m_vLvPos;
	m_vRot = e_PlayerData->m_vPlayerRot;
	m_iPlayerID = e_PlayerData->m_iPlayerID;
	m_pPlayer = e_PlayerData;
	m_eCointStatus=eCoinScore;
	m_fCoinIdleTimer = 0;

	
	for(int i = 0; i < MAXIUM_SCORE_NUMBER; ++i)
	{
		m_pScoreImage[i]->Init();
	}


	std::wstring l_strCoinName ;
	for(int i = 0; i < cPlayerBehaviorBase::COINPOPUP_COUNT; ++i)
	{
		if(m_vRot.z == 0)
		{
			if(m_iCoinIndex<2)
				m_pCoinImage[i] = m_pSilverCoinImage_D[i];
			else
				m_pCoinImage[i] = m_pGoldCoinImage_D[i];
		}
		else if(m_vRot.z == 270)
		{		
			if(m_iCoinIndex<2)
				m_pCoinImage[i] = m_pSilverCoinImage_R[i];
			else
				m_pCoinImage[i] = m_pGoldCoinImage_R[i];
		}
		else if(m_vRot.z == 180)
		{
			if(m_iCoinIndex<2)
				m_pCoinImage[i] = m_pSilverCoinImage_U[i];
			else
				m_pCoinImage[i] = m_pGoldCoinImage_U[i];
		}
		else if(m_vRot.z == 90)
		{
			if(m_iCoinIndex<2)
				m_pCoinImage[i] = m_pSilverCoinImage_L[i];
			else
				m_pCoinImage[i] = m_pGoldCoinImage_L[i];
		}
	}
	//
	m_CoinStackList.Init();
	m_CoinStackList.SetCurrentWorkingObject(m_iStackIndex);
	
	
	if(m_CoinStackList.GetCurrentWorkingObject())
	{
		m_CoinStackList.GetCurrentWorkingObject()->Init();
		int l_iCount = m_CoinStackList.GetCurrentWorkingObject()->Count();
		m_matPlayerTransfomr = cMatrix44::TranslationMatrix( m_vPlayerPos ) * cMatrix44::RotationMatrix( m_vRot );
		for( int i=0;i<l_iCount;++i )
		{
			m_CoinStackList.GetCurrentWorkingObject()->GetObject(i)->SetExtractTransform(&m_matPlayerTransfomr);
		} //next i

	}

	m_iStackPosIndex = e_iStackPosIndex;
	e_PlayerData->m_iStackPosCount[e_iStackPosIndex] = 1;

	m_OriPos = -1;
	m_NowAccessIdx = m_pPlayer->m_Coin_StackStartIdx;

	m_bAccess   = false;
	m_SetOriPos = true;
	m_bCoinMove = false;

	m_StartX  = 0.f;
	m_TargetX = 0.f;

	m_DisplayTm    = COINSTACKANITM;
	m_MoveElapseTm = 0.f;

	if ( m_pPlayer->m_Coin_StackStartIdx < 47 )
	{
		if ( m_iStackPosIndex >= m_pPlayer->m_Coin_StackStartIdx && m_iStackPosIndex <= m_pPlayer->m_Coin_StackStartIdx + 3 )
			m_OriPos = m_iStackPosIndex - m_pPlayer->m_Coin_StackStartIdx;
	}
	else 
	{
		int CondiVal = ( m_iStackPosIndex >= 0 && m_iStackPosIndex <= 2 ) ? m_iStackPosIndex + COINSTACKNUM : m_iStackPosIndex;

		if ( CondiVal >= m_pPlayer->m_Coin_StackStartIdx && CondiVal <= m_pPlayer->m_Coin_StackStartIdx + 3 )
			m_OriPos = CondiVal - m_pPlayer->m_Coin_StackStartIdx;
	}

	m_CoinStackList.GetCurrentWorkingObject()->SetPos( m_vCoinStackPos[ m_OriPos >= 0 && m_OriPos <= 3 ? m_OriPos : 3 ] );

	//m_CoinStackList.GetCurrentWorkingObject()->SetPos(m_vCoinStackPos[3]);
	
	if( m_nMonsterId>=0 )
	{
//m_nMonsterId = 1;
		if( m_nMonsterId==1 || m_nMonsterId==7 )
		{
			m_pMPDI_BigWinShow1_D->SetPos( m_vFishPos[0] );
			m_pMPDI_BigWinShow1_D->Init();
			m_pMPDI_BigWinShow1_Loop->SetPos( m_vFishPos[0] );
			m_pMPDI_BigWinShow1_Loop->Init();
			cGameApp::SoundPlay(L"12",true);
		}
		else if( m_nMonsterId==5 )
		{
			m_pMPDI_BigWinShow2_D->SetPos( m_vFishPos[0] );
			m_pMPDI_BigWinShow2_D->Init();
			m_pMPDI_BigWinShow2_Loop->SetPos( m_vFishPos[0] );
			m_pMPDI_BigWinShow2_Loop->Init();
			cGameApp::SoundPlay(L"13",true);
		}
		else if( m_nMonsterId==6 || m_nMonsterId==12 || m_nMonsterId==13 )
		{
			if( m_vRot.z >= 90 && m_vRot.z<270 )
			{
				//m_pMPDI_BigWinShow3_U->Init();
				//m_pCoinFountain_U->Emit( Vector3(cGameApp::m_svGameResolution.x/2.f, cGameApp::m_svGameResolution.y/2, 0.f), false );
			}
			else
			{
				//m_pMPDI_BigWinShow3_D->Init();
				//m_pCoinFountain->Emit( Vector3(cGameApp::m_svGameResolution.x/2.f, cGameApp::m_svGameResolution.y/2, 0.f), false );
			} //end if
			cGameApp::SoundPlay(L"14",true);
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
void cCoin::Init()
{
	Destroy();
	m_pCoinMPDIList = cGameApp::GetMPDIListByFileName((WCHAR*)COIN_MPDI);
	m_pCoinStackMPDIList = cGameApp::GetMPDIListByFileName((WCHAR*)COINSTACK_MPDI);
	m_pCoinScoreMPDIList = cGameApp::GetMPDIListByFileName((WCHAR*)COINSCORE_MPDI);


	for(int i = 0; i < MAXIUM_SCORE_NUMBER; ++i)
	{
		m_pScoreImage[i] = new cMultiPathDynamicImage( m_pCoinScoreMPDIList->GetObject(L"Player1CoinScore0") );
		m_pScoreImage[i]->Init();
	}

	
	for(int i = 0; i < cPlayerBehaviorBase::COINPOPUP_COUNT; ++i)
	{
		m_pSilverCoinImage_D[i] = new cMultiPathDynamicImage( m_pCoinMPDIList->GetObject(L"SilverCoinPopup_D") );
		m_pSilverCoinImage_R[i] = new cMultiPathDynamicImage( m_pCoinMPDIList->GetObject(L"SilverCoinPopup_R") );
		m_pSilverCoinImage_U[i] = new cMultiPathDynamicImage( m_pCoinMPDIList->GetObject(L"SilverCoinPopup_U") );
		m_pSilverCoinImage_L[i] = new cMultiPathDynamicImage( m_pCoinMPDIList->GetObject(L"SilverCoinPopup_L") );

		m_pGoldCoinImage_D[i]	= new cMultiPathDynamicImage( m_pCoinMPDIList->GetObject(L"GoldCoinPopup_D") );
		m_pGoldCoinImage_R[i]	= new cMultiPathDynamicImage( m_pCoinMPDIList->GetObject(L"GoldCoinPopup_R") );
		m_pGoldCoinImage_U[i]	= new cMultiPathDynamicImage( m_pCoinMPDIList->GetObject(L"GoldCoinPopup_U") );
		m_pGoldCoinImage_L[i]	= new cMultiPathDynamicImage( m_pCoinMPDIList->GetObject(L"GoldCoinPopup_L") );
	}
	

	for(int i = 0; i < 4; ++i)
	{
		std::wstring l_strStackName = UT::ComposeMsgByFormat(L"CoinStack_%d",i);
		cMultiPathDynamicImage*l_pStack = new cMultiPathDynamicImage( m_pCoinStackMPDIList->GetObject(l_strStackName.c_str()));
		m_CoinStackList.AddObjectNeglectExist(l_pStack);
	}

	m_CoinStackList.SetCurrentWorkingObject(0);
	if(m_CoinStackList.GetCurrentWorkingObject())
	{
		m_CoinStackList.GetCurrentWorkingObject()->Init();
		int l_iCount = m_CoinStackList.GetCurrentWorkingObject()->Count();
		m_matPlayerTransfomr = cMatrix44::TranslationMatrix( m_vPlayerPos ) * cMatrix44::RotationMatrix( m_vRot );
		for( int i=0;i<l_iCount;++i )
		{
			m_CoinStackList.GetCurrentWorkingObject()->GetObject(i)->SetExtractTransform(&m_matPlayerTransfomr);
		} //next i

	}
	m_vCoinStackPos[0] = m_CoinStackList.GetCurrentWorkingObject()->GetPos();
	m_vCoinStackPos[1] = m_vCoinStackPos[0] + Vector3(-18,0,0);
	m_vCoinStackPos[2] = m_vCoinStackPos[1] + Vector3(-18,0,0);
	m_vCoinStackPos[3] = m_vCoinStackPos[2] + Vector3(-18,0,0);


	//
	cMPDIList*	l_pMPDIList_BonusShow=0;
	//
	l_pMPDIList_BonusShow = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BIG_WIN_SHOW_1 );
	m_pMPDI_BigWinShow1_D = new cMultiPathDynamicImage( l_pMPDIList_BonusShow->GetObject(L"BigWinShow1") );
	m_pMPDI_BigWinShow1_Loop = new cMultiPathDynamicImage( l_pMPDIList_BonusShow->GetObject(L"BigWinShow1") );
	//
	l_pMPDIList_BonusShow = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BIG_WIN_SHOW_2 );
	m_pMPDI_BigWinShow2_D = new cMultiPathDynamicImage( l_pMPDIList_BonusShow->GetObject(L"BigWinShow2") );
	m_pMPDI_BigWinShow2_Loop = new cMultiPathDynamicImage( l_pMPDIList_BonusShow->GetObject(L"BigWinShow2") );
	//
	l_pMPDIList_BonusShow = cGameApp::GetMPDIListByFileName( (WCHAR*)MINI_GAME_BONUS_SHOW );
	for( int ii=0; ii<MAXIUM_SCORE_NUMBER; ii++ )
	{
		m_pMPDI_BonusShow_ScorePos_U[ii] = new cMultiPathDynamicImage( l_pMPDIList_BonusShow->GetObject(L"BonusShowBreak") );
		m_pMPDI_BonusShow_ScorePos_U[ii]->Init();
	} //next ii
	//
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cCoin::Update(float e_fElpaseTime)
{
	if(m_eCointStatus==eCoinScore)
	{
		int l_iHowManyNum = 0;
		int l_iTotalGotScore = m_iMoney;
		while( l_iTotalGotScore >0 )
		{
			l_iHowManyNum++;
			l_iTotalGotScore /= 10;
		}
		if( l_iHowManyNum>MAXIUM_SCORE_NUMBER )
		{
			l_iHowManyNum = MAXIUM_SCORE_NUMBER;
		}
		m_iHowManyScore = l_iHowManyNum;
		l_iTotalGotScore = m_iMoney;
		int ScoreCount = 0;
		for( int i = 0; i < m_iHowManyScore; i++ )
		{
			int l_iCurrentNum = l_iTotalGotScore%10;
			l_iTotalGotScore /= 10;
			std::wstring	l_strN1 = UT::ComposeMsgByFormat( L"Player%d", m_iPlayerID+1 );
			l_strN1 += UT::ComposeMsgByFormat( L"CoinScore%d", l_iCurrentNum );
			int l_iIndex = m_pScoreImage[i]->GetPuzzleImage( L"CoinScore" )->GetObjectIndexByName( l_strN1.c_str() );
			m_pScoreImage[i] ->GetObject( L"Player1CoinScore" )->ChangeAllImageByImageIndex( l_iIndex );
			m_pScoreImage[i] ->GetObject( L"Player1CoinScore" )->GetCurrentPointData()->iImageIndex = l_iIndex;
			//m_pScoreImage[i] ->SetAngle( Vector3( 0, 0, 180 ) );
			cMatrix44 m_matFish = cMatrix44::TranslationMatrix(m_vFishPos[0]) * cMatrix44::RotationMatrix(m_vRot);
			for( int j=0; j<m_pScoreImage[i]->Count(); ++j )
			{
				m_matScorePos[i] = m_matFish * cMatrix44::TranslationMatrix( Vector3(0 - (float(i)-(float(m_iHowManyScore)/2.0f)+0.5f)*30.0f, 0.0f, 0.0f) );

				m_pScoreImage[i]->GetObject(j)->SetExtractTransform( &m_matScorePos[i] );
			} //next ii
			m_pScoreImage[i]->Update(e_fElpaseTime);
			
			if(m_pScoreImage[i]->IsDone())
			{
				ScoreCount++;
			}
		}

		if( ScoreCount>= m_iHowManyScore )
		{
			for(int i = 0; i < cPlayerBehaviorBase::COINPOPUP_COUNT; ++i)
			{
				m_fSize[i] = 0;

				m_pCoinImage[i]->Init();

				//m_pCoinImage[i]->SetScale(m_fSize[i]);
				
				m_pCoinImage[i]->SetPos(m_vFishPos[i]);
				
				m_Curve[i].Clear();
				m_Curve[i].AddPoint(m_vFishPos[i],0);
				m_Curve[i].AddPoint(m_vPlayerPos,m_fFlyTime);
				m_Curve[i].Init();

				if(m_iCoinIndex<2)
					cGameApp::SoundPlay(L"9",true);
				else
					cGameApp::SoundPlay(L"10",true);
			}
			m_eCointStatus=eCoinMove;
			//
			//Big Win
			//ぃ辰Τぃ Big Win
		} //end if

//m_nMonsterId = 5;
		if( m_nMonsterId>=0 )
		{

			if( m_nMonsterId==1 || m_nMonsterId==7 )
			{
				m_pMPDI_BigWinShow1_D->Update(e_fElpaseTime);
				if( m_pMPDI_BigWinShow1_D->IsDone() )
				{
					m_pMPDI_BigWinShow1_Loop->Update(e_fElpaseTime);
				} //end if
			}
			else if( m_nMonsterId==5 )
			{
				m_pMPDI_BigWinShow2_D->Update(e_fElpaseTime);
				if( m_pMPDI_BigWinShow2_D->IsDone() )
				{
					m_pMPDI_BigWinShow2_Loop->Update(e_fElpaseTime);
				} //end if
			} //end if

			if( m_nMonsterId==1 || m_nMonsterId==7 ||
				m_nMonsterId==5 )
			{
				//Bonus Show だ计 --- BEGIN
				//だ计Τ碭计
				int l_nHowManyNum = 0;
				int l_nTotalGotScore = m_iMoney;
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
				l_nTotalGotScore = m_iMoney;
				for( int ii=0; ii<m_nHowManyScore; ii++ )
				{
					//
					int l_nCurrentNum = l_nTotalGotScore%10;
					l_nTotalGotScore /= 10;
					std::wstring	l_strN1;
					l_strN1 = UT::ComposeMsgByFormat( L"BigWinNumerial%d", l_nCurrentNum );
//					int l_iIndexPlayerUI = m_pMPDI_BonusShow_ScorePos_U[ii]->GetPuzzleImage( L"BigWin" )->GetObjectIndexByName( l_strN1.c_str() );
//					m_pMPDI_BonusShow_ScorePos_U[ii]->GetObject( L"ScorePos" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
//					m_pMPDI_BonusShow_ScorePos_U[ii]->GetObject( L"ScorePos" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
		//			m_pMPDI_BonusShow_ScorePos_U[ii]->SetAngle( Vector3( 0, 0, l_PlayerRotation.z ) );
//					m_pMPDI_BonusShow_ScorePos_U[ii]->SetAngle( Vector3( 0, 0, 180 ) );
					//
					for( int kk=0; kk<m_pMPDI_BonusShow_ScorePos_U[ii]->Count(); ++kk )
					{
//						m_matBonusShow_ScorePos[ii] = 
//							m_matScorePos[ii]
//							*  cMatrix44::TranslationMatrix( Vector3(0 - (float(ii)-(float(m_nHowManyScore)/2.0f)+0.5f)*10.0f, 120.0f, 0.0f) );
						//
//						m_pMPDI_BonusShow_ScorePos_U[ii]->GetObject(kk)->SetExtractTransform( &m_matBonusShow_ScorePos[ii] );
					} //next ii
					//
//					m_pMPDI_BonusShow_ScorePos_U[ii]->Update( e_fElpaseTime );
				} //next ii
				//Bonus Show だ计 --- END
			} //end if
		} //end if
	
	}
	else if(m_eCointStatus==eCoinMove)
	{	



//m_nMonsterId = 5;
		if( m_nMonsterId>=0 )
		{

			float l_fScale = 1.0f - m_Curve[0].GetCurrentLERPTime();
			Vector3 abc = m_Curve[0].GetCurrentPosition();
			if( m_nMonsterId==1 || m_nMonsterId==7 )
			{
				m_pMPDI_BigWinShow1_Loop->SetPos( m_Curve[0].GetCurrentPosition() );
				m_pMPDI_BigWinShow1_Loop->SetScale( l_fScale );
				m_pMPDI_BigWinShow1_Loop->Update(e_fElpaseTime);
			}
			else if( m_nMonsterId==5 )
			{
				m_pMPDI_BigWinShow2_Loop->SetPos( m_Curve[0].GetCurrentPosition() );
				m_pMPDI_BigWinShow2_Loop->SetScale( l_fScale );
				m_pMPDI_BigWinShow2_Loop->Update(e_fElpaseTime);
			} //end if


			if( m_nMonsterId==1 || m_nMonsterId==7 ||
				m_nMonsterId==5 )
			{
				//Bonus Show だ计 --- BEGIN
				//だ计Τ碭计
				int l_nHowManyNum = 0;
				int l_nTotalGotScore = m_iMoney;
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
				l_nTotalGotScore = m_iMoney;
				for( int ii=0; ii<m_nHowManyScore; ii++ )
				{
					//
					int l_nCurrentNum = l_nTotalGotScore%10;
					l_nTotalGotScore /= 10;
					std::wstring	l_strN1;
					l_strN1 = UT::ComposeMsgByFormat( L"BigWinNumerial%d", l_nCurrentNum );
//					int l_iIndexPlayerUI = m_pMPDI_BonusShow_ScorePos_U[ii]->GetPuzzleImage( L"BigWin" )->GetObjectIndexByName( l_strN1.c_str() );
//					m_pMPDI_BonusShow_ScorePos_U[ii]->GetObject( L"ScorePos" )->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
//					m_pMPDI_BonusShow_ScorePos_U[ii]->GetObject( L"ScorePos" )->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
		//			m_pMPDI_BonusShow_ScorePos_U[ii]->SetAngle( Vector3( 0, 0, l_PlayerRotation.z ) );
//					m_pMPDI_BonusShow_ScorePos_U[ii]->SetAngle( Vector3( 0, 0, 180 ) );
					//
					for( int kk=0; kk<m_pMPDI_BonusShow_ScorePos_U[ii]->Count(); ++kk )
					{
//						m_matBonusShow_ScorePos[ii] = 
//							m_matScorePos[ii]
//							*  cMatrix44::TranslationMatrix( Vector3(0 - (float(ii)-(float(m_nHowManyScore)/2.0f)+0.5f)*10.0f, 120.0f, 0.0f) );
						//
//						m_pMPDI_BonusShow_ScorePos_U[ii]->GetObject(kk)->SetExtractTransform( &m_matBonusShow_ScorePos[ii] );
					} //next ii
					//
//					m_pMPDI_BonusShow_ScorePos_U[ii]->Update( e_fElpaseTime );
				} //next ii
				//Bonus Show だ计 --- END
			} //end if


		} //end if


		int l_nCountDone = 0;
		float l_fMaxTimer =0; 
		
		l_fMaxTimer = m_pCoinImage[0]->GetTotalPlayTime();
		m_fCoinIdleTimer+=e_fElpaseTime;
		for(int i = 0; i < cPlayerBehaviorBase::COINPOPUP_COUNT; ++i)
		{
			if(m_fSize[i]<1)
			{
				m_fSize[i] += 2*e_fElpaseTime;
				//m_pCoinImage[i]->SetScale(m_fSize[i]);
			}//end if
			
			//m_pCoinImage[i]->Update(e_fElpaseTime);
			if(m_fCoinAnimationTime[i] <= m_pCoinImage[i]->GetTotalPlayTime()*0.8)
			{
				m_fCoinAnimationTime[i] +=e_fElpaseTime*0.2f*(i+2);
			}
			m_pCoinImage[i]->UpdateByGlobalTime(m_fCoinAnimationTime[i]);
			//if( m_pCoinImage[i]->IsDone() )
			if(m_fCoinIdleTimer >= l_fMaxTimer*0.8)
			{
				cGameApp::SoundPlay(L"11",true);	
				//m_pCoinImage[i]->Update(e_fElpaseTime);	
				m_Curve[i].Update(e_fElpaseTime);
				//m_Curve[i].Update(10.0f);
				m_pCoinImage[i]->SetPos( m_Curve[i].GetCurrentPosition() );
			} //end if


			if( m_Curve[i].IsCurveMoveDone() )
			{
				m_fCoinAnimationTime[i] = 0;
				l_nCountDone ++;
			} //end if
			//
			if( l_nCountDone>=cPlayerBehaviorBase::COINPOPUP_COUNT )
			{
				m_eCointStatus = eCoinStack;

				//AddScore
				m_pPlayer -> m_i64Score += m_iMoney;
				m_pPlayer -> m_iAddScoreCount--;
				m_pPlayer -> m_iCointCount--;
				
			} //end if

		} //next i
		
	}	
	else if(m_eCointStatus == eCoinStack)
	{ //帮舼
		m_bAccess = false;

		if(m_iStackPosIndex>=0)
		{
			if( m_pPlayer->m_iStackPosCount[m_iStackPosIndex] == 1 )
			{
				if ( m_pPlayer->m_Coin_StackStartIdx < 47 )
				{
					if ( m_iStackPosIndex >= m_pPlayer->m_Coin_StackStartIdx && m_iStackPosIndex <= m_pPlayer->m_Coin_StackStartIdx + 3 )
					{
						m_OriPos  = m_iStackPosIndex - m_pPlayer->m_Coin_StackStartIdx;
						m_bAccess = true;
					}
				}
				else 
				{
					int CondiVal = ( m_iStackPosIndex >= 0 && m_iStackPosIndex <= 2 ) ? m_iStackPosIndex + COINSTACKNUM : m_iStackPosIndex;

					if ( CondiVal >= m_pPlayer->m_Coin_StackStartIdx && CondiVal <= m_pPlayer->m_Coin_StackStartIdx + 3 )
					{
						m_OriPos = CondiVal - m_pPlayer->m_Coin_StackStartIdx;
						m_bAccess = true;
					}
				}

				if ( m_bAccess )
				{
					if ( m_SetOriPos && m_OriPos >= 0 && m_OriPos <= 3 )
					{
						m_SetOriPos = false;
						m_CoinStackList.GetCurrentWorkingObject()->SetPos( m_vCoinStackPos[ m_OriPos ] );
					}

					m_CoinStackList.Update(e_fElpaseTime);

					if(m_CoinStackList.GetCurrentWorkingObject()->IsDone())
					{
						if ( m_bCoinMove )
						{
							m_MoveElapseTm += e_fElpaseTime;

							Vector3 CoinPos = Vector3( 0, 0, 0 );

							if ( m_MoveElapseTm < COINSTACKANITM )
								CoinPos.x = m_StartX + ( ( m_TargetX - m_StartX ) * ( m_MoveElapseTm / COINSTACKANITM ) );
							else
							{
								m_bCoinMove    = false;
								m_MoveElapseTm = 0.f;

								CoinPos.x = m_TargetX;
							}
							m_CoinStackList.GetCurrentWorkingObject()->SetPos( CoinPos );
						}
						else if ( m_iStackPosIndex == m_pPlayer->m_Coin_StackStartIdx )
						{
							m_DisplayTm -= e_fElpaseTime;

							if ( m_DisplayTm <= 0.f )
							{
								m_DisplayTm = COINSTACKANITM;

								m_eCointStatus = eCoinNull;
								m_pPlayer->m_iStackPosCount[ m_iStackPosIndex ]--;

								m_pPlayer->m_Coin_StackStartIdx++;

								if ( m_pPlayer->m_Coin_StackStartIdx >= COINSTACKNUM )
									m_pPlayer->m_Coin_StackStartIdx = 0;

								m_NowAccessIdx = m_pPlayer->m_Coin_StackStartIdx;

								for ( int i = 1; i <= 3; i++ )
								{
									 int NextIdx = m_iStackPosIndex + i;

									 if ( NextIdx >= COINSTACKNUM )
										 NextIdx -= COINSTACKNUM;

									 for ( int j = 0; j < cCoinManager::COUNT_COINS; j++ )
									 {
										  if ( ( m_pPlayer->m_pCoinManager->m_Coin[j]->m_iStackPosIndex == NextIdx ) && ( m_pPlayer->m_iStackPosCount[ NextIdx ] == 1 ) )
										  {
											  m_pPlayer->m_pCoinManager->m_Coin[j]->m_StartX    = m_vCoinStackPos[ i ].x;
											  m_pPlayer->m_pCoinManager->m_Coin[j]->m_TargetX   = m_vCoinStackPos[ i - 1 ].x;
											  m_pPlayer->m_pCoinManager->m_Coin[j]->m_bCoinMove = true;	
											  break;
										  }
									 }
								}
							}
						}
					}
				}
			}	
		}
		else
		{
			m_eCointStatus=eCoinNull;
//			m_pPlayer -> m_iStackPosCount[0] = 0;
		}
	} //end if
	//
	if( m_nMonsterId>=0 )
	{
		if( m_nMonsterId==6 || m_nMonsterId==12 || m_nMonsterId==13 )
		{
			if( m_vRot.z >= 90 && m_vRot.z<270 )
			{

			}
			else
			{

			} //end if
			//
			m_nTimerBigWinShow3++;
			if( m_nTimerBigWinShow3%3==0 )
			{
				m_nTimerBigWinShow3 = 0;
				//
//m_iMoney = 99;
				if( m_nBigWinShow3_Coin_Current<m_iMoney )
				{
					int l_nAddNum = 1;
					if( m_iMoney<9 )
					{
						l_nAddNum = 1;
					}
					else if( m_iMoney<99 )
					{
						l_nAddNum = 1 + rand()%3;
					}
					else if( m_iMoney<999 )
					{
						l_nAddNum = m_iMoney/(123 + rand()%9);
					}
					else if( m_iMoney<9999 )
					{
						l_nAddNum = m_iMoney/(123 + rand()%9);
					}
					else if( m_iMoney<99999 )
					{
						l_nAddNum = m_iMoney/(123 + rand()%9);
					}
					else if( m_iMoney<999999 )
					{
						l_nAddNum = m_iMoney/(123 + rand()%9);
					}
					else if( m_iMoney<9999999 )
					{
						l_nAddNum = m_iMoney/(123 + rand()%9);
					}
					else
					{
						l_nAddNum = m_iMoney/(123 + rand()%9);
					} //end if
					if( l_nAddNum<1 )
					{
						l_nAddNum = 1;
					} //end if
					m_nBigWinShow3_Coin_Current += l_nAddNum;
					//
					if( m_nBigWinShow3_Coin_Current>m_iMoney )
					{
						m_nBigWinShow3_Coin_Current = m_iMoney;
					} //end if
					if( m_nBigWinShow3_Coin_Current>99999999 )
					{
						m_nBigWinShow3_Coin_Current = 99999999;
					} //end if
				} //end if
				//
			} //end if
			//
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
void cCoin::Render(void)
{

	if(m_eCointStatus==eCoinScore)
	{

//m_nMonsterId = 5;
		if( m_nMonsterId>=0 )
		{
/**/
			if( m_nMonsterId==1 || m_nMonsterId==7 )
			{
				if( m_pMPDI_BigWinShow1_D->IsDone() )
				{
					m_pMPDI_BigWinShow1_Loop->Render();
				}
				else
				{
					m_pMPDI_BigWinShow1_D->Render();
				} //end if
			}
			else if( m_nMonsterId==5 )
			{
				if( m_pMPDI_BigWinShow2_D->IsDone() )
				{
					m_pMPDI_BigWinShow2_Loop->Render();
				}
				else
				{
					m_pMPDI_BigWinShow2_D->Render();
				} //end if
			}//end if
			//
			if( m_nMonsterId==1 || m_nMonsterId==7 ||
				m_nMonsterId==5 )
			{
				for( int ii=0; ii<m_nHowManyScore; ii++ )
				{
					m_pMPDI_BonusShow_ScorePos_U[ii]->Render();
				} //next ii
			} //end if
			//
		} //end if

		if( m_nMonsterId==1 || m_nMonsterId==7 ||
			m_nMonsterId==5 )
		{
		}
		else
		{
			for(int i = 0; i < m_iHowManyScore; ++i)
			{
				m_pScoreImage[i]->Render();
			} //next i
		} //end if
	}
	else if(m_eCointStatus==eCoinMove)
	{

//m_nMonsterId = 5;
		if( m_nMonsterId>=0 )
		{
			/**/
			if( m_nMonsterId==1 || m_nMonsterId==7 )
			{
				m_pMPDI_BigWinShow1_Loop->Render();
				if( m_pMPDI_BigWinShow1_Loop->GetScale()==1.0f )
				{
					for( int ii=0; ii<m_nHowManyScore; ii++ )
					{
						m_pMPDI_BonusShow_ScorePos_U[ii]->Render();
					} //next ii
				} //end if
			}
			else if( m_nMonsterId==5 )
			{
				m_pMPDI_BigWinShow2_Loop->Render();
				if( m_pMPDI_BigWinShow2_Loop->GetScale()==1.0f )
				{
					for( int ii=0; ii<m_nHowManyScore; ii++ )
					{
						m_pMPDI_BonusShow_ScorePos_U[ii]->Render();
					} //next ii
				} //end if
			} //end if
			//
		} //end if


		if(m_iCoinIndex == 0 || m_iCoinIndex == 3)
		{
			for(int i = 0; i < cPlayerBehaviorBase::COINPOPUP_COUNT-5 ; ++i)
			{
				m_pCoinImage[i]->RenderByGlobalTime();
			}
		}
		else
		{
			for(int i = 0; i < cPlayerBehaviorBase::COINPOPUP_COUNT; ++i)
			{
				m_pCoinImage[i]->RenderByGlobalTime();
			}
		}
		
	//m_Curve.RenderCurve();
	}
	else if(m_eCointStatus == eCoinStack)
	{
		if(m_iStackPosIndex>=0)
		{
			if( m_bAccess && m_pPlayer ->m_iStackPosCount[m_iStackPosIndex] == 1 )
			{
				m_CoinStackList.Render();
			}
		}
	} //end if
	//
	if( m_nMonsterId>=0 )
	{
		if( m_nMonsterId==6 || m_nMonsterId==12 || m_nMonsterId==13 )
		{

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
void cCoin::Destroy(void)
{
	for(int i = 0; i < cPlayerBehaviorBase::COINPOPUP_COUNT; ++i)
	{
		m_pCoinImage[i];

		SAFE_DELETE(m_pSilverCoinImage_D[i] );
		SAFE_DELETE(m_pSilverCoinImage_R[i] );
		SAFE_DELETE(m_pSilverCoinImage_U[i] );
		SAFE_DELETE(m_pSilverCoinImage_L[i] );

		SAFE_DELETE(m_pGoldCoinImage_D[i] );
		SAFE_DELETE(m_pGoldCoinImage_R[i] );
		SAFE_DELETE(m_pGoldCoinImage_U[i] );
		SAFE_DELETE(m_pGoldCoinImage_L[i] );


	} //next i
	//
	for(int i = 0; i < MAXIUM_SCORE_NUMBER; ++i)
	{
		SAFE_DELETE( m_pScoreImage[i] );
	}

	for( int ii=0; ii<MAXIUM_SCORE_NUMBER; ii++ )
	{
		SAFE_DELETE( m_pMPDI_BonusShow_ScorePos_U[ii] );
	} //next ii
	SAFE_DELETE( m_pMPDI_BigWinShow1_D );
	SAFE_DELETE( m_pMPDI_BigWinShow1_Loop );
	SAFE_DELETE( m_pMPDI_BigWinShow2_D );
	SAFE_DELETE( m_pMPDI_BigWinShow2_Loop );

	SAFE_DELETE( m_pNI_BigWinShow3_Score );
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
/*
cCoinStack::cCoinStack(void)
{

}

cCoinStack::~cCoinStack(void)
{

}

void	cCoinStack::ShowCoinStack()
{

}

void	cCoinStack::Init(void)
{

}

void	cCoinStack::Update(float e_fElpaseTime)
{

}

void	cCoinStack::Render(void)
{

}

void	cCoinStack::Destroy(void)
{

}*/