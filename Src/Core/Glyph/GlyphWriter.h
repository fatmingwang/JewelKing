﻿#ifdef WIN32
#pragma once
#include "GlyphReader.h"
class GlyphWriter
{
	SHORT			m_wFontHeight;
    BYTE*			m_ValidGlyphs;
    WCHAR			m_cMaxGlyph;//the biggest character will used
	//m_TranslatorTable['s'] or m_TranslatorTable[L's'] or
	//m_TranslatorTable[L'幹'] all can indicate correct font index
    WORD*			m_TranslatorTable;
	DWORD			m_dwNumGlyphs;
	DWORD			m_dwMask;//to alpha
	//use with m_TranslatorTable[x] will indicate the correct FILE_GLYPH_ATTR
	FILE_GLYPH_ATTR* m_pGlyphs;
	HRESULT BuildTranslatorTable();
	VOID	DestroyObjects();	
public:
	//this one have no use,because I did this by myself
	HRESULT ExtractValidGlyphsFromFile( const CHAR* strFileName );

	GlyphWriter(void);
	virtual ~GlyphWriter(void);
	//must set data
	void	SetFontInfo(SHORT wHeight,DWORD e_dwColor,DWORD e_dwNumGlyph,WCHAR e_dwMaxGlyph);
	HRESULT	ExtractValidGlyphsFromRange( WORD wStartGlyph, WORD wEndGlyph );
	HRESULT	ExtractValidGlyphsFromWcharPointer( WCHAR *e_pString);
	//switch pc or xobx
	HRESULT	SaveFile(char*strFileName,bool e_bPC = true);
	HRESULT	SetGlyphAttr(DWORD e_dwIndex,FILE_GLYPH_ATTR e_FILE_GLYPH_ATTR){m_pGlyphs[e_dwIndex] = e_FILE_GLYPH_ATTR;}
	HRESULT	SetGlyphAttr(DWORD dwIndex,float fLeft,float fTop,float fRight,float fBottom,float fWidth,float fHeight)
	{
		assert(m_dwNumGlyphs>dwIndex&&"over range or skip SetNumGlyph");
		m_pGlyphs[dwIndex].fBottom = fBottom;
		m_pGlyphs[dwIndex].fLeft = fLeft;
		m_pGlyphs[dwIndex].fRight = fRight;
		m_pGlyphs[dwIndex].fTop = fTop;
		m_pGlyphs[dwIndex].fWidth = fWidth;
		m_pGlyphs[dwIndex].fHeight = fHeight;
		return S_OK;
	}
};
#endif