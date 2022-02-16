/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "Bullet.h"
#include "MonsterBase.h"
#include "GameApp.h"
#include "FishProbability.h"
#include "../Monster/TurtleBoss.h"
/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
bool	cBulletManager::m_sbCollideSkip = false;
cBulletManager::cBulletManager()
{
	m_iNumWorking = 0;
	m_BulletCollisionData.SetFromResource(true);;
	m_WebCollisionData.SetFromResource(true);
	m_ppUpdateBulletData = 0;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cBulletManager::~cBulletManager()
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
void		cBulletManager::Destroy()
{
	m_2DImageCollisionData.Destroy();
	m_BulletCollisionData.Destroy();
	m_WebCollisionData.Destroy();
	SAFE_DELETE(m_ppUpdateBulletData);
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
int	cBulletManager::f_Collide_Bullet_MiniGame( cMonsterManager* e_pMonsterManager, cBullet* e_pBullet, std::vector<cMonster*> *e_pMonsterList, cPlayerBehaviorBase*e_pPlayerBehaviorBase )
{
	cBullet*		l_pBullet		= e_pBullet;
	eBulletStatus	l_eBulletStatus = l_pBullet->m_eBulletStatus;
	bool	l_bHitted = false;
	int l_nTotalScore = 0;
	//
	for( int i=0;i<e_pMonsterManager->m_iNumMonsterAlive;++i )
	{
		cMonster*l_pMonster = e_pMonsterManager->m_ppWorkingMonster[i];
		if( !l_pMonster->IsStatusAllowToDied() )
			continue;
		//
		l_pBullet->m_pWebCllision->SetTransform( l_pBullet->m_pBulletCollision->GetTransform() );
		if( l_pMonster->IsCollide( l_pBullet->m_pWebCllision ) )
		{
			bool l_bIsRepeat = false;
			int l_nCount1 = e_pMonsterList->size();
			for( int ii=0; ii<l_nCount1; ii++ )
			{
				if( (*e_pMonsterList)[ii]->GetMonsterStatus()==eMS_CONTROL_BY_MINI_GAME || (*e_pMonsterList)[ii]->GetUniqueID()==l_pMonster->GetUniqueID() )
				{
					l_bIsRepeat = true;
					break;
				} //end if
			} //next ii
			if( l_bIsRepeat==false )
			{
				int l_nScore;
				bool l_bIsResult = cFishApp::m_spFishProbability->IsMiniGameIsMonsterDied( l_pMonster, e_pBullet->m_pMiniGame->m_nPayRate*e_pBullet->m_pMiniGame->m_nPayRateBullet, e_pPlayerBehaviorBase, &l_nScore );
				if( l_bIsResult==true )
				{
					l_nTotalScore += l_nScore;
				} //end if
				e_pMonsterList->push_back( l_pMonster );
				//碰撞到這個魚，要不要撥放特效？
			} //end if
			//
			/*
				l_pMonster->SetMonsterStatus(eMS_HITTED);
				sHittedFishData*l_pHittedFishData = &e_pMonsterManager->m_pHittedFish[e_pMonsterManager->m_iNumMonsterHitted];
				++l_pBullet->m_iNumMonsterShoot;
				l_pHittedFishData->pBullet = l_pBullet;
				l_pHittedFishData->pMonster = l_pMonster;
				l_pHittedFishData->pPlayer = l_pBullet->m_pPlayer;
				++e_pMonsterManager->m_iNumMonsterHitted;
				//immediately to dead
				//l_pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
				l_bHitted = true;
				*/
		} //end if
	} //next i
	//
	return l_nTotalScore;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void		cBulletManager::Collide(cMonsterManager*e_pMonsterManager)
{
	m_sbCollideSkip = false;
	//static	UINT	l_sui = 0;
	//++l_sui;
	//if( l_sui%3  )
	//{
		//m_sbCollideSkip = true;
		//return;
	//}
	e_pMonsterManager->m_iNumMonsterHitted = 0;
	for(int j=0;j<this->m_iNumWorking;++j)
	{
		cBullet*l_pBullet = this->m_ppUpdateBulletData[j];
		//mini game skip
		if( !l_pBullet->m_pBulletCollision || l_pBullet->m_pMiniGame )
			continue;
		eBulletStatus	l_eBulletStatus = l_pBullet->m_eBulletStatus;
		bool	l_bHitted = false;
		for( int i=0;i<e_pMonsterManager->m_iNumMonsterAlive;++i )
		{
			cMonster*l_pMonster = e_pMonsterManager->m_ppWorkingMonster[i];
			//skip monster has been controlled by mini game.
			if( l_pMonster->IsStatusAllowToDied() )
			{
				if( l_eBulletStatus == eBS_MOVING && l_pMonster->IsCollide(l_pBullet->m_pBulletCollision) )
				{
					l_pMonster->AddBiteMoney(l_pBullet->m_iPayRate);
					l_pMonster->SetMonsterStatus(eMS_HITTED);
					l_bHitted = true;
					break;
				}
				else
				if( l_eBulletStatus == eBS_EXPLOSION_DONE && l_pMonster->IsCollide(l_pBullet->m_pWebCllision) )
				{
					//if( e_pMonsterManager->m_iNumMonsterHitted < e_pMonsterManager->m_iMaxMonsterAlive )
					{
						l_pMonster->SetMonsterStatus(eMS_HITTED);
						sHittedFishData*l_pHittedFishData = &e_pMonsterManager->m_pHittedFish[e_pMonsterManager->m_iNumMonsterHitted];
						++l_pBullet->m_iNumMonsterShoot;
						l_pHittedFishData->pBullet = l_pBullet;
						l_pHittedFishData->pMonster = l_pMonster;
						l_pHittedFishData->pPlayer = l_pBullet->m_pPlayer;
						++e_pMonsterManager->m_iNumMonsterHitted;
						//immediately to dead
						//l_pMonster->SetMonsterStatus(eMS_WAITING_FOR_CLEAN);
						l_bHitted = true;
						if( cFishApp::m_sbSchoolerMasterLiProbability )
							break;
					}
				}
			}
		}
		if( !l_bHitted )
		{
			if( e_pMonsterManager->m_pTurtleBoss&&e_pMonsterManager->m_pTurtleBoss->GetMonsterStatus() != eMS_NONE )
			{
				l_bHitted = e_pMonsterManager->m_pTurtleBoss->IsCollide(l_pBullet->m_pBulletCollision);
				if( l_bHitted )
				{
					e_pMonsterManager->m_pTurtleBoss->m_iCurrentLeastBiteMoney += l_pBullet->GetPayRate();
					e_pMonsterManager->m_pTurtleBoss->SetMonsterStatus(eMS_HITTED);
					if(e_pMonsterManager->m_pTurtleBoss->IsBiteEnoughMoney())
					{
						e_pMonsterManager->m_pTurtleBoss->m_pHittedBullet = l_pBullet;
						e_pMonsterManager->m_pTurtleBoss->m_pHittedPlayer = l_pBullet->m_pPlayer;
					}
				}
			}
		}
		if( l_bHitted || l_eBulletStatus == eBS_EXPLOSION_DONE )
		{
			if( l_eBulletStatus == eBS_MOVING )
			{
				l_pBullet->m_eBulletStatus = eBS_EXPLOSION;
				l_pBullet->CastNetFishing();
				l_pBullet->m_iNumMonsterShoot = 0;
				cGameApp::SoundPlay(L"4",true);
			}
			else
			if( l_eBulletStatus == eBS_EXPLOSION_DONE )
			{
				//prepare to reuse
				l_pBullet->Init();
				--m_iNumWorking;
				m_ppUpdateBulletData[j] = m_ppUpdateBulletData[m_iNumWorking];
			}
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
cBullet*	cBulletManager::Require( int e_iPlayerIndex, int e_iLevel, int e_iPayRate )
{
	cPlayerBullet*l_pPlayerBullet = this->GetObject(e_iPlayerIndex);
	if( l_pPlayerBullet )
	{
		//產生砲彈的種類
		cBullet*l_pBullet = l_pPlayerBullet->Require( e_iLevel );
		//
		if( l_pBullet )
		{
			l_pBullet->m_iPayRate = e_iPayRate;

			//砲彈的種類
			l_pBullet->m_iPowerLevel = e_iLevel;
			//
			//change to player to call this cFishApp::m_spFishProbability->RevenueAdd(l_pBullet->m_iPayRate);
			this->m_ppUpdateBulletData[this->m_iNumWorking] = l_pBullet;
			l_pBullet->Init();
			++m_iNumWorking;
			cGameApp::SoundPlay(L"3",true);
		}
		return l_pBullet;
	}
	return 0;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBulletManager::Init()
{
	m_iNumWorking = 0;
	this->Destroy();
	if( this->Count() == 0 )
	{
		this->ParseWithMyParse("Fish/Bullet/Bullet.xml");
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBulletManager::Update(float e_fElpaseTime)
{
	for( int i=0;i<m_iNumWorking;++i )
	{
		m_ppUpdateBulletData[i]->Update(e_fElpaseTime);
		if( m_ppUpdateBulletData[i]->m_eBulletStatus == eBS_DONE )
		{
			--m_iNumWorking;
			m_ppUpdateBulletData[i]->m_eBulletStatus = eBS_IDLE;
			m_ppUpdateBulletData[i] = m_ppUpdateBulletData[m_iNumWorking];
			
		} //end if
	}
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBulletManager::Render()
{
	for( int i=0;i<m_iNumWorking;++i )
	{
		m_ppUpdateBulletData[i]->Render();
	} //end if
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBulletManager::DebugRender()
{
	for( int i=0;i<m_iNumWorking;++i )
	{
		m_ppUpdateBulletData[i]->DebugRender();
	} //end if
}

//<Root>
//	<CollisionFile FileName="" />
//	<Bullet MPDIList="00.mpdi" MovingMPDI="Moving" >
//		<LV1 Name=""/>
//	</Bullet>
//	<Web MPDIList="00.mpdi" ExlposionMPDI="Explosion">
//		<LV1 Name=""/>
//	</Web>
//</Root>

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBulletManager::ProcessCollisionFile(TiXmlElement*e_pRoot)
{
	const WCHAR*l_strFileName = e_pRoot->Attribute(L"FileName");
	if( l_strFileName )
	{
		if(!m_2DImageCollisionData.Parse(UT::WcharToChar(l_strFileName)))
		{
			UT::ErrorMsg(l_strFileName,L"parse failed");
		}
	}
}
//	<PlayerBullet MPDIList="00.mpdi" MovingMPDI="Moving" >
//		<LV1 Name=""/>
//	</PlayerBullet>

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBulletManager::ProcessPlayerBullet(TiXmlElement*e_pRoot)
{
	const WCHAR*l_strMPDIList = e_pRoot->Attribute(L"MPDIList");
	const WCHAR*l_strIndex = e_pRoot->Attribute(L"Index");
	if( l_strIndex )
	{
		if(  _wtoi(l_strIndex) >= cFishApp::m_spControlPanel->m_iPlayerCount )
			return;
	}
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strMPDIList);
	cPlayerBullet*l_pPlayerBullet = new cPlayerBullet();
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		cPlayerBullet::sBulletData*l_pBulletData = new cPlayerBullet::sBulletData();
		PARSE_ELEMENT_START(e_pRoot)
			COMPARE_NAME("BulletCollision")
			{
				cbtConcaveShapeList*l_pbtConcaveShapeList = m_2DImageCollisionData.GetObject(l_strValue);
				if( l_pbtConcaveShapeList )
				{
					l_pBulletData->m_pBulletCollision = l_pbtConcaveShapeList;
					this->m_WebCollisionData.AddObjectNeglectExist(l_pbtConcaveShapeList);
				}
				else
				{
					UT::ErrorMsg(l_strValue,L"can not find collision data");
				}
			}
			else
			COMPARE_NAME("BulletMPDI")
			{
				l_pBulletData->m_pBulletMPDI = l_pMPDIList->GetObject(l_strValue);
			}
			else
			COMPARE_NAME("Speed")
			{
				l_pBulletData->m_fSpeed = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("WebMPDI")
			{
				l_pBulletData->m_pWebMPDI = l_pMPDIList->GetObject(l_strValue);;
			}
			else
			COMPARE_NAME("WebCollsiion")
			{
				cbtConcaveShapeList*l_pbtConcaveShapeList = m_2DImageCollisionData.GetObject(l_strValue);
				if( l_pbtConcaveShapeList )
				{
					l_pBulletData->m_pbtWebCollision = l_pbtConcaveShapeList;
					this->m_WebCollisionData.AddObjectNeglectExist(l_pbtConcaveShapeList);
				}
				else
				{
					UT::ErrorMsg(l_strValue,L"can not find collision data");
				}
			}
		PARSE_NAME_VALUE_END
		l_pPlayerBullet->AddBulletData(l_pBulletData);
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	l_pPlayerBullet->ProcessBullet(m_iEachPlayerLimitCount);
	this->AddObjectNeglectExist(l_pPlayerBullet);
}


/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cBulletManager::ProcessWeb(TiXmlElement*e_pRoot)
{
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		const WCHAR*l_strColisionName = e_pRoot->Attribute(L"Name");
		cbtConcaveShapeList*l_pbtConcaveShapeList = m_2DImageCollisionData.GetObject(l_strColisionName);
		if( l_pbtConcaveShapeList )
		{
			m_BulletCollisionData.AddObjectNeglectExist(l_pbtConcaveShapeList);
		}
		else
		{
			UT::ErrorMsg(l_strColisionName,L"can not find collision data");
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
}


/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
bool	cBulletManager::MyParse(TiXmlElement*e_pRoot)
{
	Destroy();
	const WCHAR*l_strEachPlayerLimitCount = e_pRoot->Attribute(L"EachPlayerLimitCount");
	m_iEachPlayerLimitCount = 50;
	if( l_strEachPlayerLimitCount )
		m_iEachPlayerLimitCount = _wtoi(l_strEachPlayerLimitCount);
	e_pRoot = e_pRoot->FirstChildElement();
	while( e_pRoot )
	{
		const WCHAR*l_strName = e_pRoot->Value();
		COMPARE_NAME("CollisionFile")
		{
			ProcessCollisionFile(e_pRoot);
		}
		else
		COMPARE_NAME("PlayerBullet")
		{
			ProcessPlayerBullet(e_pRoot);
		}
		else
		COMPARE_NAME("Web")
		{
			ProcessWeb(e_pRoot);
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	int	l_iCount = this->Count();
	if( l_iCount > 0 )
	{
		cPlayerBullet*l_pPlayerBullet = this->GetObject(0);
		int	l_iLevel = l_pPlayerBullet->m_BulletDataList.size();
		m_ppUpdateBulletData = new cBullet*[m_iEachPlayerLimitCount*l_iCount*l_iLevel];
	}
	return true;
}
