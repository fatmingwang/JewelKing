#ifndef glLoadImage_H
#define glLoadImage_H

namespace FATMING_CORE
{
	extern bool	g_bSupportNonPowerOfTwoTexture;
	//glCopyTexSubImage2D
	void	SaveCurrentBufferToImage(char*e_strFileName,int e_iWidth,int e_iHeight);
	void	SaveBufferToImage(char*e_strFileName,int e_iWidth,int e_iHeight,void*e_pPixel);
	//=======================
	//base image that contain UV and width and height data
	//also contain the pixel data if u want to fetch it.
	//
	//=======================
		class	cSimpleGLTexture:virtual public NamedTypedObject
		{
		protected:
			//texture original size.won't be effected by power of two
			int	m_iWidth;
			int	m_iHeight;
			//UV  left,top,right,button,it could possible not be 0,0,1,1 if image is not power of 2.
			float	m_fUV[4];														//left top right buttom.
			//pixels format,GL_RGB,GL_RGBA.
			GLint	m_iPixelFormat;
			//pixel data.
			GET_SET_DEC(unsigned char*,m_pPixels,GetPixels,SetPixels);
			//texture ID.
			GLuint	m_uiImageIndex;
			//is the texture get from resource,if it's,while destruction active Destroy will be skiped.
			GET_SET_DEC(bool,m_bTextureFromResource,IsTextureFromResource,SetTextureFromResource);
		public:
		    //last image index has been used
            static  GLuint  m_suiLastUsingImageIndex;
			DEFINE_TYPE_INFO();
			//if e_bCopyPixels is true,u have to ensure the e_pPixels has delete or keep in somewhere u want keep!!
			//!!ensure u know how to use it!
			cSimpleGLTexture(unsigned char*e_pPixels,int e_iWidth,int e_iHeight,WCHAR*e_strName,bool e_bCopyPixels,bool e_bAllocateMemoryForPixelIfFetch,GLenum e_eImageType = GL_RGBA);
			cSimpleGLTexture(const char*e_strImageFileName,bool e_bFetchPixelData = false);
			cSimpleGLTexture(cSimpleGLTexture*e_pSimpleGLTexture)
			{
				SetName(e_pSimpleGLTexture->GetName());
				this->m_pWoner = e_pSimpleGLTexture->m_pWoner;
				m_bTextureFromResource = true;
				m_iPixelFormat = e_pSimpleGLTexture->GetPixelFormat();
				m_iWidth = e_pSimpleGLTexture->GetImageWidth();
				m_iHeight = e_pSimpleGLTexture->GetImageHeight();
				m_uiImageIndex = e_pSimpleGLTexture->GetImageIndex();
				memcpy(m_fUV,e_pSimpleGLTexture->GetUV(),sizeof(float)*4);
				m_pPixels = e_pSimpleGLTexture->GetPixels();				
			}
			CLONE_MYSELF(cSimpleGLTexture);
			~cSimpleGLTexture();
			void	Destroy();

			//it e_bFetchPixelData true m_pPixels will fill of pixel data.
			bool	LoadImage(const char*e_strImageFileName,bool e_bFetchPixelData = false);
			void	ApplyImage();
			GLuint	GetImageIndex(){ return m_uiImageIndex;}
			//should't call this!!,instead u should call SetImageShowWidth and SetImageShowHeight.
			void	SetImageSize(int e_iWidth,int e_iHeight){ m_iWidth = e_iWidth; m_iHeight = e_iHeight; }
			int		GetImageWidth(){ return m_iWidth; }
			int		GetImageHeight(){ return m_iHeight; }
			GLint	GetPixelFormat(){ return m_iPixelFormat; }
			//make sure the UV isze is float*4,do not change value if u r not so understand.
			void	SetUV(float*e_pfUV){ memcpy(m_fUV,e_pfUV,sizeof(float)*4); }
			float*	GetUV(){ return m_fUV; }
			inline	virtual	float* UVToTriangleStrip()
			{
				static	float l_sUV[TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT];
				//0
				l_sUV[0] = m_fUV[0];l_sUV[1] = m_fUV[1];
				//1
				l_sUV[2] = m_fUV[2];l_sUV[3] = m_fUV[1];
				//2
				l_sUV[4] = m_fUV[0];l_sUV[5] = m_fUV[3];
				//3
				l_sUV[6] = m_fUV[2];l_sUV[7] = m_fUV[3];
				return l_sUV;
			}
			inline	virtual	float* UVToTwoTriangle()
			{
				static	float l_sUV[TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT];
				//2,3   5
				//0     1,4
				//0
				l_sUV[0] = m_fUV[0];l_sUV[1] = m_fUV[3];
				//1
				l_sUV[2] = m_fUV[2];l_sUV[3] = m_fUV[3];
				//2
				l_sUV[4] = m_fUV[0];l_sUV[5] = m_fUV[1];
				//3
				l_sUV[6] = m_fUV[0];l_sUV[7] = m_fUV[1];
				//4
				l_sUV[8] = m_fUV[2];l_sUV[9] = m_fUV[3];
				//5
				l_sUV[10] = m_fUV[2];l_sUV[11] = m_fUV[1];
				return l_sUV;
			}
			inline	bool	IsSameTextureID(cSimpleGLTexture*e_pSimpleGLTexture)
			{
				return e_pSimpleGLTexture->m_uiImageIndex == m_uiImageIndex?true:false;
			}
			virtual WCHAR*DescriptionInfo()
			{
			    static  WCHAR   l_sStrDescription[MAX_PATH];
			    swprintf(l_sStrDescription,MAX_PATH,L"Size:%d,%d\nUV:%.2f,%2.f,%.2f,%.2f",m_iWidth,m_iHeight,m_fUV[0],m_fUV[1],m_fUV[2],m_fUV[3]);
			    return l_sStrDescription;
			}
		};

		//sample to get pixel by texture ID
		////Use our texture
		//glBindTexture( GL_TEXTURE_2D, m_uiImageIndex );
		//   
		////Get the pixel data
		//glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels );
		//m_iPixelFormat = GL_RGBA;

	//===================
	//while the image is not base on opengl we still could using this structure to do some data update
	//so spilt it with texture pixel data
	//===================
		class	cTexData
		{
		protected:
			GETP_SET_DEC(Vector4,m_vColor,GetColor,SetColor);						//color		
			GETP_SET_DEC(Vector3,m_vPos,GetPos,SetPos);								//render position
			GETP_SET_DEC(float,m_fAngle,GetAngle,SetAngle);							//angle
			GETP_SET_DEC(POINT,m_OffsetPos,GetOffsetPos,SetOffsetPos);				//offset position,image offset,some image size is not as big as same sequence series,to save image size we have to keep this on.
			GETP_SET_DEC(int,m_iShowWidth,GetImageShowWidth,SetImageShowWidth);		//final width show on the screen
			GETP_SET_DEC(int,m_iShowHeight,GetImageShowHeight,SetImageShowHeight);	//final height show on the screen
			GETP_SET_DEC(bool,m_bVisible,IsVisible,SetVisible);
		public:
			cTexData(int e_iWidth,int e_iHeight)
			{
				m_bVisible = true;
				m_iShowWidth = e_iWidth;
				m_iShowHeight = e_iHeight;
				m_vColor = Vector4::One;
				m_vPos = Vector3::Zero;
				m_OffsetPos.x = m_OffsetPos.y = 0;
				m_fAngle = 0.f;
			}
			cTexData(cTexData*e_pTexData)
			{
				//from this
				SetTexBehaviorData(e_pTexData);
			}
			~cTexData(){}
			void	SetPos(POINT e_Pos){ m_vPos.x = (float)e_Pos.x; m_vPos.y = (float)e_Pos.y; }
			void	SetPos(Vector2 e_vPos){ m_vPos.x = e_vPos.x; m_vPos.y = e_vPos.y; }
			void	SetCenterPos(POINT e_Pos)
			{
				m_vPos.x = e_Pos.x-m_iShowWidth/2.f;
				m_vPos.y = e_Pos.y-m_iShowHeight/2.f;			
			}
			void	SetPosByImageCenter(Vector3 e_vPos)
			{
				m_vPos.x = e_vPos.x-m_iShowWidth/2.f;
				m_vPos.y = e_vPos.y-m_iShowHeight/2.f;
			}
			void	SetDepth(float e_fDepth){ m_vPos.z = e_fDepth; }
			void	SetTexBehaviorData(cTexData*e_pTexData)
			{
				m_bVisible = true;
				m_vColor = *e_pTexData->GetColor();
				m_vPos = *e_pTexData->GetPos();
				m_OffsetPos = *e_pTexData->GetOffsetPos();
				m_fAngle = *e_pTexData->GetAngle();
				m_iShowWidth = *e_pTexData->GetImageShowWidth();
				m_iShowHeight = *e_pTexData->GetImageShowHeight();
			}
		};
		//=====================
		//combine texture pixel and 2D texture behavior data
		//=====================
		class	cSimpleGLTextureWithcTexData:public cSimpleGLTexture,public cTexData
		{
			bool	m_bMirror;
		public:
			DEFINE_TYPE_INFO();
			cSimpleGLTextureWithcTexData(const char*e_strImageFileName,bool e_bFetchPixelData)
				:cSimpleGLTexture(e_strImageFileName,e_bFetchPixelData),
				cTexData(m_iWidth,m_iHeight)
			{
				m_bMirror = false;
			}
			cSimpleGLTextureWithcTexData(cSimpleGLTextureWithcTexData*e_SimpleGLTextureWithcTexData)
			:cSimpleGLTexture(e_SimpleGLTextureWithcTexData),cTexData(e_SimpleGLTextureWithcTexData)
			{
				m_bMirror = e_SimpleGLTextureWithcTexData->m_bMirror;
			}
			~cSimpleGLTextureWithcTexData(){}
			//if the image has been scaled or set position it might get error result!!
			virtual bool    CollideTexture(int e_iX,int e_iY,bool e_bTestAlphaChannel = true,Vector4*e_pvCollidedColor = 0);
			//collide with position and rotation.
			virtual bool    CollideTextureWithTransform(int e_iX,int e_iY,bool e_bTestAlphaChannel = true);
			void	RenderRect();
			bool	IsMirror(){ return m_bMirror; }
			void	SetMirror(bool e_bMirror);
		};
}
#endif