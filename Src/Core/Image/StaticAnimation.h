#ifndef IMAGE_PLAYER_H
#define IMAGE_PLAYER_H
#include "BaseImage.h"
#include "PuzzleImage.h"
#include "BaseUVImageData.h"
namespace FATMING_CORE
{
	struct sImageShowInfo
	{
		//how long does it show?
		float	fShowSec;
		//index for fetch texture coordinate
		int		iIndex;
		//position offset
		POINT	OffsetPos;
		//width and height
		POINT	Size;
		//angle
		float	fAngle;
		//
		bool	bMirror;
		sImageShowInfo(){ fShowSec = 0.f; iIndex = -1; OffsetPos.x = OffsetPos.y = 0; fAngle = 0.f; bMirror = false; }
	};
	//=========================
	//cStaticAnimation whole data and resource all from external,so do not delete any data at all
	//here just an interface to implement the animation lerp.
	//do not using this directly fetch it by cSAList
	//=========================
	class cStaticAnimation:public cTexData,public NamedTypedObject
	{
		float	m_fTotalPlayedTime;//the time has been past while animation is setting to start.between 0-totalplayTime
		//true playinfo has change
		GET_SET_DEC(bool,m_bChangedPlayinfo,IsChangedPlayinfo,SetChangedPlayinfo);
		//if true the playinfo had invert,else not did invert
		GET_SET_DEC(bool,m_bInvertPlayinfo,IsInvertPlayinfo,SetInvertPlayinfo);
		//how many sec still nedd pass to next image
		GET_SET_DEC(float,m_fRestPlaySec,GetRestPlaySec,SetRestPlaySec);
		//
		GET_SET_DEC(float,m_fTotalPlaySec,GetTotalPlaySec,SetTotalPlaySec);
		//counting number buffer
		GET_SET_DEC(int,m_iCurrentPlayCount,GetCurrentPlayCount,SetGetCurrentPlayCount);
		//how many time we want to play,if the value is -1 it will be loop
		GET_SET_DEC(int,m_iPlayCount,GetPlayCount,SetPlayCount);
		//which image is playing now,while it's -1,then it's play over.
		GET_SET_DEC(int,m_iCurrentPlayImageIndex,GetCurrentPlayImageIndex,SetCurrentPlayImageIndex);
		//desitination image for render
		GET_SET_DEC(cPuzzleImage*,m_pPuzzleImage,GetPuzzleImage,SetPuzzleImage);
		//image data uv offset pos and name....
		GET_SET_DEC(sPuzzleData**,m_ppPuzzleData,GetPuzzleData,SetPuzzleData);
		//single image width,the max width of images,assign data at ImageResourceManager
		GET_SET_DEC(int,m_iSingleImageWidth,GetSingleImageWidth,SetSingleImageWidth);
		//single image height,the max width of images,assign data at ImageResourceManager
		GET_SET_DEC(int,m_iSingleImageHeight,GetSingleImageHeight,SetSingleImageHeight);
		//
		GET_SET_DEC(bool,m_bRetain,IsRetain,SetRetain);
	public:
		cStaticAnimation();
		cStaticAnimation(cStaticAnimation*e_pImageShowInfoState);
		CLONE_MYSELF(cStaticAnimation);
		~cStaticAnimation();
		std::vector<sImageShowInfo> m_ImageShowInfoVector;//progress = GetCurrentPlayImageIndex()/m_ImageShowInfoVector.size()
		//only calculate the one cycle play time here does not calculate about play count
		//make sure playcount is not -1(loop).
		void	ReCalculateTotalPlaySecond();
		DEFINE_TYPE_INFO();
		//to indicate the first image rest time should minus
		virtual	void	Start(float e_fMinusTime = 0.f);
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	UpdateByGlobalTime(float e_fGlobalTime);
		virtual	void	AddImageShowInfo(sImageShowInfo e_sImageShowInfo);
		virtual	void	Render(int e_iX,int e_iY,float e_fAngle = 0.f,Vector4 e_vColor = Vector4::One);
		virtual	void	Render();
		virtual	void	Render(int e_iIndexToPlay);
		virtual	void	InvertPlayInfo();//invert m_ImageShowInfoVector order
		virtual	void	PlayOrderByCommand(std::vector<int> e_PlayOrderList);	//play image byt input list order
		bool			IsDone(){ return m_iCurrentPlayImageIndex==-1?true:false; }	//is it done(play done)?
		void			RearrangeTime(float e_fNewTime);//compar to original time and reset by persentage
		float			GetPlayedTime(){ return m_fTotalPlayedTime; }//progress = GetPlayedTime()/GetTotalPlaySec()
	};
	//=======================
	//here for implement the animaion data fetch and store
	//store the cStaticAnimation list,and info for user to fetch.
	//u could rneder the image that cSAList::cBaseUVImageData::m_pBaseImage does have and call
	//Render
	//=======================
	class	cSAList:public cMulti_PI_Image,public cObjectListByName<cStaticAnimation>
	{
		//we have a list of animation play list,this indicate the index we are using
		GET_SET_DEC(int,m_iCurrentPlayAnimationIndex,GetCurrentPlayAnimationIndex,SetGetCurrentPlayAnimationIndex)
		//depend by start,and m_iCurrentPlayAnimationIndex
		GET_SET_DEC(cStaticAnimation*,m_pCurrentImageShowInfoState,GetCurrentImageShowInfoState,SetGetCurrentImageShowInfoState);
		//
		GET_SET_DEC(Vector3,m_vPos,GetPos,SetPos);
	public:
		DEFINE_TYPE_INFO();
		cSAList();
		//if u don't wanna share memory,input e_bTexCoordinateFromResource as false,so u could delete as u don't need it any more
		cSAList(cSAList*e_pImagePlayer);
		//we do not delete m_pBaseImage,because it's a pointer

		~cSAList();
		//to indicate the first image rest time should minus
		inline	void	Start(int iIndex = 0,float e_fMinusTime = 0.f)
		{
			m_iCurrentPlayAnimationIndex = iIndex;
			m_pCurrentImageShowInfoState = this->m_ObjectList[m_iCurrentPlayAnimationIndex];
			m_pCurrentImageShowInfoState->Start(e_fMinusTime);
		}
		inline	void	Start(WCHAR*e_strName,float e_fMinusTime = 0.f)
		{
			m_iCurrentPlayAnimationIndex = this->GetObjectIndexByName(e_strName);
			m_pCurrentImageShowInfoState = this->m_ObjectList[m_iCurrentPlayAnimationIndex];
			m_pCurrentImageShowInfoState->Start(e_fMinusTime);	
		}
		inline	void	Update(float e_fElpaseTime){ m_pCurrentImageShowInfoState->Update(e_fElpaseTime); }
		inline	void	UpdateByGlobalTime(float e_fGlobalTime){ m_pCurrentImageShowInfoState->UpdateByGlobalTime(e_fGlobalTime); }
		inline	void	Render(float e_fAngle = 0.f,Vector4 e_vColor = Vector4::One){ m_pCurrentImageShowInfoState->Render((int)m_vPos.x,(int)m_vPos.y,e_fAngle,e_vColor); }
		inline	void	Render(int e_iX,int e_iY,float e_fAngle = 0.f,Vector4 e_vColor = Vector4::One){ m_pCurrentImageShowInfoState->Render(e_iX,e_iY,e_fAngle,e_vColor); }
		//how many sec still nedd pass to next image
		float GetRestPlaySec(){ return m_pCurrentImageShowInfoState->GetRestPlaySec(); }
		//
		float GetTotalPlaySec(){ return m_pCurrentImageShowInfoState->GetTotalPlaySec(); }
		//which image is playing now
		int GetCurrentPlayImageIndex(){ return m_pCurrentImageShowInfoState->GetCurrentPlayImageIndex(); }
		//single image width,the max width of images
		int	GetSingleImageWidth(){ return m_pCurrentImageShowInfoState->GetSingleImageWidth(); }
		//single image height,the max width of images
		int GetSingleImageHeight(){ return m_pCurrentImageShowInfoState->GetSingleImageHeight(); }
		//
		WCHAR*	GetImageName( int e_iIndex );

		cObjectListByName<cImageIndexOfAnimation>*	ToPIImageAnimationData();
	};
}
#endif