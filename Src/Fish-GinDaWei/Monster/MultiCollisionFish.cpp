#include "stdafx.h"
#include "MultiCollisionFish.h"

//<Monster Type="Normal">
//	<Animation	MPDIList="xxx.mpdi" MPDI="xxxx" >
//	<Physical File="xxx.xml">
//	<AI Moving="xxx.xml">
//	<MonsterData Moving="xxx.xml">
//</Monster>

const wchar_t*         cMultiCollisionDataFish::TypeID( L"cMultiCollisionDataFish" );
#define	TURTLE_BOSS_SCALE	0.7f
cMultiCollisionDataFish::sCollisionData*			cMultiCollisionDataFish::sCollisionData::Clone()
{
	sCollisionData*l_pCollisionData = new sCollisionData();
	l_pCollisionData->iNumCollisionShape = iNumCollisionShape;
	l_pCollisionData->piCollisionTargetIndex = new int[iNumCollisionShape];
	memset(l_pCollisionData->piCollisionTargetIndex,0,sizeof(int)*l_pCollisionData->iNumCollisionShape);
	l_pCollisionData->pCollisionPITarget = pCollisionPITarget;
	l_pCollisionData->CollisionData.CloneFromList(&CollisionData);
	return l_pCollisionData;
}

cMultiCollisionDataFish::sCollisionData::sCollisionData(){iNumCollisionShape = 0;piCollisionTargetIndex = 0;	pCollisionPITarget = 0;}

cMultiCollisionDataFish::sCollisionData::~sCollisionData()
{
	SAFE_DELETE(piCollisionTargetIndex);
}

cMultiCollisionDataFish::cMultiCollisionDataFish(TiXmlElement*e_pElement):cNormalFish(e_pElement)
{
	m_fRadius = this->m_fRadius*(TURTLE_BOSS_SCALE-0.2f);
	sSphere	l_Sphere;
	l_Sphere.Radius = this->m_fRadius;
	cBound	l_Bound;
	l_Bound.SetSphere(l_Sphere);
	this->m_pAIMachine->GetSrcCharacter()->SetBound(&l_Bound);
	e_pElement = e_pElement->FirstChildElement();
	while( e_pElement )
	{
		const WCHAR*l_strValue = e_pElement->Value();
		if( !wcscmp(l_strValue,L"Physical") )
		{
			this->ProcessPhysicalData(e_pElement);
			return;
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}

cMultiCollisionDataFish::cMultiCollisionDataFish(cMultiCollisionDataFish*e_pNormalFish):cNormalFish(e_pNormalFish)
{
	m_fRadius = e_pNormalFish->m_fRadius;
	sSphere	l_Sphere;
	l_Sphere.Radius = this->m_fRadius;
	cBound	l_Bound;
	l_Bound.SetSphere(l_Sphere);
	this->m_pAIMachine->GetSrcCharacter()->SetBound(&l_Bound);

	size_t	l_uiSize = e_pNormalFish->m_CollisionDataList.size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		m_CollisionDataList.push_back(e_pNormalFish->m_CollisionDataList[i]->Clone());
	}
}

cMultiCollisionDataFish::~cMultiCollisionDataFish()
{
	DELETE_VECTOR(m_CollisionDataList,sCollisionData*);
}

void	cMultiCollisionDataFish::ProcessPhysicalData(TiXmlElement*e_pElement)
{
	e_pElement = e_pElement->FirstChildElement();
	while(e_pElement)
	{
		cMultiCollisionDataFish::sCollisionData*l_pCollisionData = new cMultiCollisionDataFish::sCollisionData;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("CollisionFile")
			{
				std::string	l_strFileName = UT::WcharToChar(l_strValue);
				if(!l_pCollisionData->CollisionData.Parse(l_strFileName.c_str()))
				{
					UT::ErrorMsg(l_strFileName.c_str(),"parse failed");
				}
			}
			else
			COMPARE_NAME("TargetPI")
			{
				if( m_pMPDIList )
				{
					l_pCollisionData->pCollisionPITarget = this->m_pMPDIList->GetPuzzleImage(l_strValue);
					if( !l_pCollisionData->pCollisionPITarget )
					{
						UT::ErrorMsg(L"pi not exist",l_strValue);
					}
					int	l_iNum = m_pAliveMPDI->Count();
					for( int i=0;i<l_iNum;++i )
					{
						if( m_pAliveMPDI->GetObject(i)->GetPointDataList()->size() )
						{
							if( m_pAliveMPDI->GetObject(i)->GetPointData(0)->pPI == l_pCollisionData->pCollisionPITarget )
								++l_pCollisionData->iNumCollisionShape;
						}
					}
					if( l_pCollisionData->iNumCollisionShape > 0 )
					{
						l_pCollisionData->piCollisionTargetIndex = new int[l_pCollisionData->iNumCollisionShape];
						memset(l_pCollisionData->piCollisionTargetIndex,0,sizeof(int)*l_pCollisionData->iNumCollisionShape);
						m_pAliveMPDI->SetRotateAroundOriginlPoint(true);
					}
				}
			}
		PARSE_NAME_VALUE_END
		this->m_CollisionDataList.push_back(l_pCollisionData);
		e_pElement = e_pElement->NextSiblingElement();
	}
}



bool cMultiCollisionDataFish::IsCollide(cbtShapeCollision*e_pbtShapeCollision)
{
	size_t	l_uiSize = m_CollisionDataList.size();
	for( size_t i=0;i<l_uiSize;++i  )
	{
		sCollisionData*l_pCollisionData = m_CollisionDataList[i];
		for( int j=0;j<l_pCollisionData->iNumCollisionShape;++j )
		{
			int	l_iIndex = l_pCollisionData->piCollisionTargetIndex[j];
			if(  l_iIndex>= 0 )
			{
				if(l_pCollisionData->CollisionData.Count()>l_iIndex)
				{
					if(l_pCollisionData->CollisionData[l_iIndex]->Collide(e_pbtShapeCollision))
						return true;
				}
				else
				{
					int a=0;
				}
			}
		}
	}
	return false;
}

void	cMultiCollisionDataFish::CollisionDataCheck()
{
	int			l_iCount =	m_pAliveMPDI->Count();
	size_t		l_uiSize =	m_CollisionDataList.size();
	for( size_t i=0;i<l_uiSize;++i  )
	{
		sCollisionData*l_pCollisionData = m_CollisionDataList[i];
		int	l_iIndex = 0;
		for( int j=0;j<l_iCount;++j  )
		{
			std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointList = m_pAliveMPDI->GetObject(j)->GetPointDataList();
			size_t	l_uiNumPoint = l_pPointList->size();
			for( size_t k=0;k<l_uiNumPoint;++k  )
			{
				sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pPointList)[k];
				if( l_pTexBehaviorDataWithImageIndexData->pPI == l_pCollisionData->pCollisionPITarget )
				{
					int	l_iImageIndex = l_pTexBehaviorDataWithImageIndexData->iImageIndex;
					if(l_pCollisionData->CollisionData.Count() < l_iImageIndex	)
					{
						UT::ErrorMsg(l_pTexBehaviorDataWithImageIndexData->pPI->GetName(),L"collision data is not match");
					}
				}
			}
		}
	}
}

void	cMultiCollisionDataFish::SetTransform()
{
	if( m_pAliveMPDI )
	{
		m_pAliveMPDI->SetScale(TURTLE_BOSS_SCALE);
		m_pAliveMPDI->SetAngle( this->m_fAngle );
		Vector3	l_vPos = m_pAIMachine->GetSrcCharacter()->GetWorldPosition();
		Vector2	l_vDrawSize = m_pAliveMPDI->GetDrawSize()/2.f;
		l_vPos.x +=l_vDrawSize.x*TURTLE_BOSS_SCALE;
		l_vPos.y +=l_vDrawSize.y*TURTLE_BOSS_SCALE;
		this->m_pAliveMPDI->SetPosByImageCenter(l_vPos);
		cMatrix44	l_mat =		m_pAliveMPDI->GetTransform();
		int			l_iCount =	m_pAliveMPDI->Count();
		size_t		l_uiSize =	m_CollisionDataList.size();
		for( size_t i=0;i<l_uiSize;++i  )
		{
			sCollisionData*l_pCollisionData = m_CollisionDataList[i];
			memset(l_pCollisionData->piCollisionTargetIndex,-1,sizeof(int)*l_pCollisionData->iNumCollisionShape);
			int	l_iIndex = 0;
			for( int j=0;j<l_iCount;++j  )
			{
				sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_pAliveMPDI->GetObject(j)->GetCurrentPointData();
				if( !m_pAliveMPDI->GetObject(j)->IsCurveMoveDone() && l_pTexBehaviorDataWithImageIndexData->pPI == l_pCollisionData->pCollisionPITarget )
				{
					int	l_iImageIndex = l_pTexBehaviorDataWithImageIndexData->iImageIndex;
					if( l_iImageIndex != -1  )
					{
						l_pCollisionData->piCollisionTargetIndex[l_iIndex] = l_iImageIndex;
						l_pCollisionData->CollisionData[ l_iImageIndex ]->SetTransform(l_mat);
					}
					++l_iIndex;
				}
			}
		}
	}
}
void			cMultiCollisionDataFish::DebugRender()
{
	cNormalFish::DebugRender();
	size_t	l_uiSize = m_CollisionDataList.size();
	cMatrix44	l_mat = m_pAliveMPDI->GetTransformByImageCenter();
	for( size_t i=0;i<l_uiSize;++i  )
	{
		sCollisionData*l_pCollisionData = m_CollisionDataList[i];
		for( int j=0;j<l_pCollisionData->iNumCollisionShape;++j )
		{
			if( l_pCollisionData->piCollisionTargetIndex[j] != -1 )
				l_pCollisionData->CollisionData[ l_pCollisionData->piCollisionTargetIndex[j] ]->DebugRender();
		}
	}
}