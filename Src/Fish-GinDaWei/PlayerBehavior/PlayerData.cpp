/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "PlayerData.h"
#include "../EngineTest/GameApp.h"
#include "../../Core/GamePlayUT/Event/AllEventInclude.h"
#include "../ControlPanel/AllControlPanelInclude.h"
#include <direct.h>
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cPlayerData::cPlayerData( TiXmlElement*e_pElement )
{
	m_fAutoFireTime=0.2f;
	m_bAutoPlay = false;
	m_iAutoScore = 0;
	m_iAutoLevelNumber = 10;
	m_ChangeLevelOnce = false;
	m_AutoAddScore = true;
	m_fTubeAngle = 0.0f;
	m_iLevel = 0;
	m_bReturnCoin = false;
	m_bNoCoin = false;
	
	m_pMessageExplode = 0;
	m_pMessageIOError = 0;
	m_pMessageInsert = 0;
	m_pMessageNoMoney = 0;
	m_pPlayerMPDIList = 0 ;
	m_pPlayerImage = 0 ;
	m_pPlayerImage2 = 0;
	m_pTubeMPDIList= 0;
	m_pLevelNumberImage= 0;
	m_pScoreNumberImage= 0;
	m_pCurrentMiniGame = 0;
	m_pCoinManager=0;

	m_pCoinManager=new cCoinManager();
	for(int i=0;i<cCoinManager::COUNT_COINS;++i)
	{
		m_pCoinManager->m_Coin[i]->m_eCointStatus = eCoinNull;
	}
	
	for(int i = 0; i < COINSTACKNUM; ++i)
	{
		m_iStackPosCount[i]=0;
	}


	m_pPlayerDataMiniGameCardList = new cPlayerDataMiniGameCardList();

	m_vPlayerRot.z = 0.0f;
	m_fTubeTime=0.5f;
	m_fTubeAngle = 180 - ( m_fTubeTime*180 + m_vPlayerRot.z );
	
	m_piMaxLevelNumber = 0;
	m_piAddLevelNumber = 0;
	m_piCoinToScore =	0;
	m_pScoreLimit = 0;
	m_piMinScoreBet = 0;
	m_NowAddPos = 0;
	m_Coin_StackStartIdx = 0;

	ProcessPlayer(e_pElement);
	ProcessPlayerLevel();
	ProcessPlayerAutoData();
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cPlayerData::~cPlayerData(void)
{
	SAFE_DELETE(m_pCoinManager);
	m_pPlayerDataMiniGameCardList->SetFromResource(true);
	SAFE_DELETE( m_pPlayerDataMiniGameCardList );
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::AddScore2( Vector3 Pos, cMonster* Fish, int Money )
{	
	Vector3 l_FishPos[COINPOPUP_COUNT ];
	float l_fCoinDisatnce =30 ;
	
	for(int i = 0; i < COINPOPUP_COUNT; ++i)
	{
		if(m_vPlayerRot.z == 0.0)
		{
			if(i<COINPOPUP_COUNT /2)
				l_FishPos[i]=Pos + Vector3( l_fCoinDisatnce *i, 0, 0 );
			else
				l_FishPos[i]=Pos + Vector3( l_fCoinDisatnce *(i-5), l_fCoinDisatnce, 0 );
		}
		else if(m_vPlayerRot.z == 270.0)
		{
			if(i<COINPOPUP_COUNT /2)
				l_FishPos[i]=Pos + Vector3( 0, -l_fCoinDisatnce *i, 0 );
			else
				l_FishPos[i]=Pos + Vector3( -l_fCoinDisatnce, -l_fCoinDisatnce *(i-5), 0 );
		}
		else if(m_vPlayerRot.z == 180.0)
		{
			if(i<COINPOPUP_COUNT /2)
				l_FishPos[i]=Pos + Vector3( -l_fCoinDisatnce *i, 0, 0 );
			else
				l_FishPos[i]=Pos + Vector3( -l_fCoinDisatnce *(i-5), -l_fCoinDisatnce, 0 );
		}
		else if(m_vPlayerRot.z == 90.0)
		{
			if(i<COINPOPUP_COUNT /2)
				l_FishPos[i]=Pos + Vector3( 0, l_fCoinDisatnce *i, 0 );
			else
				l_FishPos[i]=Pos + Vector3( l_fCoinDisatnce, l_fCoinDisatnce *(i-5), 0 );
		}
	}
	
	int l_iStackPosIndex = -1;

	for(int i = 0; i < COINSTACKNUM; ++i)
	{
		if( m_iStackPosCount[i] == 0 && i == m_NowAddPos )
		{
			l_iStackPosIndex = i; 
			break;
		}
	}
//	if( m_iStackPosCount[1] >=1 && m_iStackPosCount[2] >=1 && m_iStackPosCount[3] >=1 && m_iStackPosCount[4] >=1 )
//	{
//		l_iStackPosIndex = 0;
//	}

	if( l_iStackPosIndex != -1 && m_iAddScoreCount <= 50 )
	{
		for(int i=0;i< cCoinManager::COUNT_COINS;++i)
		{
			if(m_pCoinManager->m_Coin[i]->m_eCointStatus==eCoinNull && Money > 0)
			{
				m_iCointCount++;		
				m_pCoinManager->m_Coin[i]->m_iStackIndex = m_pCoinManager->StackIndex(Money);
				m_pCoinManager->m_Coin[i]->m_iCoinIndex	= m_pCoinManager->CoinIndex(Fish->GetPrize());
				m_pCoinManager->m_Coin[i]->m_nMonsterId = Fish->GetID();
				//std::string	l_sss = UT::ComposeMsgByFormat("Stack:%d",l_iStackPosIndex);
				//l_sss += UT::ComposeMsgByFormat("--%d",m_iStackPosCount[0]);
				//l_sss += UT::ComposeMsgByFormat("--%d",m_iStackPosCount[1]);
				//l_sss += UT::ComposeMsgByFormat("--%d",m_iStackPosCount[2]);
				//l_sss += UT::ComposeMsgByFormat("--%d",m_iStackPosCount[3]);
				//l_sss += UT::ComposeMsgByFormat("--%d",m_iStackPosCount[4]);
				//std::wstring l_ss2 = UT::CharToWchar(l_sss.c_str());
				//OutputDebugString(l_ss2.c_str());
				//OutputDebugString(L"\n");
				m_pCoinManager->m_Coin[i]->ShowCoin( this, l_FishPos, Money,l_iStackPosIndex);
				//m_iAddScoreCount++;

				m_NowAddPos++;

				if ( m_NowAddPos >= COINSTACKNUM )
					m_NowAddPos = 0;

				break;
			}
		}
	}
	else
	{
		this->m_i64Score += Money;
		m_iAddScoreCount--;
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::ProcessPlayerLevel(void)
{
	cNodeISAX	l_NodeISAX;
	bool l_b = l_NodeISAX.ParseDataIntoXMLNode(LEVEL_DATA);
	if(l_b)
	{
		TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
		const WCHAR*l_strCount   = l_pElement->Attribute(L"LevelCount");
		m_iLevelCount = _wtoi(l_strCount);
		l_pElement = l_pElement->FirstChildElement();
		const WCHAR*l_strName = l_pElement->Value();	

		while(l_pElement)
		{
			l_strName = l_pElement->Value();
			COMPARE_NAME("Level")
			{
				const WCHAR*l_strMin   = l_pElement->Attribute(L"Min");
				this->m_vLevelMin.push_back(_wtoi(l_strMin));
				const WCHAR*l_strMax   = l_pElement->Attribute(L"Max");
				this->m_vLevelMax.push_back(_wtoi(l_strMax));
				const WCHAR*l_strIndex = l_pElement->Attribute(L"Index");
				this->m_vLevelIndex.push_back(_wtoi(l_strIndex));
			}
			l_pElement = l_pElement->NextSiblingElement();
		}
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::ProcessPlayerAutoData(void)
{
	cNodeISAX	l_NodeISAX;
	bool l_b = l_NodeISAX.ParseDataIntoXMLNode(PLAYERAUTO_DATA);
	if( l_b )
	{	
		m_ChangeLevelOnce = true;
		TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
		l_pElement = l_pElement->FirstChildElement();
		const WCHAR*l_strName = l_pElement->Value();
		while(l_pElement)
		{
			l_strName = l_pElement->Value();
			COMPARE_NAME("PlayerAutoData")
			{
				const WCHAR*l_strAutoFireTime   = l_pElement->Attribute(L"AutoFireTime");
				m_fAutoFireTime = (float)_wtof(l_strAutoFireTime);
				const WCHAR*l_strLogScore   = l_pElement->Attribute(L"AutoScore");
				m_iAutoScore = GetInt64(l_strLogScore );
				const WCHAR*l_strLogLevelNumber = l_pElement->Attribute(L"AutoLevelNumber");
				m_iAutoLevelNumber = _wtoi(l_strLogLevelNumber);
				const WCHAR*l_strAuto = l_pElement->Attribute(L"AutoPlay");
				m_bAutoPlay= _wtoi(l_strAuto)?true:false;
				const WCHAR*l_strAddScore = l_pElement->Attribute(L"AutoAddScore");
				m_AutoAddScore = _wtoi(l_strAddScore)?true:false;
			}
			l_pElement = l_pElement->NextSiblingElement();
		}
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::ProcessPlayer( TiXmlElement*e_pElement )
{
	TiXmlElement*l_pTiXmlElement = e_pElement->FirstChildElement();
	const WCHAR*l_strName = l_pTiXmlElement->Value();
	while(l_pTiXmlElement)
	{
		l_strName = l_pTiXmlElement->Value();
		COMPARE_NAME("PlayerData")
		{
			this->ProcessPlayerData(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("IO")
		{
			this->ProcessPlayerIO(l_pTiXmlElement);
		}
		else
		COMPARE_NAME("Weapon")
		{
			this->ProcessPlayerUI(l_pTiXmlElement);
		}
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::ProcessPlayerData( TiXmlElement*e_pElement )
{
	const WCHAR*l_strPosition = e_pElement->Attribute(L"Position");
	this->m_vPlayerPos=GetVector3(l_strPosition);
	
	const WCHAR*l_strRotation = e_pElement->Attribute(L"Rotation");
	this->m_vPlayerRot=GetVector3(l_strRotation);
	m_fTubeAngle = 180 - ( m_fTubeTime*180 + m_vPlayerRot.z );
}	

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::ProcessPlayerUI( TiXmlElement*e_pElement )
{
	const WCHAR*l_strUI = e_pElement->Attribute(L"MPDIList");
	m_pPlayerMPDIList = cGameApp::GetMPDIListByFileName(l_strUI);
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::ProcessPlayerIO( TiXmlElement*e_pElement )
{
	const WCHAR*l_strRight = e_pElement->Attribute(L"Right");
	this->m_cInput_Right= (unsigned char)(l_strRight[0]);

	const WCHAR*l_strLeft = e_pElement->Attribute(L"Left");
	this->m_cInput_Left= (unsigned char)(l_strLeft[0]);

	const WCHAR*l_strFire = e_pElement->Attribute(L"Fire");
	this->m_cInput_Fire= (unsigned char)(l_strFire[0]);
	m_FireClick.SetKeyData(m_cInput_Fire);
	m_FireClick.SetPressedRepeatMouseUp(true);

	const WCHAR*l_strLevel = e_pElement->Attribute(L"Level");
	this->m_cInput_Level= (unsigned char)(l_strLevel[0]);
	m_LevelClick.SetKeyData(m_cInput_Level);
	m_LevelClick.SetPressedRepeatMouseUp(true);

	const WCHAR*l_strInsert = e_pElement->Attribute(L"Insert");
	this->m_cInput_Insert= (unsigned char)(l_strInsert[0]);
	m_InsertClick.SetKeyData(m_cInput_Insert);
	m_InsertClick.SetPressedRepeatMouseUp(true);
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::Init(void)
{

	m_piMaxLevelNumber = &cFishApp::m_spControlPanel->m_iBulletPayRateLimit;
	m_piAddLevelNumber = &cFishApp::m_spControlPanel->m_iBulletPayRateAddPoint;
	m_piCoinToScore =	&cFishApp::m_spControlPanel->m_iCoinToScore;
	m_pScoreLimit = &cFishApp::m_spControlPanel->m_i64MoneyLimit;
	m_piMinScoreBet = &cFishApp::m_spControlPanel->m_iBulletPayRateMinPoint;


	m_fDieChangeLevelTime = 10;
	m_iCointCount = 0;
	m_iAddScoreCount = 0;
	this->m_iLevelNumber = *m_piAddLevelNumber;
	//this->m_iLevel = 1;
	this->m_eStatus = PLAYER_STATUS_IDLE;
	m_eGameStage = GAME_STAGE_NORMAL;
	this->m_i64Score = m_iAutoScore;
	this->m_bAlive = false;

	m_FireClick.Init();
	m_LevelClick.Init();
	if(!m_pPlayerMPDIList)
	{
		UT::ErrorMsg(L"PlayerUI.mpdi",L"not exit");
	}
	else
	{
		//砲台，取出來之後放到指定位置和方向。
		m_pPlayerImage = new cMultiPathDynamicImage( m_pPlayerMPDIList->GetObject( L"PlayerUI2" ) );
		m_pPlayerImage->Init();

		m_pPlayerImage2 = new cMultiPathDynamicImage( m_pPlayerMPDIList->GetObject( L"PlayerUI3" ) );
		m_pPlayerImage2->Init();
		//
		//m_pPlayerImage->SetAngle(this->m_vPlayerRot);
		//m_pPlayerImage->SetPos(this->m_vPlayerPos);
		//把所有物件抓出來轉向
		m_matPlayerTransfomr = cMatrix44::TranslationMatrix( m_vPlayerPos ) * cMatrix44::RotationMatrix( m_vPlayerRot );
		
		m_matPlayerDirection = cMatrix44::RotationMatrix( m_vPlayerRot );

		for( int i=0;i<m_pPlayerImage->Count();++i )
		{
			m_pPlayerImage->GetObject(i)->SetExtractTransform(&m_matPlayerTransfomr);
			
		} //next i
		//
		for( int i=0;i<m_pPlayerImage2->Count();++i )
		{
			m_pPlayerImage2->GetObject(i)->SetExtractTransform(&m_matPlayerTransfomr);
		}
		//m_pPlayerImage->SetRotateAroundOriginlPoint(true);^
		std::wstring	l_str = UT::ComposeMsgByFormat(L"%dPS",(this->m_iPlayerID+1));
		int l_iIndexPlayerUI = m_pPlayerImage->GetPuzzleImage(L"PlayerUI")->GetObjectIndexByName(l_str.c_str());
		m_pPlayerImage->GetObject(L"1P")->ChangeAllImageByImageIndex(l_iIndexPlayerUI);

		cMultiPathDynamicImage *l_pAllPlayerPos = m_pPlayerMPDIList->GetObject(L"PlayerUI1");
		
		Vector3 l_vLvPos = l_pAllPlayerPos->GetObject(L"TubeLVNumerial1")->GetOriginalPointList()[0];
		Vector3 l_vScorePos = l_pAllPlayerPos->GetObject(L"PlayerScoreNumerial")->GetOriginalPointList()[0];


		//Vector3 l_vTubePos= m_pPlayerImage->GetObject(L"TubePos")->GetOriginalPointList()[0];
		Vector3 l_vTubePos= m_pPlayerImage2->GetObject(L"TubePos1")->GetOriginalPointList()[0] + Vector3( 98, 42.5, 0 );
		m_vMessagePos = l_pAllPlayerPos->GetObject(L"MessagePos")->GetOriginalPointList()[0];

		cMatrix44	l_mat = cMatrix44::TranslationMatrix(m_vPlayerPos)*cMatrix44::RotationMatrix(m_vPlayerRot);
		m_vLvPos = l_mat.TransformCoordinate(l_vLvPos);
		m_vScorePos = l_mat.TransformCoordinate(l_vScorePos);
		m_vTubePos = l_mat.TransformCoordinate(l_vTubePos);
		
		
	}
	
	m_pTubeMPDIList= cGameApp::GetMPDIListByFileName((WCHAR*)PLAYER_MPDI);
	if(!m_pTubeMPDIList)
	{
		UT::ErrorMsg(L"Player.mpdi",L"not exit");
	}
	else
	{
		for(int i=1;i< m_iLevelCount+1;++i)
		{
			std::wstring l_strAttackName = UT::ComposeMsgByFormat(L"Attack%d",i);
			cMultiPathDynamicImage*l_pAttack = new cMultiPathDynamicImage( m_pTubeMPDIList->GetObject(l_strAttackName.c_str()));

			if(l_pAttack)
			{
				m_AttackList.AddObjectNeglectExist(l_pAttack);
			}
			else
			{
				UT::ErrorMsg(L"Player.mpdi",L"Attack wrong");
			}
		}
		m_AttackList.SetCurrentWorkingObject(0);
	}
	


	cPuzzleImage*l_pNumber = cGameApp::GetPuzzleImageByFileName((WCHAR*)PLAYER_NUMRIAL);
	if(l_pNumber)
	{
		m_pLevelNumberImage=l_pNumber->GetNumerialImageByName(L"numeral5-0",L"numeral5-9");
		m_pLevelNumberImage->SetAngle(m_vPlayerRot.z);
		m_pScoreNumberImage = l_pNumber->GetNumerialImageByName(L"numeral1-0",L"numeral1-9");
		m_pScoreNumberImage->SetAngle(m_vPlayerRot.z);
	}
	else
	{
		UT::ErrorMsg(L"PlayerUI.pi",L"not exit");
	}


	cMPDIList* l_pMessageMPDIList=cGameApp::GetMPDIListByFileName((WCHAR*)MINI_GAME_MESSAGE);
	m_pMessageInsert = new cMultiPathDynamicImage( l_pMessageMPDIList ->GetObject(L"PleaseInsertCoin") );
	m_pMessageNoMoney = new cMultiPathDynamicImage( l_pMessageMPDIList ->GetObject(L"MoneyNotEnough") );
	m_pMessageExplode = new cMultiPathDynamicImage( l_pMessageMPDIList ->GetObject(L"MoneyLimit") );
	m_pMessageIOError = new cMultiPathDynamicImage( l_pMessageMPDIList ->GetObject(L"ConnectionError") );
	
	
	m_matMessageTransfomr = m_matPlayerTransfomr * cMatrix44::TranslationMatrix(m_vMessagePos);

	for( int ii=0; ii<m_pMessageInsert->Count(); ii++ )
	{
		m_pMessageInsert->GetObject(ii)->SetExtractTransform( &m_matMessageTransfomr );
	} //next ii
	
	for( int ii=0; ii<m_pMessageInsert->Count(); ii++ )
	{
		m_pMessageNoMoney->GetObject(ii)->SetExtractTransform( &m_matMessageTransfomr );
	} //next ii

	for( int ii=0; ii<m_pMessageInsert->Count(); ii++ )
	{
		m_pMessageExplode->GetObject(ii)->SetExtractTransform( &m_matMessageTransfomr );
	} //next ii

	for( int ii=0; ii<m_pMessageInsert->Count(); ii++ )
	{
		m_pMessageIOError->GetObject(ii)->SetExtractTransform( &m_matMessageTransfomr );
	} //next ii

	LvUp(false);
	m_pMessageInsert->Init();
	m_pMessageNoMoney->Init();
	m_pMessageExplode->Init();
	m_pMessageIOError ->Init();
	m_pCoinManager->Init();
	
	m_AutoFireTC.SetTargetTime(m_fAutoFireTime);
	m_AutoFireTC.Start();
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::Update( float e_fElpaseTime )
{
	if( this->m_eStatus == PLAYER_STATUS_IDLE )
	{

	}
	else
	{
		m_AttackList.Update(e_fElpaseTime);
		if( m_AttackList.GetCurrentWorkingObject()->IsDone() )
		{
				this->m_eStatus = PLAYER_STATUS_IDLE;
		} //end if
	} //end if

	if( m_i64Score >= *m_pScoreLimit)
	{
		m_pMessageExplode->Update(e_fElpaseTime);
	}
	if(g_pIOSMDriverInterface->IsIOError())
	{
		m_pMessageIOError->Update(e_fElpaseTime);
	}

	if( this->m_i64Score <*m_piAddLevelNumber)
	{
		this->m_bAlive=false;
		if(m_iCointCount <= 0 )
		{
			if(m_fDieChangeLevelTime < 0)
			{
				m_iLevelNumber = *m_piAddLevelNumber;
				LvUp(false);
				m_pMessageInsert->Update(e_fElpaseTime);
			}
			else
			{
				m_fDieChangeLevelTime -= e_fElpaseTime;
			}
		}
	}
	else
	{
		this->m_bAlive=true;
		m_fDieChangeLevelTime = 10 ;
	}
	if(this->m_iLevelNumber > *m_piMaxLevelNumber)
	{
		this->m_iLevelNumber = *m_piMaxLevelNumber;
	}
	if(m_bNoCoin)
	{
		m_pMessageNoMoney->Update(e_fElpaseTime);
	}

	//AutoRun SaveLog
	if(cGameApp::m_sucKeyData[106])
	{
		if(m_bAutoPlay)
		{	
			m_bAutoPlay = false; 
			m_ChangeLevelOnce =false;	
		}
		else
		{
			m_bAutoPlay = true;	
			m_ChangeLevelOnce =false;
		}//end if
	}//end if

	//
	if( cGameApp::m_sucKeyData[this->m_cInput_Left])
	{
		cGameApp::SoundPlay(L"2",true);
		if(m_fTubeTime>0.1f)
		{
			m_fTubeTime -= m_fRotateSpeed*e_fElpaseTime;
			m_fTubeAngle = 180 - ( m_fTubeTime*180 + m_vPlayerRot.z );
		}
		else
		{
			m_fTubeTime=0.1f;
		}//end if
	} //end if

	if( cGameApp::m_sucKeyData[this->m_cInput_Right])
	{
		cGameApp::SoundPlay(L"2",true);
		if(m_fTubeTime<0.9f)
		{
			m_fTubeTime += m_fRotateSpeed*e_fElpaseTime;
			m_fTubeAngle = 180 - ( m_fTubeTime*180 + m_vPlayerRot.z );
		}
		else
		{
			m_fTubeTime=0.9f;
		}
	} //end if
	//
	m_FireClick.SingnalProcess();
	m_FireClick.Update(e_fElpaseTime);



	if( m_FireClick.IsSatisfiedCondition() )
	{
		m_FireClick.Init();
		DoFire();
	} //end if

	m_LevelClick.SingnalProcess();
	m_LevelClick.Update(e_fElpaseTime);
	if(m_LevelClick.IsSatisfiedCondition())
	{
		m_LevelClick.Init();
		LvUp(true);
	}
	

	if( m_eGameStage==GAME_STAGE_MINIGAME_ING )
	{
		int l_nCount01 = m_pPlayerDataMiniGameCardList->Count();
		if( m_pPlayerDataMiniGameCardList->Count()>0 )
		{
			for( int ii=0; ii<m_pPlayerDataMiniGameCardList->Count(); ii++ )
			{
//				eMiniGameFlag abc = m_pPlayerDataMiniGameCardList->GetObject(ii)->m_nFlag;
				if( ii==0 &&  
					m_pPlayerDataMiniGameCardList->GetObject(ii)->f_GetFlag() == MINI_GAME_FLAG_GOTO_CARD_01 )
				{
					m_pPlayerDataMiniGameCardList->GetObject(ii)->f_SetFlag( MINI_GAME_FLAG_GOTO_SHOW_MESSAGE_ING );
				}
				else if( m_pPlayerDataMiniGameCardList->GetObject(ii)->f_GetFlag() == MINI_GAME_FLAG_GOTO_FINISH_99 )
				{
					m_pPlayerDataMiniGameCardList->GetObject(ii)->f_SetFlag( MINI_GAME_FLAG_NONE );
					m_pPlayerDataMiniGameCardList->RemoveObjectWithoutDelete(ii); //刪掉一張卡片
					if( m_pPlayerDataMiniGameCardList->Count()>0 )
					{
						m_pCurrentMiniGame = m_pPlayerDataMiniGameCardList->GetObject(0);
					}
					else
					{
						m_pCurrentMiniGame = 0;
						break; //跳出
					} //end if
				} //end if
				//
				if( ii<4 )
				{
					cMPDI* l_pCards = m_pPlayerDataMiniGameCardList->GetObject(ii)->m_pMPDI_Card01;
					for( int kk=0; kk<l_pCards->Count(); ++kk )
					{
						l_pCards->GetObject(kk)->SetExtractTransform( &m_matPlayerTransfomr );
					} //next kk
					//
					
					//取得小卡片位置，可是 GetOriginalPointList 是取得原始位置，沒有和砲台一起轉向？
					std::wstring l_szObject = UT::ComposeMsgByFormat( L"BonusHint%d", ii+1 );
					Vector3 l_vCardPos = m_pPlayerMPDIList->GetObject( L"PlayerUI1" )->GetObject( l_szObject.c_str() )->GetOriginalPointList()[0];
					l_pCards->SetPos( l_vCardPos );
					//
					l_pCards->Update(e_fElpaseTime);
				} //end if
				//

			} //next ii
		}
		else
		{
			m_eGameStage = GAME_STAGE_NORMAL;
		} //end if
	} //end if

	//AutoPlay
	if(	m_bAutoPlay	)
	{
		m_AutoFireTC.Update(e_fElpaseTime);
		if(m_ChangeLevelOnce)
		{
			m_iLevelNumber = m_iAutoLevelNumber;
			LvUp(false);
			m_ChangeLevelOnce = false;
		}
		if(m_AutoAddScore)
		{
			if(this->m_i64Score <= *m_piAddLevelNumber)
			{
				this->m_i64Score =m_iAutoScore;
			}
		}
		if(m_AutoFireTC.bTragetTimrReached)
		{
			if( this->m_eStatus == PLAYER_STATUS_IDLE && this->m_i64Score >= *m_piAddLevelNumber)
			{
				m_fTubeTime =	rand(0.0f,1.0f);
			}
			DoFire();
			m_AutoFireTC.SetTargetTime(m_fAutoFireTime);
			m_AutoFireTC.Start();	
		}
	}
	//
	m_pPlayerImage->Update(e_fElpaseTime);
	m_pPlayerImage2->UpdateByGlobalTime(m_fTubeTime);

	m_pCoinManager->Update(e_fElpaseTime);
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::Render(void)
{
	
	if( m_pCurrentMiniGame==0 || 
		m_pCurrentMiniGame->f_GetStage()==MINI_GAME_STAGE_04 ||
		(m_pCurrentMiniGame->f_GetFlag()<MINI_GAME_FLAG_GOTO_THUNDER_IDLE || m_pCurrentMiniGame->f_GetFlag()>MINI_GAME_FLAG_GOTO_THUNDER_FIRE_ED) )
	{

		m_pPlayerImage->Render();
		if( this->m_eStatus == PLAYER_STATUS_IDLE )
		{
			m_pPlayerImage2->RenderByGlobalTime();
		}
		else
		{	
			//m_pPlayerImage->GetObject( L"Fort" )->Render();
			//m_pPlayerImage->GetObject( L"PlayerUI" )->Render();
			m_AttackList.Render();
			m_pPlayerImage2->GetObject( L"DrainCover" )->Render();
			
		} //end if

		if(m_pLevelNumberImage)
		{
			m_pLevelNumberImage->DrawOnCenter( this->m_iLevelNumber, (int)m_vLvPos.x, (int)m_vLvPos.y );
		} //end if

		if(m_pScoreNumberImage)
		{
			m_pScoreNumberImage->Draw( this->m_i64Score, (int)m_vScorePos.x, (int)m_vScorePos.y );
		} //end if
	}
	else
	{
		this->m_iPlayerID = this->m_iPlayerID;
	} //end if
	

	if( m_eGameStage==GAME_STAGE_MINIGAME_ING )
	{
		int l_nCount01 = m_pPlayerDataMiniGameCardList->Count();
		if( m_pPlayerDataMiniGameCardList->Count()>0 )
		{
			for( int ii=0; ii<m_pPlayerDataMiniGameCardList->Count(); ii++ )
			{
				if( ii<4 )
				{
					cMPDI* l_pCards = m_pPlayerDataMiniGameCardList->GetObject(ii)->m_pMPDI_Card01;
					l_pCards->Render(); //放這邊好嗎？
				} //end if
			} //next ii
		} //end if
	} //end if

	if( this->m_i64Score <*m_piAddLevelNumber || m_bAlive == false )
	{
		if(m_iCointCount <= 0 )
		{
			if(m_fDieChangeLevelTime < 0)
			{
				m_pMessageInsert->Render();
			}
		}
	}
	if( m_i64Score >= *m_pScoreLimit)
	{
		m_pMessageExplode->Render();
	}

	if(g_pIOSMDriverInterface->IsIOError())
	{
		m_pMessageIOError->Render();
	}
	
	if(m_bNoCoin)
	{
		m_pMessageNoMoney->Render();
	}
	
	m_pCoinManager->Render();

}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::Destroy(void)
{
	m_pCoinManager->Destroy();

	SAFE_DELETE(m_pPlayerImage2);
	SAFE_DELETE(m_pPlayerImage);
	SAFE_DELETE(m_pLevelNumberImage);
	SAFE_DELETE(m_pScoreNumberImage);
	SAFE_DELETE(m_pMessageExplode);
	SAFE_DELETE(m_pMessageIOError);
	SAFE_DELETE(m_pMessageInsert);
	SAFE_DELETE(m_pMessageNoMoney);

}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 發射砲彈
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cBullet* cPlayerData::f_Fire_MiniGame( cMiniGameBase* e_pMiniGame, float e_fFireDirection )
{
	//cPuzzleImageUnit*	l_pPIUnit = this->m_pPlayerImage->GetPuzzleImage(L"PlayerUI")->GetObject(L"TubePos1");

	//this->m_iLevel：0(一發)，1(兩發)，2(三發)，3(四發)。
	cBullet* l_pMiniGameBullet = cFishApp::m_pBulletManager->Require( this->m_iPlayerID, this->m_iLevel, this->m_iLevelNumber );
	if( l_pMiniGameBullet )
	{
		l_pMiniGameBullet->m_pMiniGame = e_pMiniGame;

		f_Fire( l_pMiniGameBullet, e_fFireDirection );
	}
	else
	{
		int a=0;
	}
	return l_pMiniGameBullet;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 發射砲彈
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void cPlayerData::Fire(void)
{
	//cPuzzleImageUnit*	l_pPIUnit = this->m_pPlayerImage->GetPuzzleImage(L"PlayerUI")->GetObject(L"TubePos1");

	//this->m_iLevel：0(一發)，1(兩發)，2(三發)，3(四發)。
	cBullet* l_pBullet = cFishApp::m_pBulletManager->Require( this->m_iPlayerID, this->m_iLevel, this->m_iLevelNumber );

	f_Fire( l_pBullet, -999 );
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 發射砲彈
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void cPlayerData::f_Fire( cBullet* e_pBullet, float e_fFireDirection )
{
	this->m_eStatus = PLAYER_STATUS_ATTACK; //更改為發射狀態

	//
	cMatrix44	l_mat = m_pPlayerImage2->GetObject( L"TubePos1" )->GetRenderMatrix(); //取得砲座位置
	Vector3	l_vPos = l_mat.GetAxis( MyMath::W );
	//
	if( e_pBullet )
	{
		cFishApp::m_spFishProbability->RevenueAdd(e_pBullet->GetPayRate(),this->m_iPlayerID);
		cGameApp::SoundPlay(L"3",true);
		this->m_i64Score -= this->m_iLevelNumber;
		m_fTubeAngle = 180 - ( m_fTubeTime*180 + m_vPlayerRot.z );
		Vector3	l_vDirection = AngleToDirection( 360 - m_fTubeAngle );
		Vector3 l_vOffsetDirection;
		if(m_iLevel == 0)
		{
			l_vDirection *= 90;
			l_vOffsetDirection = Vector3(0,0,-0.6f)^l_vDirection.Normalize();
			l_vOffsetDirection *= 15;
		}
		else if(m_iLevel == 1)
		{
			l_vDirection *= 90;
			l_vOffsetDirection = Vector3(0,0,-1)^l_vDirection.Normalize();
			l_vOffsetDirection *= 15;
		}
		else if(m_iLevel == 2)
		{
			l_vDirection *= 100;
			l_vOffsetDirection = Vector3(0,0,-1)^l_vDirection.Normalize();
			l_vOffsetDirection *= 20;
		}
		else
		{
			l_vDirection *= 120;
			l_vOffsetDirection = Vector3(0,0,-0.9f)^l_vDirection.Normalize();
			l_vOffsetDirection *= 30;
		}
		Vector3 l_vFirePos = l_mat.GetAxis(MyMath::W) + l_vDirection +l_vOffsetDirection;
		//
		//發射子彈
		float l_fFireDirection = 0.0f;
		if( e_fFireDirection>-999 ) //-999 表是用發射台的角度
		{
			l_fFireDirection = e_fFireDirection;
		}
		else
		{
			l_fFireDirection = m_fTubeAngle;
		} //end if
		e_pBullet->Fire( this, l_vFirePos, l_fFireDirection );
		//
	} //end if
	//
	m_AttackList.SetCurrentWorkingObject( this->m_iLevel );
	Vector3 l_vAttackAngle = Vector3( 0, 0, 270 + (m_fTubeTime*180) );
	m_matTubeTransfomr = m_matPlayerTransfomr * cMatrix44::TranslationMatrix(Vector3(-45.5,-18.0,0)) * cMatrix44::RotationMatrix(l_vAttackAngle);
	for( int ii=0; ii<m_AttackList.GetCurrentWorkingObject()->Count(); ii++ )
	{
		m_AttackList.GetCurrentWorkingObject()->GetObject(ii)->SetExtractTransform( &m_matTubeTransfomr );
	} //next ii
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::LvUp(bool e_bInsert)
{
	
	if(e_bInsert){
		cGameApp::SoundPlay(L"1",true);
		m_fDieChangeLevelTime  = 10;
		if(this->m_iLevelNumber < *m_piMaxLevelNumber)
			this->m_iLevelNumber += *m_piAddLevelNumber;
		else
			this->m_iLevelNumber = *m_piAddLevelNumber;
	}
	//Vector3 l_vNowAngle;
	for(int i=0; i < m_iLevelCount ; ++i){
		if(this->m_iLevelNumber>=this->m_vLevelMin[i] && this->m_iLevelNumber<=this->m_vLevelMax[i])
		{
			this->m_iLevel=this->m_vLevelIndex[i];
		}
	}
	
	
	std::wstring	l_str = UT::ComposeMsgByFormat( L"LV%d-2", (this->m_iLevel+1) );
	int l_iIndexPlayerUI = m_pPlayerImage2->GetPuzzleImage( L"PlayerUI" )->GetObjectIndexByName( l_str.c_str() );
	m_pPlayerImage2->GetObject(L"TubePos1")->ChangeAllImageByImageIndex( l_iIndexPlayerUI );
	m_pPlayerImage2->GetObject(L"TubePos1")->GetCurrentPointData()->iImageIndex = l_iIndexPlayerUI;
	m_pPlayerImage2->UpdateByGlobalTime(m_fTubeTime+0.1f);
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
int cPlayerData::f_GoToMinigame( cMiniGameBase* e_pMiniGame )
{
	m_pPlayerDataMiniGameCardList->AddObjectNeglectExist( e_pMiniGame );
	//
	if( m_eGameStage==GAME_STAGE_MINIGAME_ING )
	{ //已經在執行 mini game 了
		return 11;
	}
	else
	{ //進入 minigame
		m_pCurrentMiniGame = e_pMiniGame;
		m_eGameStage = GAME_STAGE_MINIGAME_ING;
		return 22;
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerData::DoFire(void)
{
	//按下發射紐，如果是在 mini game 模式下，就要削掉一片卡片
	if(cFishApp::m_sbSceneChange == false && m_bReturnCoin == false )
	{
		if( m_eGameStage==GAME_STAGE_MINIGAME_ING )
		{
			if( m_pCurrentMiniGame )
			{
				m_pCurrentMiniGame->f_Fire();
			} //end if
		}
		else
		{
			if(	m_bAlive && m_iLevelNumber >= *m_piMinScoreBet)
			{
				if(this->m_iLevelNumber > this->m_i64Score)
				{
					if(this->m_i64Score% (*m_piAddLevelNumber) == 0)
					{
						this->m_iLevelNumber = (int)this->m_i64Score;
					}
					else
					{
						int l_iTempScore =	(int)(this->m_i64Score/ (*m_piAddLevelNumber));
						this->m_iLevelNumber = l_iTempScore* (*m_piAddLevelNumber);
					} //end if
					LvUp(false);
				} //end if
				Fire();
			}
		} //end if
	} //end if
}