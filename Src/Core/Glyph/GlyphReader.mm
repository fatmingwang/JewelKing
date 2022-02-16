#include "../stdafx.h"
#include "GlyphReader.h"
#include "../XML/XMLLoader.h"

GlyphReader::GlyphReader(void)
{
	m_wFontHeight = 0;
	m_dwMask = 0;//to alpha
    m_cMaxGlyph = 0;
    m_pTranslatorTable = 0;
	m_dwNumGlyphs = 0;
	m_pGlyphs = 0;
}

GlyphReader::~GlyphReader(void)
{
	if(m_pFontData)
		free( m_pFontData );
}

bool GlyphReader::LoadFile( const WCHAR* strFileName)
{
#ifdef __IPHONE__
	char	l_strFileName[MAX_PATH];
	FATMING_CORE::GetAppleBundelResourcePathByObjectPath(UT::WcharToChar(strFileName),l_strFileName);
	FILE*	l_pFile = fopen(l_strFileName,"rb");
#else
    // Open the file for reading
	FILE*	l_pFile = fopen(UT::WcharToChar(strFileName),"rb");
#endif
	if( !l_pFile )
		return false;
	long	l_lFileSize = GetFileSize(l_pFile);
    m_pFontData = malloc( l_lFileSize );

    if( NULL == m_pFontData )
    {
        fclose(l_pFile );
        return false;
    }

	fread(m_pFontData,sizeof(char),l_lFileSize,l_pFile);
	fclose(l_pFile );
    return true;
}

bool	GlyphReader::IsLegalCharacter(WCHAR e_Character)
{
	if( m_pTranslatorTable[e_Character]<m_dwNumGlyphs )
	{
		return true;
	}
	return false;
}

//1.Version			size:DWORD*1
//2.Height			size:SHORT*1
//3.MaskColor		size:DWORD*1
//4.MaxGlyph		size:WORD*1
//5.TranslatorTable size:WORD*(MaxGlyph+1)
//6.m_dwNumGlyphs	size:DWORD*1
//7.m_pGlyphs		size:m_dwNumGlyphs*FILE_GLYPH_ATTR
bool	GlyphReader::LoadFontDataFile(const WCHAR* strFileName)
{
	if( !LoadFile( strFileName) )
		assert(0&&"open file failed");

	BYTE*	l_pData = (BYTE*)m_pFontData;
    // Check version of file (to make sure it matches up with the FontMaker tool)
    DWORD dwFileVersion = *((DWORD*)(l_pData)); l_pData += sizeof(DWORD);    
    if( dwFileVersion == 0x00000005 )
    {
        // Parse the font data
		this->m_wFontHeight	= *((SHORT*)l_pData); l_pData += sizeof(SHORT);
		this->m_dwMask		= *((DWORD*)l_pData); l_pData += sizeof(DWORD);
        // Point to the translator string
        m_cMaxGlyph			= ((WORD*)l_pData)[0];   l_pData += sizeof(WORD);
        m_pTranslatorTable	= (WORD*)l_pData;       l_pData += sizeof(WORD)*(m_cMaxGlyph+1);

        // Read the glyph attributes from the file
        m_dwNumGlyphs		= ((DWORD*)l_pData)[0];  l_pData += sizeof(DWORD);
		m_pGlyphs = (FILE_GLYPH_ATTR*)l_pData;
    }
    else
    {
        //ATG_PrintError( "Incorrect version number on font file!\n" );
        return false;
    }
	return true;
}
