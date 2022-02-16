#ifndef SUB_MDPI
#define SUB_MDPI
#include "FatmingAnimationRule.h"
namespace FATMING_CORE
{
	//==========
	//each point could has below data to do something
	//==========
	struct	sTexBehaviorDataWithImageIndexData
	{
		Vector2	Size;			//size
		float	fAngle;			//angle
		Vector4	vColor;			//color
		int		iImageIndex;	//image index
		bool	bMirror;		//mirror
		bool	bHintPoint;		//is this point a cue?
		sTexBehaviorDataWithImageIndexData()
		{
			Size.x = Size.y = 0.f;
			fAngle = 0.f;
			vColor = Vector4::One;
			iImageIndex = -1;
			bMirror = false;
		}

		sTexBehaviorDataWithImageIndexData(Vector2 e_Size,float	e_fAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror)
		{
			Size = e_Size;
			fAngle = e_fAngle;
			vColor = e_vColor;
			iImageIndex = e_iImageIndex;
			bMirror = e_bMirror;
		}

		inline	sTexBehaviorDataWithImageIndexData	ToLerp(sTexBehaviorDataWithImageIndexData*e_pDest,float e_fLERP)
		{
			sTexBehaviorDataWithImageIndexData	l_sTexBehaviorDataWithImageIndexData;
			//because fucking 1 pixel problem,so we force it to be a intger value!
			l_sTexBehaviorDataWithImageIndexData.Size.x = (float)((int)LERP<float>(Size.x,e_pDest->Size.x,e_fLERP));
			l_sTexBehaviorDataWithImageIndexData.Size.y = (float)((int)LERP<float>(Size.y,e_pDest->Size.y,e_fLERP));
			l_sTexBehaviorDataWithImageIndexData.fAngle = LERP<float>(fAngle,e_pDest->fAngle,e_fLERP);
			l_sTexBehaviorDataWithImageIndexData.vColor = LERP<Vector4>(vColor,e_pDest->vColor,e_fLERP);
			l_sTexBehaviorDataWithImageIndexData.iImageIndex = iImageIndex;
			l_sTexBehaviorDataWithImageIndexData.bMirror = e_pDest->bMirror;
			return l_sTexBehaviorDataWithImageIndexData;
		}
	};

	//2,1,4,1
	//size,angle,color(RGBA),imageIndex
	//<PointData P1="0,0,0,0,0,0,0,0" P2="0,0,0,0,0,0,0,0" P3="0,0,0,0,0,0,0,0">
	inline	sTexBehaviorDataWithImageIndexData	GetTexBehaviorDataWithImageIndexData(const WCHAR*e_str,sPuzzleData**e_ppPuzzleData,int e_iNumData,WCHAR*e_pstrNotExistImageName = 0)
	{
		WCHAR	l_strData[TEMP_SIZE];
		swprintf(l_strData,MAX_PATH,L"%ls\0",e_str);
		sTexBehaviorDataWithImageIndexData	l_Data;
		WCHAR*	l_str = MYWCSTOK(l_strData,L",:");
		//size
		l_Data.Size.x = (float)_wtof(l_str);
		l_str = MYWCSTOK(0,L",:");
		l_Data.Size.y = (float)_wtof(l_str);
		l_str = MYWCSTOK(0,L",:");
		//angle
		l_Data.fAngle = (float)_wtof(l_str);
		l_str = MYWCSTOK(0,L",:");
		//color
		l_Data.vColor.x = _wtoi(l_str)/255.f;
		l_str = MYWCSTOK(0,L",:");
		l_Data.vColor.y = _wtoi(l_str)/255.f;
		l_str = MYWCSTOK(0,L",:");
		l_Data.vColor.z = _wtoi(l_str)/255.f;
		l_str = MYWCSTOK(0,L",:");
		l_Data.vColor.w = _wtoi(l_str)/255.f;
		l_str = MYWCSTOK(0,L",:");
		//image index
		l_Data.iImageIndex = -1;
		WCHAR	*l_strForImageName = l_str;
		for( int i=0;i<e_iNumData;++i )
		{
			if(!wcscmp(e_ppPuzzleData[i]->strFileName,l_strForImageName))
			{
				l_Data.iImageIndex = i;
				break;
			}
		}
		if(l_Data.iImageIndex == -1)
		{
			//UT::ErrorMsg(L"image data version error or image has been remove\nPress No to set image index to 0",l_strForImageName);
			l_Data.iImageIndex = 0;
			if(e_pstrNotExistImageName)
			{
				int	l_iSize = wcslen(l_strForImageName);
				memcpy(e_pstrNotExistImageName,l_strForImageName,sizeof(WCHAR)*l_iSize);
				e_pstrNotExistImageName[l_iSize] = L'\0';
			}
		}
		else
		{
			if(e_pstrNotExistImageName)
				e_pstrNotExistImageName[0] = L'\0';
		}
		//mirror
		l_str = MYWCSTOK(0,L",:");
		if( l_str )
		{
			l_Data.bMirror = _wtoi(l_str)?true:false;
		}
		return l_Data;
	}
	inline	char*TexBehaviorDataWithImageIndexDataToString(sTexBehaviorDataWithImageIndexData e_sTexBehaviorDataWithImageIndexData,sPuzzleData**e_ppPuzzleData,int e_iNumData)
	{
		assert(e_sTexBehaviorDataWithImageIndexData.iImageIndex!=-1&&e_sTexBehaviorDataWithImageIndexData.iImageIndex<e_iNumData);
		static char l_str[TEMP_SIZE];
		sprintf(l_str,"%d,%d,%.2f,%d,%d,%d,%d,%s,%d\0",
		(int)e_sTexBehaviorDataWithImageIndexData.Size.x,
		(int)e_sTexBehaviorDataWithImageIndexData.Size.y,
		e_sTexBehaviorDataWithImageIndexData.fAngle,
		(int)(e_sTexBehaviorDataWithImageIndexData.vColor.x*255),
		(int)(e_sTexBehaviorDataWithImageIndexData.vColor.y*255),
		(int)(e_sTexBehaviorDataWithImageIndexData.vColor.z*255),
		(int)(e_sTexBehaviorDataWithImageIndexData.vColor.w*255),
		UT::WcharToChar(e_ppPuzzleData[e_sTexBehaviorDataWithImageIndexData.iImageIndex]->strFileName),
		e_sTexBehaviorDataWithImageIndexData.bMirror?1:0
		);
		return l_str;
	}

	enum	eAnimationEvent
	{
		eAE_NONE = 0,//just a cue,let user detect and using.
		eAE_PARTICLE,
		eAE_SOUND,
		eAE_MAX,
	};

	struct	sHintPoint
	{
		int		iPointIndex;
		WCHAR	strEventName[MAX_PATH];//ensure all the size is n*MAX_PATH
		eAnimationEvent	AnimationEvent;
		sHintPoint(int e_iIndex,WCHAR*e_str,eAnimationEvent e_eAnimationEvent)
		{
			swprintf(strEventName,MAX_PATH,L"%ls\0",e_str);
			AnimationEvent = e_eAnimationEvent;
			iPointIndex = e_iIndex; 
		}
		sHintPoint(char*e_str)
		{
			char	l_strData[TEMP_SIZE];
			sprintf(l_strData,"%s\0",e_str);
			char*	l_str = strtok(l_strData,",:");
			//point index
			iPointIndex = atoi(l_str);
			l_str = strtok(0,",:");
			swprintf(strEventName,MAX_PATH,L"%ls\0",UT::CharToWchar(l_str));
			l_str = strtok(0,",:");
			AnimationEvent = (eAnimationEvent)atoi(l_str);
		}
		WCHAR*	ToStringData()
		{
			static WCHAR l_str[TEMP_SIZE];
			swprintf(l_str,MAX_PATH,L"%d,%ls,%d\0",iPointIndex,strEventName,(int)AnimationEvent);
			return l_str;
		}
	};
	//================
	//cCurveWithTime with a new sTexBehaviorDataWithImageIndexData list vector,
	//bad design here because cCurveWithTime and cFatmingAnimationRule both has a virtual function Start and update
	//if it occur ambigous make sure what function u real want
	//ex:((cFatmingAnimationRule*)(l_p))->Update or ((cCurveWithTime*)(l_p))->Update
	//================
	class	cCueToStartCurveWithTime:public cCurveWithTime,public cBaseUVImageData,public cFatmingAnimationRule
	{
		//if true it will rotate raound originai point(like setellite)or by self.
		GET_SET_DEC(bool,m_bRotateAroundOriginlPoint,IsRotateAroundOriginlPoint,SetRotateAroundOriginlPoint);
		//the angle rotate stick on Z axis,this one should be a matrix,but I am lazy to do it now.
		GET_SET_DEC(float,m_fAngle,GetAngle,SetAngle);
		GET_SET_DEC(float,m_fScale,GetScale,SetScale);
		//blending data
		GET_SET_DEC(bool,m_bColorBlending,IsColorBlending,SetColorBlending);
		GET_SET_DEC(GLenum,m_SrcBlendingMode,GetSrcBlendingMode,SetSrcBlendingMode);
		GET_SET_DEC(GLenum,m_DestBlendingMode,GetDestBlendingMode,SetDestBlendingMode);
		//some times we do not need offset position,
		//because the points is moving and offset position let image animation become weired.
		//GET_SET_DEC(bool,m_bAvtivePosOffset,IsAvtivePosOffset,SetAvtivePosOffset);
		//we not actually need to change size color or .....so we bkeep this flag to determinal update size or else
		GET_SET_DEC(bool,m_bBehaviorUpdateActive,IsBehaviorUpdateActive,SetBehaviorUpdateActive);
		//this one should not be exist,but I am lazy to fix it now.
		GET_SET_DEC(float,m_fDepth,GetDepth,SetDepth);
		GETP_SET_DEC(std::vector<sTexBehaviorDataWithImageIndexData>,m_PointDataList,GetPointDataList,SetPointDataList);		//the image pading on,size as same as cCurveWithTime's original data,here using LERP to find out correspnd data
		GET_SET_DEC(int,m_iCurrentImageIndex,GetCurrentImageIndex,SetCurrentImageIndex);										//the image index we are using currently
		GETP_SET_DEC(sTexBehaviorDataWithImageIndexData,m_CurrentPointData,GetCurrentPointData,SetCurrentPointData);			//curren data we are going to using
		//for hint point
		bool	m_bHintPoint;
		int		m_iCurrentHintPointDataIndex;//if is true,we would go to set correspond index for m_iCurrentHintPointData,let user fetch it quickly
		//each node in the path is possible has been assign the hint point,ensure it has sorted,in the game,because we return the count that related current hint point
		GETP_SET_DEC(std::vector<sHintPoint>,m_HintPointVector,GetHintPointVector,SetHintPointVector);
		//find out the image index by CurrentPositionIndex
		inline	int	CalculateCurrentImageIndex()
		{
			if( this->m_iLOD==1 )
				return m_iCurrentPointIndex;
			float	l_fCurrentStep = m_iCurrentPointIndex/(float)this->m_FinalTimeList.size();
			l_fCurrentStep*=this->m_OriginalTimeList.size();
	#ifdef _DEBUG
			if( l_fCurrentStep>=(int)m_PointDataList.size() )
			{
				assert(0&&"call fatming - MDPI.h");
			}
	#endif
			return (int)l_fCurrentStep;
		}
		inline	void	UpdateData()//to detec current point index and image index
		{
			if( !m_PointDataList.size() )
				return;
			int	l_iCurrentOriginalPointIndex = (int)(m_PointDataList.size()-1);
			if(!this->m_bAnimationDone)
			{
				l_iCurrentOriginalPointIndex = CalculateCurrentImageIndex();
				m_iCurrentImageIndex = m_PointDataList[l_iCurrentOriginalPointIndex].iImageIndex;
				if( l_iCurrentOriginalPointIndex+1<(int)m_PointDataList.size() )
				{
					if(m_bBehaviorUpdateActive)
					{
						m_CurrentPointData = m_PointDataList[l_iCurrentOriginalPointIndex].ToLerp(&m_PointDataList[l_iCurrentOriginalPointIndex+1],this->m_fCurrentLERPTime);
					}
					else
						m_CurrentPointData = m_PointDataList[l_iCurrentOriginalPointIndex];
				}
				else
					m_CurrentPointData = m_PointDataList[l_iCurrentOriginalPointIndex];
			}
			else
				m_CurrentPointData = m_PointDataList[l_iCurrentOriginalPointIndex];
			//hint point detect
			UINT	l_iNumHintPoint = (UINT)m_HintPointVector.size();
			if( l_iNumHintPoint )
			{
				sHintPoint*l_pHintPoint = &m_HintPointVector[0];
				for( UINT i=0;i<l_iNumHintPoint;++i )
				{
					if( l_pHintPoint[i].iPointIndex == l_iCurrentOriginalPointIndex )
					{
						m_bHintPoint = true;
						m_iCurrentHintPointDataIndex = i;
						return;
					}
				}
				m_bHintPoint = false;
			}
		}
		virtual	inline	void	InternalStart(float e_fStartTime)
		{
			cCurveWithTime::Start(e_fStartTime);
			m_iCurrentImageIndex = -1;
			this->m_bAnimationDone = false;
		}
		virtual	inline	void	InternalUpdate(float e_fElpaseTime)
		{
			if( this->m_bCurveMoveDone )
				return ;
			cCurveWithTime::Update(e_fElpaseTime);
			UpdateData();
		}
		virtual			void	InternalRender();
	public:
		DEFINE_TYPE_INFO();
		CLONE_MYSELF(cCueToStartCurveWithTime);
		cCueToStartCurveWithTime(cBaseUVImageData*e_pBaseUVImageData);
		cCueToStartCurveWithTime(cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime);
		virtual~cCueToStartCurveWithTime();			
		virtual	inline	Vector3 GetPositionByTime(float e_fGlobalTime)
		{
			m_fPastTime = e_fGlobalTime;
			float	l_fGlobalTime = e_fGlobalTime-m_fStartTime;
			if( l_fGlobalTime>=0.f )
			{
				this->m_vCurrentPosition = cCurveWithTime::GetPositionByTime(m_fPastTime-m_fStartTime);
				CalculateCurrentImageIndex();
				UpdateData();
			}
			return m_vCurrentPosition;
		}
		//virtual	void ReplacePointsByCount(int e_iNumPoint);
		//virtual	void Render(Vector3 e_vRotateCenter = Vector3(0.f,0.f,0.f));
		virtual	void	InvertOrder();
		virtual	void	RearrangeTime(float e_fPercenttage);
		//most time we do not expect a specular angle to rotate,but programmer might need to do rotate if we want.
		virtual	void	RenderByGlobalTime(float e_fTime,bool e_bRotateWithAngle);
		virtual	float	GetEndTime(){ return this->GetLastTime()+this->m_fStartTime; }
		virtual	void	SetAnimationLoop(bool e_bLoop)
		{
			m_bAnimationLoop = e_bLoop;
			this->m_bCurveLoop = e_bLoop;
		}
		//because RearrangeTime Function so I do not override it,so ensure u get last time as u expect
		//virtual	float	GetLastTime(){ if( m_fOriinalTimeList.size() )return m_fStartTime+m_fOriinalTimeList[m_fOriinalTimeList.size()-1]; return 0.f; }
		//example
		//int	l_iNumRelated;
		//sHintPoint*	l_pHintPoint = GetCurrentHintPointData(&l_iNumRelated);
		//for( int i=0;i<l_iNumRelated;++i )
		//{
		//	l_pHintPoint;//do something about
		//	l_pHintPoint++;//go to next sHintPoint,because it's fetch from vector so it's continue array;
		//}
		void	AddPoint(Vector3 e_vPos,float e_fTime,Vector2 e_Size,float e_fAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror);
		void	DelPoint(int e_iIndex);
		void	ClearPoint();
		void	InsertPoint(Vector3 e_Pos,float e_fTime,sTexBehaviorDataWithImageIndexData e_sTexBehaviorDataWithImageIndexData,int e_iIndex);
		//let the path and time store list be same,
		void	SetOriginalToFinal();
		//it could be none if it doesn't occur any hitpoint
		sHintPoint*		GetCurrentHintPointData(int *e_piNumRelated);
		void			ChangeAllImageByImageIndex(int e_iIndex);
		void			Render(sPuzzleData*e_pPuzzleData,int e_iPosX = 0,int e_iPosY = 0,float e_fAngle = 0.f,Vector2 e_vRotateCenter = Vector2(0.f,0.f));
	};
}
#endif