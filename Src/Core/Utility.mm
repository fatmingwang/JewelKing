#include "stdafx.h"
#include "Utility.h"
#include "GameplayUT/GameApp.h"
#ifdef __IPHONE__
#include <mach/mach_time.h>
#include <sys/time.h>
#else
#ifdef WIN32
#include "strsafe.h"
#pragma warning( disable : 4793 )
#pragma warning( disable : 4995 )
#include "../../Include/wglext.h"
//#include "../../Include/glExt.h"
//#define WGL_SAMPLE_BUFFERS_ARB	0x2041
//#define WGL_SAMPLES_ARB		0x2042
bool	arbMultisampleSupported	= false;
int		arbMultisampleFormat	= 0;
#else
#include "SDL.h"
#endif
#endif
#ifdef __IPHONE__
double sTimeAndFPS::dbConversion = 0.0;
#endif
namespace FATMING_CORE
{
	extern bool	g_bVBOSupported;
}
namespace UT
{
	char	g_sstrForStringToStructure[TEMP_SIZE];
	WCHAR	g_sstrForStringToStructureW[TEMP_SIZE];
	void	MyGlErrorTest()
	{
//#ifdef _DEBUG
		//GL_NO_ERROR 0
		//GL_INVALID_ENUM 0x0500
		//GL_INVALID_VALUE 0x0501
		//GL_INVALID_OPERATION 0x0502 1282
		int	l_i = glGetError();
		if( l_i != 0 )
		{
			bool	l_b0 = glIsEnabled(GL_VERTEX_ARRAY)==GL_TRUE;
			bool	l_b1 = glIsEnabled(GL_TEXTURE_COORD_ARRAY)==GL_TRUE;
			bool	l_b2 = glIsEnabled(GL_NORMAL_ARRAY)==GL_TRUE;
			bool	l_b3 = glIsEnabled(GL_COLOR_ARRAY)==GL_TRUE;
			int a=0;
			assert(0);
		}
//#endif	
	}
	sTimeAndFPS::sTimeAndFPS()
	{
		uiElpaseTime = 0;
		uiPreviousTime = 0;
		uiCurrentTime = 0;

		uiNumFrame = 0;
		fTimeFor1Sec = 0.f;
		memset(strFrameRate,0,sizeof(strFrameRate));
		sprintf(strFrameRate,"%s fps","0");
#ifdef __IPHONE__
		if( dbConversion == 0 )
		{
			dbConversion = 1.0;//avoid enter again
			mach_timebase_info_data_t l_Info;
			kern_return_t err = mach_timebase_info(&l_Info);
			if( err == 0 )
			    dbConversion = 1e-9*(double)l_Info.numer/(double)l_Info.denom;
				//dbConversion = 1e-9;//*(double)l_Info.numer/(double)l_Info.denom;
		}
#endif
	}
	void	sTimeAndFPS::Update()//total game running time
	{
		uiPreviousTime = uiCurrentTime;
#ifdef WIN32
		uiCurrentTime = GetTickCount();
#else
#ifdef __IPHONE__
		uiCurrentTime = mach_absolute_time();
#else
		uiCurrentTime = SDL_GetTicks();
#endif
#endif
		uiElpaseTime = uiCurrentTime-uiPreviousTime;
#ifndef __IPHONE__		
		fElpaseTime = (float)uiElpaseTime/1000.f;
#else
		fElpaseTime = (float)((double)uiElpaseTime*dbConversion);
#endif
		uiNumFrame++;
		fTimeFor1Sec+=fElpaseTime;
		if(fTimeFor1Sec>=1.f)
		{
			fTimeFor1Sec-=(int)fTimeFor1Sec;
			sprintf(strFrameRate,"%i fps",uiNumFrame);
			uiNumFrame = 0;
		}
	}

	char* sTimeAndFPS::GetFPS()
	{
		return strFrameRate;
	}

	sTimeCounter::sTimeCounter(float e_fTargetTime)
	{
		SetTargetTime(e_fTargetTime);
		bLoop = false;
	}

	void	sTimeCounter::SetTargetTime(float e_fTargetTime)
	{
		bTragetTimrReached = false;
		fRestTime = e_fTargetTime;
		this->fTargetTime = e_fTargetTime;	
	}

	float	sTimeCounter::GetLERP()
	{
		float	l_fLERP = (1-fRestTime/fTargetTime);
		return l_fLERP>1?1:l_fLERP;
	}

	void	sTimeCounter::SetLoop(bool e_bLoop)
	{
		this->bLoop = e_bLoop;
		bTragetTimrReached = false;
	}

	//m_TimToFire.Start(m_TimToFire.fRestTime*-1);//start with past time set test as nagetive
	void	sTimeCounter::Start(float e_fElpaseTime)
	{
		bTragetTimrReached = false;
		fRestTime = fTargetTime-e_fElpaseTime;
	}

	void	sTimeCounter::Update(float e_fElpaseTime)
	{
		if( bTragetTimrReached )
		{
			if( bLoop )
				bTragetTimrReached = false;
			else
				return;
		}
		fRestTime-=e_fElpaseTime;
		if( fRestTime<=0.f )
		{
			bTragetTimrReached = true;
			if( bLoop )
			{
				fRestTime = fTargetTime+fRestTime;
				//if( -fRestTime>fTargetTime )
				//{
				//	fRestTime = fTargetTime;
				//}
				//else
				//	fRestTime = fTargetTime+fRestTime;
			}
		}
	}

#ifdef WIN32
	HGLRC	InitOpenGL(HWND e_pHwnd,bool e_bInitGlewInit,HDC e_HdcMV)
	{
		static	bool	l_b =false;
		GLuint PixelFormat;													// Will Hold The Selected Pixel Format
		PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
		{
			sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
			1,																// Version Number
			PFD_DRAW_TO_WINDOW |											// Format Must Support Window
			PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,												// Must Support Double Buffering
			PFD_TYPE_RGBA,													// Request An RGBA Format
			32,																// Select Our Color Depth
			0, 0, 0, 0, 0, 0,												// Color Bits Ignored
			1,																// No Alpha Buffer
			0,																// Shift Bit Ignored
			0,																// No Accumulation Buffer
			0, 0, 0, 0,														// Accumulation Bits Ignored
			16,																// 16Bit Z-Buffer (Depth Buffer)  
			0,																// No Stencil Buffer
			0,																// No Auxiliary Buffer
			PFD_MAIN_PLANE,													// Main Drawing Layer
			0,																// Reserved
			0, 0, 0															// Layer Masks Ignored
		};
		//fuck multi windows occur error
		//e_HdcMV = GetDC (e_pHwnd);									// Grab A Device Context For This Window
		if (e_HdcMV == 0)												// Did We Get A Device Context?
		{
			assert(0);
		}
		PixelFormat = ChoosePixelFormat (e_HdcMV, &pfd);				// Find A Compatible Pixel Format
		if (PixelFormat == 0)												// Did We Find A Compatible Format?
		{
			assert(0);
		}
		if( arbMultisampleFormat != 0 )//if u want to setup multi sample u have to ensure arbMultisampleFormat have been assigned
			PixelFormat = arbMultisampleFormat;
		if (SetPixelFormat (e_HdcMV, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
		{
			assert(0);
		}
		HGLRC	l_HGLRC = wglCreateContext (e_HdcMV);						// Try To Get A Rendering Context
		if (l_HGLRC == 0)												// Did We Get A Rendering Context?
		{
			assert(0);												// Return False
		}

		// Make The Rendering Context Our Current Rendering Context
		if (wglMakeCurrent (e_HdcMV, l_HGLRC) == FALSE)
		{
			assert(0);
		}
		if( e_bInitGlewInit && !l_b )
		{
			l_b  =true;
			GLenum	l_eErrorID = glewInit();
			assert(l_eErrorID == 0);
		}
		FATMING_CORE::g_bVBOSupported = IsExtensionSupported( "GL_ARB_vertex_buffer_object" );
		MyGlErrorTest();
		return l_HGLRC;
	}
	bool WGLisExtensionSupported(const char *extension)
	{
		const size_t extlen = strlen(extension);
		const char *supported = NULL;

		// Try To Use wglGetExtensionStringARB On Current DC, If Possible
		PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

		if (wglGetExtString)
			supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

		// If That Failed, Try Standard Opengl Extensions String
		if (supported == NULL)
			supported = (char*)glGetString(GL_EXTENSIONS);

		// If That Failed Too, Must Be No Extensions Supported
		if (supported == NULL)
			return false;

		// Begin Examination At Start Of String, Increment By 1 On False Match
		for (const char* p = supported; ; p++)
		{
			// Advance p Up To The Next Possible Match
			p = strstr(p, extension);

			if (p == NULL)
				return false;						// No Match

			// Make Sure That Match Is At The Start Of The String Or That
			// The Previous Char Is A Space, Or Else We Could Accidentally
			// Match "wglFunkywglExtension" With "wglExtension"

			// Also, Make Sure That The Following Character Is Space Or NULL
			// Or Else "wglExtensionTwo" Might Match "wglExtension"
			if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
				return true;						// Match
		}
	}
			
	bool InitMultisample(HINSTANCE hInstance,HWND hWnd)
	{  
		// See If The String Exists In WGL!
		if (!WGLisExtensionSupported("WGL_ARB_multisample"))
		{
			arbMultisampleSupported=false;
			return false;
		}

		// Get Our Pixel Format
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB =
			(PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		if (!wglChoosePixelFormatARB)
		{
			// We Didn't Find Support For Multisampling, Set Our Flag And Exit Out.
			arbMultisampleSupported=false;
			return false;
		}

		// Get Our Current Device Context. We Need This In Order To Ask The OpenGL Window What Attributes We Have
		HDC hDC = GetDC(hWnd);

		int pixelFormat;
		bool valid;
		UINT numFormats;
		float fAttributes[] = {0,0};

		// These Attributes Are The Bits We Want To Test For In Our Sample
		// Everything Is Pretty Standard, The Only One We Want To 
		// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
		// These Two Are Going To Do The Main Testing For Whether Or Not
		// We Support Multisampling On This Hardware
		int iAttributes[] = { WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
			WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,24,
			WGL_ALPHA_BITS_ARB,8,
			WGL_DEPTH_BITS_ARB,16,
			WGL_STENCIL_BITS_ARB,0,
			WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
			WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
			WGL_SAMPLES_ARB, 4 ,						// Check For 4x Multisampling
			0,0};

		// First We Check To See If We Can Get A Pixel Format For 4 Samples
		valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats)?true:false;
	 
		// if We Returned True, And Our Format Count Is Greater Than 1
		if (valid && numFormats >= 1)
		{
			arbMultisampleSupported	= true;
			arbMultisampleFormat	= pixelFormat;	
			return arbMultisampleSupported;
		}

		// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
		iAttributes[19] = 2;
		valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats)?true:false;
		if (valid && numFormats >= 1)
		{
			arbMultisampleSupported	= true;
			arbMultisampleFormat	= pixelFormat;	 
			return arbMultisampleSupported;
		}
		  
		// Return The Valid Format
		return  arbMultisampleSupported;
	}
	bool IsExtensionSupported( char* szTargetExtension )
	{
		const unsigned char *pszExtensions = NULL;
		const unsigned char *pszStart;
		unsigned char *pszWhere, *pszTerminator;

		// Extension names should not have spaces
		pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
		if( pszWhere || *szTargetExtension == '\0' )
			return false;

		// Get Extensions String
		pszExtensions = glGetString( GL_EXTENSIONS );

		// Search The Extensions String For An Exact Copy
		pszStart = pszExtensions;
		for(;;)
		{
			pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
			if( !pszWhere )
				break;
			pszTerminator = pszWhere + strlen( szTargetExtension );
			if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
				if( *pszTerminator == ' ' || *pszTerminator == '\0' )
					return true;
			pszStart = pszTerminator;
		}
		return false;
	}
#endif
	//--------------------------------------------------------------------------------------
	// Name: DebugSpewV()
	// Desc: Internal helper function
	//--------------------------------------------------------------------------------------
	VOID DebugSpewV( const CHAR* strFormat, const va_list pArgList )
	{
		CHAR str[2048];
		// Use the secure CRT to avoid buffer overruns. Specify a count of
		// _TRUNCATE so that too long strings will be silently truncated
		// rather than triggering an error.
		vsnprintf( str, sizeof(str), strFormat, pArgList );
#ifdef WIN32
		OutputDebugStringA( str );
#else
#ifdef _DEBUG
		printf(str);
#endif
#endif
	}
	VOID DebugSpewV( const WCHAR* strFormat, const va_list pArgList )
	{
		WCHAR str[2048];
		// Use the secure CRT to avoid buffer overruns. Specify a count of
		// _TRUNCATE so that too long strings will be silently truncated
		// rather than triggering an error.
#ifdef WIN32
		_vsnwprintf( str, sizeof(str), strFormat, pArgList );
#else
		vswprintf( str, sizeof(str), strFormat, pArgList );
#endif
#ifdef WIN32
		OutputDebugString( str );
#else
#ifdef _DEBUG
		//printf(str);
#endif
#endif	
	}

	void	ErrorMsg(char*e_strErrMsg1,char*e_strErrMsg2)
	{
#ifdef WIN32
		WCHAR	l_temp1[TEMP_SIZE];
		WCHAR	l_temp2[TEMP_SIZE];
		CHAR_TO_WCHAR(e_strErrMsg1,l_temp1);
		CHAR_TO_WCHAR(e_strErrMsg2,l_temp2);
		ErrorMsg(l_temp1,l_temp2);
#else
#ifdef _DEBUG
		printf("%s, %s\n\0",e_strErrMsg1,e_strErrMsg2);
#endif
#endif
	}
	eDataType	GetDataType(const WCHAR*e_str)
	{
		if( !e_str )
			return eDT_MAX;
		if( !wcscmp(e_str,L"int") )
			return eDT_INT;
		if( !wcscmp(e_str,L"float") )
			return eDT_FLOAT;
		if( !wcscmp(e_str,L"byte") )
			return eDT_BYTE;
		if( !wcscmp(e_str,L"double") )
			return eDT_DOUBLE;
		if( !wcscmp(e_str,L"wstring") )
			return eDT_WSTRING;
		if( !wcscmp(e_str,L"string") )
			return eDT_STRING;
		if( !wcscmp(e_str,L"void") )
			return eDT_VOID;
		if( !wcscmp(e_str,L"Point") )
			return eDT_POINT;
		if( !wcscmp(e_str,L"Vector2") )
			return eDT_VECTOR2;
		if( !wcscmp(e_str,L"Vector3") )
			return eDT_VECTOR3;
		if( !wcscmp(e_str,L"Vector4") )
			return eDT_VECTOR4;
		if( !wcscmp(e_str,L"int64") )
			return eDT_INT64;
		if( !wcscmp(e_str,L"uint64") )
			return eDT_UINT64;
		return eDT_MAX;
	}
	eDataType	GetDataType(const char*e_str)
	{
		if( !strcmp(e_str,"int") )
			return eDT_INT;
		if( !strcmp(e_str,"float") )
			return eDT_FLOAT;
		if( !strcmp(e_str,"byte") )
			return eDT_BYTE;
		if( !strcmp(e_str,"double") )
			return eDT_DOUBLE;
		if( !strcmp(e_str,"wstring") )
			return eDT_WSTRING;
		if( !strcmp(e_str,"string") )
			return eDT_STRING;
		if( !strcmp(e_str,"void") )
			return eDT_VOID;
		if( !strcmp(e_str,"int64") )
			return eDT_INT64;
		if( !strcmp(e_str,"uint64") )
			return eDT_UINT64;
		return eDT_MAX;
	}
	void	ErrorMsg(const char*e_strErrMsg1,char*e_strErrMsg2){ ErrorMsg((char*)e_strErrMsg1,(char*)e_strErrMsg2);  }
	void	ErrorMsg(char*e_strErrMsg1,const char*e_strErrMsg2){ ErrorMsg((char*)e_strErrMsg1,(char*)e_strErrMsg2); }
	void	ErrorMsg(const char*e_strErrMsg1,const char*e_strErrMsg2){ ErrorMsg((char*)e_strErrMsg1,(char*)e_strErrMsg2); }
	void	ErrorMsg(const wchar_t*e_strErrMsg1,const wchar_t*e_strErrMsg2)
	{
#ifdef WIN32
		if( FATMING_CORE::cGameApp::m_sbShowErrorMsgBox )
		{
			try
			{
				if(MessageBox(0,e_strErrMsg1,e_strErrMsg2,MB_YESNO)==6)
				{
					assert(0);
					//exit(0);
				}
			}
			catch(const std::exception& ex)
			{
				const char*l_str = ex.what();
				int a=0;
			}
		}
		else
		if( FATMING_CORE::cGameApp::m_spstrErrorMsgString )
		{
			*FATMING_CORE::cGameApp::m_spstrErrorMsgString += e_strErrMsg1;
			*FATMING_CORE::cGameApp::m_spstrErrorMsgString += L"  ";
			*FATMING_CORE::cGameApp::m_spstrErrorMsgString += e_strErrMsg2;
			*FATMING_CORE::cGameApp::m_spstrErrorMsgString += L"\n";
			SystemErrorCheck();
		}
#else
#ifdef _DEBUG
		char	l_temp1[TEMP_SIZE];
		char	l_temp2[TEMP_SIZE];
		WCHAR_TO_CHAR(e_strErrMsg1,l_temp1);
		WCHAR_TO_CHAR(e_strErrMsg2,l_temp2);
		printf("%s, %s\n\0",l_temp1,l_temp2);
#endif
#endif
	}
	void	SystemErrorCheck()
	{
		return;
#ifdef _WIN32
#ifdef _DEBUG
		static	bool	l_b = false;
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError(); 
		if( dw && !l_b )
		{
			l_b = true;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );

			// Display the error message and exit the process

			lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
				(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)L"Error")+40)*sizeof(TCHAR)); 
			StringCchPrintf((LPTSTR)lpDisplayBuf, 
				LocalSize(lpDisplayBuf),
				TEXT("%s failed with error %d: %s"), 
				L"Error", dw, lpMsgBuf);
			OutputDebugString((WCHAR*)lpDisplayBuf);
			OutputDebugString(L"\n");
			//MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

			LocalFree(lpMsgBuf);
			LocalFree(lpDisplayBuf);
		}
#endif
#endif
	}
	//--------------------------------------------------------------------------------------
	// Name: DebugSpew()
	// Desc: Prints formatted debug spew
	//--------------------------------------------------------------------------------------
	// variadic_macros.cpp
	//#include <stdio.h>
	//#define EMPTY
	//
	//#define CHECK1(x, ...) if (!(x)) { printf(__VA_ARGS__); }
	//#define CHECK2(x, ...) if ((x)) { printf(__VA_ARGS__); }
	//#define CHECK3(...) { printf(__VA_ARGS__); }
	//#define MACRO(s, ...) printf(s, __VA_ARGS__)
	//
	//int main() {
	//   CHECK1(0, "here %s %s %s", "are", "some", "varargs1(1)\n");
	//   CHECK1(1, "here %s %s %s", "are", "some", "varargs1(2)\n");   // won't print
	//
	//   CHECK2(0, "here %s %s %s", "are", "some", "varargs2(3)\n");   // won't print
	//   CHECK2(1, "here %s %s %s", "are", "some", "varargs2(4)\n");
	//
	//   // always invokes printf in the macro
	//   CHECK3("here %s %s %s", "are", "some", "varargs3(5)\n");
	//
	//   MACRO("hello, world\n");
	//   // MACRO("error\n", EMPTY);   would cause C2059
	//}
	// 
	//  Copy Code 
	//here are some varargs1(1)
	//here are some varargs2(4)
	//here are some varargs3(5)
	//hello, world
	// 
	void	ErrorMsgByFormat(char*e_strErrMsg1,...)
	{
		va_list pArgList;
		va_start( pArgList, e_strErrMsg1 );
		DebugSpewV( e_strErrMsg1, pArgList );
		va_end( pArgList );		
	}
	std::wstring			ComposeMsgByFormat(WCHAR*e_strErrMsg1,...)
	{
		std::wstring	l_str;
		va_list pArgList;
		va_start( pArgList, e_strErrMsg1 );
		WCHAR str[2048];
#ifdef _WIN32
		_vsnwprintf( str, sizeof(str), e_strErrMsg1, pArgList );
#else
		vswprintf( str, sizeof(str), e_strErrMsg1, pArgList );
#endif
		va_end( pArgList );
		l_str = str;
		return l_str;
	}
	std::string			ComposeMsgByFormat(char*e_strErrMsg1,...)
	{
		std::string	l_str;
		va_list pArgList;
		va_start( pArgList, e_strErrMsg1 );
		char str[2048];
		vsnprintf( str, sizeof(str), e_strErrMsg1, pArgList );
		va_end( pArgList );
		l_str = str;
		return l_str;	
	}
	void					ErrorMsgByFormat(WCHAR*e_strErrMsg1,...)
	{
		va_list pArgList;
		va_start( pArgList, e_strErrMsg1 );
		DebugSpewV( e_strErrMsg1, pArgList );
		va_end( pArgList );			
	}
//===============================
//
//===============================
	WCHAR*	GetFileNameWithoutFullPath(const WCHAR*e_pString,bool e_bStripExtensionFileName)
	{
		static WCHAR	l_temp[TEMP_SIZE];
		memset(l_temp,0,sizeof(l_temp));
		int l_iForStripExtensionFileName = 0;
		int	l_iStringLen = (int)wcslen(e_pString);
		for(int i=l_iStringLen-1;i!=0;--i)
		{
			if( e_bStripExtensionFileName )
			{
				if( e_pString[i] == L'.' )
					l_iForStripExtensionFileName = l_iStringLen-i;
			}

			if( e_pString[i] == L'\\' || e_pString[i] == L'/' )
			{
				int l_iNameLEngth = l_iStringLen-i-1-l_iForStripExtensionFileName;
				memcpy(l_temp,e_pString+i+1,sizeof(WCHAR)*l_iNameLEngth);
				l_temp[l_iNameLEngth] = L'\0';
				return l_temp;
			}
		}
		int l_iNameLEngth = l_iStringLen-l_iForStripExtensionFileName;
		memcpy(l_temp,e_pString,sizeof(WCHAR)*l_iNameLEngth);
		l_temp[l_iNameLEngth] = L'\0';
		return l_temp;
	}
//===============================
//
	//const char* file = "79979.fat";
	// const char* extension;
    //extension = strrchr( file, '.');

    //if (extension)
    //    extension++; //start looking after the .
    //else
    //    return false; //don't try to place guess the file type

    //int formatCount = sizeof(Model::formatTable) / sizeof(Model::FormatInfo);

    ////try to match by format first
    //for ( int ii = 0; ii < formatCount; ii++) {
    //    if ( ! strcasecmp( formatTable[ii].extension, extension)) {
    //        //extension matches, load it
    //        //return formatTable[ii].reader( file, *this);
    //    }
    //}
//===============================
	WCHAR*	GetFileExtensionName(WCHAR*e_pString)
	{
		static WCHAR l_stemp[TEMP_SIZE];
		memset(l_stemp,0,sizeof(l_stemp));
		int	l_iExtensionNameSize = 0;
		int	l_iFileNameLength = (int)wcslen(e_pString)-1;
		//until find L'.'
		for( int i = l_iFileNameLength;i>0;--i )
		{
			if( e_pString[i] == L'.' )
			{//set all extension name
				for( int j = l_iExtensionNameSize;j>0 ;--j )
					l_stemp[l_iExtensionNameSize-j] = e_pString[l_iFileNameLength-j+1];
				l_stemp[l_iExtensionNameSize] = L'\0';
				return l_stemp;
			}
			l_iExtensionNameSize++;
		}
		return 0;
	}

	char*	GetFileNameWithoutFullPath(const char*e_pString,bool e_bStripExtensionFileName)
	{
		static char	l_temp[TEMP_SIZE];
		memset(l_temp,0,sizeof(l_temp));
		int l_iForStripExtensionFileName = 0;
		int	l_iStringLen = (int)strlen(e_pString);
		for(int i=l_iStringLen-1;i!=0;--i)
		{
			if( e_bStripExtensionFileName )
			{
				if( e_pString[i] == '.' &&l_iForStripExtensionFileName==0)
					l_iForStripExtensionFileName = l_iStringLen-i;
			}

			if( e_pString[i] == '\\' || e_pString[i] == '/' )
			{
				int l_iNameLEngth = l_iStringLen-i-1-l_iForStripExtensionFileName;
				memcpy(l_temp,e_pString+i+1,sizeof(char)*l_iNameLEngth);
				l_temp[l_iNameLEngth] = '\0';
				return l_temp;
			}
		}
		int l_iNameLEngth = l_iStringLen-l_iForStripExtensionFileName;
		memcpy(l_temp,e_pString,sizeof(char)*l_iNameLEngth);
		l_temp[l_iNameLEngth] ='\0';
		return l_temp;
	}

	char*	GetFileExtensionName(char*e_pString)
	{
		static char l_stemp[TEMP_SIZE];
		memset(l_stemp,0,sizeof(l_stemp));
		int	l_iExtensionNameSize = 0;
		int	l_iFileNameLength = (int)strlen(e_pString)-1;
		//until find '.'
		for( int i = l_iFileNameLength;i>0;--i )
		{
			if( e_pString[i] == '.' )
			{//set all extension name
				for( int j = l_iExtensionNameSize;j>0 ;--j )
					l_stemp[l_iExtensionNameSize-j] = e_pString[l_iFileNameLength-j+1];
				l_stemp[l_iExtensionNameSize] = '\0';
				return l_stemp;
			}
			l_iExtensionNameSize++;
		}
		return 0;
	}

	char*	GetDirectoryWithoutFileName(const char*e_str)
	{
		static char	l_strTemp[TEMP_SIZE];
		int	l_i = (int)strlen(e_str);
		for(int i= l_i-1;i>0;--i)
		{
			if( e_str[i] == '/'||e_str[i] == '\\' )
			{
				sprintf(l_strTemp,"%s\n",e_str);
				l_strTemp[i+1] = '\0';
				return l_strTemp;
			}
		}
		return "./";
	}

	bool IsFileExists( const char* FileName )
	{
		//will not work if you do not have read permissions
		//to the file, but if you don''t have read, it
		//may as well not exist to begin with.
		FILE* fp = NULL;
#ifdef __IPHONE__
		char	l_strFileName[MAX_PATH];
		GetAppleBundelResourcePathByObjectPath(FileName,l_strFileName);
		fp = fopen( l_strFileName, "rb" );
#else
		fp = fopen( FileName, "rb" );
#endif
		if( fp != NULL ){fclose( fp );return true;}
		//remove windows error message
		SystemErrorCheck();
		return false;
	}
	bool					RemoveFile( const char* FileName )
	{
		int	l_iResult = remove(FileName);
		return l_iResult==0?true:false;
	}

	Vector4	ViewRectToOpenGLScissor(Vector4 e_v2DViewRange,Vector2 e_vViewPort,Vector2 e_vGameResolution,eDeviceDirection e_eDeviceDirection)
	{
		Vector2	l_vScale(1.f,1.f);
		switch(e_eDeviceDirection)
		{
			case eDD_PORTRAIT:
			case eDD_UPSIDE_DOWN:
				l_vScale = Vector2(e_vGameResolution.x/e_vViewPort.x,e_vGameResolution.y/e_vViewPort.y);
				break;
			case eDD_LANDSCAPE_LEFT:
			case eDD_LANDSCAPE_RIGHT:
				l_vScale = Vector2(e_vGameResolution.x/e_vViewPort.y,e_vGameResolution.y/e_vViewPort.x);
				break;
		}
		e_v2DViewRange.x /= l_vScale.x;
		e_v2DViewRange.y /= l_vScale.y;
		e_v2DViewRange.z /= l_vScale.x;
		e_v2DViewRange.w /= l_vScale.y;
		Vector4	l_vScissor;
		float	l_fWidth = (e_v2DViewRange.z-e_v2DViewRange.x);
		float	l_fHeight = (e_v2DViewRange.w-e_v2DViewRange.y);
		switch(e_eDeviceDirection)
		{
			case eDD_PORTRAIT:
				l_vScissor.x = e_v2DViewRange.x;
				l_vScissor.y = e_vViewPort.y-e_v2DViewRange.y-l_fHeight;
				break;
			case eDD_UPSIDE_DOWN:
				l_vScissor.x = e_vViewPort.x-(e_v2DViewRange.x+l_fWidth);
				l_vScissor.y = e_v2DViewRange.y;
				break;
			case eDD_LANDSCAPE_LEFT:
				l_vScissor.x = e_v2DViewRange.y;
				l_vScissor.y = e_v2DViewRange.x;
				break;
			case eDD_LANDSCAPE_RIGHT:
			{
				Vector2	l_vOriginalPos(e_v2DViewRange.x,e_v2DViewRange.y);
				e_v2DViewRange.y = l_vOriginalPos.x;
				e_v2DViewRange.x = e_vViewPort.x-l_vOriginalPos.y;
				l_vScissor.x = e_v2DViewRange.x-l_fHeight;
				l_vScissor.y = e_vViewPort.y-(e_v2DViewRange.y+l_fWidth);
			}
				break;
		}
		switch(e_eDeviceDirection)
		{
			case eDD_PORTRAIT:
			case eDD_UPSIDE_DOWN:
				l_vScissor.z = l_fWidth;
				l_vScissor.w = l_fHeight;
				break;
			case eDD_LANDSCAPE_LEFT:
			case eDD_LANDSCAPE_RIGHT:
				l_vScissor.w = l_fWidth;
				l_vScissor.z = l_fHeight;
				break;
		}
		return l_vScissor;
	}

	Vector4	ViewRectToOpenGLScissor(Vector4 e_v2DViewRange)
	{
		return ViewRectToOpenGLScissor(e_v2DViewRange,FATMING_CORE::cGameApp::m_svViewPortSize,FATMING_CORE::cGameApp::m_svGameResolution,FATMING_CORE::cGameApp::m_seDeviceDirection);
	}
//==================math
	//101~106	A~F
	//48~57		0~9
	//97~102	a~f
	//http://social.msdn.microsoft.com/forums/en-US/vcgeneral/thread/d56a52f0-f9e2-45e5-be16-7e5ebcf7169e
	char	CharToHex(char e_c)
	{
		char	l_cValue = -1;
		if( e_c >='0' && e_c<= '9' )
		{
			l_cValue = e_c-'0';
		}
		else
		if( e_c >='a' && e_c<= 'f' )
		{
			l_cValue = e_c-'a'+10;
		}
		else
		if( e_c >='A' && e_c<= 'F' )
		{
			l_cValue = e_c-'A'+10;
		}
		else
		{
			assert(0);
			return -1;
		}
		return l_cValue;
	}

	int		HexadecimalStringToDecimal(char*e_strValue)
	{
		int	l_iNum = (int)strlen(e_strValue);
		if( l_iNum <= 0  )
			return 0;
		int	l_iValue = 0;
		int	l_iHexValue = 16;
		char	l_cOrder[8] = {0,0,0,0,0,0,0,0};//0x00000000
		for( int i=0;i<l_iNum;++i )//set order from units start
			l_cOrder[l_iNum-i-1] = e_strValue[i];

		l_iValue += CharToHex(l_cOrder[0]);
		for( int i = 1; i < l_iNum ; ++i )
		{
			l_iValue += CharToHex(l_cOrder[i])*l_iHexValue;
			l_iHexValue*=16;
		}
		return l_iValue;
	}
	//hex copy to string
	//char Address[100];
	//unsigned int Add = 0x000CA;
	//sprintf(Address, "%05X", Add);
	int		DecimalStringToHexadcimal(char*e_strValue)
	{
		return strtol(e_strValue,NULL,16);
	}
    int	FindChacterIndexFromStartBuffer(WCHAR*e_String,WCHAR e_char,int e_iLookForIndex)
    {
		int j = (int)wcslen(e_String);
		//int l_iNum = 0;
		for( int i=0;i<j;++i )
		{
			if( e_String[i] == e_char )
			{
			    //fuck.....what is this for.....
			    //if( e_iLookForIndex == e_iLookForIndex )
				    return i;
				//++l_iNum;
			}
		}
		return -1;
    }
	int	FindChacterIndexFromBackBuffer(WCHAR*e_String,WCHAR e_char)
	{
		int	l_iLastSlash = -1;
		int j = (int)wcslen(e_String)-1;
		for( ;j>0;--j )
		{
			if( e_String[j] == e_char )
			{
				l_iLastSlash = j;
				return l_iLastSlash;
			}
		}
		return l_iLastSlash;	
	}

	int	FindChacterIndexFromBackBuffer(char*e_String,char e_char)
	{
		int	l_iLastSlash = -1;
		int j = (int)strlen(e_String)-1;
		for( ;j>0;--j )
		{
			if( e_String[j] == e_char )
			{
				l_iLastSlash = j;
				return l_iLastSlash;
			}
		}
		return l_iLastSlash;
	}

	long				GetFileSize( FILE*e_pFile)
	{
		//store curent position
		long l_lCurrentPosition = ftell( e_pFile );
		//go to end
		int	l_i = fseek(e_pFile,0,SEEK_END);
		assert(l_i == 0&&"file search size error");
		long l_lFilesize = ftell( e_pFile );
		//restore position
		fseek(e_pFile,l_lCurrentPosition,SEEK_SET);
		return l_lFilesize;
	}


	std::wstring			GetSystemTimeForFile(bool e_bCN)
	{
		std::wstring	l_str;
#ifdef WIN32
		SYSTEMTIME l_st;
//		GetSystemTime(&l_st);
		GetLocalTime(&l_st);
		if( e_bCN )
		{
			l_str += ValueToStringW(l_st.wYear);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wMonth);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wDay);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wHour);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wMinute);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wSecond);
		}
		else
		{
			l_str += ValueToStringW(l_st.wMonth);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wDay);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wYear);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wHour);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wMinute);
			l_str += L"_";
			l_str += ValueToStringW(l_st.wSecond);	
		}
#endif	
		return l_str;
	}

	std::wstring			GetSystemTime(bool e_bCN)
	{
		std::wstring	l_str;
#ifdef WIN32
		SYSTEMTIME l_st;
//		GetSystemTime(&l_st);
		GetLocalTime(&l_st);
		if( e_bCN )
		{
			l_str += ValueToStringW(l_st.wYear);
			l_str += L".";
			l_str += ValueToStringW(l_st.wMonth);
			l_str += L".";
			l_str += ValueToStringW(l_st.wDay);
			l_str += L".";
			l_str += ValueToStringW(l_st.wHour);
			l_str += L":";
			l_str += ValueToStringW(l_st.wMinute);
			l_str += L":";
			l_str += ValueToStringW(l_st.wSecond);
		}
		else
		{
			l_str += ValueToStringW(l_st.wMonth);
			l_str += L".";
			l_str += ValueToStringW(l_st.wDay);
			l_str += L".";
			l_str += ValueToStringW(l_st.wYear);
			l_str += L".";
			l_str += ValueToStringW(l_st.wHour);
			l_str += L":";
			l_str += ValueToStringW(l_st.wMinute);
			l_str += L":";
			l_str += ValueToStringW(l_st.wSecond);	
		}
#endif
		return l_str;
	}
//end namespace UT
}
