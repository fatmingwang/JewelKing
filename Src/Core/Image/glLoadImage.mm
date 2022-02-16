#include "../stdafx.h"
#include "glLoadImage.h"
#include "SimplePrimitive.h"
#include "../Utility.h"
#include "BinaryToTexture.h"
#include "../GameplayUT/GameApp.h"
#include "../GameplayUT/BinaryFile.h"
#ifdef WIN32
#include "../../../Include/IL/il.h"
//if u like to link under windows copy and paste to the main.cpp
//#pragma comment(lib, "../../lib/unicode/Devil.lib")
#else
#ifdef __IPHONE__
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIImage.h>
#import <UIKit/UIImagePickerController.h>
#else
#include "IL/il.h"//linux
#endif
#endif
#ifdef __IPHONE__
//saving an image 
UIImage * glToUIImage()
{
    //int l_iWidth  = (int)cGameApp::m_svViewPortSize.x;
    //int l_iHeight  = (int)cGameApp::m_svViewPortSize.y;
    int l_iWidth  = (int)FATMING_CORE::cGameApp::m_svViewPortSize.y;
    int l_iHeight  = (int)FATMING_CORE::cGameApp::m_svViewPortSize.x;

    NSInteger myDataLength =  l_iWidth * l_iHeight * 4;
 
    // allocate array and read pixels into it.
    GLubyte *buffer = (GLubyte *) malloc(myDataLength);
    glReadPixels(0, 0, l_iWidth, l_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
 
    // gl renders "upside down" so swap top to bottom into new array.
    // there's gotta be a better way, but this works.
    GLubyte *buffer2 = (GLubyte *) malloc(myDataLength);
    for(int y = 0; y < l_iHeight; y++)
    {
        for(int x = 0; x < l_iWidth * 4; x++)
        {
            buffer2[(l_iHeight-1 - y) * l_iWidth * 4 + x] = buffer[y * 4 * l_iWidth + x];
        }
    }
 
    // make data provider with data.
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, buffer2, myDataLength, NULL);
 
    // prep the ingredients
    int bitsPerComponent = 8;
    int bitsPerPixel = 32;
    int bytesPerRow = 4 * l_iWidth;
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
 
    // make the cgimage
    CGImageRef imageRef = CGImageCreate(l_iWidth, l_iHeight, bitsPerComponent, bitsPerPixel, bytesPerRow, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
 
    // then make the uiimage from that
    UIImage *myImage = [UIImage imageWithCGImage:imageRef];
    return myImage;
}
 
void    captureToPhotoAlbum()
{
    UIImage *image = glToUIImage();
    UIImageWriteToSavedPhotosAlbum(image, nil, nil, nil);
    //[image release];
} 
#endif 
namespace FATMING_CORE
{
	//if graphic card does not support non power of two,set as false
	bool	g_bSupportNonPowerOfTwoTexture = true;

	//GLfloat	g_fMAG_FILTERValue = GL_NEAREST;
	//GLfloat	g_fMIN_FILTERValue = GL_NEAREST;
	GLfloat	g_fMAG_FILTERValue = GL_LINEAR;
	GLfloat	g_fMIN_FILTERValue = GL_LINEAR;
    GLuint  cSimpleGLTexture::m_suiLastUsingImageIndex = -1;
	UINT	g_iAteVideoMomory = 0;
	void	VideoMemoryAddCheck(int e_iWidth,int e_iHeight,int e_iChannel,const char*e_stre_strFileName)
	{
		int	l_iValue = e_iWidth*e_iHeight*e_iChannel;
		g_iAteVideoMomory += l_iValue;
#ifdef WIN32
#ifdef _DEBUG
		float	l_fMB = l_iValue/1024.f/1024.f;
		float	l_fTotalMB = g_iAteVideoMomory/1024.f/1024.f;
		std::string	l_str = UT::ComposeMsgByFormat("%s:\t\t\t\t\t\t%.2fMB,\nVideoMomory Use:%.2fMB\n",e_stre_strFileName,l_fMB,l_fTotalMB);
		if(cGameApp::m_spLogFile)
		{
			cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		}
		else
		{
			OutputDebugString(ValueToStringW(l_str.c_str()));
		}
#endif
#endif
	}
	void	VideoMemoryMinusCheck(int e_iWidth,int e_iHeight,int e_iChannel)
	{
#ifdef WIN32
#ifdef _DEBUG
		g_iAteVideoMomory -= e_iWidth*e_iHeight*e_iChannel;
		float	l_fMB = g_iAteVideoMomory/1024.f/1024.f;
		std::string	l_str = UT::ComposeMsgByFormat("VideoMomory Use:%.2fMB",l_fMB);
		if(cGameApp::m_spLogFile)
		{
			cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		}
		else
		{
			OutputDebugString(ValueToStringW(l_str.c_str()));
		}
#endif
#endif
	}

	//IL_NO_ERROR
	//No detectable error has occured.
	//IL_INVALID_ENUM, ILU_INVALID_ENUM, ILUT_INVALID_ENUM
	//An unacceptable enumerated value was passed to a function.
	//IL_OUT_OF_MEMORY, ILU_OUT_OF_MEMORY, ILUT_OUT_OF_MEMORY
	//Could not allocate enough memory in an operation.
	//IL_FORMAT_NOT_SUPPORTED
	//The format a function tried to use was not able to be used by that function.
	//IL_INTERNAL_ERROR, ILU_INTERNAL_ERROR
	//A serious error has occurred. Please e-mail DooMWiz with the conditions leading up to this error being reported.
	//IL_INVALID_VALUE, ILU_INVALID_VALUE, ILUT_INVALID_VALUE
	//An invalid value was passed to a function or was in a file.
	//IL_ILLEGAL_OPERATION, ILU_ILLEGAL_OPERATION, ILUT_ILLEGAL_OPERATION
	//The operation attempted is not allowable in the current state. The function returns with no ill side effects.
	//IL_ILLEGAL_FILE_VALUE
	//An illegal value was found in a file trying to be loaded.
	//IL_INVALID_FILE_HEADER
	//A file's header was incorrect.
	//IL_INVALID_PARAM, ILU_INVALID_PARAM, ILUT_INVALID_PARAM
	//An invalid parameter was passed to a function, such as a NULL pointer.
	//IL_COULD_NOT_OPEN_FILE, ILUT_COULD_NOT_OPEN_FILE
	//Could not open the file specified. The file may already be open by another app or may not exist.
	//IL_INVALID_EXTENSION
	//The extension of the specified filename was not correct for the type of image-loading function.
	//IL_FILE_ALREADY_EXISTS
	//The filename specified already belongs to another file. To overwrite files by default, call ilEnable with the IL_FILE_OVERWRITE parameter.
	//IL_OUT_FORMAT_SAME
	//Tried to convert an image from its format to the same format.
	//IL_STACK_OVERFLOW, ILUT_STACK_OVERFLOW
	//One of the internal stacks was already filled, and the user tried to add on to the full stack.
	//IL_STACK_UNDERFLOW, ILUT_STACK_UNDERFLOW
	//One of the internal stacks was empty, and the user tried to empty the already empty stack.
	//IL_INVALID_CONVERSION
	//An invalid conversion attempt was tried.
	//IL_LIB_JPEG_ERROR
	//An error occurred in the libjpeg library.
	//IL_LIB_PNG_ERROR
	//An error occurred in the libpng library.
	//IL_UNKNOWN_ERROR
	//No function sets this yet, but it is possible (not probable) it may be used in the future.
	//ILUT_NOT_SUPPORTED
	//A type is valid but not supported in the current build.
	//here si the idea for store screen buffer to 4 block
	//http://www.gamedev.net/community/forums/topic.asp?topic_id=489893
	//here we divided screen to 4 parts
	//       
	//  |---|---|
	//  | 1 | 2 |
	//  |---|---|
	//  | 3 | 4 |
	//  |---|---|



	//Lord_Evil, you talk about GL_PACK as if the alignment was its only parameter. There are many parameters you can adjust,
	//and alignment is indeed one of them, but I'm talking about adjusting row skip, pixel skip and row length.
	//
	//GLubyte *img = new GLubyte[1280*1024*4];
	//
	//glPixelStorei(GL_PACK_ROW_LENGTH, 1280);
	//
	//// Block 1
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 512);
	//glReadPixels(0, 512, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//
	//// Block 2
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 640);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 512);
	//glReadPixels(640, 512, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//
	//// Block 3
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	//glReadPixels(0, 0, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);
	//
	//// Block 4
	//glPixelStorei(GL_PACK_SKIP_PIXELS, 640);
	//glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	//glReadPixels(640, 0, 640, 512, GL_RGBA, GL_UNSIGNED_BYTE, img);


	//=====================
	//simple method to store the specific area to buffer
	//x, y, width, and height indicate the area of the screen you want to capture,
	//where x and y are the lower left of the rectangle, 
	//and width and height are the dimensions. 
	//If you want to read the whole screen, just set both x and y to 0, and width and height to the dimensions of the screen.

	//def readScreen(x, y, width, height):
	//    """ Read in the screen information in the area specified """
	//    glFinish()
	//    glPixelStorei(GL_PACK_ALIGNMENT, 4)
	//    glPixelStorei(GL_PACK_ROW_LENGTH, 0)
	//    glPixelStorei(GL_PACK_SKIP_ROWS, 0)
	//    glPixelStorei(GL_PACK_SKIP_PIXELS, 0)
	//
	//    data = glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE)
	//
	//    return data
	//

	//=====================






	//==============
	//http://ubuntuforums.org/showthread.php?t=396281
	//wybiral said:
	//when u loading a image with alpha u 'd call below beofre apply drawing
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.5);


	//http://www.gamedev.net/community/forums/topic.asp?topic_id=452780&forum_id=25&gforum_id=0
	//gluBuild2DMipmaps is very slow (+ buggy)
	//
	//if a texture is not power of 2 sized then gluBuild2DMipmaps will first resize it so it is, i assume thats where the problem lies somehow.
	//
	//to artifically see if mipmapping is working draw the texture with a high texture_lod_bias
	//perhaps draw the texture on a fullscreen sized polygon
	//+ then with a keypress cycle through the mipmap levels


	//If you want to use the GL_GENERATE_MIPMAP you don't use gluBuild2DMipmaps() you use glTexImage2D() and call the correct glTexParameteri() with true in it.
	//
	//The above method works, and will automatically generate mipmaps for your dynamic textures for all levels above base level.
	//
	//With GenerateMipmapEXT() it will generate all mipmaps above base also but when you choose to not the driver.
	//==============
	//glCopyTexSubImage2D
	
//- (void)saveImage:(UIImage*)image:(NSString*)imageName
//{ 
//    NSData *imageData = UIImagePNGRepresentation(image); //convert image into .png format.     
//    NSFileManager *fileManager = [NSFileManager defaultManager];//create instance of NSFileManager     
//    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); //create an array and store result of our search for the documents directory in it     
//    NSString *documentsDirectory = [paths objectAtIndex:0]; //create NSString object, that holds our exact path to the documents directory     
//    NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png", imageName]]; //add our image to the path     
//    [fileManager createFileAtPath:fullPath contents:imageData attributes:nil]; //finally save the path (image)
//    NSLog(@"image saved");
//}
// 
////removing an image
// 
//- (void)removeImage:(NSString*)fileName {
// 
//NSFileManager *fileManager = [NSFileManager defaultManager];
// 
//NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
// 
//NSString *documentsDirectory = [paths objectAtIndex:0];
// 
//NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png", fileName]];
// 
//[fileManager removeItemAtPath: fullPath error:NULL];
// 
//NSLog(@"image removed");
// 
//}
// 
////loading an image
// 
//- (UIImage*)loadImage:(NSString*)imageName {
// 
//NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
// 
//NSString *documentsDirectory = [paths objectAtIndex:0];
// 
//NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png", imageName]];
// 
//return [UIImage imageWithContentsOfFile:fullPath];
// 
//Now, you can easily save an image like:
//
//1
//[self saveImage: myUIImage: @"myUIImageName"];
//or load it like:
//
//1
//myUIImage = [self loadImage: @"myUIImageName"];
//or remove it like:
//
//1
//[self removeImage: @"myUIImageName"

	void	SaveCurrentBufferToImage(char*e_strFileName,int e_iWidth,int e_iHeight)
	{
	#ifndef __IPHONE__
		ilEnable(IL_FILE_OVERWRITE);
		unsigned char *l_pPixelData = new unsigned char[e_iWidth*e_iHeight*3];
		glReadPixels( 0, 0, e_iWidth, e_iHeight, GL_RGB, GL_UNSIGNED_BYTE, l_pPixelData );
		ILuint uiID;
		ilGenImages( 1, &uiID );
		ilBindImage( uiID );
		ilTexImage( e_iWidth, e_iHeight, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, l_pPixelData);
	#ifdef WIN32
		ilSave( IL_PNG, (WCHAR*)e_strFileName );
	#else
		ilSave( IL_PNG, e_strFileName );
	#endif
		ilDeleteImages(1, &uiID); // Because we have already copied image data into texture data
		delete l_pPixelData;
    #else//for iphone,save into album
        captureToPhotoAlbum();
	#endif
	}

    void	SaveBufferToImage(char*e_strFileName,int e_iWidth,int e_iHeight,void*e_pPixel)
    {
    #ifndef __IPHONE__
		ILuint uiID;
		ilGenImages( 1, &uiID );
		ilBindImage( uiID );
		ilTexImage( e_iWidth, e_iHeight, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, e_pPixel);
	#ifdef WIN32
		ilSave( IL_PNG, (WCHAR*)e_strFileName );
	#else
		ilSave( IL_PNG, e_strFileName );
	#endif
		ilDeleteImages(1, &uiID); // Because we have already copied image data into texture data
    #else//for iphone,save into album
        captureToPhotoAlbum();		
    #endif
    }

	const wchar_t*     cSimpleGLTexture::TypeID( L"cSimpleGLTexture" );
	const wchar_t*     cSimpleGLTextureWithcTexData::TypeID( L"cSimpleGLTextureWithcTexData" );
	//===============
	//
	//===============
	cSimpleGLTexture::cSimpleGLTexture(unsigned char*e_pPixels,int e_iWidth,int e_iHeight,WCHAR*e_strName,bool e_bCopyPixels,bool e_bAllocateMemoryForPixelIfFetch,GLenum e_eImageType)
	{
	    m_pPixels = 0;
		m_fUV[0] = 0.f; m_fUV[1] = 0.f; m_fUV[2] = 1.f; m_fUV[3] = 1.f;
		m_uiImageIndex = 0;
		this->m_iHeight = e_iHeight;
		this->m_iWidth = e_iWidth;
		int	l_iNumChannel = 4;
		if( e_bCopyPixels )
		{
			if( e_bAllocateMemoryForPixelIfFetch )
			{
				int	l_iSize = m_iHeight*m_iWidth;
				if( e_eImageType == GL_RGBA )
				{
					m_pPixels = new unsigned char[l_iSize*4];
					memcpy(m_pPixels,e_pPixels,sizeof(unsigned char)*l_iSize*4);
				}
				else
				if( e_eImageType == GL_RGB )
				{
					m_pPixels = new unsigned char[l_iSize*3];
					memcpy(m_pPixels,e_pPixels,sizeof(unsigned char)*l_iSize*3);
					l_iNumChannel = 3;
				}
			}
			else
				m_pPixels = e_pPixels;
		}
		else
			m_pPixels = 0;
		m_bTextureFromResource = false;
		m_iPixelFormat = e_eImageType;

		glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
		assert(m_uiImageIndex!=0&&"opengl init???");
		GLenum	l_GLenum = glGetError();
#ifdef 	OPENGLES_2_X
		glActiveTexture( GL_TEXTURE0 );
#endif
		m_suiLastUsingImageIndex = -1;
		this->ApplyImage();/* Binding of texture name */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear  interpolation for magnification filter */
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear  interpolation for minifying filter */
		//quick way to find out power of two.
		//if (width & (width - 1)) or (height & (height - 1)):
		int	l_iWidthPO2 = power_of_two(m_iWidth);
		int	l_iHeightPO2 = power_of_two(m_iHeight);
		GLint texSize; glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
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
			char*l_pNewPixelData = TextureToPowOfTwo((char*)e_pPixels,m_iWidth,m_iHeight,m_iPixelFormat==GL_RGBA?true:false);
			glTexImage2D(GL_TEXTURE_2D, 0, l_iNumChannel, l_iWidthPO2,
				l_iHeightPO2, 0,l_iNumChannel==4?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE,l_pNewPixelData); // Texture specification.	
			GLenum	l_error = glGetError();
			//assign new UV
			m_fUV[2] = (float)m_iWidth/l_iWidthPO2;
			m_fUV[3] =  (float)m_iHeight/l_iHeightPO2;
			delete l_pNewPixelData;
		}
		else
		{
			assert((m_iWidth<=texSize||m_iHeight<=texSize)&&"texture size is too big then card support");
			glTexImage2D(GL_TEXTURE_2D, 0, l_iNumChannel, m_iWidth,
			  m_iHeight, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,e_pPixels); // Texture specification.
		}
		VideoMemoryAddCheck(this->m_iWidth,this->m_iHeight,m_iPixelFormat == GL_RGBA?4:3,"programmer image");
	}
	//===============
	//
	//===============
	cSimpleGLTexture::cSimpleGLTexture(const char*e_strImageFileName,bool e_bFetchPixelData)
	{
	    m_pPixels = 0;
		m_uiImageIndex = 0;
		m_bTextureFromResource = false;
		m_fUV[0] = 0.f; m_fUV[1] = 0.f; m_fUV[2] = 1.f; m_fUV[3] = 1.f;
		m_uiImageIndex = 0;
		this->m_iHeight = 0;
		this->m_iWidth = 0;
		if( !e_strImageFileName )
		{//if image resource does't need to read,the texture resource is come from external,so we do not to delete it
			this->m_bTextureFromResource = true;
			return;
		}
		LoadImage(e_strImageFileName,e_bFetchPixelData);
	}
	//===============
	//
	//===============
	cSimpleGLTexture::~cSimpleGLTexture()
	{
		if( !m_bTextureFromResource )
		{
			VideoMemoryMinusCheck(this->m_iWidth,this->m_iHeight,m_iPixelFormat == GL_RGBA?4:3);
			Destroy();
		}
	}
	//===============
	//
	//===============
	bool	cSimpleGLTexture::LoadImage(const char*e_strImageFileName,bool e_bFetchPixelData)
	{
		assert(e_strImageFileName);
		Destroy();
		char*	l_strImageName = UT::GetFileNameWithoutFullPath(e_strImageFileName);
		wchar_t*	l_strImageNameW = CharToWchar(l_strImageName);
		//is loading the same image again?
		if( this->IsSameName(l_strImageNameW) )
			return false;
		//delete old one if existed
		Destroy();
		//asign new name
		this->SetName(l_strImageNameW);
	#ifndef __IPHONE__
		ILuint l_uiDevilTexID;
		ilGenImages(1, &l_uiDevilTexID); /* Generation of one image name */
		ilBindImage(l_uiDevilTexID); /* Binding of image name */
		//if the error is ocurr ensure ilInit(); is called
	#ifdef WIN32
		ILboolean	l_bSuccess = ilLoadImage((WCHAR*)(e_strImageFileName)); /* Loading of image "image.jpg" */
		if( !l_bSuccess )
			l_bSuccess = ilLoadImage(UT::CharToWchar(e_strImageFileName)); /* Loading of image "image.jpg" */
	#else
		ILboolean	l_bSuccess = ilLoadImage(e_strImageFileName); /* Loading of image "image.jpg" */
	#endif
		if(!l_bSuccess)
		{
			ILenum   l_ErrorIF = ilGetError();
			while( l_ErrorIF != IL_NO_ERROR )
			{
				//http://www-f9.ijs.si/~matevz/docs/DevIL/il/f00032.htm
				//http://openil.sourceforge.net/tuts/tut_3/index.htm
				char	l_str[TEMP_SIZE];
				sprintf(l_str,"ErrorID:%d\0,wathc glLoadImage.cpp top.",l_ErrorIF);
				UT::ErrorMsg(e_strImageFileName,l_str);
				l_ErrorIF = ilGetError();
			}
		}
		else
		{
			ILenum	l_ILenum = ilGetInteger(IL_IMAGE_BPP)==3?IL_RGB:IL_RGBA;
			l_bSuccess = ilConvertImage(l_ILenum, IL_UNSIGNED_BYTE); /* Convert every colour component into
			unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
			if( !l_bSuccess )
				assert(0&&"convert color error");
			glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
			assert(m_uiImageIndex!=0&&"opengl init???");
			GLenum	l_GLenum = glGetError();
#ifdef 	OPENGLES_2_X
			glActiveTexture( GL_TEXTURE0 );
#endif
			m_suiLastUsingImageIndex = -1;
			this->ApplyImage();
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, g_fMAG_FILTERValue ); /* We will use linear  interpolation for magnification filter */
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, g_fMIN_FILTERValue ); /* We will use linear  interpolation for minifying filter */
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear  interpolation for magnification filter */
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear  interpolation for minifying filter */
		    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			m_iWidth = ilGetInteger(IL_IMAGE_WIDTH);
			m_iHeight = ilGetInteger(IL_IMAGE_HEIGHT);
			//GL_RGB,GL_RGBA.
			int	l_iNumChannel = ilGetInteger(IL_IMAGE_BPP);
			if( l_iNumChannel == 4 )
				m_iPixelFormat = GL_RGBA;
			else
			if( l_iNumChannel == 3 )
				m_iPixelFormat = GL_RGB;
			else
			{
				UT::ErrorMsg("do not support such format!!","l_iNumChannel");
			}
			//quick way to find out power of two.
			//if (width & (width - 1)) or (height & (height - 1)):
			int	l_iWidthPO2 = power_of_two(m_iWidth);
			int	l_iHeightPO2 = power_of_two(m_iHeight);
			GLint texSize; glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
			//make sure power of 2,because not every fukcing graphic card support it
			//but if u exactly sure it do support power of 2 u could mark this.
			if( !g_bSupportNonPowerOfTwoTexture&&(l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight) )
			{
				assert((l_iWidthPO2<=texSize||l_iHeightPO2<=texSize)&&"texture size is too big then card support");
	#ifdef WIN32
	#ifdef _DEBUG
				OutputDebugString(l_strImageNameW);
				OutputDebugString(L"  image has not correct respond width and height,because none power of 2\n");
				if( m_iPixelFormat == GL_RGB )
					OutputDebugString(L"UV is changed,because image size is not to become power of 2");
	#endif
	#endif
				char*l_pNewPixelData = TextureToPowOfTwo((char*)ilGetData(),m_iWidth,m_iHeight,l_iNumChannel==4?true:false);
				glTexImage2D(GL_TEXTURE_2D, 0, l_iNumChannel, l_iWidthPO2,
					l_iHeightPO2, 0,l_iNumChannel==4?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE,l_pNewPixelData); // Texture specification.	
				GLenum	l_error = glGetError();
				//assign new UV
				m_fUV[2] = (float)m_iWidth/l_iWidthPO2;
				m_fUV[3] =  (float)m_iHeight/l_iHeightPO2;
				if( e_bFetchPixelData )
				{//because we wanna to detected the pixels alpha for collide so we have to resize
					m_pPixels = (unsigned char*)l_pNewPixelData;
					//make m_iWidth and height to power of 2
					//m_iWidth = l_iWidthPO2;//although the texture size has been changed,but the target render size we need to hold original
					//m_iHeight = l_iHeightPO2;
				}
				else
				{//we do not need the alpha so modify UV to show the part we needed
					delete l_pNewPixelData;
				}
			}
			else
			{
				assert((m_iWidth<=texSize||m_iHeight<=texSize)&&"texture size is too big then card support");
				glTexImage2D(GL_TEXTURE_2D, 0, l_iNumChannel, m_iWidth,
				  m_iHeight, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,ilGetData()); // Texture specification.
				if( e_bFetchPixelData )
				{
					int	l_iDataSize = m_iWidth*m_iHeight*(GL_RGB==m_iPixelFormat?3:4);
					m_pPixels = (unsigned char*)malloc(l_iDataSize);
					memcpy(m_pPixels,ilGetData(),sizeof(char)*l_iDataSize);
				}
			}
			ilDeleteImages(1, &l_uiDevilTexID); // Because we have already copied image data into texture data
		}
	#else
		m_iPixelFormat = GL_RGBA;//although it's stupid,but all image format is png.
		CGImageRef spriteImage;
		CGContextRef spriteContext;
		GLubyte *spriteData;
		//in the iPhone image does't need to add bundle directory.
		NSString*l_strTextureName = [[NSString alloc]initWithUTF8String:e_strImageFileName];
		spriteImage = [UIImage imageNamed:l_strTextureName].CGImage;
		assert(spriteImage);
		[l_strTextureName release];
		// Get the width and height of the image
		m_iWidth = CGImageGetWidth(spriteImage);
		m_iHeight = CGImageGetHeight(spriteImage);
		if(spriteImage)
		{
			// Allocated memory needed for the bitmap context
			spriteData = (GLubyte *) malloc(m_iWidth * m_iHeight * 4);
			// Uses the bitmatp creation function provided by the Core Graphics framework. 
			spriteContext = CGBitmapContextCreate(spriteData, m_iWidth, m_iHeight, 8, m_iWidth * 4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
			// After you create the context, you can draw the sprite image to the context.
			CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)m_iWidth, (CGFloat)m_iHeight), spriteImage);
			// You don't need the context at this point, so you need to release it to avoid memory leaks.
			CGContextRelease(spriteContext);
			//
			glGenTextures(1, &m_uiImageIndex); /* Texture name generation */
			//		GLenum	l_GLenum = glGetError();
			//		assert(l_GLenum);
#ifdef 	OPENGLES_2_X
			glActiveTexture( GL_TEXTURE0  );
#endif
			m_suiLastUsingImageIndex = -1;
			this->ApplyImage();
			// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);				
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear  interpolation for magnification filter */
		    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			int	l_iWidthPO2 = power_of_two(m_iWidth);
			int	l_iHeightPO2 = power_of_two(m_iHeight);
			if( l_iWidthPO2!=m_iWidth||l_iHeightPO2!=m_iHeight )//make it power of two
			{
				char*l_pNewPixelData = TextureToPowOfTwo((char*)spriteData,m_iWidth,m_iHeight,true);
				glTexImage2D(GL_TEXTURE_2D, 0, m_iPixelFormat, l_iWidthPO2,l_iHeightPO2, 0,GL_RGBA, GL_UNSIGNED_BYTE,l_pNewPixelData); // Texture specification.	
				//assign new UV
				m_fUV[2] = (float)m_iWidth/(float)l_iWidthPO2;
				m_fUV[3] =  (float)m_iHeight/(float)l_iHeightPO2;		
				if( e_bFetchPixelData )
				{//because we wanna to detected the pixels alpha for collide so we have to resize
					m_pPixels = (unsigned char*)l_pNewPixelData;
				}
				else
				{//we do not need the alpha so modify UV to show the part we needed
					delete l_pNewPixelData;
				}		
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth,
							 m_iHeight, 0,m_iPixelFormat, GL_UNSIGNED_BYTE,spriteData); // Texture specification.
				if( e_bFetchPixelData )
				{
					int	l_iDataSize = m_iWidth*m_iHeight*4;
					m_pPixels = (unsigned char*)malloc(l_iDataSize);
					memcpy(m_pPixels,spriteData,sizeof(char)*l_iDataSize);
				}		
			}
			//
			free(spriteData);
		}
	#endif
		//if loading failed show alert
		if(!m_uiImageIndex)
		{
			ErrorMsg(L"opengl process Image Failed",UT::CharToWchar(e_strImageFileName));
			return false;
		}
		VideoMemoryAddCheck(this->m_iWidth,this->m_iHeight,m_iPixelFormat == GL_RGBA?4:3,e_strImageFileName);
		return true;
	}
	void	cSimpleGLTexture::ApplyImage()
	{
	    if( m_suiLastUsingImageIndex != m_uiImageIndex )
	    {
		    glBindTexture( GL_TEXTURE_2D, m_uiImageIndex);
		    m_suiLastUsingImageIndex = m_uiImageIndex;
		}
	}
	//===============
	//
	//===============
	void	cSimpleGLTexture::Destroy()
	{
		if( m_uiImageIndex )
		{
			glDeleteTextures(1,&m_uiImageIndex);
			m_uiImageIndex = 0;
			if(m_pPixels)//if the pixels data is not from resource,free it
				free(m_pPixels);
		}
	}

	//===============
	//
	//===============
    bool    cSimpleGLTextureWithcTexData::CollideTexture(int e_iX,int e_iY,bool e_bTestAlphaChannel,Vector4*e_pvCollidedColor)
	{
		e_iY = (int)(e_iY-this->m_vPos.y);
        e_iX = (int)(e_iX-this->m_vPos.x);
		if( e_iX<m_OffsetPos.x||e_iY<m_OffsetPos.y||m_OffsetPos.x+m_iShowWidth<e_iX||m_OffsetPos.y+m_iShowHeight<e_iY )
			return false;
        if( !e_bTestAlphaChannel || !m_pPixels )
            return true;
        int l_iLocalPosX = e_iX-m_OffsetPos.x;
        int l_iLocalPosY = e_iY-m_OffsetPos.y;
        unsigned char*l_pPixels = m_pPixels;
        assert(m_iPixelFormat == GL_RGBA);
        //m_iWidth to get original size
        int l_iPixelsID = (l_iLocalPosX+(l_iLocalPosY*this->m_iWidth))*4;
        //+3 for alpha
#ifdef _DEBUG
        unsigned char   l_Color[4] = { l_pPixels[l_iPixelsID],l_pPixels[l_iPixelsID+1],l_pPixels[l_iPixelsID+2],l_pPixels[l_iPixelsID+3] };
#endif
        if(l_pPixels[l_iPixelsID+3] != 0)
		{
			unsigned char   l_Color[4] = { l_pPixels[l_iPixelsID],l_pPixels[l_iPixelsID+1],l_pPixels[l_iPixelsID+2],l_pPixels[l_iPixelsID+3] };
			Vector4 e_vColor(l_Color[0]/255.f,l_Color[1]/255.f,l_Color[2]/255.f,l_Color[3]/255.f);
			if( e_pvCollidedColor )
				*e_pvCollidedColor = e_vColor;
            return true;
		}
        return false;
	}
	//==================
	//move point to image's local position
	//now set image;s center as 0,0,so set localX and Localy tp correspond position(width for minus half image)
	//
	//==================
    bool    cSimpleGLTextureWithcTexData::CollideTextureWithTransform(int e_iX,int e_iY,bool e_bTestAlphaChannel)
    {
		//fuck result is wrong.
        int l_iLocalY = (int)(e_iY-this->m_vPos.y);
        int l_iLocalX = (int)(e_iX-this->m_vPos.x);
		cMatrix44	l_matrix = cMatrix44::Identity;
		Vector3 l_vPos = Vector3(l_iLocalX-this->m_iShowWidth/2.f,l_iLocalY-this->m_iShowHeight/2.f,0);
		l_matrix = l_matrix.ZAxisRotationMatrix(MyMath::DegToRad(this->m_fAngle));
		Vector3 l_vFinalPos = l_matrix.TransformVector(l_vPos);
		l_iLocalY = (int)l_vFinalPos.y+this->m_iShowHeight/2;
		l_iLocalX = (int)l_vFinalPos.x+this->m_iShowWidth/2;
		if( l_iLocalX<0||l_iLocalY<0 )
			return false;
		if( l_iLocalX>this->m_iShowWidth||l_iLocalY>this->m_iShowHeight )
			return false;
		//it might be a PuzzleImageUnit so get correct width size
		int l_iActulWidthSize = m_iWidth;
		//fuck.....
        //if( !g_bSupportNonPowerOfTwoTexture && this->GetOwner() )
        //{
          //  l_iActulWidthSize = UT::power_of_two(m_iWidth);
        //}
		int	l_iIndex = l_iLocalY*l_iActulWidthSize+l_iLocalX;
		if( l_iIndex<0 )
			return false;
		if( !e_bTestAlphaChannel )
        {
            RECT    l_rc = {this->m_OffsetPos.x,this->m_OffsetPos.y,this->m_iShowWidth,this->m_iShowHeight};
            if(UT::RectCollidePoint(l_rc,l_iLocalX,l_iLocalY))
                return true;
            return false;
        }
		unsigned char l_Alpha = this->m_pPixels[l_iIndex*4];//argb
		//unsigned char l_Alpha = this->m_pPixels[l_iIndex*4+3];//rgba
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

	void	cSimpleGLTextureWithcTexData::RenderRect()
	{
		GLRender::RenderRectangle(Vector2(m_vPos.x,m_vPos.y),(float)this->m_iShowWidth,(float)this->m_iShowHeight,Vector4::One,this->m_fAngle);
	}

	void	cSimpleGLTextureWithcTexData::SetMirror(bool e_bMirror)
	{
		if(e_bMirror == m_bMirror)
			return;
		m_bMirror = e_bMirror;
		float	l_fX[2] = {m_fUV[0],m_fUV[2]};
		m_fUV[0] = l_fX[1];
		m_fUV[2] = l_fX[0];
	}
}