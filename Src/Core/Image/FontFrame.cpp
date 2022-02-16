#include "../stdafx.h"
#include "FontFrame.h"
#include "SimplePrimitive.h"
#include "ImageParser.h"
//cFontWithFrame::cFontWithFrame(char* e_strFontName,cSpriteGLTexture*e_pFrameImage)
//:cGlyphFontRender(e_strFontName)
//{
//	m_pImageResourceManager = 0;
//	SetFrameImage(e_pFrameImage);
//	m_iFrameWidth = 0;
//	m_iFrameHeight  = 0;
//	m_strForRender = 0;
//	m_TextGapOffset.x = 1;
//	m_TextGapOffset.y = 1;
//	m_iFonrHeight = (int)m_pGlyphReader->GetCharInfo(0).fHeight;
//}
//
//cFontWithFrame::cFontWithFrame(char* e_strFontName)
//:cGlyphFontRender(e_strFontName)
//{
//	char	l_strForStripExtensionName[TEMP_SIZE];
//	sprintf(l_strForStripExtensionName,"%s\0",UT::GetFileNameWithoutFullPath(e_strFontName));
//	char	l_strForDirectory[TEMP_SIZE];
//	if( UT::GetDirectoryWithoutFileName(e_strFontName) )
//		sprintf(l_strForDirectory,"%s\0",UT::GetDirectoryWithoutFileName(e_strFontName));
//	else
//		l_strForDirectory[0] = '\0';
//
//	m_pImageResourceManager = new cImageParser();
//	char	l_strFrameImageName[TEMP_SIZE];
//	sprintf(l_strFrameImageName,"%s%sFrameImage.fat\0",l_strForDirectory,l_strForStripExtensionName);
//	bool	l_b = m_pImageResourceManager->Parse(l_strFrameImageName);
//	assert(l_b&&"cFontWithFrame parse frame image file failed");
//	m_iFrameWidth = 0;
//	m_iFrameHeight  = 0;
//	m_strForRender = 0;
//	m_TextGapOffset.x = 1;
//	m_TextGapOffset.y = 1;
//	m_iFonrHeight = (int)m_pGlyphReader->GetCharInfo(0).fHeight;
//	SetFrameImage((cSpriteGLTexture*)(*m_pImageResourceManager)[0]);
//}
//
//cFontWithFrame::~cFontWithFrame()
//{
//	SAFE_DELETE(m_pImageResourceManager);
//}
//
//void	cFontWithFrame::SetFrameImage(cSpriteGLTexture*e_pFrameImage)
//{
//	m_pFrameImage = e_pFrameImage;
//	m_Margin.x = e_pFrameImage->GetSingleImageWidth();
//	m_Margin.y = e_pFrameImage->GetSingleImageHeight();
//}
//
//void	cFontWithFrame::SetUpFont(WCHAR*e_strText)
//{
//	m_iFrameWidth = 0;
//	m_iFrameHeight  = m_iFonrHeight;
//	int	l_iCurrentWidth = 0;//to 0 if character is '\n'
//
//	this->m_strForRender = e_strText;
//	UINT	l_iLength = wcslen(e_strText);
//	for( UINT i=0;i<l_iLength;++i )			//judge the frame width and height by L'\n'.
//	{
//		if( e_strText[i] == L'\n' )
//		{
//			l_iCurrentWidth = 0;
//			m_iFrameHeight+=m_iFonrHeight;
//		}
//		else
//		{
//			if( e_strText[i] == 13 )//13,10 equal next line
//				continue;
//			l_iCurrentWidth += (int)m_pGlyphReader->GetCharInfo(e_strText[i]).fWidth;
//			if( m_iFrameWidth < l_iCurrentWidth )
//			{
//				m_iFrameWidth = l_iCurrentWidth;
//			}
//		}
//	}
//	int	l_iImageWidth = m_pFrameImage->GetSingleImageWidth();
//	int	l_iImageHeight = m_pFrameImage->GetSingleImageHeight();
//	//m_iFrameWidth = m_iFrameWidth+l_iImageWidth*2;//for left and right
//	//m_iFrameHeight = m_iFrameHeight+l_iImageHeight*2;//for up and down
//	//here for sdl to filled the widht and height
//	//in opengl does't need 
//	int	l_iRestWidth = m_iFrameWidth%l_iImageWidth;
//	int	l_iRestHeight = m_iFrameHeight%l_iImageHeight;
//	if( l_iRestWidth )
//	{
//		m_iFrameWidth = m_iFrameWidth-l_iRestWidth+l_iImageWidth;
//	}
//	if( l_iRestHeight )
//	{
//		m_iFrameHeight = m_iFrameHeight-l_iRestHeight+l_iImageHeight;
//	}
//}
//
//
//void	cFontWithFrame::Render(int e_iX,int e_iY)
//{
//	//int	l_iOffsetWidth = -1;//because we divide frame image to 3*3 size,so it probably have 1 pixel offset so here give it a tricky offset
//	assert(m_strForRender&&"have not set text yet");
//	int	l_iImageWidth = m_pFrameImage->GetSingleImageWidth();
//	int	l_iImageHeight = m_pFrameImage->GetSingleImageHeight();
//
//	POINT	l_LT = {e_iX,e_iY};
//	POINT	l_RT = {e_iX+m_iFrameWidth+l_iImageWidth,e_iY};
//	POINT	l_RD = {l_RT.x,e_iY+m_iFrameHeight+l_iImageHeight};
//	POINT	l_LD = {e_iX,l_RD.y};
////opengl
//	//bk color
//	m_pFrameImage->GetImage()->ApplyImage();
//	float	*l_pfTexCoordinate = &m_pFrameImage->GetTexCoordinate()[eD_BK_COLOR*4];
//	GLRender::DrawQuad(l_pfTexCoordinate,m_iFrameWidth,m_iFrameHeight,e_iX+l_iImageWidth,e_iY+l_iImageHeight,1.f,1.f,0);
//	//LT,RT,RD,LD
//	m_pFrameImage->Render(l_LT.x,l_LT.y,eD_LT);
//	m_pFrameImage->Render(l_RT.x,l_RT.y,eD_RT);
//	m_pFrameImage->Render(l_RD.x,l_RD.y,eD_RD);
//	m_pFrameImage->Render(l_LD.x,l_LD.y,eD_LD);
//	//Row
//	l_pfTexCoordinate = &m_pFrameImage->GetTexCoordinate()[eD_ROW_UP*4];
//	GLRender::DrawQuad(l_pfTexCoordinate,
//		m_iFrameWidth,l_iImageHeight,
//		e_iX+l_iImageWidth,e_iY,
//		1.f,1.f,0);
//	l_pfTexCoordinate = &m_pFrameImage->GetTexCoordinate()[eD_ROW_DOWN*4];
//	GLRender::DrawQuad(l_pfTexCoordinate,
//		m_iFrameWidth,l_iImageHeight,
//		e_iX+l_iImageWidth,e_iY+m_iFrameHeight+l_iImageHeight,
//		1.f,1.f,0);
//	//column
//	l_pfTexCoordinate = &m_pFrameImage->GetTexCoordinate()[eD_COLUMN_LEFT*4];
//	GLRender::DrawQuad(l_pfTexCoordinate,
//		l_iImageWidth,m_iFrameHeight,
//		e_iX,e_iY+l_iImageHeight,
//		1.f,1.f,0);
//	l_pfTexCoordinate = &m_pFrameImage->GetTexCoordinate()[eD_COLUMN_RIGHT*4];
//	GLRender::DrawQuad(l_pfTexCoordinate,
//		l_iImageWidth,m_iFrameHeight,
//		e_iX+l_iImageWidth+m_iFrameWidth,e_iY+l_iImageHeight,
//		1.f,1.f,0);
//	this->RenderFont(e_iX+l_iImageWidth,e_iY+l_iImageHeight,m_strForRender);
////FT2
//	//m_pFrameImage->Render(l_LT.x,l_LT.y,eD_LT);
//	//m_pFrameImage->Render(l_RT.x,l_RT.y,eD_RT);
//	//m_pFrameImage->Render(l_RD.x,l_RD.y,eD_RD);
//	//m_pFrameImage->Render(l_LD.x,l_LD.y,eD_LD);
//
//	//POINT	l_NewLT = l_LT;
//	//l_NewLT.x+=l_iImageWidth;
//	//l_NewLT.y+=l_iImageHeight;
//
//	//int	l_iNumHeight = m_iFrameHeight/l_iImageHeight;
//	//int	l_iNumWidth = m_iFrameWidth/l_iImageWidth;
//	//for( int i=0;i<l_iNumHeight;++i )
//	//{
//	//	int	l_iYPos = i*l_iImageHeight;
//	//	for( int j=0;j<l_iNumWidth;++j )
//	//	{
//	//		int	l_iXPos = j*l_iImageWidth;
//	//		//for blank
//	//		m_pFrameImage->Render(l_NewLT.x+l_iXPos,l_NewLT.y+l_iYPos,eD_BK_COLOR);
//
//	//		//for eD_Column_Right,eD_COLUMN_LEFT
//	//		if( j == l_iNumWidth-1 )
//	//			m_pFrameImage->Render(l_RT.x,l_NewLT.y+l_iYPos,eD_COLUMN_RIGHT);
//	//		if( j == 1 )
//	//			m_pFrameImage->Render(l_LT.x,l_NewLT.y+l_iYPos,eD_COLUMN_LEFT);
//
//	//		//for eD_ROW_UP,eD_ROW_DOWN
//	//		if( i == 1 )
//	//			m_pFrameImage->Render(l_NewLT.x+l_iXPos,l_LT.y,eD_ROW_UP);
//	//		if( i== l_iNumHeight-1 )
//	//			m_pFrameImage->Render(l_NewLT.x+l_iXPos,l_RD.y,eD_ROW_DOWN);
//	//	}
//	//}
//	//this->RenderFont(e_iX+l_iImageWidth,e_iY+l_iImageHeight,m_strForRender);
//}
//
//void	cFontWithFrame::ForceRender(int e_iX,int e_iY,WCHAR*e_strText)
//{
//	SetUpFont(e_strText);
//	Render(e_iX,e_iY);
//}