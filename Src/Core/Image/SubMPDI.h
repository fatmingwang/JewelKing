#ifndef SUB_MPDI
#define SUB_MPDI
#include "FatmingAnimationRule.h"
#include "PuzzleImage.h"
#include "BaseUVImageData.h"
namespace FATMING_CORE
{
	//==========
	//each point could has below data to do something
	//==========
	struct	sTexBehaviorDataWithImageIndexData
	{
		Vector2	Size;			//size
		Vector3	vAngle;			//angle
		Vector4	vColor;			//color
		int		iImageIndex;	//image index
		bool	bMirror;		//mirror
		bool	bHintPoint;		//is this point a cue?
		cPuzzleImage*pPI;       //the pi it used,related of iImageIndex
		cPuzzleImageUnit*   GetPuzzleImageUnit(){ return (*pPI)[iImageIndex]; }
		sPuzzleData*        GetPuzzleData(){ return pPI->GetPuzzleData()[iImageIndex]; }
		Vector2 GetScale();
		sTexBehaviorDataWithImageIndexData();
		sTexBehaviorDataWithImageIndexData(sTexBehaviorDataWithImageIndexData*e_pTexBehaviorDataWithImageIndexData);
		sTexBehaviorDataWithImageIndexData(Vector2 e_Size,Vector3 e_vAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror,cPuzzleImage*e_pPI);
        void    DumpData(sTexBehaviorDataWithImageIndexData*e_pDest);

		inline	void	ToLerp(sTexBehaviorDataWithImageIndexData*e_pDest,float e_fLERP,sTexBehaviorDataWithImageIndexData*e_pSaveTo)
		{
			//because fucking 1 pixel problem,so we force it to be a intger value!
			e_pSaveTo->Size.x = (float)((int)LERP<float>(Size.x,e_pDest->Size.x,e_fLERP));
			e_pSaveTo->Size.y = (float)((int)LERP<float>(Size.y,e_pDest->Size.y,e_fLERP));
			e_pSaveTo->vAngle = LERP<Vector3>(vAngle,e_pDest->vAngle,e_fLERP);
			e_pSaveTo->vColor = LERP<Vector4>(vColor,e_pDest->vColor,e_fLERP);
			e_pSaveTo->iImageIndex = iImageIndex;
			e_pSaveTo->bMirror = e_pDest->bMirror;
			e_pSaveTo->pPI = e_pDest->pPI;
		}
	};

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
	};
	enum	eImageAnchorType
	{
		eIAT_LEFT_UP = 0,
		eIAT_CENTER_UP,
		eIAT_RIGHT_UP,
		eIAT_LEFT_CENTER,
		eIAT_CENTER_CENTER,
		eIAT_RIGHT_CENTER,
		eIAT_LEFT_DOWN,
		eIAT_CENTER_DOWN,
		eIAT_RIGHT_DOWN,
		eIAT_MAX,
	};

	WCHAR*	ValueToStringW(eImageAnchorType e_eAnchorType);
	eImageAnchorType	GetAnchorType(const WCHAR*e_str);
	class	cMultiPathDynamicImage;
	//================
	//cCurveWithTime with a new sTexBehaviorDataWithImageIndexData list vector,
	//bad design here because cCurveWithTime and cFatmingAnimationRule both has a virtual function Start and update
	//if it occur ambigous make sure what function u real want
	//ex:((cFatmingAnimationRule*)(l_p))->Update or ((cCurveWithTime*)(l_p))->Update
	//================
	class	cCueToStartCurveWithTime:public cCurveWithTime,public cMulti_PI_Image,public cFatmingAnimationRule
	{
		cMatrix44			m_matRenderMatrix;
		friend class		cMultiPathDynamicImage;
		//the pos to anchor image size change
		GET_SET_DEC(eImageAnchorType,m_eAnchorType,GetAnchorType,SetAnchorType);
		//we might set a specific offset,ex set it into 3D world or else
		//but most time it won't do any thing.
		GET_SET_DEC(cMatrix44*,m_pExtractTransform,GetExtractTransform,SetExtractTransform);
		//if true it will rotate raound originai point(like setellite)or by self.
		GET_SET_DEC(bool,m_bRotateAroundOriginlPoint,IsRotateAroundOriginlPoint,SetRotateAroundOriginlPoint);
		//the angle rotate stick on Z axis,this one should be a matrix,but I am lazy to do it now.
		GET_SET_DEC(Vector3,m_vAngle,GetAngle,SetAngle);
		GETP_SET_DEC(Vector3,m_vRotationAnglePosOffset,GetRotationAnglePosOffset,SetRotationAnglePosOffset);
		GET_SET_DEC(float,m_fScale,GetScale,SetScale);
		//blending data,if all cCueToStartCurveWithTime under its parent MPDI,it could optmize to speed up,but I am not doing it right now,fatming,fuck
		GET_SET_DEC(bool,m_bColorBlending,IsColorBlending,SetColorBlending);
		GET_SET_DEC(GLenum,m_SrcBlendingMode,GetSrcBlendingMode,SetSrcBlendingMode);
		GET_SET_DEC(GLenum,m_DestBlendingMode,GetDestBlendingMode,SetDestBlendingMode);
		//some times we do not need offset position,
		//because the points is moving and offset position let image animation become weired.
		//GET_SET_DEC(bool,m_bAvtivePosOffset,IsAvtivePosOffset,SetAvtivePosOffset);
		//we not actually need to change size color or .....so we bkeep this flag to determinal update size or else
		GET_SET_DEC(bool,m_bBehaviorUpdateActive,IsBehaviorUpdateActive,SetBehaviorUpdateActive);
		GETP_SET_DEC(std::vector<sTexBehaviorDataWithImageIndexData*>,m_PointDataList,GetPointDataList,SetPointDataList);		//the image pading on,size as same as cCurveWithTime's original data,here using LERP to find out correspnd data
		GET_SET_DEC(sTexBehaviorDataWithImageIndexData*,m_pCurrentPointData,GetCurrentPointData,SetCurrentPointData);			//curren data we are going to using
		//for hint point
		bool				m_bHintPoint;
		int					m_iCurrentHintPointDataIndex;//if is true,we would go to set correspond index for m_iCurrentHintPointData,let user fetch it quickly
		//each node in the path is possible has been assign the hint point,ensure it has sorted,in the game,because we return the count that related current hint point
		GETP_SET_DEC(std::vector<sHintPoint>,m_HintPointVector,GetHintPointVector,SetHintPointVector);
		//find out the image index by CurrentPositionIndex
		int					CalculateCurrentPointIndex();
		virtual		void	UpdateData();
		virtual		void	InternalInit();
		virtual		void	InternalUpdate(float e_fElpaseTime);
		virtual		void	InternalRender();
		cMatrix44  			GetRenderPuzzleDataAndMatrix(sTexBehaviorDataWithImageIndexData*e_pImageData,int e_iStride,float*e_pOutVertices,float*e_pOutTextureUV,sPuzzleData*e_pPuzzleData,Vector3 e_vPos,Vector3 e_vAngle = Vector3::Zero);
		//force to render,input the position u want to show,default m_vCurrentPos and m_vPos is working inside
		void				RenderPuzzleData(sPuzzleData*e_pPuzzleData,Vector3 e_vPos,Vector3 e_vAngle = Vector3::Zero);
	public:
		DEFINE_TYPE_INFO();
		CLONE_MYSELF(cCueToStartCurveWithTime);
		cCueToStartCurveWithTime();
		cCueToStartCurveWithTime(cMulti_PI_Image*e_pMulti_PI_Image);
		cCueToStartCurveWithTime(cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime);
		virtual~cCueToStartCurveWithTime();
		int	GetCurrentImageIndex(){ return this->m_pCurrentPointData->iImageIndex; }
		//prevent ambiguous
		inline	virtual void	Init(){ cFatmingAnimationRule::Init(); }
		//prevent ambiguous
		inline	virtual void	Render(){ cFatmingAnimationRule::Render(); }
		//prevent ambiguous
		inline	virtual void	Update(float e_fElpaseTime){ cFatmingAnimationRule::Update(e_fElpaseTime); }
		virtual void			RenderFirsttFrameImage(bool e_bBlending,Vector4 e_vColor);
		virtual	Vector3			GetPositionByTime(float e_fGlobalTime);
		//virtual	void ReplacePointsByCount(int e_iNumPoint);
		//virtual	void	Render(Vector3 e_vRotateCenter = Vector3(0.f,0.f,0.f));
		virtual	void			InvertOrder();
		virtual	void			RearrangeTime(float e_fPercenttage);
		//most time we do not expect a specular angle to rotate,but programmer might need to do rotate if we want.
		virtual	void			RenderByGlobalTime(float e_fTime,bool e_bRotateWithAngle);
		virtual	void			RenderLastFrame();
		virtual	float			GetEndTime(){ return this->GetLastTime()+this->m_fStartTime; }
		virtual	void			SetAnimationLoop(bool e_bLoop);
		//
		virtual bool			IsAnimationDone(){ return this->m_bAnimationLoop?false:this->IsCurveMoveDone(); }
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
		sTexBehaviorDataWithImageIndexData*GetPointData(int e_iIndex);
		void					AddPoint(Vector3 e_vPos,float e_fTime,Vector2 e_Size,Vector3 e_vAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror,cPuzzleImage*e_pPI,bool e_bCheckOptmize = false);
		void					AddPoint(Vector3 e_vPos,float e_fTime,Vector2 e_Size,float e_fAngle,Vector4 e_vColor,int e_iImageIndex,bool e_bMirror,cPuzzleImage*e_pPI,bool e_bCheckOptmize = false);
		void					DelPoint(int e_iIndex);
		void					ClearPoint();
		void					InsertPoint(Vector3 e_Pos,float e_fTime,sTexBehaviorDataWithImageIndexData*e_pTexBehaviorDataWithImageIndexData,int e_iIndex);
		//let the path and time store list be same,
		void					SetOriginalToFinal();
		//do not call this every frame because it reset all color.
		void					SetColor(Vector4 e_vColor);
		void					SetCurrentColor(Vector4 e_vColor);
		//it could be none if it doesn't occur any hitpoint
		sHintPoint*				GetCurrentHintPointData(int *e_piNumRelated);
		//if the image is different,and you wanna sacle image too set e_bScalingImage as true.
		void					ChangeAllImageByImageIndex(int e_iIndex,bool e_bScalingImage = false);
		void					Render(sPuzzleData*e_pPuzzleData,int e_iPosX = 0,int e_iPosY = 0);
		//======================================================
		//below function is change point data,do not call those function at game,peformance is bad!
		//======================================================		
		//set point data rotation,rotate by curve's center
		void					RotateCurveWithCurveCenter(Quaternion e_Quaternion,bool e_bRotateImage);
		//rotate with a specific center position
		void					RotateCurveWithSpecificCenterPosition(Vector3 e_vCenterPos,Quaternion e_Quaternion,bool e_bRotateImage);
		void					SetScaleForPointData(float e_fScale);
		//set first image as e_fAngle and minus the offset for each point
		void					ImageRotation(Vector3 e_vAngle);
		void					ImageRelativeRotation(Vector3 e_vAngle);
		//to collide current data,usually all the data is same.
		//only support non scale collide,because I am lazy to do this.
		virtual	bool			Collide(int e_iPosX,int e_iPosY);
		//for collide debug
		void					RenderCollide();
		//for optmize render,ensure all pi is same texture,its triangles not triangle strip
		//force to fetch mean hwile time is over progress it still get last frame's data
		//2 triangles 6 points(xyz),total size 18 float
		bool					GetRenderData(float*e_pfVertices,float*e_pfUV,float*e_pfColor,bool e_bForceToFetch = true);
		//for editor if this PI is not in using return false
		bool					IsUsingPuzzleImage(cPuzzleImage*e_pPI);
		//
		cMatrix44				GetRenderMatrix(){return m_matRenderMatrix;}
	};

	typedef cCueToStartCurveWithTime	cSubMPDI;
}
#endif