#ifndef XMLLOADER_H
#define XMLLOADER_H
#include <map>
#ifdef TIXML_USE_STL
	#include <iostream>
	#include <sstream>
	using namespace std;
#else
	#include <stdio.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
	#include <crtdbg.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#endif
//=============================================
//for iphone xml chinese parse
//http://stackoverflow.com/questions/2041238/xml-parsing-problem-in-iphone
//<?xml version="1.0" encoding="gb2312"?>
//NSString stringWithContentsOfURL:@"http://..."
//                         encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_2312_80)
//                            error:NULL];
// (void)parser:(NSXMLParser *)parser parseErrorOccurred:(NSError *)parseError {
//    NSString * errorString = [NSString stringWithFormat:@"Unable to download story feed from web site (Error code %i )", [parseError code]];
//    NSLog(@"error parsing XML: %@", errorString);
//
//    UIAlertView * errorAlert = [[UIAlertView alloc] initWithTitle:@"Error loading content" message:errorString delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
//    [errorAlert show];
//http://www.iphonesdkarticles.com/2008/11/localizing-iphone-apps-part-1.html
//NSLocale *currentUsersLocale = [NSLocale currentLocale];
//NSLog(@"Current Locale: %@", [currentUsersLocale localeIdentifier]);
// 
////Output
//Current Locale: en_US
//}
//
//
//
//PARSE_NAME_VALUE_START
//	COMPARE_NAME("")
//	{
//
//	}
//	else
//	COMPARE_NAME("")
//	{

//	}
//PARSE_NAME_VALUE_END
//
//
//

#include "tinyxml.h"
namespace FATMING_CORE
{
	//here should using # instead
	#define	COMPARE_VALUE_WITH_DEFINE(x)	if( !wcscmp(l_strValue, x) )
	#define	COMPARE_NAME_WITH_DEFINE(x)		if( !wcscmp(l_strName, x) )
	#define	COMPARE_VALUE(x)				if( !wcscmp(l_strValue,L## x) )
	#define	COMPARE_NAME(x)					if( !wcscmp(l_strName,L## x) )
	//
	//is strcmp should instead by strcmpi?
	#define	COMPARE_ELEMENT_VALUE(str)	if( !wcscmp(m_pCurrentTiXmlElement->Value(),L## str) )
	//
	#define PARSE_NAME_VALUE_START	TiXmlAttribute*	l_pAttribute = m_pCurrentTiXmlElement->FirstAttribute();	\
									const WCHAR*	l_strValue = 0;												\
									const WCHAR*	l_strName = 0;												\
									while(l_pAttribute)															\
									{																			\
										l_strValue = l_pAttribute->Value();										\
										l_strName = l_pAttribute->Name();

	#define PARSE_ELEMENT_START(p)	TiXmlAttribute*	l_pAttribute = p->FirstAttribute();	\
									const WCHAR*	l_strValue = 0;												\
									const WCHAR*	l_strName = 0;												\
									while(l_pAttribute)															\
									{																			\
										l_strValue = l_pAttribute->Value();										\
										l_strName = l_pAttribute->Name();

	//
	#define PARSE_NAME_VALUE_END	else{UT::ErrorMsg((WCHAR*)l_strName,L"unsupport Attribute");}l_pAttribute = l_pAttribute->Next();}
	//go to next without query
	#define TO_NEXT_VALUE			l_pAttribute = l_pAttribute->Next();}

	#define	VALUE_TO_INT			_wtoi(l_strValue)
	#define	VALUE_TO_FLOAT			(float)_wtof(l_strValue)
	#define	VALUE_TO_BOLLEAN		VALUE_TO_INT?true:false
	#define	VALUE_TO_POINT  		GetPoint(l_strValue)
	#define	VALUE_TO_VECTOR2		GetVector2(l_strValue)
	#define	VALUE_TO_VECTOR3		GetVector3(l_strValue)
	#define	VALUE_TO_VECTOR4		GetVector4(l_strValue)

	#define	ATTRIBUT_VALUE(Element,Name) Element->Attribute(Name)
#ifdef _DEBUG
#define	ELEMENT_VALUE_ASSERT_CHECK(Element,CompareValue){const WCHAR*l_strValue__79979 = Element->Value();if( wcscmp(CompareValue,l_strValue__79979) )assert(0&&"element value is not match");}
#else
#define	ELEMENT_VALUE_ASSERT_CHECK(Element,Value)
#endif

	//#include "../Utility.h"
	#ifdef WIN32
	//#include "../../../Include/iconv/iconv.h"
	#else
	#include "iconv.h"
	#endif
	void    XMLHandleElementDataDebugInfo (TiXmlElement*e_pTiXmlElement);
	//u could set it into xml,so it sould be called in each element is loaded
	typedef void    (*LoadingProgress)( );
	//
	extern LoadingProgress	g_pLoadingProgressCallBack;
	//=============
	//parse and loading data class
	//=============
#ifndef DEFINE_FILE_EXTENSION_NAME_INFO
#define DEFINE_FILE_EXTENSION_NAME_INFO() \
    public: \
	virtual const char*        ExtensionName() const { return ExtensionNameID; }\
	static const  char*        ExtensionNameID;
#endif
    //fuck only works for windows
    #define EXTENSION_DEFINE_MARCO(TYPE,MyExtensionName) const char*##TYPE::ExtensionNameID( #MyExtensionName );
    //locale = setlocale(LC_ALL, "");
    //make ansi charater to unicode??
	class ISAXCallback//:virtual	public NamedTypedObject
	{
	protected:
	    GET_SET_DEC(TiXmlDocument*,m_pDoc,GetDoc,SetDoc);
		//parse xml file and return a root element
		TiXmlElement*	LoadingXML(const char*e_strFileName);

		bool	m_bShowLoadingState;
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement){}
		//do some resource organsize or data arrange if Parse is called
		virtual	void	InternalParse(){}
	protected:
		std::wstring	m_strErrorMsg;	
		char m_strCurrentDirectory[TEMP_SIZE];
		char m_strFileName[TEMP_SIZE];
		//if true,the xml parser will go as
		//1->2->3->4->5->6->
		//6->5->4->3->2->1
		bool	m_bSetRunBack;
	protected:
		char*	StringAddDirectory(char*e_strValue);
		char*	StringAddDirectory(const char*e_strValue);
		//get current xml element
		TiXmlElement*	m_pCurrentTiXmlElement;
	//================
	//further goal,automatic to kill and replace data or else from old name vector
	//================
		//kill all old imageresource
		//#define KILL_OLD_IMAGE				
		//don't kill old image resource,if same name don't loading new image resource
		//#define KEEP_AND_MERGE_OLD_IMAGE
		//don't kill old image resource,if same name loading new image resource
		//#define KEEP_AND_REPLACE_OLD_IMAGE
		//idea for store old object name list to compare new list and old list to kill or don't loading same object
		//protected:
		//	std::vector<WCHAR*>	m_OldObjectNameVector;
		TiXmlElement*	ParserAllElement(TiXmlElement*e_pTiXmlElement);
	public:
		//if true the class implement ISAXCallback could decide the error message show or not
		bool	m_bShowErrorMsg;
		//DEFINE_TYPE_INFO();
		DEFINE_FILE_EXTENSION_NAME_INFO();
		ISAXCallback(bool e_bSetRunBack = false);
		~ISAXCallback(){SAFE_DELETE(m_pDoc);}
		//if e_bRemoveOldResource is true,u have to implement InternalParse to remove the resource u don't want keep
		virtual	bool	Parse(const char*e_strFileName,bool e_bShowLoadingState = false);
		char*	GetFileName(){ return m_strFileName; }
		bool	HasErrorMessage(){ if( wcslen(m_strErrorMsg.c_str()) )return true;return false; }
		const   WCHAR*GetErrorMsg(){ return m_strErrorMsg.c_str(); }
		bool	Export(const char*e_strFileName);
		bool	Export(const WCHAR*e_strFileName);
	};

	//cXMLloader	l_l("C:/Documents and Settings/Fatming/A?�ڡ�/MT_Tank/tinyxml_2_5_3/tinyxml/Fire.fat");
	//cTestXMLSAX	l_cTestXMLSAX;
	//l_l.ParseDataIntoXMLNode(&l_cTestXMLSAX);

	//=============
	//get whole data into sXMLNode by ParseDataIntoXMLNode
	//=============
	TiXmlElement*	SearchTiXmlElement(TiXmlElement* e_pNode,void* e_pPointer,TiXmlElement*(*callback)( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer ) );


	TiXmlElement	*FindElementByName( TiXmlElement*e_pCallbackFrame ,void* e_pcallbackPointer );

	//cNodeISAX	l_NodeISAX;
	//bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(e_strEffectSetupData);
	//if( l_b )
	//{
	//	TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
	//	const WCHAR*l_strMaximumObject = l_pElement->Attribute(L"MaximumObject");
	//	const WCHAR*l_strDistanceToDisappear = l_pElement->Attribute(L"DistanceToDisappear");
	//	const WCHAR*l_strMPDIFileName = l_pElement->Attribute(L"MPDIFileName");
	//	if( l_strMaximumObject )
	//	{
	//	
	//	}
	//	if( l_strDistanceToDisappear )
	//	{
	//	
	//	}
	//	if( l_strMPDIFileName )
	//	{
	//	
	//	}
	//}
	//this one is not parse recursively,you have to find the element that u want,and analyize it to generate the data u want
	//internal call MyParse,this one do not support parse animation(e_bShowLoadingState)
	class cNodeISAX:public ISAXCallback
	{
		//find need element and parse element not recursively!
		//you should overwrite this if u call ParseWithMyParse
		virtual	bool	MyParse(TiXmlElement*e_pRoot){return true;}
		GET_SET_DEC(TiXmlElement*,m_pRootElement,GetRootElement,SetRootElement);
	public:
	    //DEFINE_TYPE_INFO();
		cNodeISAX();
		~cNodeISAX();
		//call this will store m_pTiXmlDoc,so we could use it.
		virtual	bool	ParseDataIntoXMLNode(const char*e_strFileName);
		virtual	bool	ParseWithMyParse(const char*e_strFileName,bool e_bDeleteXLDocument = true);
		TiXmlElement*	GetXmlElementByNameFromRoot( WCHAR*e_pString ){ return SearchTiXmlElement(m_pRootElement,(void*)e_pString,FindElementByName);  }
		//only get root's child
		TiXmlElement*	GetChildOfRootXmlElementByName( const WCHAR*e_pString );
		TiXmlElement*	GetXmlElementByNameFromElement( const WCHAR*e_pString,TiXmlElement*e_pTiXmlElement ){ return SearchTiXmlElement(e_pTiXmlElement,(void*)e_pString,FindElementByName); }
		TiXmlElement*	GetChildOfRootXmlElementByAttribueName(const  WCHAR*e_pElementName,const WCHAR*e_strAttributeKeyWord,const WCHAR*e_strAttributeName=L"Name" );
		TiXmlElement*	GetRootChildElementByIndex( int e_iIndex );
		int				RootChildrenCount();
	};

		//const char*l_strFulscreen = l_pTiXmlElement->Attribute("FullScreen");
		//iconv_t cd = libiconv_open("wchar_t", "UTF-8");
	 //   iconv(cd, 0, 0, 0, 0);
		//WCHAR	l_str[260];
		//memset(l_str,0,sizeof(WCHAR)*260);
	 //   char *outptr = (char*)l_str;
	 //   size_t outsize = 260 * sizeof(wchar_t);
	 //   char *inptr = (char*)l_strFileName;
	 //   size_t insize = strlen(l_strFileName);
	 //   size_t res = iconv(cd, (char**) & inptr, &insize, &outptr, &outsize);
		//MessageBox(0,L"",l_str,MB_OK);
		//libiconv_close(cd);


	#ifdef __IPHONE__
	//because iphone using bundle resource,so the resource store in its specific path.
	void GetAppleBundelResourcePathByObjectPath(const char*e_strSrc,char*e_strDest);
	void GetIphoneAppFilePath(char* o_buffer, char* filename);
	#endif
}
#endif