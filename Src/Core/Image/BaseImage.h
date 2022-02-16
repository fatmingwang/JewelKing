#ifndef BASEIMAGE_H
#define BASEIMAGE_H

#include "glLoadImage.h"

namespace FATMING_CORE
{
	//=====================
	//simplist image
	//=====================
	//left up right down
	//<Common Name="BackGround/BKImage.png" UV="0,0,1,1" />
	class cBaseImage:public cSimpleGLTextureWithcTexData
	{
	public:
		DEFINE_TYPE_INFO();
		cBaseImage(const char*e_strImageName,bool e_bFetchPixels = false):cSimpleGLTextureWithcTexData(e_strImageName,e_bFetchPixels){}
		cBaseImage(cBaseImage*e_pBaseImage):cSimpleGLTextureWithcTexData(e_pBaseImage){}					//here is clone,but be sure do not delete original one before all resource release
		virtual ~cBaseImage(){}
		virtual	void	Render(int e_iX,int e_iY);				//render by indicate position without offset position.
		virtual	void	Render(Vector3 e_vPos);					//render by indicate position without offset position.
		virtual	void	RenderWithOffset(int e_iX,int e_iY);	//render with offset position.
		virtual	void	RenderWithOffset(Vector3 e_vPos);		//render with offset position.
		virtual	void	RenderWithOffset();		                //render with offset position.
		virtual	void	Render();								//render by texture behavior.
		virtual	void	Render(cMatrix44 e_Mat);                //render by matrix
		virtual	void	RenderBySpecificPos(Vector3 e_vStartPos,Vector3 e_vEndPos);//change the width by 2 points distance
	};

	//===============
	//mostly ui usually has constanst position
	//===============
	//<UI Name="388.png" Position="0,0" UV="0,0,1,1" />
	class cUIImage:public cBaseImage
	{
		int	m_iCollisionImageWidth;	//if hardware does not support non power of two,we have to know real image size,so we could fetch right pixel to detect collide
		int	m_iCollisionImageHeight;
		//the pixels over this point is useless,only active on editor,game morde should as same as width and height
		GET_SET_DEC(POINT,m_RightDownStripOffPos,GetRightDownStripOffPos,SetRightDownStripOffPos);
		//for editor
		GET_SET_DEC(POINT,m_OriginalImageSize,GetOriginalImageSize,SetOriginalImageSize);
	public:
		//for PI ediotr
		cUIImage*m_pEditorAttachParent;
		Vector3	m_vEditorAttachParentRelativePos;
		DEFINE_TYPE_INFO();
		cUIImage(cUIImage*e_pUIImage);
		cUIImage(char*e_strImageName);
		//it will pointer pixel data to e_pPixelsData and will delete the data,so do not delete the data from input
		cUIImage(WCHAR*e_strName,char*e_pPixelsData,int e_iWidth,int e_iHeight,GLint e_iDataFormat);//create by pixle data
		virtual ~cUIImage();
		virtual	void	RenderWithOffset();
		//this collide include rotation data(Angle around Z axis).
		//not same as cSimpleGLTextureWithcTexData and cTexData
		virtual bool	Collide(int e_iLocalX,int e_iLocalY);//input local space coordinate or it does't collide,RECT(0,0,right,buttom).
		bool	CollideWithOutAlphaTest(int e_iLocalX,int e_iLocalY);
		//the real pixels exit size is,right down poisition minus left top position
		POINT	GetImageRealSize()
		{//add 1 for offset start at 0,0
			POINT	l_Size = { m_RightDownStripOffPos.x-this->m_OffsetPos.x+1,m_RightDownStripOffPos.y-this->m_OffsetPos.y+1 };
			return l_Size;
		}
	};
}
#endif