/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "../EngineTest/MiniGameBase.h"
#include "MiniGameStage.h"

#include "../EngineTest/GameApp.h"
#include "../EngineTest/FishProbability.h"
#include "../EngineTest/SceneChange.h"
#include "../Monster/FishGroup.h"
//
cClickMouseBehavior *g_Key_l;
cClickMouseBehavior *g_Key_F1;
cClickMouseBehavior *g_Key_F2;
cClickMouseBehavior *g_Key_F3;
cClickMouseBehavior *g_Key_F4;
cClickMouseBehavior *g_Key_F5;
//
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameManager::cMiniGameManager(void)
{
	m_nTimes_1=0;
	g_Key_l = new cClickMouseBehavior;
	g_Key_l->SetKeyData('L');
	g_Key_F1 = new cClickMouseBehavior;
	g_Key_F1->SetKeyData('s');
	g_Key_F2 = new cClickMouseBehavior;
	g_Key_F2->SetKeyData('t');
	g_Key_F3 = new cClickMouseBehavior;
	g_Key_F3->SetKeyData('u');
	g_Key_F4 = new cClickMouseBehavior;
	g_Key_F4->SetKeyData('v');
	g_Key_F5 = new cClickMouseBehavior;
	g_Key_F5->SetKeyData('x');
	//

}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameManager::~cMiniGameManager(void)
{
	delete g_Key_l;
	delete g_Key_F1;
	delete g_Key_F2;
	delete g_Key_F3;
	delete g_Key_F4;
	delete g_Key_F5;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameManager::Init(void)
{
	for( int ii=0; ii<COUNT_PLATE; ii++ )
	{
		cMiniGameBase *l_pGame = 0;
		if( ii<10 )
		{
			l_pGame = new cMiniGameStage01();
		}
		else if( ii<20 )
		{
			l_pGame = new cMiniGameStage02();
		}
		else if( ii<30 )
		{
			l_pGame = new cMiniGameStage03();
		}
		else if( ii<40 )
		{
			l_pGame = new cMiniGameStage04();
		} //end if
		this->AddObjectNeglectExist( l_pGame );
		l_pGame->Init();
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
void	cMiniGameManager::Update( float e_fElpaseTime )
{
//
	//g_Key_l->SingnalProcess();
	//g_Key_l->Update(e_fElpaseTime);
	//g_Key_F1->SingnalProcess();
	//g_Key_F1->Update(e_fElpaseTime);
	//g_Key_F2->SingnalProcess();
	//g_Key_F2->Update(e_fElpaseTime);
	//g_Key_F3->SingnalProcess();
	//g_Key_F3->Update(e_fElpaseTime);
	//g_Key_F4->SingnalProcess();
	//g_Key_F4->Update(e_fElpaseTime);
	//g_Key_F5->SingnalProcess();
	//g_Key_F5->Update(e_fElpaseTime);
//	
//	if( g_Key_l->IsSatisfiedCondition() )
//	{
//		g_Key_l->Init();
//		//
//		int l_nType = m_nTimes_1%13;
//		m_nTimes_1++;
//		
//
//
//		if( m_nTimes_1>1 )
//		{
//			cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"",false );
//			cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"",false );
//			cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"",false );
//			//cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"" );
//			//cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"" );
//			return;
//		} //end if
//
//
//		//魚種的id0~12, miniGame 名字
//		//旋風彈
//		cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"BonusTornado",false );
//		cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"BonusTornado",false );
//
//		//魚種的id0~12, miniGame 名字
//		//落雷砲(殺相同的魚)
////		cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"BonusThunderBomb" );
////		cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"BonusThunderBomb" );
//
//		//魚種的id0~12, miniGame 名字
//		//冷凍彈
////		cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"BonusIceBomb" );
////		cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"BonusIceBomb" );
//
//		//魚種的id0~12, miniGame 名字
//		//雷霆炸彈(拉霸)
////		cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"BonusBomb" );
////		cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( l_nType, L"BonusBomb" );
//	}
//	else 
	//if( g_Key_F1->IsSatisfiedCondition() )
	//{
	//	g_Key_F1->Init();
	//	//
	//	//旋風彈
	//	cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( 0, L"BonusTornadoX",false );
	//}
	//else if( g_Key_F2->IsSatisfiedCondition() )
	//{
	//	g_Key_F2->Init();
	//	//
	//	//落雷彈
	//	cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( 0, L"BonusThunderBombX" ,false);
	//}
	//else if( g_Key_F3->IsSatisfiedCondition() )
	//{
	//	g_Key_F3->Init();
	//	//
	//	//冷凍彈
	//	cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( 0, L"BonusIceBombX" ,false);
	//}
	//else if( g_Key_F4->IsSatisfiedCondition() )
	//{
	//	g_Key_F4->Init();
	//	//
	//	//雷霆彈
	//	cFishApp::m_spFishProbability->MonsterGenerateWithMiniGame( 0, L"BonusBombX",false );
	//}
	//else if( g_Key_F5->IsSatisfiedCondition() )
	//{
	//	g_Key_F5->Init();
	//	//
	//	f_GoTo_MiniGame_05();
	//	//
	//} //end if
//
	//
	for( int ii=0; ii<this->Count(); ii++ )
	{
		this->GetObject(ii)->Update(e_fElpaseTime);
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameManager::Render(void)
{
	for( int ii=0; ii<this->Count(); ii++ )
	{
		this->GetObject(ii)->Render();
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameManager::Render2(void)
{
	for( int ii=0; ii<this->Count(); ii++ )
	{
		this->GetObject(ii)->Render2();
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameManager::Destroy(void)
{
	for( int ii=0; ii<this->Count(); ii++ )
	{
		this->GetObject(ii)->Destroy();
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cMiniGameManager::f_GoTo_MiniGame_05(void)
{
//	cFishApp::m_spSceneChange->m_pFishGroup2->f_PrizeII_Call( 10.0f );
	cFishApp::m_spMonsterManager->BossOut();
} 

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cMiniGameBase*	cMiniGameManager::f_RequireSpecificMiniGame( WCHAR* e_strName, cMonster* e_pMonster )
{
	int l_nX1 = 0;
	int l_nX2 = 0;
	bool l_bAutoExplode = false;
	//旋風彈
	if( !wcscmp(e_strName,L"BonusTornado") )
	{
		l_nX1 = 0*COUNT_PLATE/4;
		l_nX2 = 1*COUNT_PLATE/4;
	}
	else if( !wcscmp(e_strName,L"BonusTornadoX") )
	{
		l_nX1 = 0*COUNT_PLATE/4;
		l_nX2 = 1*COUNT_PLATE/4;
		l_bAutoExplode = true;
	}
	//落雷砲
	else if( !wcscmp(e_strName,L"BonusThunderBomb") )
	{
		l_nX1 = 1*COUNT_PLATE/4;
		l_nX2 = 2*COUNT_PLATE/4;
	}
	else if( !wcscmp(e_strName,L"BonusThunderBombX") )
	{
		l_nX1 = 1*COUNT_PLATE/4;
		l_nX2 = 2*COUNT_PLATE/4;
		l_bAutoExplode = true;
	}
	//冷凍彈
	else if( !wcscmp(e_strName,L"BonusIceBomb") )
	{
		l_nX1 = 2*COUNT_PLATE/4;
		l_nX2 = 3*COUNT_PLATE/4;
	}
	else if( !wcscmp(e_strName,L"BonusIceBombX") )
	{
		l_nX1 = 2*COUNT_PLATE/4;
		l_nX2 = 3*COUNT_PLATE/4;
		l_bAutoExplode = true;
	}
	//雷霆炸彈
	else if( !wcscmp(e_strName,L"BonusBomb") )
	{
		l_nX1 = 3*COUNT_PLATE/4;
		l_nX2 = 4*COUNT_PLATE/4;
	}
	else if( !wcscmp(e_strName,L"BonusBombX") )
	{
		l_nX1 = 3*COUNT_PLATE/4;
		l_nX2 = 4*COUNT_PLATE/4;
		l_bAutoExplode = true;
	} //end if
	//
	for( int ii=l_nX1; ii<l_nX2; ii++ )
	{
		eMiniGameFlag abc = this->GetObject(ii)->f_GetFlag();
		if( this->GetObject(ii)->f_GetFlag() == MINI_GAME_FLAG_NONE )
		{
			this->GetObject(ii)->m_pStickyMonster = e_pMonster;
			this->GetObject(ii)->m_bAutoExplode = l_bAutoExplode;
			this->GetObject(ii)->f_SetFlag( MINI_GAME_FLAG_STICKY_MONSTER ); //一定要放在最後面，因為 SetFlag 裡面會自動 Update 一次。
			break;
		} //end if
	} //end if
	//
	return 0;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
bool	cMiniGameManager::IsMiniGameUsing()
{
	int l_nX1 = COUNT_PLATE;
	int l_nX2 = COUNT_PLATE;
	//
	for( int ii=l_nX1; ii<l_nX2; ii++ )
	{
		eMiniGameFlag abc = this->GetObject(ii)->f_GetFlag();
		if( this->GetObject(ii)->f_GetFlag() != MINI_GAME_FLAG_NONE )
		{
			return true;
		} //end if
	} //end if
	return false;
}
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
bool cMiniGameManager::IsMiniGameUsing(WCHAR*e_strName)
{
	int l_nX1 = 0;
	int l_nX2 = 0;
	//旋風彈
	if( !wcscmp(e_strName,L"BonusTornado") )
	{
		l_nX1 = 0*COUNT_PLATE/4;
		l_nX2 = 1*COUNT_PLATE/4;
	}
	else if( !wcscmp(e_strName,L"BonusTornadoX") )
	{
		l_nX1 = 0*COUNT_PLATE/4;
		l_nX2 = 1*COUNT_PLATE/4;
	}
	//落雷砲
	else if( !wcscmp(e_strName,L"BonusThunderBomb") )
	{
		l_nX1 = 1*COUNT_PLATE/4;
		l_nX2 = 2*COUNT_PLATE/4;
	}
	else if( !wcscmp(e_strName,L"BonusThunderBombX") )
	{
		l_nX1 = 1*COUNT_PLATE/4;
		l_nX2 = 2*COUNT_PLATE/4;
	}
	//冷凍彈
	else if( !wcscmp(e_strName,L"BonusIceBomb") )
	{
		l_nX1 = 2*COUNT_PLATE/4;
		l_nX2 = 3*COUNT_PLATE/4;
	}
	else if( !wcscmp(e_strName,L"BonusIceBombX") )
	{
		l_nX1 = 2*COUNT_PLATE/4;
		l_nX2 = 3*COUNT_PLATE/4;
	}
	//雷霆炸彈
	else if( !wcscmp(e_strName,L"BonusBomb") )
	{
		l_nX1 = 3*COUNT_PLATE/4;
		l_nX2 = 4*COUNT_PLATE/4;
	}
	else if( !wcscmp(e_strName,L"BonusBombX") )
	{
		l_nX1 = 3*COUNT_PLATE/4;
		l_nX2 = 4*COUNT_PLATE/4;
	} //end if
	//
	for( int ii=l_nX1; ii<l_nX2; ii++ )
	{
		eMiniGameFlag abc = this->GetObject(ii)->f_GetFlag();
		if( this->GetObject(ii)->f_GetFlag() != MINI_GAME_FLAG_NONE )
		{
			return true; //只要找到有一個在使用, 就跳出去
		} //end if
	} //end if

	return false;
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