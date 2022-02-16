/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "Fish.h"
#include "MultiCollisionFish.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/FileNameDefine.h"
#include "../EngineTest/FishProbability.h"
#include "../EngineTest/PlayerBehaviorBase.h"
#include "../EngineTest/Bullet.h"
#include "FishGroup.h"
#include "TurtleBoss.h"
#include "../../Core/GamePlayUT/Event/AllEventInclude.h"
#include "../ControlPanel/AllControlPanelInclude.h"
/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cMonsterManager::cMonsterManager()
{
	m_iNumMonsterHitted = 0;
	m_pHittedFish = 0;//amount is m_iMaxMonsterAlive
	m_iMaxMonsterAlive = 60;
	m_iNumMonsterAlive = 0;
	m_spFAIBehaviorParser = new cFAIBehaviorParser;
	m_ppWorkingMonster = 0;
	m_pTurtleBoss = 0;
	m_pAllFishGroupFile = 0;
	m_pMultiFishGroup = 0;
	m_iTotalAliveMonsterPrize = 0;
	memset(m_iCurrentBodyTypeCount,0,eBT_MAX*sizeof(int));
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cMonsterManager::~cMonsterManager()
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
cMonster*	cMonsterManager::ProcessMonster(TiXmlElement*e_pTiXmlElement,const WCHAR*l_strTypeID)
{
	cMonster*l_pMonster = 0;
	if( !wcscmp(l_strTypeID,cNormalFish::TypeID) )
	{
		l_pMonster = new cNormalFish(e_pTiXmlElement);
	}
	else
	if( !wcscmp(l_strTypeID,cMultiCollisionDataFish::TypeID) )
	{
		l_pMonster = new cMultiCollisionDataFish(e_pTiXmlElement);
	}
	return l_pMonster;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
//<Monster Name="Test" Type="Normal">
bool	cMonsterManager::MyParse(TiXmlElement*e_pRoot)
{
	m_iMaxMonsterAliveWithPlayerCount.clear();
	PARSE_ELEMENT_START(e_pRoot)
		POINT	l_Table = VALUE_TO_POINT;
		this->m_iMaxMonsterAliveWithPlayerCount[l_Table.x] = l_Table.y;
	TO_NEXT_VALUE
	m_iMaxMonsterAlive = m_iMaxMonsterAliveWithPlayerCount[cFishApp::m_spControlPanel->m_iPlayerCount];
	if( m_iMaxMonsterAlive <= 0 || m_iMaxMonsterAlive >79979 )
		m_iMaxMonsterAlive = 100;
	SAFE_DELETE(m_pHittedFish);
	//generate plaers count multiply each player's bullet
	//int	l_iTotalHittedFish = cFishApp::m_spPlayerManager->Count()*cFishApp::m_pBulletManager->GetEachPlayerLimitCount();
	//give a big amount....FUCK
	m_pHittedFish = new sHittedFishData[3000];	
	e_pRoot = e_pRoot->FirstChildElement();
	while(e_pRoot)
	{
		std::wstring	l_strMonsterName;
		int	l_iID = -1;
		if( !wcscmp(e_pRoot->Value(),L"FishBodyTypeAmount"))
		{
			TiXmlAttribute*l_pTiXmlAttribute = e_pRoot->FirstAttribute();
			eBodyType	l_eBodyType = GetBodyType(l_pTiXmlAttribute->Name());
			int	l_iCount = _wtoi(l_pTiXmlAttribute->Value());
			this->m_iMaxBodyTypeCount[l_eBodyType] = l_iCount;
		}
		else
		if( !wcscmp(e_pRoot->Value(),L"FishGroupFile"))
		{
			const WCHAR*l_strFileName = e_pRoot->Attribute(L"FileName");
			if( l_strFileName )
			{
				SAFE_DELETE(m_pMultiFishGroup);
				m_pMultiFishGroup = new cFishGroup(UT::WcharToChar(l_strFileName));
			}
		}
		else
		if( !wcscmp(e_pRoot->Value(),L"Monster"))
		{
			cMonster*l_pMonster = 0;
			eBodyType	l_eBodyType = eBT_SMALLE;
			int	l_iID = this->Count();
			PARSE_ELEMENT_START(e_pRoot)
				COMPARE_NAME("Name")
				{
					l_strMonsterName = l_strValue;
				}
				else
				COMPARE_NAME("Type")
				{
					l_pMonster = ProcessMonster(e_pRoot,l_strValue);
					if( l_pMonster )
					{
						l_pMonster->SetName(l_strMonsterName.c_str());
						l_pMonster->m_iID = l_iID;
						if(!this->AddObject(l_pMonster))
						{
							UT::ErrorMsg(l_pMonster->GetName(),L"exists");
							delete l_pMonster;
						}
					}
				}
				else
				COMPARE_NAME("ID")
				{
					l_iID = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("MonsterReserve")
				{
					const WCHAR*l_strFishCount = e_pRoot->Attribute(L"MonsterReserve");
					if( l_strFishCount )
					{
						m_iEachMonsterReserve[l_iID] = _wtoi(l_strFishCount);
					}
					else
					{
						m_iEachMonsterReserve[l_iID] = 10;
					}
				}
				else
				COMPARE_NAME("FishGroupFile")
				{
					cFishGroup*l_pFishGroup = new cFishGroup(UT::WcharToChar(l_strValue));
					if(!m_pAllFishGroupFile->AddObject(l_pFishGroup))
					{
						UT::ErrorMsg(l_strValue,L"this group exists!!");
					}
					l_pFishGroup->SetName(l_strMonsterName.c_str());
				}
				else
				COMPARE_NAME("BodyType")
				{
					l_eBodyType = GetBodyType(l_strValue);
					l_pMonster->SetBodyType(l_eBodyType);
				}
				else
				COMPARE_NAME("Scale")
				{
					l_pMonster->m_fScale = VALUE_TO_FLOAT;
				}
			PARSE_NAME_VALUE_END
		}
		e_pRoot = e_pRoot->NextSiblingElement();
	}
	return true;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMonsterManager::Init()
{
	this->Destroy();
	m_iTotalAliveMonsterPrize = 0;
	m_pAllFishGroupFile = new cObjectListByName<cFishGroup>;
	m_spFAIBehaviorParser = new cFAIBehaviorParser;
	bool	l_b = this->ParseWithMyParse(MONSTER_DATA);
	if( !l_b )
	{
		UT::ErrorMsg(MONSTER_DATA,"parse failed");
	}
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cMonster*l_pCloneMonster = dynamic_cast<cMonster*>(this->GetObject(i));
		cObjectListByName<cMonster>*l_pFishList = new cObjectListByName<cMonster>;
		l_pFishList->SetName(l_pCloneMonster->GetName());
		m_AllFish.AddObjectNeglectExist(l_pFishList);
		l_pCloneMonster->CollisionDataCheck();
		int	l_iReserveCount = this->m_iEachMonsterReserve[i];
		std::wstring	l_str;
		for( int j=0;j<l_iReserveCount;++j )
		{
			cMonster*l_pMonster = dynamic_cast<cMonster*>(l_pCloneMonster->Clone());
			l_str = UT::ComposeMsgByFormat(L"%s%d",l_pMonster->GetName(),j);
			l_pMonster->SetName(l_str.c_str());
			l_pFishList->AddObjectNeglectExist(l_pMonster);
		}
	}
	m_ppWorkingMonster = new cMonster*[this->m_iMaxMonsterAlive];
	for( int i=0;i<m_iMaxMonsterAlive;++i )
		m_ppWorkingMonster[i] = 0;
	//m_pTurtleBoss = new cTurtleBoss();
	//this->BossOut();
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMonsterManager::Update(float e_fElpaseTime)
{
	memset(m_iCurrentBodyTypeCount,0,eBT_MAX*sizeof(int));
	m_iTotalAliveMonsterPrize = 0;
	for( int i=0;i<m_iNumMonsterAlive;++i )
	{
		cMonster*l_pMonster = m_ppWorkingMonster[i];
		l_pMonster->Update(e_fElpaseTime);
		if(l_pMonster->m_eMonsterStatus == eMS_WAITING_FOR_CLEAN )
		{
			--m_iNumMonsterAlive;
			m_ppWorkingMonster[i] = m_ppWorkingMonster[m_iNumMonsterAlive];
			l_pMonster->SetMonsterStatus(eMS_NONE);
		}
		m_iCurrentBodyTypeCount[l_pMonster->GetBodyType()] += 1;
		m_iTotalAliveMonsterPrize += l_pMonster->GetPrize();
		//cNormalFish*l_pNormalFish = (cNormalFish*)this->GetObject(i);
		//l_pNormalFish->Update(e_fElpaseTime);
		//if(l_pNormalFish->IsAIDone())
		//{
		//	if(rand()%2)
		//	{
		//		//bool	l_bDirection
		//		Vector3	l_vStartPos = Vector3(-150,frand(0,cGameApp::m_svGameResolution.y),0);
		//		Vector3	l_vEndPos = Vector3(cGameApp::m_svGameResolution.x+10,frand(0,cGameApp::m_svGameResolution.y),0);
		//		l_pNormalFish->SetPath(l_vStartPos,l_vEndPos,frand(100,300));
		//	}
		//	else
		//	{
		//		l_pNormalFish->SetPath();
		//	}
		//	l_pNormalFish->Init();
		//}
	}
	if( m_pTurtleBoss )
		m_pTurtleBoss->Update(e_fElpaseTime);
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMonsterManager::LastRender()
{
	for( int i=0;i<m_iNumMonsterAlive;++i )
	{
		if( m_ppWorkingMonster[i]->GetMonsterStatus() == eMS_DIED_SHOW )
			m_ppWorkingMonster[i]->Render();
	}
}

void	cMonsterManager::Render()
{
	int	l_iIDCount = this->Count();
	for( int j=0;j<l_iIDCount;++j )
	{
		for( int i=0;i<m_iNumMonsterAlive;++i )
		{
			if( m_ppWorkingMonster[i]->GetMonsterStatus() != eMS_DIED_SHOW )
			{
				if( j== m_ppWorkingMonster[i]->GetID() )
					m_ppWorkingMonster[i]->Render();
			}
		}
	}
	if(m_pTurtleBoss)
		m_pTurtleBoss->Render();
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMonsterManager::DebugRender()
{
	for( int i=0;i<m_iNumMonsterAlive;++i )
	{
		m_ppWorkingMonster[i]->DebugRender();
	}
	std::wstring	l_str = UT::ComposeMsgByFormat(L"Fish:%d",m_iNumMonsterAlive);
	cGameApp::m_spGlyphFontRender->RenderFont(0,200,l_str.c_str());
	if( m_pTurtleBoss )
		m_pTurtleBoss->DebugRender();
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cMonsterManager::Destroy()
{
	m_iNumMonsterAlive = 0;
	m_iNumMonsterHitted = 0;
	m_iMaxMonsterAliveWithPlayerCount.clear();
	m_iEachMonsterReserve.clear();
	m_AllFish.Destroy();
	cObjectListByName<cMonster>::Destroy();
	if( m_ppWorkingMonster )
	{
		for( int i=0;i<m_iMaxMonsterAlive;++i )
			m_ppWorkingMonster[i] = 0;
		SAFE_DELETE_ARRAY(m_ppWorkingMonster);
	}
	SAFE_DELETE(m_pHittedFish);
	SAFE_DELETE(m_spFAIBehaviorParser);
	SAFE_DELETE(m_pTurtleBoss);
	SAFE_DELETE(m_pAllFishGroupFile);
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cMonster*	cMonsterManager::Require(int e_iFishIndex,bool e_bForceFetch)
{
	cMonster*l_pMonster = this->GetObject(e_iFishIndex);
	if( l_pMonster )
	{
		return Require(l_pMonster->GetName(),e_bForceFetch);
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
cMonster*	cMonsterManager::Require(const WCHAR*e_strFishName,bool e_bForceFetch)
{
	if(m_iNumMonsterAlive<m_iMaxMonsterAlive)
	{
		//cMonster*l_pMonster = this->GetObject(e_strFishName);
		int	l_iIndex = this->GetObjectIndexByName(e_strFishName);
		if( l_iIndex != -1 )
		{
			cMonster*l_pMonster = 0;
			cObjectListByName<cMonster>*l_pFishList = this->m_AllFish[l_iIndex];
			int	l_iCount = l_pFishList->Count();
			for( int i=0;i<l_iCount;++i )
			{
				l_pMonster = l_pFishList->GetObject(i);
				eBodyType l_eBodyType = l_pMonster->GetBodyType();
				if(this->m_iCurrentBodyTypeCount[l_eBodyType] +1 <=this->m_iMaxBodyTypeCount[l_eBodyType] || e_bForceFetch)
				{
					this->m_iCurrentBodyTypeCount[l_eBodyType] += 1;
					eMonsterStatus	l_eMonsterStatus = l_pMonster->GetMonsterStatus();
					if( l_eMonsterStatus == eMS_NONE )
					{
						this->m_ppWorkingMonster[m_iNumMonsterAlive] = l_pMonster;
						++m_iNumMonsterAlive;
						Vector3	l_vPos = cFishApp::m_spFishProbability->GetRandomPos(l_pMonster);
						l_pMonster->SetPos(l_vPos);
						l_pMonster->Init();
						l_pMonster->SetMonsterStatus(eMS_ALIVE);
						return l_pMonster;
					}
				}
				l_pMonster = 0;
			}
		}
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
int	cMonsterManager::FishGroupGo(WCHAR*e_strFishName)
{
	cFishGroup*l_pFishGroup = this->m_pAllFishGroupFile->GetObject(e_strFishName);
	if( l_pFishGroup )
	{
		int	l_iCount = l_pFishGroup->m_FishGroup.Count();
		if( l_iCount == 0 )
			return 0;
		int	l_iValue = rand()%l_iCount;
		return l_pFishGroup->FishGroupGo(l_iValue);
	}
	return 0;
}

int		cMonsterManager::FishGroupGo()
{
	if(!m_pMultiFishGroup)
		return 0;
	int	l_iCount = m_pMultiFishGroup->m_FishGroup.Count();
	if( l_iCount == 0 )
		return 0;
	int	l_iValue = rand()%l_iCount;
	return m_pMultiFishGroup->FishGroupGo(l_iValue);

}
/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
int	cMonsterManager::FishGroupGo(int e_iFishID,int e_iCount)
{
	int	l_iGoCount = 0;
	float	l_fTimeOffset = 0.5f;
	if(this->m_iMaxMonsterAlive-this->m_iNumMonsterAlive>e_iCount)
	{
		cMonster*l_pFirstMonster = this->Require(e_iFishID);
		if( !l_pFirstMonster )
			return 0;
		Vector3	l_vPos = cFishApp::m_spFishProbability->GetRandomPos(l_pFirstMonster);
		l_pFirstMonster->SetPos(l_vPos);
		l_pFirstMonster->SetPath();
		cCurveWithTime l_Path = *l_pFirstMonster->GetPath();
		Vector3	l_vStartPos = l_Path.GetOriginalPointList()[0];
		std::vector<Vector3>l_PosList = l_Path.GetOriginalPointList();
		std::vector<float>l_TimeList = l_Path.GetOriginalTimeList();
		size_t	l_uiSize = l_PosList.size();
		l_PosList.insert(l_PosList.begin()+1,l_PosList[0]);
		l_TimeList.insert(l_TimeList.begin()+1,l_fTimeOffset);
		l_uiSize = l_PosList.size();
		for(size_t i=2;i<l_uiSize;++i  )
		{
			l_TimeList[i] += l_fTimeOffset;
		}
		e_iCount -= 1;
		for( int i=0;i<e_iCount;++i )
		{
			l_Path.SetOriginalTimeList(l_TimeList);
			l_Path.SetOriginalPointList(l_PosList);
			l_Path.DoLOD();
			cMonster*l_pMonster = this->Require(e_iFishID);
			if( l_pMonster )
			{
				//fuck...performance is fucking slow.because path set twice
				l_pMonster->SetPath();
				l_pMonster->SetPos(l_vPos);
				l_pMonster->SetPath(&l_Path);
			}
			else
				return i;
			for(size_t i=1;i<l_uiSize;++i  )
			{
				l_TimeList[i] += l_fTimeOffset;
			}
		}
		return e_iCount;
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
int		cMonsterManager::GetSmallestPrizeFishID(int*e_piPrize)
{
	int	l_iCount = this->m_AllFish.Count();
	int	l_iSmallestPrize = 799979;
	int	l_iSmallestIndex = -1;
	for( int i=0;i<l_iCount;++i )
	{
		if(l_iSmallestPrize > m_AllFish[i]->GetObject(0)->GetPrize() )
		{
			l_iSmallestPrize = m_AllFish[i]->GetObject(0)->GetPrize();
			l_iSmallestIndex = i;
			*e_piPrize = l_iSmallestPrize;
		}
	}
	return l_iSmallestIndex;
}

int		cMonsterManager::GetSmallestPrizeFishIDFromAliveFish(int*e_piPrize)
{
	int	l_iSmallestPrize = 799979;
	int	l_iSmallestIndex = -1;
	for( int i=0;i<this->m_iNumMonsterAlive;++i )
	{
		cMonster*l_pMonster = this->m_ppWorkingMonster[i];
		if( l_pMonster->IsStatusAllowToDied() )
		{
			if(l_iSmallestPrize > l_pMonster->GetPrize() )
			{
				l_iSmallestPrize = m_ppWorkingMonster[i]->GetPrize();
				l_iSmallestIndex = i;
				*e_piPrize = l_iSmallestPrize;
			}
		}
	}
	return l_iSmallestIndex;
}

void	cMonsterManager::BossOut()
{
	if( m_pTurtleBoss )
	{
		m_pTurtleBoss->Init();
		m_pTurtleBoss->SetMonsterStatus(eMS_ALIVE);
	}
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void		cMonsterManager::MonsterLeaveScene()
{
	for( int i=0;i<this->m_iNumMonsterAlive;++i )
	{
		cMonster*l_pMonster = this->m_ppWorkingMonster[i];
		if( l_pMonster->IsStatusAllowToDied() )
		{
			l_pMonster->MonsterLeaveScene();
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

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
