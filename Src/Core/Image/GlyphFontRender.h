#ifndef Glyph_Font_Render
#define Glyph_Font_Render
#include "../Glyph/GlyphReader.h"
#include "BaseImage.h"
namespace FATMING_CORE
{
	//==========================
	//change color and angle by m_pFontImage->SetColor SetAngle;
	//==========================
	class cGlyphFontRender:public NamedTypedObject
	{
		GET_SET_DEC(GlyphReader*,m_pGlyphReader,GetGlyphReader,SetGlyphReader);
		GET_SET_DEC(float,m_fDepth,GetDepth,SetDepth);
		cBaseImage* m_pFontImage;
		GET_SET_DEC(float,m_fScale,GetScale,SetScale);
		Vector2	*m_pvVertexBuffer;
		Vector2	*m_pvTextureUVBuffer;
		Vector4	*m_pvColorBuffer;
		//for how many words
		int		m_iBufferLength;
	public:
		DEFINE_TYPE_INFO();
		cGlyphFontRender(const char* e_strFontName,int e_iVertexBufferSize = 300);
		cGlyphFontRender(int e_iVertexBufferSize = 300);
		cGlyphFontRender(cGlyphFontRender*e_pGlyphFontRender);
		CLONE_MYSELF(cGlyphFontRender);
		~cGlyphFontRender();
		cBaseImage* GetFontImage(){ return m_pFontImage; }
		//it should put all data at a block and call at once,not like now call by separate
		inline void	RenderFont(int e_iX,int e_iY,const WCHAR*e_pString){RenderFont((float)e_iX,(float)e_iY,e_pString);}
		void		RenderFont(float e_fX,float e_fY,const WCHAR*e_pString);
		void		RenderFont(float e_fX,float e_fY,const WCHAR*e_pString,Vector4*e_pDrawRect);
		void		SetFontColor(Vector4 e_vColor);
		Vector2		GetRenderSize(const WCHAR*e_strText);
	};

	//class	cTextRender:public NamedTypedObject
	//{
	//	std::wstring	m_strText;
	//	Vector3			m_vPos;
	//	//
	//	Vector4*		m_pvTextBGColor;
	//public:
	//	cTextRender();
	//};
//end namespace FATMING_CORE
}
#endif