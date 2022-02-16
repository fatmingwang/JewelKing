/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "FishGroup.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/MonsterBase.h"
#include "../EngineTest/FishProbability.h"

namespace FATMING_CORE
{
	extern bool	g_bSkipImageLoad;
}

char*g_strCommonVS = "									\
attribute vec3 VSPosition;								\
attribute vec3 VSNormal;								\
attribute vec4 VSColor;									\
attribute vec2 VSTexcoord;								\
uniform mat4 matVP;										\
uniform mat4 matW;										\
varying vec2 PSTexcoord;								\
varying vec4 PSColor;									\
void main()												\
{														\
	gl_Position = matVP*matW*vec4(VSPosition,1);		\
	PSTexcoord = VSTexcoord;							\
	PSColor = VSColor;									\
}";

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cFishGroup::cFishGroup( char* e_strFishGroupMPDI )
{
	this->SetName(UT::GetFileNameWithoutFullPath(e_strFishGroupMPDI));
	m_pMPDI_Winnings1 = 0;
	if( !cFishApp::m_spMonsterManager )
		return;
	g_bSkipImageLoad = true;
	cAnimationParser	l_AnimationParser;
	if(l_AnimationParser.Parse(e_strFishGroupMPDI))
	{
		cMPDIList*l_pMPDIlist = l_AnimationParser.GetMPDIList(0);
		for( int i=0;i<l_pMPDIlist->Count();++i )
		{
			cMPDI*l_pMPDI = l_pMPDIlist->GetObject(i);
			cFishGroupPointData*l_pFishGroupPointData = new cFishGroupPointData();
			l_pFishGroupPointData->m_vDrawSize = l_pMPDI->GetDrawSize();
			for( int j=0;j<l_pMPDI->Count();++j )
			{
				cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(j);
				//
				std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointList = l_pSubMPDI->GetPointDataList();
				if( l_pPointList->size() )
				{
					const WCHAR*l_strFishName = (*l_pPointList)[0]->pPI->GetObject((*l_pPointList)[0]->iImageIndex)->GetName();
					int	l_iFishIndex = cFishApp::m_spMonsterManager->GetObjectIndexByName(l_strFishName);
					if( l_iFishIndex == -1 )
					{
						UT::ErrorMsg(l_strFishName,L"there is no such fish name!");
					}
					cFishGroupPointData::sFishPosList l_FishPos;
					l_FishPos.iFishIndex = l_iFishIndex;
					std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointDataList = l_pSubMPDI->GetPointDataList();
					l_FishPos.vPosList = l_pSubMPDI->GetOriginalPointList();
					for(size_t i = 0;i<l_pPointDataList->size();++i)
					{
						Vector2	l_vSize = (*l_pPointDataList)[i]->Size/2.f;
						l_FishPos.vPosList[i].x += l_vSize.x;
						l_FishPos.vPosList[i].y += l_vSize.y;
					}
					l_pFishGroupPointData->m_FishGroupData.push_back(l_FishPos);
				}
			}
			m_FishGroup.AddObjectNeglectExist(l_pFishGroupPointData);
		}
	}
	else
	{
		UT::ErrorMsg(e_strFishGroupMPDI,"file parse failed");
	}
	l_AnimationParser.Destroy();
	g_bSkipImageLoad = false;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : William
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cFishGroup::cFishGroup( int e_nId, char* e_strFishGroupMPDI )
{
	m_pMPDI_Winnings1 = 0;
	if( !cFishApp::m_spMonsterManager )
		return;
	g_bSkipImageLoad = true;
	cAnimationParser	l_AnimationParser;
	if(l_AnimationParser.Parse(e_strFishGroupMPDI))
	{
		cMPDIList*l_pMPDIlist = l_AnimationParser.GetMPDIList(0);
		//
		//points, Winning02.mpdi -> index = 0
		for( int i=0;i<1;++i )
		{
			cMPDI*l_pMPDI = l_pMPDIlist->GetObject(i);
			cFishGroupPointData*l_pFishGroupPointData = new cFishGroupPointData();
			l_pFishGroupPointData->m_vDrawSize = l_pMPDI->GetDrawSize();
			int l_nCount2 = l_pMPDI->Count();
			for( int j=0; j<l_nCount2; ++j )
			{
				cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(j);
				//
				std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointList = l_pSubMPDI->GetPointDataList();
				if( l_pPointList->size() )
				{
					const WCHAR*l_strFishName = (*l_pPointList)[0]->pPI->GetObject((*l_pPointList)[0]->iImageIndex)->GetName();
					int	l_iFishIndex = cFishApp::m_spMonsterManager->m_AllFish.GetObjectIndexByName(l_strFishName);
					if( l_iFishIndex == -1 )
					{
						UT::ErrorMsg(l_strFishName,L"there is no such fish name!");
					}
					cFishGroupPointData::sFishPosList l_FishPos;
					l_FishPos.iFishIndex = l_iFishIndex;
					std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointDataList = l_pSubMPDI->GetPointDataList();
					l_FishPos.vPosList = l_pSubMPDI->GetOriginalPointList();
					//
					int l_nCount3 = (int)l_pPointDataList->size();
					for(int k = 0; k<l_nCount3; ++k)
					{
						Vector2	l_vSize = (*l_pPointDataList)[k]->Size/2.f;
						l_FishPos.vPosList[k].x += l_vSize.x;
						l_FishPos.vPosList[k].y += l_vSize.y;
					} //next k
					l_pFishGroupPointData->m_FishGroupData.push_back(l_FishPos);
				}
			} //next j
			m_FishGroup.AddObjectNeglectExist(l_pFishGroupPointData);
		} //next i

		//path, Winning02.mpdi -> index = 1, 2
		for( int i=1; i<3; ++i )
		{
			cMPDI*l_pMPDI = l_pMPDIlist->GetObject(i);
			cFishGroupPointData*l_pFishGroupPointData = new cFishGroupPointData();
			int l_nCount2 = l_pMPDI->Count();
			for( int j=0;j<l_nCount2;++j )
			{
				cSubMPDI*l_pSubMPDI = l_pMPDI->GetObject(j);
				const WCHAR*l_str = l_pSubMPDI->GetName();
				cCurveWithTime	l_CurveWithTime(l_pSubMPDI);
				l_pFishGroupPointData->vCurveList.push_back( l_CurveWithTime );
			}
//int abc = l_pFishGroupPointData->vCurveList.size();
			m_FishGroup.AddObjectNeglectExist( l_pFishGroupPointData );
		} //next i
		
	}
	else
	{
		UT::ErrorMsg(e_strFishGroupMPDI,"file parse failed");
	} //next i
	//
	l_AnimationParser.Destroy();
	g_bSkipImageLoad = false;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cFishGroup::~cFishGroup()
{
	SAFE_DELETE( m_pMPDI_Winnings1 );
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cFishGroup::FishGroupGoLeftToRight(int e_iIndex,float e_fTime)
{
	Vector3	l_vStartPos = Vector3(-cGameApp::m_svGameResolution.x,0.f,0.f);
	Vector3	l_vEndPos = Vector3(cGameApp::m_svGameResolution.x,0.f,0.f);
	cCurveWithTime	l_TargetPath;
	l_TargetPath.AddPoint(l_vStartPos,0.f);
	l_TargetPath.AddPoint(l_vEndPos,e_fTime);
	FishGroupGo(e_iIndex,l_TargetPath,true);
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cFishGroup::FishGroupGRightTooLeft(int e_iIndex,float e_fTime)
{
	Vector3	l_vStartPos = Vector3(cGameApp::m_svGameResolution.x,0.f,0.f);
	Vector3	l_vEndPos = Vector3(-cGameApp::m_svGameResolution.x,0.f,0.f);
	cCurveWithTime	l_TargetPath;
	l_TargetPath.AddPoint(l_vStartPos,0.f);
	l_TargetPath.AddPoint(l_vEndPos,e_fTime);
	FishGroupGo(e_iIndex,l_TargetPath,true);
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
int	cFishGroup::FishGroupGo(int e_iIndex,cCurveWithTime e_TargetPath,bool e_bForceFetch)
{
	cFishGroup::cFishGroupPointData*l_pFishGroupPointData = this->m_FishGroup.GetObject(e_iIndex);
	Vector3	l_vStartPoint = e_TargetPath.GetOriginalPointList()[0];
	Vector3	l_vEndPoint = e_TargetPath.GetOriginalPointList()[1];
	float	l_fEndTime = e_TargetPath.GetOriginalTimeList()[1];
	int	l_iNumMonsterGo = 0;
	if( l_pFishGroupPointData )
	{
		size_t	l_uiSize =  l_pFishGroupPointData->m_FishGroupData.size();
		for( size_t	i=0;i<l_uiSize;++i )
		{
			cFishGroup::cFishGroupPointData::sFishPosList*l_pFishPosList = &l_pFishGroupPointData->m_FishGroupData[i];
			int	l_iFishId = l_pFishPosList->iFishIndex;
			size_t	l_uiSize2 = l_pFishPosList->vPosList.size();
			for( size_t	j=0;j<l_uiSize2;++j )
			{
				cMonster*l_pMonster = cFishApp::m_spMonsterManager->Require(l_iFishId,e_bForceFetch);
				if( !l_pMonster )
				{
					break;
				}
				cCurveWithTime	l_TranslatePath;
				Vector3	l_vPos = l_pFishPosList->vPosList[j];
				l_vPos = l_pFishPosList->vPosList[j]+l_vStartPoint;
				l_TranslatePath.AddPoint(l_vPos,0.f);
				l_vPos = l_pFishPosList->vPosList[j]+l_vEndPoint;
				l_TranslatePath.AddPoint(l_vPos,l_fEndTime);
				l_pMonster->SetPos(l_vPos);
				l_pMonster->Init();
				l_TranslatePath.SetCalAngle(true);
				l_pMonster->SetPath(&l_TranslatePath);
				++l_iNumMonsterGo;
			}
		}
	}
	return l_iNumMonsterGo;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
int	cFishGroup::FishGroupGo(int e_iIndex,bool e_bForceFetch)
{
	cCurveWithTime	l_TargetPath;
	cFishGroup::cFishGroupPointData*l_pFishGroupPointData = this->m_FishGroup.GetObject(e_iIndex);
	Vector3	l_vFirstFishPos;
	Vector3	l_vFirstPointPos;
	int	l_iNumMonsterGo = 0;
	if( l_pFishGroupPointData )
	{
		size_t	l_uiSize =  l_pFishGroupPointData->m_FishGroupData.size();
		for( size_t	i=0;i<l_uiSize;++i )
		{
			cFishGroup::cFishGroupPointData::sFishPosList*l_pFishPosList = &l_pFishGroupPointData->m_FishGroupData[i];
			int	l_iFishId = l_pFishPosList->iFishIndex;
			size_t	l_uiSize2 = l_pFishPosList->vPosList.size();
			for( size_t	j=0;j<l_uiSize2;++j )
			{
				cMonster*l_pMonster = cFishApp::m_spMonsterManager->Require(l_iFishId,e_bForceFetch);
				if( !l_pMonster )
					break;
				cCurveWithTime	l_TranslatePath;
				if( j == 0 && i==0 )
				{
					l_TargetPath = l_pMonster->GetPath();
					l_vFirstFishPos = cFishApp::m_spFishProbability->GetRandomPos(l_pFishGroupPointData->m_vDrawSize.Length());
					l_vFirstPointPos = l_pFishPosList->vPosList[0];
				}
				l_TranslatePath = l_TargetPath;
				Vector3	l_vPos = l_pFishPosList->vPosList[j];
				Vector3	l_vPosOffset = 	l_vPos-l_vFirstPointPos;
				l_vPos = l_vFirstFishPos+l_vPosOffset;
				l_TranslatePath.Translate(l_vPosOffset);
				l_pMonster->SetPos(l_vPos);
				l_pMonster->Init();
				l_pMonster->SetPath(&l_TranslatePath);
				++l_iNumMonsterGo;
			}
		}
	}
	return l_iNumMonsterGo;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : William
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cFishGroup::f_PrizeII_Call( float e_fTime )
{
	Vector3	l_vStartPos = Vector3( cGameApp::m_svGameResolution.x/3,	0.f,	0.f );
	Vector3	l_vEndPos	= Vector3( cGameApp::m_svGameResolution.x/3,	0.f,	0.f );
	cCurveWithTime	l_TargetPath;
	l_TargetPath.AddPoint(l_vStartPos,0.f);
	l_TargetPath.AddPoint(l_vEndPos,e_fTime);
	//
	f_PrizeII_Go( l_TargetPath );
}


/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : William
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cFishGroup::f_PrizeII_Go( cCurveWithTime e_TargetPath )
{
	cCurveWithTime	l_TargetPath;
	Vector3	l_vFirstFishPos;
	Vector3	l_vFirstPointPos;

	cFishGroup::cFishGroupPointData* l_pFishGroupPointData1 = this->m_FishGroup.GetObject(0);
	cFishGroup::cFishGroupPointData* l_pFishGroupPointData2 = this->m_FishGroup.GetObject(1);
	cFishGroup::cFishGroupPointData* l_pFishGroupPointData3 = this->m_FishGroup.GetObject(2);

	size_t	l_uiSizeSubMPDI2 =  l_pFishGroupPointData2->vCurveList.size();
	size_t	l_uiSizeSubMPDI3 =  l_pFishGroupPointData3->vCurveList.size();
	size_t	l_uiSizeSubMPDI1 =  l_pFishGroupPointData1->m_FishGroupData.size();
	for( size_t	i=0; i<(size_t)m_FishGroup.Count(); ++i ) //how many index in mpdi
	{
		size_t	l_uiSize = 0;
		if( i==0 )
		{
			l_uiSize = l_pFishGroupPointData1->m_FishGroupData.size();
		}
		else if( i==1 )
		{
			l_uiSize = l_pFishGroupPointData2->vCurveList.size();
		}
		else if( i==2 )
		{
			l_uiSize = l_pFishGroupPointData3->vCurveList.size();
		}
		//
		for( size_t kk=0; kk<l_uiSize; kk++ )
		{
			size_t	l_uiSize2 = 0;
			int	l_iFishId = 0;
			if( i==0 )
			{
				cFishGroup::cFishGroupPointData::sFishPosList* l_pFishPosList = 0;
				l_pFishPosList = &l_pFishGroupPointData1->m_FishGroupData[i];
				l_uiSize2 = l_uiSizeSubMPDI1;
				l_iFishId = l_pFishPosList->iFishIndex;
			}
			else if( i==1 )
			{
				l_uiSize2 = l_uiSizeSubMPDI2;
				l_iFishId = 0;
			}
			else if( i==2 )
			{
				l_uiSize2 = l_uiSizeSubMPDI3;
				l_iFishId = 8;
			}

			for( size_t	j=0; j<l_uiSize2; ++j )
			{
				cMonster* l_pMonster = cFishApp::m_spMonsterManager->Require( l_iFishId,true );
				if( !l_pMonster )
				{
					if( i == 0 )
					{
						int a=0;
					}
					break;
				}
				//
				cCurveWithTime	l_TranslatePath;
				

				if( l_uiSize2==l_uiSizeSubMPDI2 )
				{
					l_TranslatePath = l_pFishGroupPointData2->vCurveList[j];
				}
				else if( l_uiSize2==l_uiSizeSubMPDI3 )
				{
					l_TranslatePath = l_pFishGroupPointData3->vCurveList[j];
				}
				else
				{
					Vector3 l_vKing = l_pFishGroupPointData1->m_FishGroupData[0].vPosList[0];
					l_TranslatePath.AddPoint( l_vKing, 0.0f );
					l_TranslatePath.AddPoint( l_vKing, 38.0f );
					l_TranslatePath.AddPoint( l_vKing+Vector3(frand(-1,1),frand(-1,1),0), 38.1f );
				}


				//
	//			l_pMonster->SetPos(l_vPos);
				l_pMonster->Init();
				l_TranslatePath.SetLOD(5);
				l_TranslatePath.SetCalAngle( true );
				l_pMonster->SetPath( &l_TranslatePath );
			} //next j
		} //next kk
	} //next i
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

