#include "../stdafx.h"
#include "MathUtility.h"


namespace UT
{
	std::vector<int>	GenerateRandomTable(int e_iCount,int e_iMaxValue)
	{
		vector<int>	l_uiAllMagicIDVector;
		l_uiAllMagicIDVector.reserve(e_iCount);
		while( l_uiAllMagicIDVector.size() != e_iCount )
		{
			int	l_iMagicID = ::rand()%(e_iMaxValue);
			bool	l_bSameMagicID = false;
			for( UINT j=0;j<l_uiAllMagicIDVector.size();j++ )
			{
				if( l_uiAllMagicIDVector[j] == l_iMagicID )
				{					
					l_bSameMagicID = true;
					break;
				}
			}
			if( !l_bSameMagicID )
				l_uiAllMagicIDVector.push_back(l_iMagicID);
		}
		return l_uiAllMagicIDVector;
	}
	float	GetRadianBy2Point(float e_SrcX,float e_SrcY,float e_DestX,float e_DestY)
	{
		float l_fX = e_DestX - e_SrcX;
		float l_fY = e_DestY - e_SrcY;
		return atan2(l_fX,l_fY);
		//just using atan2 instead below code

		//if( abs(l_fX)<1E-10 && l_fY<1E-10 )
		//	return 0;
		//if( l_fY>l_fX )
		//{
		//	if( l_fY>0 )
		//		return D3DXToDegree(atan(l_fX/l_fY));
		//	return D3DXToDegree(atan(l_fX/l_fY)+D3DX_PI);
		//}
		//if( l_fX>0 )
		//	return D3DXToDegree(0.5f*D3DX_PI-atan(l_fY/l_fX));
		//return D3DXToDegree(-0.5f*D3DX_PI-atan(l_fY/l_fX));
	}

	float	GetAngleBy2Point(Vector3 e_vSrc,Vector3 e_vDest)
	{
		//180 for Z invert
		//return 180-D3DXToDegree(atan2(l_fX,l_fY));
		return D3DXToDegree(GetRadianBy2Point(e_vSrc.x,e_vSrc.y,e_vDest.x,e_vDest.y));
	}

	float GetAngleBy2Point(float e_SrcX,float e_SrcY,float e_DestX,float e_DestY)
	{
		return D3DXToDegree(GetRadianBy2Point(e_SrcX,e_SrcY,e_DestX,e_DestY));
	}

	Vector3	AngleToDirection(float e_fAngle)
	{
		cMatrix44	l_mat = cMatrix44::ZAxisRotationMatrix(D3DXToRadian(e_fAngle));
		return l_mat.GetAxis(MyMath::X);
	}

	int		GetClosestPointIndex(Vector2 e_vPos,std::vector<Vector2>*e_pPosVector,float e_fOffsetDis)
	{
		int	l_iFocusPoint = -1;
		Vector2	l_vMousePos = e_vPos;
		std::vector<Vector2> l_vPointList = *e_pPosVector;
		float	l_fClosetDistance = e_fOffsetDis;
		int	l_iLength = (int)l_vPointList.size();
		for( int i=0;i<l_iLength;++i )
		{
			Vector2 l_vPos = l_vPointList[i];
			Vector2	l_vDis = l_vPos-l_vMousePos;
			float	l_fDis = l_vDis.Length();
			if( l_fClosetDistance>l_fDis )
			{
				l_fClosetDistance = l_fDis;
				l_iFocusPoint = i;
			}
		}
		return l_iFocusPoint;
	}

	int		GetClosestPointIndex(Vector3 e_vPos,std::vector<Vector3>*e_pPosVector,float e_fOffsetDis)
	{
		int	l_iFocusPoint = -1;
		Vector3	l_vMousePos = e_vPos;
		std::vector<Vector3> l_vPointList = *e_pPosVector;
		float	l_fClosetDistance = e_fOffsetDis;
		int	l_iLength = (int)l_vPointList.size();
		for( int i=0;i<l_iLength;++i )
		{
			Vector3 l_vPos = l_vPointList[i];
			Vector3	l_vDis = l_vPos-l_vMousePos;
			float	l_fDis = l_vDis.Length();
			if( l_fClosetDistance>l_fDis )
			{
				l_fClosetDistance = l_fDis;
				l_iFocusPoint = i;
			}
		}
		return l_iFocusPoint;	
	}

	Vector3	rand(Vector3 e_vMin,Vector3 e_vMax)
	{
		return Vector3(frand(e_vMin.x,e_vMax.x),
		frand(e_vMin.y,e_vMax.y),
		frand(e_vMin.z,e_vMax.z));
	}

	Vector2	rand(Vector2 e_vMin,Vector2 e_vMax)
	{
		return Vector2(frand(e_vMin.x,e_vMax.x),frand(e_vMin.y,e_vMax.y));
	}

	int rand( int e_iMin,int e_iMax )
	{
		int	l_iRandValue = (e_iMax-e_iMin);
		if( l_iRandValue == 0 )
			return e_iMin;
		int l_iVal = ::rand()%l_iRandValue;
		return e_iMin+l_iVal;
	}

	float rand( float fMin, float fMax )
	{
		return frand(fMin,fMax);
	}

	POINT	rand(POINT	e_Min,	POINT	e_Max)
	{
		POINT	l_Point = {rand(e_Min.x,e_Max.x),rand(e_Min.y,e_Max.y)};
		return l_Point;
	}

//end namespace UT
}