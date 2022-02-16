#include "../stdafx.h"
#include "StaticAnimation.h"
#include "SimplePrimitive.h"
#include "PuzzleImage.h"
namespace FATMING_CORE
{
	const wchar_t*     cSAList::TypeID( L"cSAList" );
	const wchar_t*     cStaticAnimation::TypeID( L"cStaticAnimation" );


	cStaticAnimation::cStaticAnimation():cTexData(0,0)
	{
		m_bRetain = false;
		m_bChangedPlayinfo = false;
		m_bInvertPlayinfo = false;
		m_fRestPlaySec = 0.f;
		m_fTotalPlaySec = 0.f;
		m_iCurrentPlayCount = 0;
		m_iPlayCount = 1;
		m_iCurrentPlayImageIndex = -1;
		m_pPuzzleImage = 0;
		m_fTotalPlayedTime = 0.f;
	}

	cStaticAnimation::cStaticAnimation(cStaticAnimation*e_pImageShowInfoState):cTexData(e_pImageShowInfoState)
	{
		m_bRetain = e_pImageShowInfoState->m_bRetain;
		m_bChangedPlayinfo = e_pImageShowInfoState->IsChangedPlayinfo();
		m_bInvertPlayinfo = e_pImageShowInfoState->IsInvertPlayinfo();
		m_fRestPlaySec = e_pImageShowInfoState->GetRestPlaySec();
		m_fTotalPlaySec = e_pImageShowInfoState->GetTotalPlaySec();
		m_iCurrentPlayCount = 0;
		m_iPlayCount = e_pImageShowInfoState->GetPlayCount();
		m_iCurrentPlayImageIndex = e_pImageShowInfoState->GetCurrentPlayImageIndex();
		m_pPuzzleImage = e_pImageShowInfoState->GetPuzzleImage();
		this->m_ppPuzzleData = e_pImageShowInfoState->GetPuzzleData();
		this->m_ImageShowInfoVector = e_pImageShowInfoState->m_ImageShowInfoVector;
		this->SetName(e_pImageShowInfoState->GetName());
		this->m_iSingleImageHeight = e_pImageShowInfoState->GetSingleImageHeight();
		this->m_iSingleImageWidth = e_pImageShowInfoState->GetSingleImageWidth();
		m_fTotalPlayedTime = 0.f;
		this->m_vPos = e_pImageShowInfoState->m_vPos;
	}

	cStaticAnimation::~cStaticAnimation()
	{

	}

	void	cStaticAnimation::Update(float e_fElpaseTime)
	{
		if( m_iCurrentPlayImageIndex == -1 )
			return;
		int	l_iNumImage = (int)m_ImageShowInfoVector.size();
		assert( m_iCurrentPlayImageIndex <= l_iNumImage && " play index is bigger than max play image index " );
		m_fRestPlaySec -= e_fElpaseTime;
		m_fTotalPlayedTime+=e_fElpaseTime;
		while( this->m_fRestPlaySec <= 0 )
		{
			m_iCurrentPlayImageIndex++;
			//the image max accessed
			if( l_iNumImage<=m_iCurrentPlayImageIndex )
			{
				if( m_iPlayCount == -1  )
				{
					m_iCurrentPlayImageIndex = 0;
					Start(m_fRestPlaySec);
					return;
				}
				else
				if( m_iCurrentPlayCount < m_iPlayCount-1 )
				{
					m_iCurrentPlayCount++;
					m_iCurrentPlayImageIndex = 0;
				}
				else//stop play
				{
					m_iCurrentPlayImageIndex = -1;
					m_fTotalPlayedTime = 0.f;
					return;
				}
			}
			assert(m_iCurrentPlayImageIndex >= 0 &&"forgot to call start before update");
			assert(m_ImageShowInfoVector[m_iCurrentPlayImageIndex].fShowSec>0.f&&"time can't be zero");
			this->m_fRestPlaySec += m_ImageShowInfoVector[m_iCurrentPlayImageIndex].fShowSec;
		}
	}

	void	cStaticAnimation::UpdateByGlobalTime(float e_fGlobalTime)
	{
		if( this->m_fTotalPlaySec<e_fGlobalTime )
		{
			m_iCurrentPlayImageIndex = -1;
			return;
		}
		int	l_iNum = (int)m_ImageShowInfoVector.size();
		float	l_fTime = 0.f;
		for( int i=0;i<l_iNum;++i )
		{
			l_fTime+=m_ImageShowInfoVector[i].fShowSec;
			if( l_fTime>=e_fGlobalTime )
			{
				m_iCurrentPlayImageIndex = i;
				m_fRestPlaySec = l_fTime-e_fGlobalTime;
				return;
			}
		}
	}

	void	cStaticAnimation::ReCalculateTotalPlaySecond()
	{
		m_fTotalPlaySec = 0.f;
		for(UINT i=0;i<m_ImageShowInfoVector.size();++i)
		{
			sImageShowInfo	*l_pImageShowInfo = &m_ImageShowInfoVector[i];
			this->m_fTotalPlaySec+=l_pImageShowInfo->fShowSec;
		}
	}

	void	cStaticAnimation::Start(float e_fMinusTime)
	{
		assert(m_ImageShowInfoVector.size()&&"have not assign image yet");
		m_iCurrentPlayCount = 0;
		m_iCurrentPlayImageIndex = 0;
		m_fRestPlaySec = m_ImageShowInfoVector[0].fShowSec;
		m_fTotalPlayedTime = 0.f;
		if( e_fMinusTime<0.f )
			Update(e_fMinusTime);
	}

	void	cStaticAnimation::AddImageShowInfo(sImageShowInfo e_sImageShowInfo)
	{
		m_ImageShowInfoVector.push_back(e_sImageShowInfo);
		m_fTotalPlaySec += e_sImageShowInfo.fShowSec;

	}

	void	cStaticAnimation::Render()
	{
		if( !m_bVisible )
			return;
		//preciate......
		//assert(this->m_iCurrentPlayImageIndex>-1&&"imageplayer forgot to start");
		if( this->m_iCurrentPlayImageIndex != -1)
		{
			//while using multi thread it could occor some problem here,because while somthing is delete and at the moment render is active and the size is changed,
			//then data occor error
			if( m_ImageShowInfoVector.size()>(UINT)m_iCurrentPlayImageIndex )
			{
				sImageShowInfo*l_pImageShowInfo = &m_ImageShowInfoVector[this->m_iCurrentPlayImageIndex];
				POINT	*l_pPos = &l_pImageShowInfo->OffsetPos;
				POINT	*l_pSize = &l_pImageShowInfo->Size;
				float	l_fAngle = l_pImageShowInfo->fAngle;
				m_pPuzzleImage->ApplyImage();
				if( !l_pImageShowInfo->bMirror )
					DrawQuadWithTextureAndColorAndCoordinate(this->m_vPos.x+l_pPos->x,this->m_vPos.y+l_pPos->y,this->m_vPos.z,l_pSize->x,l_pSize->y,m_vColor,m_ppPuzzleData[l_pImageShowInfo->iIndex]->fUV,l_fAngle+this->m_fAngle);
				else
					DrawMirrorQuadWithTextureAndColorAndCoordinate(this->m_vPos.x+l_pPos->x,this->m_vPos.y+l_pPos->y,this->m_vPos.z,l_pSize->x,l_pSize->y,m_vColor,m_ppPuzzleData[l_pImageShowInfo->iIndex]->fUV,l_fAngle+this->m_fAngle);
			}
		}
	}

	void	cStaticAnimation::Render(int e_iX,int e_iY,float e_fAngle,Vector4 e_vColor)
	{
		if( !m_bVisible )
			return;
		//preciate......
		//assert(this->m_iCurrentPlayImageIndex>-1&&"imageplayer forgot to start");
		if( this->m_iCurrentPlayImageIndex != -1)
		{
			//while using multi thread it could occor some problem here,because while somthing is delete and at the moment render is active and the size is changed,
			//then data occor error
			if( m_ImageShowInfoVector.size()>(UINT)m_iCurrentPlayImageIndex )
			{
				sImageShowInfo*l_pImageShowInfo = &m_ImageShowInfoVector[this->m_iCurrentPlayImageIndex];
				POINT	*l_pPos = &l_pImageShowInfo->OffsetPos;
				POINT	*l_pSize = &l_pImageShowInfo->Size;
				float	l_fAngle = l_pImageShowInfo->fAngle;
				m_pPuzzleImage->ApplyImage();
				if( !l_pImageShowInfo->bMirror )
					DrawQuadWithTextureAndColorAndCoordinate((float)e_iX+l_pPos->x,(float)e_iY+l_pPos->y,this->m_vPos.z,l_pSize->x,l_pSize->y,e_vColor,m_ppPuzzleData[l_pImageShowInfo->iIndex]->fUV,l_fAngle+e_fAngle);
				else
					DrawMirrorQuadWithTextureAndColorAndCoordinate((float)e_iX+l_pPos->x,(float)e_iY+l_pPos->y,this->m_vPos.z,l_pSize->x,l_pSize->y,e_vColor,m_ppPuzzleData[l_pImageShowInfo->iIndex]->fUV,l_fAngle+e_fAngle);
			}
		}
	}

	void	cStaticAnimation::Render(int e_iIndexToPlay)
	{
		if( e_iIndexToPlay != -1 && e_iIndexToPlay<(int)m_ImageShowInfoVector.size() )
		{
			sImageShowInfo*l_pImageShowInfo = &m_ImageShowInfoVector[e_iIndexToPlay];
			POINT	*l_pPos = &l_pImageShowInfo->OffsetPos;
			POINT	*l_pSize = &l_pImageShowInfo->Size;
			float	l_fAngle = l_pImageShowInfo->fAngle;
			m_pPuzzleImage->ApplyImage();
			if( !l_pImageShowInfo->bMirror )
				DrawQuadWithTextureAndColorAndCoordinate((float)l_pPos->x,+(float)l_pPos->y,this->m_vPos.z,l_pSize->x,l_pSize->y,m_vColor,m_ppPuzzleData[l_pImageShowInfo->iIndex]->fUV,l_fAngle+m_fAngle);
			else
				DrawMirrorQuadWithTextureAndColorAndCoordinate((float)l_pPos->x,(float)l_pPos->y,this->m_vPos.z,l_pSize->x,l_pSize->y,m_vColor,m_ppPuzzleData[l_pImageShowInfo->iIndex]->fUV,l_fAngle+m_fAngle);
		}
	}

	void	cStaticAnimation::InvertPlayInfo()//invert m_ImageShowInfoVector order
	{
		m_bInvertPlayinfo = m_bInvertPlayinfo?false:true;
		int	l_iNum = (int)m_ImageShowInfoVector.size();
		std::vector<sImageShowInfo> l_ImageShowInfoVector_Invert;
		l_ImageShowInfoVector_Invert.reserve(l_iNum);
		for( int i=l_iNum-1;i>-1;--i )
		{
			l_ImageShowInfoVector_Invert.push_back(m_ImageShowInfoVector[i]);
		}
		m_ImageShowInfoVector = l_ImageShowInfoVector_Invert;
	}

	void	cStaticAnimation::PlayOrderByCommand(std::vector<int> e_PlayOrderList)
	{
		m_bChangedPlayinfo = true;
		assert(e_PlayOrderList.size()&&" Imageplay order error,size is zero");
		std::vector<sImageShowInfo>	l_ImageShowInfoVector = this->m_ImageShowInfoVector;
		m_ImageShowInfoVector.clear();
		m_ImageShowInfoVector.reserve(e_PlayOrderList.size());
		for(UINT i=0;i< e_PlayOrderList.size();++i )
			m_ImageShowInfoVector.push_back(l_ImageShowInfoVector[e_PlayOrderList[i]]);
	}

	void	cStaticAnimation::RearrangeTime(float e_fNewTime)
	{
		if( m_fTotalPlaySec != 0.f )
		{
			float	l_fPercentage = e_fNewTime/this->m_fTotalPlaySec;
			int	l_iNum = (int)m_ImageShowInfoVector.size();
			for(int i=0;i<l_iNum;++i)
			{
				sImageShowInfo*l_pImageShowInfo = &m_ImageShowInfoVector[i];
				l_pImageShowInfo->fShowSec = l_pImageShowInfo->fShowSec*l_fPercentage;
			}
			m_fTotalPlaySec = e_fNewTime;
		}
	}

	//====================================
	//
	//====================================
	cSAList::cSAList()
	{
		m_iCurrentPlayAnimationIndex = 0;
		m_pCurrentImageShowInfoState = 0;
	}

	cSAList::cSAList(cSAList*e_pImagePlayer)
	{
		for( int i=0;i<e_pImagePlayer->Count();++i )
		{
			cStaticAnimation*l_pImageShowInfoState = new cStaticAnimation((*e_pImagePlayer)[i]);
			m_ObjectList.push_back(l_pImageShowInfoState);
		}
		m_iCurrentPlayAnimationIndex = 0;
		m_pCurrentImageShowInfoState = 0;
	}

	cSAList::~cSAList()
	{
	}

	WCHAR*	cSAList::GetImageName( int e_iIndex ){ return this->m_pPuzzleImage->GetPuzzleData()[e_iIndex]->strFileName; }

	cObjectListByName<cImageIndexOfAnimation>*	cSAList::ToPIImageAnimationData()
	{
		int	l_iCount = this->Count();
		if( l_iCount )
		{
			cObjectListByName<cImageIndexOfAnimation>*l_pImageIndexOfAnimationList = new cObjectListByName<cImageIndexOfAnimation>;
			for( int i=0;i<l_iCount;++i )
			{
				cStaticAnimation*l_pStaticAnimation = this->GetObject(i);
				cImageIndexOfAnimation*l_pImageIndexOfAnimation = new cImageIndexOfAnimation(l_pStaticAnimation);
				bool	l_b = l_pImageIndexOfAnimationList->AddObject(l_pImageIndexOfAnimation);
				assert(l_b);
			}
			return l_pImageIndexOfAnimationList;
		}
		return 0;
	}
}