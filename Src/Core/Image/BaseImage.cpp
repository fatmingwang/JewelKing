#include "../stdafx.h"
#include "BaseImage.h"
#include "SimplePrimitive.h"
#include "BinaryToTexture.h"
#include "../GLSL/Shader.h"
namespace FATMING_CORE
{
	const wchar_t*     cBaseImage::TypeID( L"cBaseImage" );
	const wchar_t*     cUIImage::TypeID( L"cUIImage" );
	//====================
	//
	//====================
	void	cBaseImage::Render(int e_iX,int e_iY)
	{
		if( !m_bVisible )
			return;
		this->ApplyImage();
		GLRender::DrawQuadWithTextureAndColorAndCoordinate((float)e_iX,(float)e_iY,this->m_vPos.z,this->m_iShowWidth,this->m_iShowHeight,this->m_vColor,m_fUV,this->m_fAngle);
	}

	void	cBaseImage::Render(Vector3 e_vPos)
	{
		if( !m_bVisible )
			return;
		this->ApplyImage();
		GLRender::DrawQuadWithTextureAndColorAndCoordinate(e_vPos.x,e_vPos.y,e_vPos.z,this->m_iShowWidth,this->m_iShowHeight,this->m_vColor,m_fUV,this->m_fAngle);
	}
	//====================
	//
	//====================	
    void	cBaseImage::RenderWithOffset()
    {
		if( !m_bVisible )
			return;
		ApplyImage();
		DrawQuadWithTextureAndColorAndCoordinate(this->m_vPos.x+this->m_OffsetPos.x,this->m_vPos.y+this->m_OffsetPos.y,this->m_vPos.z,this->m_iShowWidth,this->m_iShowHeight,m_vColor,this->m_fUV,this->m_fAngle);
    }
	//====================
	//
	//====================
	void	cBaseImage::Render()
	{
		if( !m_bVisible )
			return;
		ApplyImage();
		DrawQuadWithTextureAndColorAndCoordinate(this->m_vPos.x,this->m_vPos.y,this->m_vPos.z,this->m_iShowWidth,this->m_iShowHeight,m_vColor,this->m_fUV,this->m_fAngle);
	}
	
    void	cBaseImage::Render(cMatrix44 e_Mat)
    {
		if( !m_bVisible )
			return;    
        ApplyImage();
		float l_fWidth = m_iShowWidth/2.f;
		float l_fHeight = m_iShowHeight/2.f;
		float	l_Vertices[] = { -l_fWidth,-l_fHeight,
								 l_fWidth, -l_fHeight,
								 -l_fWidth, l_fHeight,
								 l_fWidth,l_fHeight};
/*        float l_fWidth = m_iShowWidth;
		float l_fHeight = m_iShowHeight;
		float	l_Vertices[] = { 0,0,
								 l_fWidth,0,
								 0, l_fHeight,
								 l_fWidth,l_fHeight};	*/							 
		float	l_fTexPointer[] = {  m_fUV[0],m_fUV[1],
								   m_fUV[2],m_fUV[1],
								   m_fUV[0],m_fUV[3],
								   m_fUV[2],m_fUV[3],};
        DrawQuadWithMatrix(l_Vertices,l_fTexPointer,this->m_vColor,e_Mat);
    }

	void	cBaseImage::RenderBySpecificPos(Vector3 e_vStartPos,Vector3 e_vEndPos)
	{
		Vector3	l_vUpDirection = e_vStartPos-e_vEndPos;
		if( l_vUpDirection.Length()<=0.1f )
			return;
		Vector3	l_vRightDirection = l_vUpDirection.Normalize()^Vector3(0,0,-1);
		int	l_iWidth =	this->m_iShowHeight;
		Vector3	l_v2 = l_vRightDirection*(float)l_iWidth+e_vStartPos;
		Vector3	l_v3 = l_vRightDirection*(float)-l_iWidth+e_vStartPos;
		Vector3	l_v1 = l_vRightDirection*(float)l_iWidth+e_vEndPos;;
		Vector3	l_v0 = l_vRightDirection*(float)-l_iWidth+e_vEndPos;
		ApplyImage();
		float*l_pfTex = this->GetUV();
		float	l_pfTexPointer[8] = { l_pfTex[2],l_pfTex[1],
									l_pfTex[2],l_pfTex[3],
									l_pfTex[0],l_pfTex[1],
									l_pfTex[0],l_pfTex[3] };

		float	l_pVertices[8] = { l_v3.x,l_v3.y,l_v2.x,l_v2.y,l_v0.x,l_v0.y,l_v1.x,l_v1.y};
		myGlUVPointer(2,l_pfTexPointer);
		myGlVertexPointer(2,l_pfTexPointer);
		ASSIGN_2D_COLOR(m_vColor);
		MY_GLDRAW_ARRAYS(GL_TRIANGLE_STRIP, 0, 4);
	}

	void	cBaseImage::RenderWithOffset(Vector3 e_vPos)
	{
		if( !m_bVisible )
			return;
		Render(Vector3(e_vPos.x+this->m_OffsetPos.x,e_vPos.y+this->m_OffsetPos.y,0.f));
	}
	void	cBaseImage::RenderWithOffset(int e_iX,int e_iY)
	{
		if( !m_bVisible )
			return;
		Render(e_iX+this->m_OffsetPos.x,e_iY+this->m_OffsetPos.y);
	}
	//====================
	//
	//====================
	cUIImage::cUIImage(char*e_strImageName):cBaseImage(e_strImageName,true)
	{
		m_fAngle = 0.f;m_vPos.x = 0;m_vPos.y = 0;
		m_OriginalImageSize.x = -1;
		m_OriginalImageSize.y = -1;
		m_RightDownStripOffPos.x = m_iWidth-1;
		m_RightDownStripOffPos.y = m_iHeight-1;
		if( !g_bSupportNonPowerOfTwoTexture )
		{
			m_iCollisionImageWidth = UT::power_of_two(m_iWidth);
			m_iCollisionImageHeight = UT::power_of_two(m_iHeight);
		}
		else
		{
			m_iCollisionImageWidth = m_iWidth;
			m_iCollisionImageHeight = m_iHeight;	
		}
		m_pEditorAttachParent = 0;
		m_vEditorAttachParentRelativePos = Vector3::Zero;
	}

	cUIImage::cUIImage(WCHAR*e_strName,char*e_pPixelsData,int e_iWidth,int e_iHeight,GLint e_iDataFormat):cBaseImage(0,false)
	{
		this->SetName(e_strName);
		m_OriginalImageSize.x = -1;
		m_OriginalImageSize.y = -1;
		m_fUV[0] = 0.f; m_fUV[1] = 0.f; m_fUV[2] = 1.f; m_fUV[3] = 1.f;
		this->m_bTextureFromResource = false;
		m_iPixelFormat = e_iDataFormat;
		m_fAngle = 0.f;m_vPos.x = 0;m_vPos.y = 0; 
		this->m_iHeight = e_iHeight;
		this->m_iWidth = e_iWidth;
		m_RightDownStripOffPos.x = e_iWidth-1;
		m_RightDownStripOffPos.y = e_iHeight-1;
		m_iCollisionImageWidth = e_iWidth;
		m_iCollisionImageHeight = e_iHeight;
		this->m_iShowHeight = m_iHeight;
		this->m_iShowWidth = m_iWidth;
		m_pPixels = (unsigned char*)e_pPixelsData;
		int	l_iWidthPO2 = power_of_two(m_iWidth);
		int	l_iHeightPO2 = power_of_two(m_iHeight);
		GLint texSize; glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
		int	l_iChannel = m_iPixelFormat == GL_RGB?3:4;
		//make sure power of 2,because not every fukcing graphic card support it
		//but if u exactly sure it do support power of 2 u could mark this.
		if( !g_bSupportNonPowerOfTwoTexture&&(l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight) )
		{
			assert((l_iWidthPO2<=texSize||l_iHeightPO2<=texSize)&&"texture size is too big then card support");
	#ifdef WIN32
	#ifdef _DEBUG
			OutputDebugString(e_strName);
			OutputDebugString(L"  image has not correct respond width and height,because none power of 2\n");
			if( m_iPixelFormat == GL_RGB )
				OutputDebugString(L"UV is changed,because image size is not to become power of 2");
	#endif
	#endif
			bool e_balpah = false;
			if( m_iPixelFormat==GL_RGBA || m_iPixelFormat == GL_BGRA )
				e_balpah = true;
			char*l_pNewPixelData = TextureToPowOfTwo((char*)e_pPixelsData,m_iWidth,m_iHeight,e_balpah);
			//assign new UV
			m_fUV[2] = (float)m_iWidth/l_iWidthPO2;
			m_fUV[3] =  (float)m_iHeight/l_iHeightPO2;
			delete e_pPixelsData;
			m_pPixels = (unsigned char*)l_pNewPixelData;
			this->m_uiImageIndex = CreateTextureFromBinary(l_iChannel, m_pPixels,l_iWidthPO2,l_iHeightPO2,e_iDataFormat);
			m_iWidth = l_iWidthPO2;
			m_iHeight = l_iHeightPO2;
		}
		else
		{
			this->m_uiImageIndex = CreateTextureFromBinary( l_iChannel,m_pPixels,m_iWidth,m_iHeight,e_iDataFormat);

		}
		assert(m_uiImageIndex!=0);
		m_pEditorAttachParent = 0;
		m_vEditorAttachParentRelativePos = Vector3::Zero;
	}

	cUIImage::cUIImage(cUIImage*e_pUIImage):cBaseImage(e_pUIImage)
	{
		m_fAngle = 0.f;m_vPos.x = 0;m_vPos.y = 0;
		m_OriginalImageSize.x = -1;
		m_OriginalImageSize.y = -1;
		m_RightDownStripOffPos.x = m_iWidth-1;
		m_RightDownStripOffPos.y = m_iHeight-1;
		if( !g_bSupportNonPowerOfTwoTexture )
		{
			m_iCollisionImageWidth = UT::power_of_two(m_iWidth);
			m_iCollisionImageHeight = UT::power_of_two(m_iHeight);
		}
		else
		{
			m_iCollisionImageWidth = m_iWidth;
			m_iCollisionImageHeight = m_iHeight;
		}
		m_pEditorAttachParent = e_pUIImage;
		m_vEditorAttachParentRelativePos = Vector3::Zero;
	}

	cUIImage::~cUIImage()
	{
	}

	void	cUIImage::RenderWithOffset()
	{
		if( !m_bVisible )
			return;
		cBaseImage::Render((int)this->m_vPos.x-this->m_OffsetPos.x,(int)m_vPos.y-this->m_OffsetPos.y);
	}


	bool	cUIImage::Collide(int e_iLocalX,int e_iLocalY)
	{
		if(m_iPixelFormat == GL_RGB)//there is no alpha so we can't find any info
		{
			if( e_iLocalX<this->m_iCollisionImageWidth&&e_iLocalX>=0 )
				if( e_iLocalY<this->m_iCollisionImageHeight&&e_iLocalY>=0 )
					return true;
			return false;
		}
		cMatrix44	l_matrix;
		Vector3 l_vPos = Vector3(this->m_iHeight/2.f-e_iLocalY,e_iLocalX-this->m_iWidth/2.f,0);
		l_matrix = l_matrix.Identity;
		l_matrix = l_matrix.ZAxisRotationMatrix(MyMath::DegToRad(360-this->m_fAngle));
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
		e_iLocalY = (int)(-l_vFinalPos.x)+this->m_iHeight/2;
		e_iLocalX = (int)l_vFinalPos.y+this->m_iWidth/2;
		if( e_iLocalX<0||e_iLocalY<0 )
			return false;
		if( e_iLocalX>m_iWidth||e_iLocalY>m_iHeight )
			return false;
		int	l_iIndex = e_iLocalY*this->m_iCollisionImageWidth+e_iLocalX;
		if( l_iIndex<0 )
			return false;
        //+4!!!!!!!!!?FUCK...
		unsigned char l_Alpha = this->m_pPixels[l_iIndex*4];//argb
		//UINT32	l_Alpha2 = *(UINT32*)&this->m_pPixels[l_iIndex*4];//argb
	//	unsigned char	l_cR = this->m_pPixels[l_iIndex*4+1];
	//	unsigned char	l_cG = this->m_pPixels[l_iIndex*4+2];
	//	unsigned char	l_cB = this->m_pPixels[l_iIndex*4+4];

		if( l_Alpha != 0 )
		{//there is alpha with color is default empty color,so it's not collide
			//if( l_cR==255&&
			//	l_cG==255&&
			//	l_cB==255)
			//	return false;
			//this->Destroy();
			////memset(m_pPixels,130,sizeof(m_iWidth*m_iHeight)*4);
			//glGenTextures(1,&m_uiImageIndex);
			//glBindTexture( GL_TEXTURE_2D, m_uiImageIndex );
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//m_pPixels[l_iIndex*4] = 125;
			//m_pPixels[l_iIndex*4+1] = 125;
			//m_pPixels[l_iIndex*4+2] = 125;
			//m_pPixels[l_iIndex*4+3] = 125;
			//glTexImage2D(GL_TEXTURE_2D,
			//  0,
			//  GL_RGBA,
			//  m_iWidth,
			//  m_iHeight,
			//  0,
			//  GL_RGBA ,
			//  GL_UNSIGNED_BYTE,
			//  m_pPixels);
			return true;
		}
		return false;
	}

	bool	cUIImage::CollideWithOutAlphaTest(int e_iLocalX,int e_iLocalY)
	{
		if(m_iPixelFormat == GL_RGB)//there is no alpha so we can't find any info
		{
			if( e_iLocalX<this->m_iCollisionImageWidth&&e_iLocalX>=0 )
				if( e_iLocalY<this->m_iCollisionImageHeight&&e_iLocalY>=0 )
					return true;
			return false;
		}
		cMatrix44	l_matrix;
		Vector3 l_vPos = Vector3(this->m_iHeight/2.f-e_iLocalY,e_iLocalX-this->m_iWidth/2.f,0);
		l_matrix = l_matrix.Identity;
		l_matrix = l_matrix.ZAxisRotationMatrix(MyMath::DegToRad(360-this->m_fAngle));
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
		e_iLocalY = (int)(-l_vFinalPos.x)+this->m_iHeight/2;
		e_iLocalX = (int)l_vFinalPos.y+this->m_iWidth/2;
		if( e_iLocalX<0||e_iLocalY<0 )
			return false;
		if( e_iLocalX>m_iWidth||e_iLocalY>m_iHeight )
			return false;
			
		if( e_iLocalX>(this->m_RightDownStripOffPos.x-this->m_OffsetPos.x)||e_iLocalY>this->m_RightDownStripOffPos.y-this->m_OffsetPos.y )
			return false;			
		int	l_iIndex = e_iLocalY*this->m_iCollisionImageWidth+e_iLocalX;
		if( l_iIndex<0 )
			return false;
		return true;
	}
}