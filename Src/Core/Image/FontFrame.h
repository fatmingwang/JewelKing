#ifndef FONT_FRAME_H
#define FONT_FRAME_H
////#include "GlyphFontRender.h"
////class cImageParser;
////class	cFontWithFrame:public cGlyphFontRender
////{
//===============================
//     LT			RT
//      |-         -|
//
//
//	    |_		   _|
//		LD			RD
//
//			- Row
//			| Column
//===============================
////	enum eDirection
////	{
////		eD_LT = 0,
////		eD_ROW_UP,
////		eD_RT,
////		eD_COLUMN_LEFT,
////		eD_BK_COLOR,
////		eD_COLUMN_RIGHT,
////		eD_LD,
////		eD_ROW_DOWN,
////		eD_RD,
////	};
////									//0  2  8   6  1      7       3            5            4
////	cSpriteGLTexture* m_pFrameImage;//LT,RT,RD,LD,Row Up,Row Down,Column left,Column Down,bk color
////
////	GET_SET_DEC(int,m_iFrameWidth,GetFrameWidth,SetFrameWidth);	//
////	GET_SET_DEC(int,m_iFrameHeight,GetFrameHeight,SetFrameHeight);	//
////	GET_SET_DEC(POINT,m_Margin,GetMargin,SetMargin);				//the margin to the border(edge).
////	WCHAR*	m_strForRender;
////	int	m_iFonrHeight;
////	GET_SET_DEC(POINT,m_TextGapOffset,GetTextGapOffset,SetTextGapOffset);	//the text gap offset.
////	cImageParser*m_pImageResourceManager;//for  cSpriteGLTexture is not from resource
////public:
////	cFontWithFrame(char* e_strFontName,cSpriteGLTexture*e_pFrameImage);
////	cFontWithFrame(char* e_strFontName);
////	~cFontWithFrame();
////
////	cSpriteGLTexture*	GetFrameImage(){ return m_pFrameImage; }
////	WCHAR*	GetCurrentRenderText(){ return m_strForRender;  }
////	void	SetFrameImage(cSpriteGLTexture*e_pFrameImage);		//at the same moment set up margin
////	void	SetUpFont(WCHAR*e_strText);							//setup interal width and height
////	void	Render(int e_iX,int e_iY);	//after setup,we could rende the data already existed
////
////	void	ForceRender(int e_iX,int e_iY,WCHAR*e_strText);
////};

//===========================================
//parse text and read text frame,
//also need to save data from char to wchar,
//also have to wchar to char
//
//===========================================

//class	cTextManager:public cNodeISAX,public cFontWithFrame
//{
//
//};

#endif