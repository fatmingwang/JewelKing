#include "../stdafx.h"
#include "SimplePrimitive.h"
#include "BaseImage.h"
#include "PuzzleImage.h"
#include "NumerialImage.h"
#include "StaticAnimation.h"
namespace FATMING_CORE
{
	const wchar_t*     cPuzzleImage::TypeID( L"cPuzzleImage" );
	const wchar_t*     cPuzzleImageUnit::TypeID( L"cPuzzleImageUnit" );

	cImageIndexOfAnimation::cImageIndexOfAnimation(cImageIndexOfAnimation*e_pImageIndexOfAnimation)
	{
		m_pfEndTime = 0;
	    m_pNameList = 0;
	    this->SetName(e_pImageIndexOfAnimation->GetName());
	    if( e_pImageIndexOfAnimation->m_pNameList )
	    {
	        m_pNameList = new std::vector<std::wstring>;
            *m_pNameList = *e_pImageIndexOfAnimation->m_pNameList;
        }
        m_ImageAnimationDataList = e_pImageIndexOfAnimation->m_ImageAnimationDataList;
	}

	cImageIndexOfAnimation::cImageIndexOfAnimation(cStaticAnimation*e_pStaticAnimation)
	{
		m_pfEndTime = 0;
		m_pNameList = 0;
		this->SetName(e_pStaticAnimation->GetName());
		size_t l_iSize = e_pStaticAnimation->m_ImageShowInfoVector.size();
		m_ImageAnimationDataList.resize(l_iSize);
		sImageIndexAndTimeGap	l_sImageIndexAndTimeGap;
#ifdef _DEBUG
		this->m_pNameList = new std::vector<std::wstring>;
		this->m_pNameList->reserve(l_iSize);
#endif
		for(size_t i=0;i<l_iSize;++i  )
		{
			l_sImageIndexAndTimeGap.fTimeGap = e_pStaticAnimation->m_ImageShowInfoVector[i].fShowSec;
			l_sImageIndexAndTimeGap.iIndex = e_pStaticAnimation->m_ImageShowInfoVector[i].iIndex;
			m_ImageAnimationDataList[i] = l_sImageIndexAndTimeGap;
#ifdef _DEBUG
			this->m_pNameList->push_back(e_pStaticAnimation->GetPuzzleImage()->GetObject(l_sImageIndexAndTimeGap.iIndex)->GetName());
#endif
		}
	}
	
	void    cImageIndexOfAnimation::GenerateImageIndexByPI(cPuzzleImage*e_pPI,std::vector<std::wstring>   *e_pNameList)
	{
	    size_t  l_iSize = e_pNameList->size();
	    m_ImageAnimationDataList.resize(l_iSize);
	    for( size_t i=0;i<l_iSize;++i )
	    {
			m_ImageAnimationDataList[i].iIndex = e_pPI->GetObjectIndexByName((*e_pNameList)[i].c_str());
	    }
	}

	int	cImageIndexOfAnimation::GetImageIndexByCurrentTime( float e_fTime )
	{
		float	l_fModulus = UT::GetFloatModulus(e_fTime,this->GetEndTime());
		assert(m_ImageAnimationDataList.size()>0);
		int	l_iIndex = 0;
		float	l_fTargetGap = m_ImageAnimationDataList[l_iIndex].fTimeGap;
		while( l_fModulus > l_fTargetGap )
		{
			l_fModulus -= l_fTargetGap;
			++l_iIndex;
			l_fTargetGap = m_ImageAnimationDataList[l_iIndex].fTimeGap;
		}
		return m_ImageAnimationDataList[l_iIndex].iIndex;
	}
		
	cPuzzleImageUnit::cPuzzleImageUnit(sPuzzleData*e_pPuzzleData,GLuint e_uiTextureIndex,cPuzzleImage*e_pPuzzleImageParent,bool e_bFetchPixels)
	:cBaseImage(0,false)
	{
		SetName(e_pPuzzleData->strFileName);
		m_pNext = 0;
		m_pPrior = 0;
		this->m_pParent = e_pPuzzleImageParent;
		this->m_bTextureFromResource = true;
		memcpy(this->m_fUV,e_pPuzzleData->fUV,sizeof(float)*4);
		this->m_uiImageIndex = e_uiTextureIndex;
		this->m_OffsetPos = e_pPuzzleData->OffsetPos;
		this->m_iShowWidth  = e_pPuzzleData->Size.x;
		this->m_iShowHeight = e_pPuzzleData->Size.y;
		this->m_iWidth = e_pPuzzleImageParent->GetImageWidth();
		this->m_iHeight = e_pPuzzleImageParent->GetImageHeight();
		this->m_iPixelFormat = e_pPuzzleImageParent->GetPixelFormat();
		m_pPuzzleData = e_pPuzzleData;
		if( e_bFetchPixels )
		{
		    int l_iChannel = 4;//assume channel is 4.
		    if( m_iPixelFormat == GL_RGB )
		    {
		        l_iChannel = 3;
		    }
		    unsigned char*l_pPixels = e_pPuzzleImageParent->GetPixels();
		    int l_iIndex = (e_pPuzzleImageParent->GetImageWidth()*e_pPuzzleData->ShowPosInPI.y+e_pPuzzleData->ShowPosInPI.x)*l_iChannel;
		    this->m_pPixels = &l_pPixels[l_iIndex];
		}
	}

	cPuzzleImageUnit::cPuzzleImageUnit(cPuzzleImageUnit*e_pPuzzleImageUnit):cBaseImage((cBaseImage*)e_pPuzzleImageUnit)
	{
		SetName(e_pPuzzleImageUnit->GetName());
		m_pPuzzleData = e_pPuzzleImageUnit->m_pPuzzleData;
		m_pNext = e_pPuzzleImageUnit->GetNext();
		m_pPrior = e_pPuzzleImageUnit->GetPrior();
		this->m_pParent = e_pPuzzleImageUnit->GetParent();
		//???? it has done at simpleGLTexture
		this->m_bTextureFromResource = true;
		memcpy(this->m_fUV,e_pPuzzleImageUnit->GetUV(),sizeof(float)*4);
		this->m_uiImageIndex = e_pPuzzleImageUnit->GetImageIndex();
		this->m_OffsetPos = *e_pPuzzleImageUnit->GetOffsetPos();
		this->m_iShowWidth = this->m_iWidth = *e_pPuzzleImageUnit->GetImageShowWidth();
		this->m_iShowHeight = this->m_iHeight = *e_pPuzzleImageUnit->GetImageShowHeight();
	}
	cPuzzleImageUnit::cPuzzleImageUnit(cBaseImage*e_pBaseImage):cBaseImage(e_pBaseImage)
	{
		SetName(e_pBaseImage->GetName());
		m_pPuzzleData = 0;
		m_pNext = 0;
		m_pPrior = 0;
		this->m_pParent = 0;
		//this->m_bTextureFromResource = true;
	}

	cPuzzleImage::cPuzzleImage(cPuzzleImage*e_pPuzzleImage):cBaseImage(e_pPuzzleImage)
	{
		m_pImageIndexOfAnimation = e_pPuzzleImage->m_pImageIndexOfAnimation;
		m_iNumImage = e_pPuzzleImage->GetNumImage();
		m_ppPuzzleData = e_pPuzzleImage->GetPuzzleData();
		m_pfAllChildrenTriangleStripUV = e_pPuzzleImage->m_pfAllChildrenTriangleStripUV;
		m_pfAllChildrenTwoTriangleUV = e_pPuzzleImage->m_pfAllChildrenTwoTriangleUV;
		this->CopyListPointer(e_pPuzzleImage);
	}

	cPuzzleImage::cPuzzleImage(char*e_strName,std::vector<sPuzzleData> *e_pPuzzleDataList,bool e_bGenerateAllUnit,bool e_bFetchPixels)
	:cBaseImage(e_strName,e_bFetchPixels)
	{
		m_pImageIndexOfAnimation = 0;
		m_pfAllChildrenTriangleStripUV = 0;
		m_pfAllChildrenTwoTriangleUV = 0;
		if( e_pPuzzleDataList )
		{
			m_ppPuzzleData = new sPuzzleData*[e_pPuzzleDataList->size()];
			m_iNumImage = (int)e_pPuzzleDataList->size();
			for( int i=0;i<m_iNumImage;++i )
			{
				sPuzzleData l_DestsPuzzleData = (*e_pPuzzleDataList)[i];
				m_ppPuzzleData[i] = new sPuzzleData(l_DestsPuzzleData.strFileName,l_DestsPuzzleData.fUV,l_DestsPuzzleData.OffsetPos,l_DestsPuzzleData.Size,l_DestsPuzzleData.OriginalSize,l_DestsPuzzleData.ShowPosInPI);
				if( e_bGenerateAllUnit )
				{
					cPuzzleImageUnit*l_p = new cPuzzleImageUnit(m_ppPuzzleData[i],this->m_uiImageIndex,this,e_bFetchPixels);
					l_p->SetName(m_ppPuzzleData[i]->strFileName);
					this->AddObject(l_p);
				}
			}
		}
	}

	cPuzzleImage::~cPuzzleImage()
	{
		if( !this->m_bTextureFromResource&&m_ppPuzzleData )
		{
		    assert( this->m_bFromResource == false &&"both ot them shoudl not cloned object " );
		    SAFE_DELETE(m_pImageIndexOfAnimation);
		    SAFE_DELETE(m_pfAllChildrenTriangleStripUV);
		    SAFE_DELETE(m_pfAllChildrenTwoTriangleUV);
			for( int i=0;i<this->m_iNumImage;++i )
				SAFE_DELETE(m_ppPuzzleData[i]);
			SAFE_DELETE_ARRAY(m_ppPuzzleData);
		}
		//DELETE_VECTOR(m_PuzzleImageChildrenList,cPuzzleImage*);
	}

	void	cPuzzleImage::GenerateAllPuzzleImageUnit()
	{
		if( m_iNumImage == Count() )
			return;
		std::vector<cPuzzleImageUnit*>	AllPuzzleImageUnit;
		for( int i=0;i<this->m_iNumImage;++i )
		{
			cPuzzleImageUnit*l_p = new cPuzzleImageUnit(m_ppPuzzleData[i],this->m_uiImageIndex,this,this->GetPixels()?true:false);
			l_p->SetName(m_ppPuzzleData[i]->strFileName);
			bool	l_b = this->AddObject(l_p);
			if( !l_b )
			{
				SAFE_DELETE(l_p);
			}
			else
				AllPuzzleImageUnit.push_back(l_p);
		}
		cPuzzleImageUnit*l_pPrior = 0;
		for( int i=0;i<this->m_iNumImage-1;++i )
		{
			AllPuzzleImageUnit[i]->SetPrior(l_pPrior);
			AllPuzzleImageUnit[i]->SetNext(AllPuzzleImageUnit[i+1]);
			l_pPrior = AllPuzzleImageUnit[i];
		}
		AllPuzzleImageUnit[m_iNumImage-1]->SetPrior(l_pPrior);
		AllPuzzleImageUnit[m_iNumImage-1]->SetNext(0);
	}


	void	cPuzzleImage::Render(int e_iIndex)
	{
		if( !m_bVisible )
			return;
		(*this)[e_iIndex]->Render();
	}

	float*   cPuzzleImage::GetAllChildrenTriangleStripUV()
	{
	    if(!m_pfAllChildrenTriangleStripUV)
	    {
            assert(m_iNumImage);
            m_pfAllChildrenTriangleStripUV = new float[TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT*m_iNumImage];
            for(int i=0;i<this->m_iNumImage;++i)
                memcpy(&m_pfAllChildrenTriangleStripUV[TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT*i],
                this->GetObject(i)->UVToTriangleStrip(),sizeof(float)*TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT);	
        }
        return m_pfAllChildrenTriangleStripUV;
	}
	
	float*   cPuzzleImage::GetAllChildrenTwoTriangleUV()
	{
	    if(!m_pfAllChildrenTwoTriangleUV)
	    {
            assert(m_iNumImage);
            m_pfAllChildrenTwoTriangleUV = new float[TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT*m_iNumImage];
            for(int i=0;i<this->m_iNumImage;++i)
                memcpy(&m_pfAllChildrenTwoTriangleUV[TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT*i],
                this->GetObject(i)->UVToTwoTriangle(),sizeof(float)*TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT);	
        }
        return m_pfAllChildrenTwoTriangleUV;
	}

	cNumerialImage*	cPuzzleImage::GetNumerialImageByName(WCHAR*e_str0ImaneName,WCHAR*e_str9ImaneName)
	{
		cPuzzleImageUnit*l_pPuzzleImageUnit0 = GetObject(e_str0ImaneName);
		cPuzzleImageUnit*l_pPuzzleImageUnit9 = GetObject(e_str9ImaneName);
		if( l_pPuzzleImageUnit0&&l_pPuzzleImageUnit9 )
		{
			cNumerialImage*l_pNumerialImage = new cNumerialImage(l_pPuzzleImageUnit0,l_pPuzzleImageUnit9);
			return l_pNumerialImage;
		}
		return 0;	
	}

	cNumerialImage*	cPuzzleImage::GetNumerialImageByName(WCHAR*e_strNumerImageName)
	{
		cPuzzleImageUnit*l_pPuzzleImageUnit = GetObject(e_strNumerImageName);
		if( l_pPuzzleImageUnit )
		{
			cNumerialImage*l_pNumerialImage = new cNumerialImage(l_pPuzzleImageUnit,l_pPuzzleImageUnit->GetUV());
			return l_pNumerialImage;
		}
		return 0;
	}


	cSAList*	cPuzzleImage::GetSAByImageData()
	{
		if( this->m_pImageIndexOfAnimation )
		{
			cSAList*l_pSAList = new cSAList();
			l_pSAList->SetName(this->GetName());
			int	l_iCount = m_pImageIndexOfAnimation->Count();
			for(  int i=0;i<l_iCount;++i )
			{
				cImageIndexOfAnimation*l_pImageIndexOfAnimation = m_pImageIndexOfAnimation->GetObject(i);
				cStaticAnimation*l_pStaticAnimation = new cStaticAnimation();
				l_pStaticAnimation->SetName(l_pImageIndexOfAnimation->GetName());
				l_pStaticAnimation->SetPuzzleData(this->m_ppPuzzleData);
				l_pStaticAnimation->SetPuzzleImage(this);
				sImageShowInfo	l_sImageShowInfo;
				if( l_pImageIndexOfAnimation->m_ImageAnimationDataList.size() )
				{
					size_t	l_iImageIndexListSize = l_pImageIndexOfAnimation->m_ImageAnimationDataList.size();
					for( size_t j=0;j<l_iImageIndexListSize;++j )
					{
						l_sImageShowInfo.iIndex = l_pImageIndexOfAnimation->m_ImageAnimationDataList[j].iIndex;
						//l_sImageShowInfo.fShowSec = l_pImageIndexOfAnimation->m_ImageAnimationDataList[j].fTimeGap;
						l_sImageShowInfo.fShowSec = 0.1f;
						l_sImageShowInfo.Size = this->m_ppPuzzleData[l_sImageShowInfo.iIndex]->Size;
						l_pStaticAnimation->AddImageShowInfo(l_sImageShowInfo);
					}
				}
				l_pSAList->AddObject(l_pStaticAnimation);
			}
			return l_pSAList;
		}
		return 0;
	}
	
	char*       cPuzzleImage::GetFileName(const WCHAR*e_strObjectName)
	{
	    sprintf(g_sstrForStringToStructure,"%s.pi",UT::WcharToChar(e_strObjectName));
	    return g_sstrForStringToStructure;
	}

	char*       cPuzzleImage::GetFileName(const char*e_strObjectName)
	{
	    sprintf(g_sstrForStringToStructure,"%s.pi",e_strObjectName);
	    return g_sstrForStringToStructure;
	}
}