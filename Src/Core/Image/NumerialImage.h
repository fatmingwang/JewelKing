#ifndef NUMERIAL_H
#define NUMERIAL_H
#include "BaseImage.h"
namespace FATMING_CORE
{
	//class	cNumerialImage:public cSpriteGLTexture
	//{
	//	int	m_iEachImageWidth;
	//public:
	//	DEFINE_TYPE_INFO();
	//	cNumerialImage(char*e_strImageName);
	//	~cNumerialImage();

	//	void	Draw(int	e_iValue,int e_iPosX,int e_iPosY);
	//	//aligment by
	//	void	DrawFromDownToUp(int	e_iValue,int e_iPosX,int e_iPosY,float e_fRotate = 90.f);
	//	void	DrawFromUpToDown(int	e_iValue,int e_iPosX,int e_iPosY,float e_fRotate = 270.f);
	//};


	//=======================
	//set color and angle by BaseImage
	//=======================
//	<Numerial Name="NumerialImage/CardNI.png" />
	class	cNumerialImage:public cBaseImage
	{
		bool	m_bImageFromesource;
		GET_SET_DEC(float*,m_pfTexCoordinate,GetTexCoordinate,SetCoordinate);
		GET_SET_DEC(int,m_iNumIndex,GetNumIndex,SetNumIndex);
		//single image width
		GET_SET_DEC(int,m_iSingleImageWidth,GetSingleImageWidth,SetSingleImageWidth);
		//single image height
		GET_SET_DEC(int,m_iSingleImageHeight,GetSingleImageHeight,SetSingleImageHeight);
		//right to left please set eD_LEFT,up to down please set eD_DOWN,else not support
		GET_SET_DEC(eDirection,m_eDirection,GetDirection,SetDirection);
		void	Draw(const char*e_strData,int e_iCount,int e_iPosX,int e_iPosY,float*e_pmat = 0,bool e_bCenter = false);
	public:
		DEFINE_TYPE_INFO();
		cNumerialImage(char*e_strImageName);
		cNumerialImage(cBaseImage*e_pImage,float*e_pftexCoordinate);//copy from specific image and specific tex coordinate,ex:puzzle image
		cNumerialImage(cBaseImage*e_pImage0,cBaseImage*e_pImage9);
		~cNumerialImage();
		//this one is right to left,if e_pmat is null apply normal one or apply a matrix to it.
		void	Draw(int	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat = 0,bool e_bCenter = false);
		void	Draw(int64	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat = 0,bool e_bCenter = false);
		//
		void	DrawOnCenter(int	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat = 0);
		void	DrawOnCenter(int64	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat = 0);
		//max u value for texture space
		float	GetMaxU(){ return m_pfTexCoordinate[38]; }
	};
}
#endif