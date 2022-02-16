#ifndef _CORE_DOTNET_TOOLS_H_
#define _CORE_DOTNET_TOOLS_H_
#include "../Core/AllCoreInclude.h"
#include "DotNetCommonTools.h"
//ensure u have include AllFeelingLihInclude.h
//and it under common runtime environment

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
//namespace CDNT
namespace DNCT
{
	inline	System::Windows::Forms::TreeNode^	PIToTreeNode(cPuzzleImage*e_pPI)
	{
        System::Windows::Forms::TreeNode^l_pTreeNode = gcnew System::Windows::Forms::TreeNode(String(e_pPI->GetCharName()).ToString());
        l_pTreeNode->Name = String(e_pPI->GetCharName()).ToString();
        for( int i=0;i<e_pPI->Count();++i )
        {
			l_pTreeNode->Nodes->Add(String((*e_pPI)[i]->GetCharName()).ToString())->Name = String((*e_pPI)[i]->GetCharName()).ToString();
        }
        return l_pTreeNode;
	}
	//only export name,whatever the type they are.
	inline	void	ImagerParserAddIntiTreeView(cImageParser*e_pImageParser,System::Windows::Forms::TreeView^e_pTreeView,bool e_bClearTreeViewData)
	{
	    if( e_bClearTreeViewData )
	        e_pTreeView->Nodes->Clear();
	    for(int i=0;i<e_pImageParser->Count();++i)
	    {
	        if( (*e_pImageParser)[i]->Type() == cPuzzleImage::TypeID )
	        {
                e_pTreeView->Nodes->Add(PIToTreeNode(dynamic_cast<cPuzzleImage*>((*e_pImageParser)[i])));
	        }
	        else
	        {
	            e_pTreeView->Nodes->Add(String((*e_pImageParser)[i]->GetCharName()).ToString())->Name = String((*e_pImageParser)[i]->GetCharName()).ToString();
	        }
	    }
	}

	inline	bool	RemoveTextureFromSelectedNodesTreeView(System::Windows::Forms::TreeView^e_pTreeView,cImageParser*e_pImageParser)
	{
		if(!e_pTreeView->SelectedNode)
			return false;
		if(!e_pTreeView->SelectedNode->Parent)
		{
			 if( !e_pImageParser->GetObject(DNCT::GcStringToWchar(e_pTreeView->SelectedNode->Text)) )
			 {
				WARNING_MSG("there is no such texture in the image parser!!");
				return false;
			 }
			 else
				return e_pImageParser->RemoveObject(DNCT::GcStringToWchar(e_pTreeView->SelectedNode->Text));
		}
		else
		{
			 WARNING_MSG("please select pi root");
			 return false;
		}
	}

	template<class T>inline	void DumpObjectListIntoListBox(System::Windows::Forms::ListBox^e_pListbox,T e_pList,bool e_bClearListBoxData = false,bool e_bAddIndex = true)
	{
		if( e_bClearListBoxData )
			e_pListbox->Items->Clear();
		for( int i=0;i<e_pList->Count();++i )
		{
		    if(e_bAddIndex)
			    e_pListbox->Items->Add(i.ToString()+":"+DNCT::WcharToGcstring((*e_pList)[i]->GetName()));
			else
			    e_pListbox->Items->Add(DNCT::WcharToGcstring((*e_pList)[i]->GetName()));
			
		}
	}

	inline	TiXmlElement*TreeNodeToTinyXMLNode(GCFORM::TreeNode^e_pNode,TiXmlElement*e_pParent)
	{
		TiXmlElement*l_pNewNode = new TiXmlElement(DNCT::GcStringToWchar(e_pNode->Text));
		if( e_pParent )
			e_pParent->LinkEndChild(l_pNewNode);
		TiXmlElement*l_pPreviousChildNode = 0;
		for each( GCFORM::TreeNode^l_pChildNode in  e_pNode->Nodes )
		{
			//it's comment
			if(l_pChildNode->Text->Equals(XML_COMMENT))
			{
				continue;
			}
			wstring	l_strName = DNCT::GcStringToWchar(e_pNode->Name);
			//it's element
			if( l_pChildNode->Text->Length == 0 )
			{
				TiXmlElement*l_pNewChildNode = TreeNodeToTinyXMLNode(l_pChildNode,l_pNewNode);
				if( !l_pPreviousChildNode )
				{
					l_pNewNode->LinkEndChild(l_pNewChildNode);
				}
				else
				{
					l_pNewNode->InsertAfterChild(l_pPreviousChildNode,*l_pNewChildNode);
				}
				l_pPreviousChildNode = l_pNewChildNode;
			}
			else//it's attribute
			{
				wstring	l_strText = DNCT::GcStringToWchar(e_pNode->Text);
				l_pNewNode->SetAttribute( l_strName.c_str(),l_strText.c_str() );
			}
			
		}
		return l_pNewNode;
	}

	inline	UINT	OpenglToDotNetColor(UINT32 e_RGBA)
	{
		return e_RGBA;
		UInt32 R = (e_RGBA&0xff000000)>>16;
		UInt32 G = (e_RGBA&0x00ff0000);
		UInt32 B = (e_RGBA&0x0000ff00)<<16;
		UInt32 A = (e_RGBA&0x000000ff);
		return R|G|B|A;	
	}

	System::Drawing::Bitmap^OpenImageFile(System::String^e_strFileName);
	inline	System::Drawing::Bitmap^OpenImageFile(System::String^e_strFileName,cUIImage**e_pUIImage)
	{
		System::Drawing::Bitmap^l_pImage;
		String^l_strExtensionName = System::IO::Path::GetExtension(e_strFileName);
		if( l_strExtensionName->Equals(".dds")||
			l_strExtensionName->Equals(".DDS"))
		{
			assert(*e_pUIImage == 0);
			cUIImage*l_pUIImage = 0;
			l_pUIImage = new cUIImage(DNCT::GcStringToChar(e_strFileName));
			//cSimpleGLTexture	l_SimpleGLTexture(DNCT::GcStringToChar(e_strFileName),true);
			int	l_iChannel = l_pUIImage->GetPixelFormat()==GL_RGB?3:4;
			System::Drawing::Imaging::PixelFormat	l_PizelFormat = l_iChannel==4?System::Drawing::Imaging::PixelFormat::Format32bppArgb:System::Drawing::Imaging::PixelFormat::Format24bppRgb;
			l_pImage = gcnew System::Drawing::Bitmap( l_pUIImage->GetImageWidth(),l_pUIImage->GetImageHeight(),l_PizelFormat);
			System::Drawing::Rectangle l_Rect; l_Rect.X = 0; l_Rect.Y = 0; l_Rect.Width = l_pUIImage->GetImageWidth(); l_Rect.Height = l_pUIImage->GetImageHeight();
			System::Drawing::Imaging::BitmapData^ l_pBd = l_pImage->LockBits(l_Rect,System::Drawing::Imaging::ImageLockMode::WriteOnly,l_PizelFormat);
			void* pScan0 = (void*)(l_pBd->Scan0);
			memcpy(pScan0,l_pUIImage->GetPixels(),sizeof(byte)*l_Rect.Width*l_Rect.Height*l_iChannel);
			if( l_iChannel == 4 )
			{
				//conver to dot net color
				//rgba to abgr
				UInt32* d = (UInt32*)pScan0;
				int l_iSize = l_pBd->Width * l_pBd->Height;
				for (int i = 0; i < l_iSize; i++)
				{
					*d = OpenglToDotNetColor(*d);
					++d;
				}
			}
			l_pImage->UnlockBits(l_pBd);
			POINT	l_OriginalImageSize = {l_pImage->Width,l_pImage->Height};
			l_pUIImage->SetOriginalImageSize(l_OriginalImageSize);
			*e_pUIImage = l_pUIImage;
		}
		else
		{
			l_pImage = gcnew System::Drawing::Bitmap(e_strFileName);
		}
		return l_pImage;
	}
};
using namespace DNCT;
#endif