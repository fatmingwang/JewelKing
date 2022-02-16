#include "../stdafx.h"
#include "XMLLoader.h"
#include "../Utility.h"
#include "../Image/GlyphFontRender.h"
#include "../GameplayUT/GameApp.h"
#include "../GameplayUT/BinaryFile.h"
#ifdef __IPHONE__
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIImage.h>
#endif
namespace FATMING_CORE
{
	//const wchar_t*     ISAXCallback::TypeID( L"ISAXCallback" );
	//const wchar_t*     cNodeISAX::TypeID( L"cNodeISAX" );

	const char*ISAXCallback::ExtensionNameID( "Fuck" );

	LoadingProgress	g_pLoadingProgressCallBack = 0;

	#ifdef __IPHONE__
	//because iphone using bundle resource,so the resource store in its specific path.
	void GetAppleBundelResourcePathByObjectPath( const char*e_strSrc,char*e_strDest)
	{
		char	l_strFileName[MAX_PATH];
		sprintf(l_strFileName,"%s\0",UT::GetFileNameWithoutFullPath(e_strSrc,false));
		NSString* l_str = 0;
		NSString*l_NSDirectory = 0;
		NSString*l_strName = [[NSString alloc]initWithUTF8String:l_strFileName];
        char	l_strDirectory[MAX_PATH];
		char*l_strDirectoryName = UT::GetDirectoryWithoutFileName(e_strSrc);
		if( l_strDirectoryName )
		{
		    sprintf(l_strDirectory,"%s\0",l_strDirectoryName);
		    l_NSDirectory = [[NSString alloc]initWithUTF8String:l_strDirectory];
		}
		l_str = [[NSBundle mainBundle] pathForResource:l_strName ofType:nil inDirectory:l_NSDirectory ];
		//ensure u add the right type in the XCode
		const char* datechar = [l_str UTF8String];
		assert(e_strDest);
		sprintf(e_strDest,"%s\0",datechar);
	}
	void GetIphoneAppFilePath(char* o_buffer, char* ifilename)
	{
		NSArray *Paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *documentsDirectory = [Paths objectAtIndex:0];
		sprintf(o_buffer, "%s/%s", [documentsDirectory UTF8String], ifilename);
	}
	#endif

	ISAXCallback::ISAXCallback(bool e_bSetRunBack)
	{
		m_pCurrentTiXmlElement = 0;
		m_strFileName[0] = '\0';
		m_strCurrentDirectory[0] = '\0';
		m_bSetRunBack = e_bSetRunBack;
		m_bShowErrorMsg = false;
		m_pDoc = 0;
		#ifdef WIN32
			m_bShowErrorMsg = true; 
		#else
		#ifdef _DEBUG
			m_bShowErrorMsg = true;
		#endif
		#endif
	}
	
	char*	ISAXCallback::StringAddDirectory(const char*e_strValue)
	{
		return	StringAddDirectory((char*)e_strValue);
	}

	char*	ISAXCallback::StringAddDirectory(char*e_strValue)
	{
		static char	l_strValue[TEMP_SIZE];
		sprintf(l_strValue,"%s%s\0",m_strCurrentDirectory,e_strValue);
		return l_strValue;
	}

	TiXmlElement*	ISAXCallback::LoadingXML(const char*e_strFileName)
	{
	    SAFE_DELETE(m_pDoc);
		m_strErrorMsg.clear();
		sprintf(m_strFileName,"%s\0",e_strFileName);
		//this->SetName(UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strFileName,false)));
#ifdef __IPHONE__
		char	l_strFileName[MAX_PATH];
		GetAppleBundelResourcePathByObjectPath(e_strFileName,l_strFileName);
		m_pDoc = new TiXmlDocument(UT::CharToWchar(datechar));
#else
		m_pDoc = new TiXmlDocument( UT::CharToWchar(e_strFileName) );
#endif
		bool loadOkay = m_pDoc->LoadFile();
		if(!loadOkay)
		{
		    SAFE_DELETE(m_pDoc);
			m_strErrorMsg = L"load file error!utf8?!!!fuck\n";
			std::string	l_str = UT::ComposeMsgByFormat("file %s open failed! uft8 or file is not exist!?",e_strFileName);
			if( cGameApp::m_spLogFile )
				cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
#ifdef WIN32
			OutputDebugString(L"encode error or file not exist or xml format error??");
#endif
				return 0;
		}
		TiXmlNode* l_pFirstNode = m_pDoc->FirstChild();
		TiXmlElement* itemElement = 0;
		itemElement = l_pFirstNode->ToElement();
		while( !itemElement )
		{
			itemElement = l_pFirstNode->NextSibling()->ToElement();
			l_pFirstNode = l_pFirstNode->NextSibling();
			if( !l_pFirstNode )
			{
				m_strErrorMsg += L"there is no FirstNode\n";
				return 0;
			}
		}
		assert(itemElement&&"empty element,check name for none numeral");
//iphone does't need to translate it because all file have to translate by loader api...
		char*   l_pDirectory =  GetDirectoryWithoutFileName(e_strFileName);
		if( l_pDirectory )
			sprintf(m_strCurrentDirectory,"%s\0",l_pDirectory);
#ifdef WIN32
#ifdef _DEBUG
//	OutputDebugString(UT::CharToWchar(e_strFileName));
//	OutputDebugString(L"	Parsing Start\n");
#endif
#endif
		InternalParse();
		return itemElement;
	}

	bool    ISAXCallback::Parse(const char*e_strFileName,bool e_bShowLoadingState)
	{
		m_strErrorMsg.clear();
		TiXmlElement*l_pRootElement = ISAXCallback::LoadingXML((char*)e_strFileName);
		if( !l_pRootElement )
			return false;
		if(cGameApp::m_spstrErrorMsgString)
		{
			if(cGameApp::m_spstrErrorMsgString->length())
			{
				UT::ErrorMsg(cGameApp::m_spstrErrorMsgString->c_str(),L"before parse xml file there are error message not clear...");
			}
			cGameApp::m_spstrErrorMsgString->clear();
		}
		bool	l_bErrorMsgStringExist = FATMING_CORE::cGameApp::m_spstrErrorMsgString?true:false;
		bool	l_bWhoErrorMsg = FATMING_CORE::cGameApp::m_sbShowErrorMsgBox;
		FATMING_CORE::cGameApp::m_sbShowErrorMsgBox = false;
		if(!l_bErrorMsgStringExist)
		{
			FATMING_CORE::cGameApp::m_spstrErrorMsgString = new std::wstring();
		}
		m_bShowLoadingState = e_bShowLoadingState;
		try
		{
			ParserAllElement(l_pRootElement);
		}
		catch(...)
		{
			if(m_bShowErrorMsg&&wcslen(m_strErrorMsg.c_str()))
			{
				UT::ErrorMsg((WCHAR*)this->m_strErrorMsg.c_str(),L"Error message");
			}		
		}
		if(m_bShowErrorMsg&&wcslen(m_strErrorMsg.c_str()))
		{
			UT::ErrorMsg((WCHAR*)this->m_strErrorMsg.c_str(),L"Error message");
		}
		SAFE_DELETE(m_pDoc);
		if( FATMING_CORE::cGameApp::m_spstrErrorMsgString->length() )
		{
			FATMING_CORE::cGameApp::m_sbShowErrorMsgBox = true;
			UT::ErrorMsg((WCHAR*)FATMING_CORE::cGameApp::m_spstrErrorMsgString->c_str(),L"Error message");
			FATMING_CORE::cGameApp::m_spstrErrorMsgString->clear();
		}
		if(!l_bErrorMsgStringExist)
		{
			SAFE_DELETE(FATMING_CORE::cGameApp::m_spstrErrorMsgString);
		}
		FATMING_CORE::cGameApp::m_sbShowErrorMsgBox = l_bWhoErrorMsg;
		return true;
	}

	bool	ISAXCallback::Export(const WCHAR*e_strFileName)
	{
		if(m_pDoc)
		{
			return m_pDoc->SaveFile(UT::WcharToChar(e_strFileName));
		}
		return false;
	}

	bool	ISAXCallback::Export(const char*e_strFileName)
	{
		if(m_pDoc)
		{
			return m_pDoc->SaveFile(e_strFileName);
		}
		return false;
	}

	void    XMLHandleElementDataDebugInfo(TiXmlElement*e_pTiXmlElement)
	{
	#ifdef WIN32
	#ifdef _DEBUG
			const WCHAR*     l_str = e_pTiXmlElement->Value();
			TiXmlAttribute*l_pAttribute = e_pTiXmlElement->FirstAttribute();
			if(l_pAttribute)
			{
					const WCHAR*     l_Test;
					const WCHAR*     l_strAttribute = 0;
					while( l_pAttribute )
					{
							l_Test = l_pAttribute->Value();
							l_strAttribute = l_pAttribute->Name();
							if(l_Test)
							{
	                                OutputDebugString(L">>");
	                                OutputDebugString((l_strAttribute));
							}
							l_pAttribute = l_pAttribute->Next();
					}
			}
	        OutputDebugString(L"\n");
	#endif
	#endif
	}

	TiXmlElement*   g_pTiXmlElement;
	TiXmlElement*   ISAXCallback::ParserAllElement(TiXmlElement*e_pTiXmlElement)
	{
			if(this->m_bShowLoadingState&&cGameApp::m_spGlyphFontRender)
			{
				if(g_pLoadingProgressCallBack)
					g_pLoadingProgressCallBack();
			}
	#ifdef WIN32
	#ifdef _DEBUG
			g_pTiXmlElement = e_pTiXmlElement;
			//while(g_pTiXmlElement->Parent())
			//{
			//		g_pTiXmlElement = (TiXmlElement*)g_pTiXmlElement->Parent();
	  //              OutputDebugString(L"    ");
			//}
	        //OutputDebugString(e_pTiXmlElement->Value());
			//XMLHandleElementDataDebugInfo(e_pTiXmlElement);
	#endif
	#endif
			//1st all chilren       element
			//2nd all nextslibing   element
			m_pCurrentTiXmlElement = e_pTiXmlElement;
			HandleElementData(e_pTiXmlElement);
			if( e_pTiXmlElement->FirstChildElement() )
			{
					ParserAllElement( e_pTiXmlElement->FirstChildElement());
			}
			//go back
			m_pCurrentTiXmlElement = e_pTiXmlElement;
			if( m_bSetRunBack )
			{
				if( !e_pTiXmlElement->m_bDone )
				{
	#ifdef WIN32
	#ifdef _DEBUG
			g_pTiXmlElement = e_pTiXmlElement;
			while(g_pTiXmlElement->Parent())
			{
					g_pTiXmlElement = (TiXmlElement*)g_pTiXmlElement->Parent();
	//                OutputDebugString(L"    ");
			}
	//        OutputDebugString(UT::CharToWchar(e_pTiXmlElement->Value()));
	//		OutputDebugString(L"__End");
	//		OutputDebugString(L"\n");
	#endif
	#endif
					e_pTiXmlElement->m_bDone = true;
					HandleElementData(e_pTiXmlElement);
				}
			}
			if( e_pTiXmlElement->NextSiblingElement() )
			{
					ParserAllElement( e_pTiXmlElement->NextSiblingElement());  
			}
			return e_pTiXmlElement;
	}

	//int	g_iLevel = 0;
	//void	sXMLNode::ShowDebugInfo(sXMLNode *e_XMLNode)
	//{
	//#ifdef	WIN32
	//	for( int i=0;i<g_iLevel;++i )
	//		OutputDebugString(L"	");
	//	OutputDebugString(L"TagName:");
	//	OutputDebugString(UT::CharToWchar(e_XMLNode->strTagName));
	//	OutputDebugString(L">>");
	//	for( UINT i=0;i<e_XMLNode->AttributeList.size();++i )
	//	{
	//		OutputDebugString(UT::CharToWchar(e_XMLNode->AttributeList[i].strAttribute));
	//		OutputDebugString(L">>");
	//	}
	//	OutputDebugString(L"\n");
	//	for( UINT i=0;i<e_XMLNode->ChildNodeList.size();++i )
	//		ShowDebugInfo(e_XMLNode->ChildNodeList[i]);
	//	for( UINT i=0;i<e_XMLNode->SiblingNodeList.size();++i )
	//		ShowDebugInfo(e_XMLNode->SiblingNodeList[i]);
	//#else
	//	printf(L"TagName:");
	//	printf(UT::CharToWchar(e_XMLNode.strTagName.c_str()));
	//	for( UINT i=0;i<e_XMLNode.AttributeList.size();++i )
	//	{
	//		printf(L"Attribute:");
	//		printf(UT::CharToWchar(e_XMLNode.AttributeList[i].strAttribute.c_str()));
	//		OutputDebugString(L",");
	//	}
	//	printf(L"\n");
	//	for( UINT i=0;i<e_XMLNode.ChildNodeList.size();++i )
	//		ShowDebugInfo(e_XMLNode.ChildNodeList[i]);
	//	for( UINT i=0;i<e_XMLNode.SiblingNodeList.size();++i )
	//		ShowDebugInfo(e_XMLNode.SiblingNodeList[i]);	
	//#endif
	//}

	TiXmlElement*	SearchTiXmlElement(TiXmlElement* e_pNode,void* e_pPointer,TiXmlElement*(*callback)( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer ) )
	{
		TiXmlElement*	l_pTiXmlElement = (*callback)(e_pNode,e_pPointer);
		if( l_pTiXmlElement )
			return l_pTiXmlElement;
		if( e_pNode->NextSiblingElement() != NULL)
		{		
			return SearchTiXmlElement( e_pNode->NextSiblingElement(),e_pPointer,callback);
		}
		if (e_pNode->FirstChildElement() != NULL)
		{
			return SearchTiXmlElement( e_pNode->FirstChildElement(),e_pPointer,callback);
		}

		return 0;
	}

	TiXmlElement	*FindElementByName( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer )
	{
		WCHAR*	e_strName = (WCHAR*)e_pcallbackPointer;
		const WCHAR*l_strNodeNAme = e_pCallbackFrame->Value();
		if( !wcscmp(l_strNodeNAme,e_strName) )
			return e_pCallbackFrame;
		return 0;
	}

	cNodeISAX::cNodeISAX()
	{
		m_pRootElement = 0;
	}

	cNodeISAX::~cNodeISAX()
	{

	}
	bool	cNodeISAX::ParseDataIntoXMLNode(const char*e_strFileName)
	{
		SystemErrorCheck();
		m_strErrorMsg.c_str();
		SAFE_DELETE(m_pDoc);
	#ifdef __IPHONE__
		char	l_strFileName[MAX_PATH];
		GetAppleBundelResourcePathByObjectPath(e_strFileName,l_strFileName);
		m_pDoc = new TiXmlDocument(UT::CharToWchar(l_strFileName));
	#else
		m_pDoc = new TiXmlDocument(UT::CharToWchar(e_strFileName));
	#endif
		bool loadOkay = m_pDoc->LoadFile();
		if(!loadOkay)
		{
			SAFE_DELETE(m_pDoc);
			m_strErrorMsg += L"Open file falied ";
			m_strErrorMsg += UT::CharToWchar(e_strFileName);
			return false;
		}
	//iphone does't need to translate it because all file have to translate by loader api...
		char*   l_pDirectory =  GetDirectoryWithoutFileName(e_strFileName);
		if( l_pDirectory )
			sprintf(m_strCurrentDirectory,"%s\0",l_pDirectory);
		TiXmlNode* node = 0;
		node  = m_pDoc->FirstChild();
		m_pRootElement = node->ToElement();
		if( !m_pRootElement )
			m_pRootElement = m_pDoc->FirstChild()->NextSibling()->ToElement();
		assert(m_pRootElement&&"empty element,check name for none numeral");
		SystemErrorCheck();
		return true;
	}

	bool	cNodeISAX::ParseWithMyParse(const char*e_strFileName,bool e_bDeleteXLDocument)
	{
		m_strErrorMsg.c_str();
	    SAFE_DELETE(m_pDoc);
		TiXmlElement*l_pRootElement = ISAXCallback::LoadingXML(e_strFileName);
		this->m_pRootElement = l_pRootElement;
		if( !l_pRootElement )
			return false;
		bool    l_b = MyParse(l_pRootElement);
		if( e_bDeleteXLDocument )
		    SAFE_DELETE(m_pDoc);
		return l_b;
	}

	TiXmlElement*	cNodeISAX::GetChildOfRootXmlElementByName( const WCHAR*e_pString )
	{
		TiXmlElement*l_pTiXmlElement = m_pRootElement->FirstChild()->ToElement();
		while(l_pTiXmlElement)
		{
			if(!wcscmp(l_pTiXmlElement->Value(),e_pString))
			{
				return l_pTiXmlElement;
			}
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
		}
		return 0;
	}
	
    TiXmlElement*	cNodeISAX::GetChildOfRootXmlElementByAttribueName( const WCHAR*e_pElementName,const WCHAR*e_strAttributeKeyWord,const WCHAR*e_strAttributeName )
    {
		if( m_pRootElement )
		{
			TiXmlElement*l_pTiXmlElement = m_pRootElement->FirstChildElement();
			while(l_pTiXmlElement)
			{
				if(!wcscmp(l_pTiXmlElement->Value(),e_pElementName))
				{
					const WCHAR*l_strAttributeKeyWord = l_pTiXmlElement->Attribute(e_strAttributeName);
					if( l_strAttributeKeyWord )
					{
						if( !wcscmp(l_strAttributeKeyWord,e_strAttributeKeyWord) )
							return l_pTiXmlElement;
					}
				}
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			}
		}
		return 0;    
    }

	TiXmlElement*	cNodeISAX::GetRootChildElementByIndex( int e_iIndex )
	{
		if( m_pRootElement )
		{
			int	l_iIndex = 0;
			TiXmlElement*l_pTiXmlElement = m_pRootElement->FirstChildElement();
			while(l_pTiXmlElement)
			{
				if( l_iIndex == e_iIndex )
					return l_pTiXmlElement;
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
				++l_iIndex;

			}
		}
		return 0;
	}

	int		cNodeISAX::RootChildrenCount()
	{
		if( m_pRootElement )
		{
			int	l_iIndex = 0;
			TiXmlElement*l_pTiXmlElement = m_pRootElement->FirstChildElement();
			while(l_pTiXmlElement)
			{
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
				++l_iIndex;
			}
			return l_iIndex;
		}
		return 0;	
	}
}