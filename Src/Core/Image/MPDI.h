#ifndef MPDI_H
#define MPDI_H

#include "../Math/Curve.h"
#include "StaticAnimation.h"
#include "SubMPDI.h"
#include "VideoSubMPDI.h"
namespace FATMING_CORE
{
	//=====================
	//MPDI = Multi Path Dynamic Image
	//multi path ad eah path has its own behavior,image change color blending angle and size and else
	//each render's position is set by center of image
	//=====================

	//<cMPDIList Name="" Count="" />
		//<MultiPathDynamicImage Name="79979" Count="1"	Playcount="1" TotalPlayTime="" />
			//<cCueToStartCurveWithTime Name="" StartTime="1" Count="3" />
			//<CurveData Name="79979" Count ="3" LOD="1" />
			//<CurvePoint  P1="0,0" P2="10,20" P3="20,20"  />
			//<CurveTime   P1="0.5" P2="0.5" P3="0.5"/>
			//<ImageIndex P1="1" P2="2" P3="3" >
		//<MultiPathDynamicImage/>
	//<cMPDIList/>
	//==================
	//if the e_pImageResourceManager input is not exited or be empty
	//parsing the data and new a resource manager,or u coul d add it to destination resource manager
	//
	//==================
 //   cGameApp::m_spAnimationParser->Parse("C:/Users/Fatming/Desktop/MPDI/MPDI/ACT_01.mpdi");
 //   cMultiPathDynamicImage*g_pMultiPathDynamicImage = 0;
 //   g_pMultiPathDynamicImage = cGameApp::m_spAnimationParser->GetMPDI(0)->GetObject(0);
 //   g_pMultiPathDynamicImage->Start();    
 //   g_pMultiPathDynamicImage->Update(e_fElpaseTime);
	//g_pMultiPathDynamicImage->Render();
	//the time calculate has a small offset.........
	//ex: 300 second might has 0.2second...but after test a hour result might be different
	//ex: 120                  0.2
	//I am lazy to test...
	class	cMultiPathDynamicImage:public cFatmingGroupBehaviorList<cCueToStartCurveWithTime>,public cMulti_PI_Image
	{
	private:
		Vector3			m_vPos;
	protected:
		//
		GET_SET_DEC(bool,m_bStopAtLastFrame,IsStopAtLastFrame,SetStopAtLastFrame);
		//call CheckRenderOptmize to change render mode,while AddObject is called,make sure CheckRenderOptmize is caled
		bool	m_bRenderOptmize;
		//MPDI's width and height
		GET_SET_DEC(Vector2,m_vDrawSize,GetDrawSize,SetDrawSize);
		//scale for image size
		float			m_fScale;
		Vector3			m_vAngle;
		bool			m_bHitHintPoint;
		//
		Vector4*		m_pViewPort;
	public:
		DEFINE_TYPE_INFO();
		CLONE_MYSELF(cMultiPathDynamicImage);
		//for create
		cMultiPathDynamicImage(cMulti_PI_Image*e_pMulti_PI_Image);
		//for clone
		cMultiPathDynamicImage(cMultiPathDynamicImage*e_pMultiPathDynamicImage);
		~cMultiPathDynamicImage();

		//after add object or remove object call this
		void			RefreshData();
		//here we should write a shader code for optmize for different pi texture,but I am lazy to do it now.
		void			CheckRenderOptmize();
		//please only call once while data is changed.
		void			FindoutAllPointsCenter();
		//
		virtual	void	Merge(cMultiPathDynamicImage*e_pMultiPathDynamicImage);
		//
		Vector3			GetPos(){return m_vPos;}
		Vector3			GetPosByImageCenter();
		//the animation is possible not in the original position(0,0),so we minus the start position(m_vOffsetPos),let it show at the position as we expected.
		//this one is set image render by left up of image,if u would like to render by image center call SetPosByImageCenter.
		virtual	void	SetPos(Vector3 e_vPos);
		virtual	void	SetTransform(cMatrix44 e_mat);
		virtual	void	SetTransformByImageCenter(cMatrix44 e_mat);
		cMatrix44		GetTransform();
		cMatrix44		GetTransformByImageCenter();
		//wanna image center show at the position we indicate
		void			SetPosByImageCenter(Vector3 e_vPos);
		//a scale value to change size at the game
		virtual	void	SetScale(float e_fValue);
		//to effect children's size
		float			GetScale(){ return m_fScale; }
		//
		Vector3			GetAngle(){ return m_vAngle; }
		virtual void	SetAngle(Vector3 e_vAngle);
		void			SetAngle(float e_fZangle);
		cMatrix44		GetMatrix();
		void            SetColor(Vector4 e_vColor);
		void            SetCurrentColor(Vector4 e_vColor);
		//you could could call update and assign last time and minus a little bit offset to get same result
		void			RenderLastFrameImage();
		void			RenderFirsttFrameImage(bool e_bBlending = true,Vector4 e_vColor = Vector4::One);
		virtual	void	Update(float e_fElpaseTime);
		bool			IsHitHintPoint(){ return m_bHitHintPoint; }
		//make all its children to render by one bunch
		virtual	void	Render();
		virtual	void	RenderByGlobalTime();
		//virtual	void	Init(){ cFatmingGroupBehaviorList<cCueToStartCurveWithTime>::Init(); }
		//assume we only have one cCueToStartCurveWithTime,so input is 0,or u should indicate specific one
		Vector3			GetLastFramePos(int e_iIndex = 0);
		Vector3			GetPathCurrentPos(int e_iIndex = 0);
		Vector3			GetPathCurrentPosWithPos(int e_iIndex = 0);

		virtual	bool	IsRotation(){ return this->m_vAngle.Length() != 0.f&&m_vPos != m_vRotationCenter; }


		//all path rotate around it's original center or self rotation
		void			SetRotateAroundOriginlPoint(bool e_b);
		//======================================================
		//below function is change point data,do not call those function at game,peformance is bad!
		//======================================================
		//add a specific relative position to all child.
		void			SetRelativePosToChild(Vector3 e_vPos);
		//set a offset position by center position
		void			SetCenterPosToChild(Vector3 e_vPos);
		void			SetSelfRotationToChild(Quaternion e_Quaternion,bool e_bRotateImage);
		void			SetCenterRotationToChild(Quaternion e_Quaternion,bool e_bRotateImage);
		//for all point data's size at editor,do not call this at game
		void			SetScaleForAllChildPointData(float e_fValue);
		//
		void			RenderCollide();
		//index for index of cCueToStartCurveWithTime
		bool			Collide(int e_iPosX,int e_iPosY,int e_iIndex);
		//Collide All
		virtual	bool    Collide(int e_iPosX,int e_iPosY);
		Vector4*		GetViewPort(){return m_pViewPort;};
		void			SetViewPort(Vector4 e_vViewPort);
		void			RemoveViewPort();
		bool			IsUsingPI(cPuzzleImage*e_pPI,bool e_bRemoveReferenceIfNotInUsing = true);
		void			SetExtraTransform(cMatrix44* e_pmat);
	};

	typedef cMultiPathDynamicImage	cMPDI;
	//=========================
	//an interface just like image player
	//=========================
	class	cMPDIList:public cObjectListByName<cMultiPathDynamicImage>,public cMulti_PI_Image
	{
		//we have a list of animation play list,this indicate the index we are using
		GET_SET_DEC(int,m_iCurrentPlayAnimationIndex,GetCurrentPlayAnimationIndex,SetGetCurrentPlayAnimationIndex)
		//depend by start,and m_iCurrentPlayAnimationIndex
		cMultiPathDynamicImage*m_pCurrentMultiPathDynamicImage;
		//
		Vector3	m_vPos;
	public:
		//if true reorder vertex sort,ensure u have set this back or 2D render view will be wrong
		static bool	m_b3DCameraView;

		DEFINE_TYPE_INFO();
		cMPDIList();
		//for clone
		cMPDIList(cMPDIList*e_pcMPDIList);
		~cMPDIList(){}
		Vector3	GetPos(){ return m_vPos; }
		//as u calling this,current MPDI will change too.
		virtual	void	SetPos(Vector3 e_vPos)
		{
			this->m_vPos = e_vPos;
			if( m_pCurrentMultiPathDynamicImage)
				m_pCurrentMultiPathDynamicImage->SetPos(e_vPos);
		}
		inline	void	Start(int iIndex = 0)
		{
			m_iCurrentPlayAnimationIndex = iIndex;
			m_pCurrentMultiPathDynamicImage = this->m_ObjectList[m_iCurrentPlayAnimationIndex];
			m_pCurrentMultiPathDynamicImage->Init();
		}

		inline	void	Start(WCHAR*e_strName)
		{
			m_iCurrentPlayAnimationIndex = this->GetObjectIndexByName(e_strName);
			m_pCurrentMultiPathDynamicImage = this->m_ObjectList[m_iCurrentPlayAnimationIndex];
			m_pCurrentMultiPathDynamicImage->Init();
		}

		inline	void	Update(float e_fElpaseTime){ m_pCurrentMultiPathDynamicImage->Update(e_fElpaseTime); }
		inline	void	UpdateByGlobalTime(float e_fGlobalTime){ m_pCurrentMultiPathDynamicImage->UpdateByGlobalTime(e_fGlobalTime); }
		inline	void	Render(){ m_pCurrentMultiPathDynamicImage->Render();  }
		inline	void	RenderByGlobalTime(){ m_pCurrentMultiPathDynamicImage->RenderByGlobalTime(); }
		//
		float GetTotalPlaySec(){ return m_pCurrentMultiPathDynamicImage->GetTotalPlayTime(); }
		//if there is no any data use this pi,return true and remove the data,or return false and do not remove the data
		//only remove reference not delete from animation parser!
		bool    RemovePIReference(cPuzzleImage*e_pPuzzleImage,std::wstring*e_pInUsingDataInfo);
		void    ForceRemovePIReference(cPuzzleImage*e_pPuzzleImage);
		//
		void	Merge(cMPDIList*e_pMPDIList);


		//set original name xxxx_mpdi as xxxx.mpdi
		static  char*GetFileName(WCHAR*e_strObjectName);
		static  WCHAR*FileToMPDIListName(const WCHAR*e_strObjectName);
		static  WCHAR*FileToMPDIListName(const char*e_strFileName);
	};
}
#endif