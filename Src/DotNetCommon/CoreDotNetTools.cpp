#include "stdAfx.h"
#include "CoreDotNetTools.h"
#include "../../Include/IL/il.h"
namespace DNCT
{
	System::Drawing::Bitmap^OpenImageFile(System::String^e_strFileName)
	{
		System::Drawing::Bitmap^l_pppp;
		ILuint s_iImageID;


		System::Drawing::Bitmap^l_pImage;
		String^l_strExtensionName = System::IO::Path::GetExtension(e_strFileName);
		if( l_strExtensionName->Equals(".dds")||
			l_strExtensionName->Equals(".DDS"))
		{
			ilGenImages(1, &s_iImageID);
			ilBindImage(s_iImageID);

			if (0==ilLoadImage((WCHAR*)DNCT::GcStringToChar(e_strFileName)))
			{
				return l_pppp;
			}
			int iW = ilGetInteger(IL_IMAGE_WIDTH);
			int iH = ilGetInteger(IL_IMAGE_HEIGHT);
			bool bFlip = ilGetInteger(IL_IMAGE_ORIGIN) == IL_ORIGIN_LOWER_LEFT;

			bool bResize = false;
			System::Drawing::Bitmap^ pBmp = gcnew System::Drawing::Bitmap(iW, iH, System::Drawing::Imaging::PixelFormat::Format32bppArgb);				

			System::Drawing::Rectangle rect; rect.X = 0; rect.Y = 0; rect.Width = iW; rect.Height = iH;

			System::Drawing::Imaging::BitmapData^ pBd = pBmp->LockBits(rect, 
				System::Drawing::Imaging::ImageLockMode::WriteOnly,
				System::Drawing::Imaging::PixelFormat::Format32bppArgb);

			void* pScan0 = (void*)(pBd->Scan0);
			ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE); // support for non 32bit images..
			//windows if BGRA
			//ilCopyPixels(0, 0, 0, iW, iH, 1, IL_BGRA, IL_UNSIGNED_BYTE, pScan0);
			//
			ilCopyPixels(0, 0, 0, iW, iH, 1, IL_RGBA, IL_UNSIGNED_BYTE, pScan0);
			
			ilDeleteImages(1, &s_iImageID);

			pBmp->UnlockBits(pBd);

			if (bFlip) pBmp->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);


			return pBmp;
		}
		else
		{
			l_pImage = gcnew System::Drawing::Bitmap(e_strFileName);
		}
		return l_pImage;	
	}
};