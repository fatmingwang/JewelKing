#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_

#include <map>
#include "Collision.h"
#include "../Glh.h"
#ifdef WIN32
#pragma warning( disable : 4996 )
#endif
#include "../VS_TO_GNN.h"
namespace UT
{
	//get a distance releste from now to start between a range.
	//situation1.
	//    Start      TargetPos   CurrentPos       End
	//situation2.
	//    Start      CurrentPos  TargetPos	      End
	inline	float	GetDistanceFromCurrentToStartOfLoop(float e_fPosStart,float e_fPosEnd,float e_fCurrentPos,float e_fTarget)
	{
		if( e_fTarget>e_fCurrentPos )
		{
			return e_fTarget-e_fCurrentPos;
		}
		return (e_fPosEnd-e_fCurrentPos)+(e_fTarget-e_fPosStart);
	}
	void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
				   GLfloat centerx, GLfloat centery, GLfloat centerz,
				   GLfloat upx, GLfloat upy, GLfloat upz);
	/* projection du point (objx,objy,obz) sur l'ecran (winx,winy,winz) */
	GLint 
	gluProject(GLfloat objx, GLfloat  objy, GLfloat  objz,
		   const GLfloat  model[16], const GLfloat  proj[16],
		   const GLint viewport[4],
		   GLfloat  * winx, GLfloat  * winy, GLfloat  * winz);

	void gluPerspective(  GLfloat  fovy,  GLfloat  aspect,  GLfloat  zNear,  GLfloat  zFar);
//#endif
	inline	float	GetFloatModulus(float e_fValue,float e_fDivision){int	l_iModulus = (int)(e_fValue/e_fDivision);return e_fValue-(l_iModulus*e_fDivision);}
	//count for how many do we want?the max value for random range,
	//below is a sample to generate a 01234 random table
	//vector<int>	l_uiAllMagicIDVector1 = GenerateRandomValueVector(5,5);
	std::vector<int>	GenerateRandomTable(int e_iCount,int e_iMaxValue);
	//generate a random vector from input data
	template<class T> std::vector<T>	GenerateRandomTable(std::vector<T>*e_pData)
	{
		int	l_Size = (int)e_pData->size();
		std::vector<int>l_RandomTable = GenerateRandomTable(l_Size,l_Size);
		std::vector<T>l_ResultTable;
		l_ResultTable.resize(l_Size);
		for( int i=0;i<l_Size;++i )
		{
			l_ResultTable[i] = ((*e_pData)[l_RandomTable[i]]);
		}
		return l_ResultTable;
	}
	Vector3	AngleToDirection(float e_fAngle);
	float	GetRadianBy2Point(float e_SrcX,float e_SrcY,float e_DestX,float e_DestY);
	//Y up is 0,RightX is -90,return value in degree
	float	GetAngleBy2Point(float e_SrcX,float e_SrcY,float e_DestX,float e_DestY);
	//only compare xy.,return value in degree
	float	GetAngleBy2Point(Vector3 e_vSrc,Vector3 e_vDest);

	//to find out the closest point from a vector,if the distance is smaller than e_fOffsetDis,the condition is success
	int		GetClosestPointIndex(Vector2 e_vPos,std::vector<Vector2>*e_pPosVector,float e_fOffsetDis = 20.f);
	int		GetClosestPointIndex(Vector3 e_vPos,std::vector<Vector3>*e_pPosVector,float e_fOffsetDis = 20.f);

	inline	POINT	operator+=(POINT e_Point1,POINT e_Point2){e_Point1.x+=e_Point2.x;e_Point1.y+=e_Point2.y;return e_Point1;}
	inline	POINT	operator-=(POINT e_Point1,POINT e_Point2){e_Point1.x-=e_Point2.x;e_Point1.y-=e_Point2.y;return e_Point1;}
	inline	POINT	operator+(POINT e_Point1,POINT e_Point2){e_Point1.x+=e_Point2.x;e_Point1.y+=e_Point2.y;return e_Point1;}
	inline	POINT	operator-(POINT e_Point1,POINT e_Point2){e_Point1.x-=e_Point2.x;e_Point1.y-=e_Point2.y;return e_Point1;}

	template <class TYPE>inline	TYPE	LERP(TYPE e_fSrc,TYPE e_fDest,float e_fLERPValue )
	{
		return (TYPE)((e_fDest-e_fSrc)*e_fLERPValue+e_fSrc);
	}

	inline	int power_of_two(int input)
	{
		int value = 1;

		while ( value < input ) {
			value <<= 1;
		}
		return value;
	}
	//inline Vector3	WorldToScreen(float objX,float objY,float objZ,float *e_pfProjectionMatrix = 0,float *e_pfModelViewMatrix = 0)
	inline Vector3	WorldToScreen(float objX,float objY,float objZ,float *e_pfProjectionMatrix,float *e_pfModelViewMatrix)
	{
		GLint realy;
		float win_x,win_y,win_z;
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
        gluProject(objX,objY,objZ,e_pfModelViewMatrix,e_pfProjectionMatrix,viewport,&win_x,&win_y,&win_z);
		realy = viewport[3]-(GLint)win_y -1;

		Vector3 temp((float)win_x,(float)realy,(float)win_z);
		return temp;
	}
	//the view port here original start always be X=0,Y=0,however we have changed it,
	//it should be look like this.
	//int	l_iViewPort[4] = {0,0,Width,Height};
	inline Vector3	WorldToScreen(float objX,float objY,float objZ,float*e_pmatProjection,float*e_pmatMV,int *e_piViewPort)
	{
		GLint realy;
		float win_x,win_y,win_z;
		gluProject(objX,objY,objZ,e_pmatMV,e_pmatProjection,e_piViewPort,&win_x,&win_y,&win_z);
		realy = e_piViewPort[3]-(GLint)win_y -1;

		Vector3 temp((float)win_x,(float)realy,(float)win_z);
		return temp;
	}

	//e_matVP  =	ProjectionTransform*ViewTransform();
	//e_vScreenPos.z is betwnne 0 to 1
	inline	Vector3	ScreenToWorld(Vector3 e_vScreenPos,cMatrix44 e_matVP,Vector2 e_vViewPortSize)
	{
		//the z indicate how far will it be
		Vector4	l_vWolrdPos = Vector4( (e_vScreenPos.x /e_vViewPortSize.x-0.5f)*2, (-e_vScreenPos.y /e_vViewPortSize.y+0.5f)*2,e_vScreenPos.z,1);
		l_vWolrdPos = e_matVP.Inverted().TransformCoordinate(l_vWolrdPos);
		float	l_iInverseW = 1 / l_vWolrdPos.w;
		l_vWolrdPos = l_vWolrdPos *l_iInverseW;
		return *(Vector3*)&l_vWolrdPos;
	}

	inline	float	CalculateDifferenceBetweenRadius(float e_fSrc,float e_fTarget)
	{
		e_fSrc = GetFloatModulus(e_fSrc,D3DX_PI*2);
		e_fTarget = GetFloatModulus(e_fTarget,D3DX_PI*2);
		float	l_fDifferenceRadian = e_fTarget-e_fSrc;
		while (l_fDifferenceRadian < -D3DX_PI)
			l_fDifferenceRadian += D3DX_PI*2;
		while (l_fDifferenceRadian > D3DX_PI)
			l_fDifferenceRadian -= D3DX_PI*2;
		return l_fDifferenceRadian;
	}

	inline	float	CalculateDifferenceBetweenAngles(float e_fSrc,float e_fTarget)
	{
		float	l_fDifferenceAngle = e_fTarget-e_fSrc;
		while (l_fDifferenceAngle < -180)
			l_fDifferenceAngle += 360;
		while (l_fDifferenceAngle > 180)
			l_fDifferenceAngle -= 360;
		return l_fDifferenceAngle;
	}
	//get the percetange has been past between 2 value
	inline	float	GetTimeGapLERP( float e_fStart,float e_fEnd,float e_fCurrent )
	{
		float	l_fGap = e_fEnd-e_fStart;
		assert(l_fGap>0.f&&"soory I am lazy to do safe function here...");
		float	l_fPast = e_fCurrent-e_fStart;
		//it's possible over than 1
		return l_fPast/l_fGap;
	}
	Vector3	rand(Vector3 e_vMin,Vector3 e_vMax);
	Vector2	rand(Vector2 e_vMin,Vector2 e_vMax);
	int		rand(int	e_iMin,	int		e_iMax);
	float	rand(float	e_fMin,	float	e_fMax);
	POINT	rand(POINT	e_Min,	POINT	e_Max);
	template <class T>	struct	sMinMaxData
	{
		T			Min;
		T			Max;
		//T			Value;
		inline sMinMaxData<T> operator =(Vector2 e_vData)
		{
			sMinMaxData<T>	l_Data = {Min = (T)e_vData.x,Max = (T)e_vData.y};
			return l_Data;
		}
		inline	T	Rand()
		{
			//Value = rand(Min,Max);
			//return Value;
			return rand(Min,Max);
		}
	};
	//for each
	//template<class InputIterator, class Function>Function for_each(InputIterator first, InputIterator last, Function f)
	//{
	//	for ( ; first!=last; ++first ) f(*first);
	//		return f;
	//}
#ifndef RAND_MAX
#define RAND_MAX 0x7fff
#endif
	inline float frand( float fMin, float fMax )
	{
		double fRange = fMax - fMin;
		double fVal = ( (double)::rand() * fRange ) / (double)RAND_MAX;
		return (float)fVal + fMin;
	}
//end namespace UT
}

#endif