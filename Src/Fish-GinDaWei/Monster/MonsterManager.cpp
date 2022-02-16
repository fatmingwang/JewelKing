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
	m_pFishGroup = 0;
	m_pTurtleBoss = 0;
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
	m_pHittedFish = new sHittedFishData[m_iMaxMonsterAlive];
	e_pRoot = e_pRoot->FirstChildElement();
	while(e_pRoot)
	{
		std::wstring	l_strMonsterName;
		cMonster*l_pMonster = 0;
		int	l_iID = -1;
		PARSE_ELEMENT_START(e_pRoot)
			COMPARE_NAME("Name")
			{
				l_strMonsterName = l_strValue;
			}
			else
			COMPARE_NAME("Type")
			{
				l_pMonster = ProcessMonster(e_pRoot,l_strValue);
			}
			else
			COMPARE_NAME("ID")
			{
				l_iID = VALUE_TO_INT;
			}
			else
			COMPARE_NAME("MonsterReserve")
			{
				int	l_iID = this->Count();
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
		PARSE_NAME_VALUE_END
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
	m_spFAIBehaviorParser = new cFAIBehaviorParser;
	bool	l_b = this->ParseWithMyParse(MONSTER_DATA);
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
	m_pFishGroup = new cFishGroup(MONSTER_MANAGER_FISH_GROUP_FILE);
	m_ppWorkingMonster = new cMonster*[this->m_iMaxMonsterAlive];
	for( int i=0;i<m_iMaxMonsterAlive;++i )
		m_ppWorkingMonster[i] = 0;
	m_pTurtleBoss = new cTurtleBoss();
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
void	cMonsterManager::Render()
{
	for( int i=0;i<m_iNumMonsterAlive;++i )
	{
		m_ppWorkingMonster[i]->Render();
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
	SAFE_DELETE(m_pFishGroup);
	if( m_ppWorkingMonster )
	{
		for( int i=0;i<m_iMaxMonsterAlive;++i )
			m_ppWorkingMonster[i] = 0;
		SAFE_DELETE_ARRAY(m_ppWorkingMonster);
	}
	SAFE_DELETE(m_pHittedFish);
	SAFE_DELETE(m_spFAIBehaviorParser);
	SAFE_DELETE(m_pTurtleBoss);
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cMonster*	cMonsterManager::Require(int e_iFishIndex)
{
	cMonster*l_pMonster = this->GetObject(e_iFishIndex);
	if( l_pMonster )
		return Require(l_pMonster->GetName());
	return 0;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cMonster*	cMonsterManager::Require(const WCHAR*e_strFishName)
{
	cMonster*l_pMonster = 0;
	if(m_iNumMonsterAlive<m_iMaxMonsterAlive)
	{
		//cMonster*l_pMonster = this->GetObject(e_strFishName);
		int	l_iIndex = this->GetObjectIndexByName(e_strFishName);
		if( l_iIndex != -1 )
		{
			cObjectListByName<cMonster>*l_pFishList = this->m_AllFish[l_iIndex];
			int	l_iCount = l_pFishList->Count();
			for( int i=0;i<l_iCount;++i )
			{
				l_pMonster = l_pFishList->GetObject(i);
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
				l_pMonster = 0;
			}
		}
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
bool	cMonsterManager::FishGroupGo()
{
	if( m_pFishGroup )
	{
		int l_nCount1 = m_pFishGroup->m_FishGroup.Count();
		if( l_nCount1>0 )
		{
			this->m_pFishGroup->FishGroupGo(rand()%m_pFishGroup->m_FishGroup.Count());
		} //end if
	} //end if
	return true;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
bool	cMonsterManager::FishGroupGo(int e_iFishID,int e_iCount)
{
	int	l_iGoCount = 0;
	float	l_fTimeOffset = 0.5f;
	if(this->m_iMaxMonsterAlive-this->m_iNumMonsterAlive>e_iCount)
	{
		cMonster*l_pFirstMonster = this->Require(e_iFishID);
		if( !l_pFirstMonster )
			return false;
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
				return false;
			for(size_t i=1;i<l_uiSize;++i  )
			{
				l_TimeList[i] += l_fTimeOffset;
			}
		}
		return true;
	}
	return false;
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
	m_pTurtleBoss->Init();
	m_pTurtleBoss->SetMonsterStatus(eMS_ALIVE);
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

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
