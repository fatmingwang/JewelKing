#include "../stdafx.h"
#include "GlyphFontRender.h"
#include "SimplePrimitive.h"
#include "../GLSL/Shader.h"
namespace FATMING_CORE
{
	const wchar_t*         cGlyphFontRender::TypeID( L"cGlyphFontRender" );

	cGlyphFontRender::cGlyphFontRender(int e_iVertexBufferSize)
	{
		m_pFontImage = 0;
		m_pGlyphReader = 0;
		m_fDepth = 0.f;
		m_fScale = 1.f;
		m_pvVertexBuffer = new Vector2[4*e_iVertexBufferSize];//one quad four vertex,a vertex 2 data(x,y)
		m_pvTextureUVBuffer = new Vector2[4*e_iVertexBufferSize];
		m_pvColorBuffer = new Vector4[4*e_iVertexBufferSize];
		m_iBufferLength = e_iVertexBufferSize;
		for( int i=0;i<e_iVertexBufferSize*4;++i )
			m_pvColorBuffer[i] = Vector4::One;
	}

	cGlyphFontRender::cGlyphFontRender(const char* e_strFontName,int e_iVertexBufferSize)
	{
		m_pvVertexBuffer = new Vector2[4*e_iVertexBufferSize];//one quad four vertex,a vertex 2 data(x,y)
		m_pvTextureUVBuffer = new Vector2[4*e_iVertexBufferSize];
		m_pvColorBuffer = new Vector4[4*e_iVertexBufferSize];
		m_iBufferLength = e_iVertexBufferSize;
		for( int i=0;i<e_iVertexBufferSize*4;++i )
			m_pvColorBuffer[i] = Vector4::One;
		m_fDepth = 0.f;
		m_fScale = 1.f;
		char	l_strForStripExtensionName[TEMP_SIZE];
		sprintf(l_strForStripExtensionName,"%s\0",UT::GetFileNameWithoutFullPath(e_strFontName));
		char	l_strForDirectory[TEMP_SIZE];
		if( UT::GetDirectoryWithoutFileName(e_strFontName) )
			sprintf(l_strForDirectory,"%s\0",UT::GetDirectoryWithoutFileName(e_strFontName));
		else
			l_strForDirectory[0] = '\0';
		SetName(UT::CharToWchar(l_strForStripExtensionName));
		char	l_str[TEMP_SIZE];
		sprintf(l_str,"%s%s.png",l_strForDirectory,l_strForStripExtensionName);
		m_pFontImage = new cBaseImage(l_str);
		if(m_pFontImage->GetImageIndex() == 0 )
		{
			SAFE_DELETE(m_pFontImage);
			m_pGlyphReader = 0;
			return;
		}
		m_pGlyphReader = new GlyphReader();
		sprintf(l_str,"%s%s.FontInfo",l_strForDirectory,l_strForStripExtensionName);
		if(!m_pGlyphReader->LoadFontDataFile(UT::CharToWchar(l_str)))
			UT::ErrorMsg("read font info failed",l_str);

	}
	cGlyphFontRender::cGlyphFontRender(cGlyphFontRender*e_pGlyphFontRender)
	{
		assert(0&&"cGlyphFontRender lazy to implement clone");
	}

	cGlyphFontRender::~cGlyphFontRender()
	{
		SAFE_DELETE(m_pGlyphReader);
		SAFE_DELETE(m_pFontImage);
		SAFE_DELETE(m_pvVertexBuffer);
		SAFE_DELETE(m_pvTextureUVBuffer);
		SAFE_DELETE(m_pvColorBuffer);
	}

	void	cGlyphFontRender::RenderFont(float e_fX,float e_fY,const WCHAR*e_pString)
	{
		RenderFont(e_fX,e_fY,e_pString,0);
	}

	void	cGlyphFontRender::RenderFont(float e_fX,float e_fY,const WCHAR*e_pString,Vector4*e_pDrawRect)
	{
#ifdef _DEBUG
		std::wstring	l_str = e_pString;
#endif
		int	l_iLen = (int)wcslen(e_pString);
		if( l_iLen == 0 )
			return;
		float	l_fXOffset = 0.f;
		float	l_fYOffset = 0.f;
		float	l_fFonyHeight = 0.f;
		for( int i=0;i<l_iLen;++i )
		{
			if(!m_pGlyphReader->IsLegalCharacter(e_pString[i]))
				continue;
			l_fFonyHeight =	m_pGlyphReader->GetCharInfo(e_pString[i]).fHeight*m_fScale;
		}
		float	l_fMaxWidth = 0.f;
		float	l_fMaxHeight = l_fFonyHeight;
		int	l_iAliveIndex = 0;
		for(int i=0;i<l_iLen;++i)
		{
			float*l_pfVertexData = (float*)&m_pvVertexBuffer[l_iAliveIndex*6];
			float*l_pfTextData = (float*)&m_pvTextureUVBuffer[l_iAliveIndex*6];
			//unicode
			if( e_pString[i] == 13 && ((i+1<l_iLen)&&e_pString[i+1] == 10) )
			{
				continue;
			}
			//ansi
			if( e_pString[i] != L'\n' )
			{
				if(!m_pGlyphReader->IsLegalCharacter(e_pString[i]))
					continue;
				FILE_GLYPH_ATTR l_FILE_GLYPH_ATTR = this->m_pGlyphReader->GetCharInfo(e_pString[i]);
				float   l_fCharacterWidth = l_FILE_GLYPH_ATTR.fWidth*m_fScale;
				float   l_fCharacterHeight = l_FILE_GLYPH_ATTR.fHeight*m_fScale;
				//pos
				*l_pfVertexData = l_fXOffset;							++l_pfVertexData;
				*l_pfVertexData = l_fYOffset;							++l_pfVertexData;
				*l_pfVertexData = l_fXOffset+l_fCharacterWidth;			++l_pfVertexData;
				*l_pfVertexData = l_fYOffset;							++l_pfVertexData;
				*l_pfVertexData = l_fXOffset;							++l_pfVertexData;
				*l_pfVertexData = l_fYOffset+l_fCharacterHeight;		++l_pfVertexData;
				
				*l_pfVertexData = l_fXOffset;							++l_pfVertexData;
				*l_pfVertexData = l_fYOffset+l_fCharacterHeight;		++l_pfVertexData;
				*l_pfVertexData = l_fXOffset+l_fCharacterWidth;			++l_pfVertexData;
				*l_pfVertexData = l_fYOffset;							++l_pfVertexData;
				*l_pfVertexData = l_fXOffset+l_fCharacterWidth;			++l_pfVertexData;
				*l_pfVertexData = l_fYOffset+l_fCharacterHeight;

				//UV
				*l_pfTextData = l_FILE_GLYPH_ATTR.fLeft;				++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fTop;					++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fRight;				++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fTop;					++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fLeft;				++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fBottom;				++l_pfTextData;

				*l_pfTextData = l_FILE_GLYPH_ATTR.fLeft;				++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fBottom;				++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fRight;				++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fTop;					++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fRight;				++l_pfTextData;
				*l_pfTextData = l_FILE_GLYPH_ATTR.fBottom;
				
				l_fXOffset += l_fCharacterWidth;
				if( l_fMaxWidth<l_fXOffset )
					l_fMaxWidth = l_fXOffset;
				++l_iAliveIndex;
			}
			else
			{
				//memset(l_pfVertexData,0,sizeof(float)*8);
				//memset(l_pfTextData,0,sizeof(float)*8);
				l_fXOffset = 0;
				l_fYOffset += l_fFonyHeight;
				l_fMaxHeight  += l_fFonyHeight;
			}
		}
		if( l_iAliveIndex == 0 )
			return;
		if( e_pDrawRect )
		{
			e_pDrawRect->x = e_fX;
			e_pDrawRect->y = e_fY;
			e_pDrawRect->z = e_fX+l_fMaxWidth;
			e_pDrawRect->w = e_fY+l_fMaxHeight;
		}
		float	l_fHalfWidth = l_fMaxWidth;
		float	l_fHalfHeight = l_fMaxHeight;
		for(int i=0;i<l_iAliveIndex;++i)
		{
			float*l_pfVertexData = (float*)&m_pvVertexBuffer[i*6];
			*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
			*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
			*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;

			*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;
			*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight; ++l_pfVertexData;			
			*l_pfVertexData -= l_fHalfWidth;	++l_pfVertexData; *l_pfVertexData -= l_fHalfHeight;
		}
		cBaseShader*l_p2DShader = GetCurrentShader();
		UseShaderProgram(DEFAULT_SHADER);
		m_pFontImage->ApplyImage();
		cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(e_fX+l_fHalfWidth,e_fY+l_fHalfHeight,0.f));
		FATMING_CORE::SetupShaderWorldMatrix(l_mat);
		myGlVertexPointer(2,m_pvVertexBuffer);
		myGlUVPointer(2,m_pvTextureUVBuffer);
		myGlColorPointer(4,m_pvColorBuffer);
		MY_GLDRAW_ARRAYS(GL_TRIANGLES, 0, 6*l_iAliveIndex);
		UseShaderProgram(l_p2DShader);
	}

	void	cGlyphFontRender::SetFontColor(Vector4 e_vColor)
	{
		for( int i=0;i<m_iBufferLength*4;++i )
			m_pvColorBuffer[i] = e_vColor;
	}

	Vector2		cGlyphFontRender::GetRenderSize(const WCHAR*e_strText)
	{
		int	l_iLen = (int)wcslen(e_strText);
		if( l_iLen == 0 )
			return Vector2::Zero;
		float	l_fXOffset = 0.f;
		float	l_fYOffset = 0.f;
		float	l_fFonyHeight = 0.f;
		for( int i=0;i<l_iLen;++i )
		{
			if(!m_pGlyphReader->IsLegalCharacter(e_strText[i]))
				continue;
			l_fFonyHeight =	m_pGlyphReader->GetCharInfo(e_strText[i]).fHeight*m_fScale;
		}
		float	l_fMaxWidth = 0.f;
		float	l_fMaxHeight = l_fFonyHeight;
		for(int i=0;i<l_iLen;++i)
		{
			if( e_strText[i] != L'\n' )
			{
				if(!m_pGlyphReader->IsLegalCharacter(e_strText[i]))
					continue;
				FILE_GLYPH_ATTR l_FILE_GLYPH_ATTR = this->m_pGlyphReader->GetCharInfo(e_strText[i]);
				float   l_fCharacterWidth = l_FILE_GLYPH_ATTR.fWidth*m_fScale;
				float   l_fCharacterHeight = l_FILE_GLYPH_ATTR.fHeight*m_fScale;
				l_fXOffset += l_fCharacterWidth;
				if( l_fMaxWidth<l_fXOffset )
					l_fMaxWidth = l_fXOffset;
			}
			else
			{
				l_fXOffset = 0;
				l_fYOffset += l_fFonyHeight;
				l_fMaxHeight  += l_fFonyHeight;
			}
		}
		return Vector2(l_fMaxWidth,l_fMaxHeight);
	}
}