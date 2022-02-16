#include "../stdafx.h"
#include "GameApp.h"
#ifndef __IPHONE__
#include "../../include/IL/il.h"
#endif
#ifdef WIN32
#include <direct.h>
#endif
#include "../Sound/SoundManager.h"
#include "../Particle/AllParticleInclude.h"
#include "BinaryFile.h"
#include "../Synchronization/AllFUThreadInclude.h"
//#include <wininet.h>
//#pragma comment(lib,"Wininet.lib")
//bool bConnect=InternetCheckConnection(L"http://www.google.com",
//              FLAG_ICC_FORCE_CONNECTION ,0);

//bool IsWindows7()
//{
//    OSVERSIONINFOEX osvi;
//    bool bOsVersionInfoEx=false;
//    DWORD dwType=0UL;
//    bool bWin7=false;
//    ZeroMemory(&osvi, sizeof(SYSTEM_INFO));
//    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
//    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
//    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
//        return -1;
//    if(osvi.dwMajorVersion==6)
//    {
//        if(osvi.dwMinorVersion==1)
//        {
//            if(osvi.wProductType==VER_NT_WORKSTATION)
//            {
//                bWin7=true;
//            }
//        }
//    }
//    return bWin7;
//}
namespace	FATMING_CORE
{
	POINT	ConvertCoordinate(int e_iPosX,int e_iPosY,Vector2 e_vViewPort)
	{
		Vector2		l_vScale = cGameApp::GetViewPortAndGameResolutionScale();
		POINT	l_Pos;
		switch(cGameApp::m_seDeviceDirection)
		{
			case eDD_PORTRAIT://do nothing
				l_Pos.x = e_iPosX;
				l_Pos.y = e_iPosY;
			break;
			case eDD_UPSIDE_DOWN:
				l_Pos.x = (int)e_vViewPort.x-e_iPosX;
				l_Pos.y = (int)e_vViewPort.y-e_iPosY;
			break;
			case eDD_LANDSCAPE_LEFT://ensure view port is right,x and y swap
				l_Pos.x = (int)e_vViewPort.y-e_iPosY;
				l_Pos.y = e_iPosX;
			break;//ensure view port is right,x and y swap
			case eDD_LANDSCAPE_RIGHT:
				l_Pos.x = e_iPosY;
				l_Pos.y = (int)e_vViewPort.x-e_iPosX;
			break;
			default:
				assert(0&&"ConvertCoordinate fuck");
				break;
		}
		l_Pos.x = (int)(l_Pos.x*l_vScale.x);
		l_Pos.y = (int)(l_Pos.y*l_vScale.y);
		return l_Pos;
	}

#ifdef WIN32
	HDC									cGameApp::m_sHdc = 0;
	HANDLE								cGameApp::m_sHandle = 0;
#endif
	float								cGameApp::m_sfDebugValue = 1.f;
	bool								cGameApp::m_sbLeave = false;
	bool								cGameApp::m_sbSpeedControl = false;
	UT::sTimeAndFPS						cGameApp::m_sTimeAndFPS;
	cPhaseManager*						cGameApp::m_spPhaseManager = 0;
	cPaticleManager*					cGameApp::m_spPaticleManager = 0;
	cBehaviorPaticleManager*			cGameApp::m_spBehaviorPaticleManager = 0;
	cGlyphFontRender*					cGameApp::m_spGlyphFontRender = 0;
	Vector2								cGameApp::m_svViewPortSize(1920.f,1080.f);
	Vector2								cGameApp::m_svGameResolution(1920.f,1080.f);
	Vector2								cGameApp::m_svGameScale(1.f,1.f);
	POINT								cGameApp::m_sMousePosition = {0,0};
	bool								cGameApp::m_sbFullScreen = false;
	bool    							cGameApp::m_bMouseHover = false;;
	bool								cGameApp::m_sbTouched = false;
	bool								cGameApp::m_sb2Touched = false;
	bool								cGameApp::m_sbGamePause = false;
	bool								cGameApp::m_sbDeviceExist = false;	
	sMultiTouchPoints*					cGameApp::m_spMultiTouchPoints = 0;
	cAnimationParser*					cGameApp::m_spAnimationParser = 0;
	cImageParser*						cGameApp::m_spImageParser = 0;
	cSoundParser*						cGameApp::m_spSoundParser = 0;
	cObjectListByName<cCurveManager>*	cGameApp::m_spPathFileList;
	bool								cGameApp::m_sbShowErrorMsgBox = true;
	std::wstring*						cGameApp::m_spstrErrorMsgString = 0;
	bool								cGameApp::m_sucKeyData[MAX_PATH];
	bool								cGameApp::m_sbEnableMouseSingnal = true;
	bool								cGameApp::m_sbEnableKeyboardSingnal = true;
#ifdef _DEBUG
	bool								cGameApp::m_sbDebugFunctionWorking = true;
#else
	bool								cGameApp::m_sbDebugFunctionWorking = false;
#endif
	cBinaryFile*						cGameApp::m_spLogFile = 0;
	cFUThreadPool*						cGameApp::m_spThreadPool = 0;
	//cClickMouseBehavior*				cGameApp::m_spClickMouseBehavior = 0;
#ifndef __IPHONE__
	eDeviceDirection					cGameApp::m_seDeviceDirection = eDD_PORTRAIT;
	//eDeviceDirection					cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_LEFT;
	//eDeviceDirection					cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_RIGHT;
	//eDeviceDirection					cGameApp::m_seDeviceDirection = eDD_UPSIDE_DOWN;
#else
	eDeviceDirection					cGameApp::m_seDeviceDirection = eDD_LANDSCAPE_LEFT;
#endif

#ifdef WIN32
	cGameApp::cGameApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize)
#else
	cGameApp::cGameApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize)
#endif
	{
		if( !m_spLogFile )
		{
			m_spLogFile = new cBinaryFile();
#ifdef WIN32
			int	l_iValue = _mkdir("Log");
			//if(-1 != l_iValue)
			{
				std::wstring	l_strSystemTime = L"Log/log_";
				l_strSystemTime += UT::GetSystemTimeForFile(true);
				l_strSystemTime += L".txt";
				m_spLogFile->Writefile(UT::WcharToChar(l_strSystemTime.c_str()),false,false);
			}
#else
			m_spLogFile->Writefile("log.txt",false,false);
#endif
		}
		SystemErrorCheck();
#ifdef WIN32
		PrintMemoryInfo();
		ilInit();
		if( e_Hwnd )
		{
			OpenglInit(e_Hwnd);
			//m_sHdc = GetDC(e_Hwnd);
			m_sHandle = e_Hwnd;
		}
#endif
		memset(m_sucKeyData,0,sizeof(bool)*MAX_PATH);
        m_bMouseHover = false;

		m_svGameResolution = e_vGameResolution;
		//switch(cGameApp::m_seDeviceDirection)
		//{
		//	case eDD_PORTRAIT:
		//	case eDD_UPSIDE_DOWN:
		//		m_vGameResolution = e_vGameResolution;
		//		break;
		//	case eDD_LANDSCAPE_LEFT:
		//	case eDD_LANDSCAPE_RIGHT:
		//		m_vGameResolution.x = e_vGameResolution.y;
		//		m_vGameResolution.y = e_vGameResolution.x;
		//		break;
		//}
		m_svViewPortSize = e_vViewportSize;
		m_spMultiTouchPoints = new sMultiTouchPoints();
		//if(!m_spClickMouseBehavior)
		//	m_spClickMouseBehavior = new cClickMouseBehavior();
        if( !m_spAnimationParser )
        {
			m_spThreadPool = new cFUThreadPool;
	        m_spAnimationParser = new cAnimationParser();
			m_spImageParser = m_spAnimationParser->GetAllBaseImageList();
	        m_spSoundParser = new cSoundParser();
			m_spBehaviorPaticleManager = new cBehaviorPaticleManager();
			m_spPaticleManager = m_spBehaviorPaticleManager->m_pPaticleManager;
			m_spPathFileList = new cObjectListByName<cCurveManager>;
	    }
#if defined WIN32 || defined LINUX
	        char *locale;
            //locale = setlocale(LC_ALL, "");
#endif
		SystemErrorCheck();
	}

	cGameApp::~cGameApp()
	{
		SystemErrorCheck();
		Destroy();
#ifdef WIN32
		//PrintMemoryInfo();
		NamedTypedObject::DumpUnReleaseInfo();
#endif
		SystemErrorCheck();
		SAFE_DELETE(m_spLogFile);
		SAFE_DELETE(m_spThreadPool);
	}
#ifdef WIN32
	void	cGameApp::OpenglInit(HWND e_Hwnd)
	{
		m_sHdc = GetDC(e_Hwnd);
		UT::InitOpenGL(e_Hwnd,true,m_sHdc);
	}
#endif
	void	cGameApp::Init()
	{
		m_sbSpeedControl = this->m_sbDebugFunctionWorking;
		m_sbLeave = false;
		SystemErrorCheck();
		//This hint can improve the speed of texturing when perspective- correct texture coordinate interpolation isn't needed, such as when using a glOrtho() projection.
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
#ifdef OPENGLES_2_X
		g_bVBOSupported = true;
		glUseProgram( 0 );
#else
		glEnableClientState(GL_VERTEX_ARRAY);
#endif
		m_spLogFile->WriteToFileImmediatelyWithLine("init shader");
		//2d image shader
		CreateShader(g_bCommonVSClientState);
		//for non texture shader
		CreateShader(g_bCommonVSNoTextureClientState,NO_TEXTURE_SHADER);
		m_spLogFile->WriteToFileImmediatelyWithLine("init shader ok");
		m_sTimeAndFPS.Update();
		m_spLogFile->WriteToFileImmediatelyWithLine("parse font data");
		if( !m_spGlyphFontRender )
		{
			m_spGlyphFontRender = new cGlyphFontRender("Font");
			if(!m_spGlyphFontRender->GetFontImage())
				SAFE_DELETE(m_spGlyphFontRender);
		}
		if( !m_spGlyphFontRender)
			m_spLogFile->WriteToFileImmediatelyWithLine("parse font data failed");
		else
			m_spLogFile->WriteToFileImmediatelyWithLine("parse font data ok");
		if( !m_spPhaseManager )
			m_spPhaseManager = new cPhaseManager();
		if( !m_spstrErrorMsgString )
			m_spstrErrorMsgString = new std::wstring;
		//m_spClickMouseBehavior->SetCollisionRange(Vector4(0,0,this->m_svGameResolution.x,this->m_svGameResolution.y));
		m_sTimeAndFPS.Update();
		srand(m_sTimeAndFPS.uiCurrentTime);
		SystemErrorCheck();
	}

	void	cGameApp::Destroy()
	{
		m_spLogFile->WriteToFileImmediatelyWithLine("Destroy start");
		SystemErrorCheck();
		//SAFE_DELETE(m_spClickMouseBehavior);
		SAFE_DELETE(m_spPhaseManager);
		SAFE_DELETE(m_spMultiTouchPoints);
		SAFE_DELETE(m_spGlyphFontRender);
		SAFE_DELETE(m_spSoundParser);
		SAFE_DELETE(m_spstrErrorMsgString);
	    SAFE_DELETE(m_spAnimationParser);
		SAFE_DELETE(m_spBehaviorPaticleManager);
		SAFE_DELETE(m_spPathFileList);
		DeleteShader(NO_TEXTURE_SHADER);
		DeleteShader();
#ifndef __IPHONE__
		ilShutDown();
#endif
		SystemErrorCheck();
		m_spLogFile->WriteToFileImmediatelyWithLine("all Destroy done");
	}

	void	cGameApp::Update(float e_fElpaseTime)
	{
		SystemErrorCheck();
		m_spPhaseManager->Update(e_fElpaseTime);
		SystemErrorCheck();
	}

	void	cGameApp::Render()
	{
#ifndef OPENGLES_2_X
		glEnableClientState(GL_VERTEX_ARRAY);
#endif
		SystemErrorCheck();
		//the comment part is required if u are not using GameApp::Run
		//DEFAULT_SHADER = L"MyPrtShader";
		//NO_TEXTURE_SHADER =L"MyPrtShaderNoTexture";
		//cSimpleGLTexture::m_suiLastUsingImageIndex = -1;
		//FATMING_CORE::UseShaderProgram();
	    cSimpleGLTexture::m_suiLastUsingImageIndex = -1;
		m_spPhaseManager->Render();
		SystemErrorCheck();
	}

	void	cGameApp::Run()
	{
		m_sTimeAndFPS.Update();
		float	l_fElpaseTime = m_sTimeAndFPS.fElpaseTime;
		UseShaderProgram();
#ifdef _DEBUG
		if( m_sbGamePause )
			l_fElpaseTime = 0.f;
#endif
		Update(m_sbSpeedControl?l_fElpaseTime*this->m_sfDebugValue:l_fElpaseTime);
		Render();
	}

	void	cGameApp::MouseDown(int e_iPosX,int e_iPosY)
	{
		//if( !m_sbEnableMouseSingnal )
		//	return;
		m_sMousePosition = ConvertCoordinate(e_iPosX,e_iPosY,this->m_svViewPortSize);
		m_sbTouched = true;
		//m_spClickMouseBehavior->MouseDown(e_iPosX,e_iPosY);
		if(this->m_sb2Touched)
            return;        
		if( m_spPhaseManager )
			m_spPhaseManager->MouseDown(m_sMousePosition.x,m_sMousePosition.y);
	}

	void	cGameApp::MouseMove(int e_iPosX,int e_iPosY)
	{
		//if( !m_sbEnableMouseSingnal )
		//	return;
        m_sMousePosition = ConvertCoordinate(e_iPosX,e_iPosY,this->m_svViewPortSize);
		//m_spClickMouseBehavior->MouseMove(e_iPosX,e_iPosY);
        if(m_sb2Touched)
            return;
		if( m_spPhaseManager )
			m_spPhaseManager->MouseMove(m_sMousePosition.x,m_sMousePosition.y);
	}

	void	cGameApp::MouseUp(int e_iPosX,int e_iPosY)
	{
		//if( !m_sbEnableMouseSingnal )
		//	return;
        m_sMousePosition = ConvertCoordinate(e_iPosX,e_iPosY,this->m_svViewPortSize);
		m_sbTouched = false;
		//m_spClickMouseBehavior->MouseMove(e_iPosX,e_iPosY);
        if(m_sb2Touched)
            return;
		if( m_spPhaseManager )
			m_spPhaseManager->MouseUp(m_sMousePosition.x,m_sMousePosition.y);
	}
	
    void    cGameApp::TouchSignalProcess(int e_iPosX,int e_iPosY,bool e_bTouch)
    {
		//if( !m_sbEnableMouseSingnal )
		//	return;
	    if( e_bTouch )
	    {
		    if( m_bMouseHover )
			    MouseMove(e_iPosX,e_iPosY);
		    else
			    MouseDown(e_iPosX,e_iPosY);
		    m_bMouseHover = true;
		    return;
	    }
		else
			m_sMousePosition = ConvertCoordinate(e_iPosX,e_iPosY,this->m_svViewPortSize);
	    if(m_bMouseHover)
		    MouseUp(e_iPosX,e_iPosY);
	    m_bMouseHover = false;
    }

	void	cGameApp::KeyDown(char e_char)
	{
		//if(!m_sbEnableKeyboardSingnal)
		//	return;
		m_spPhaseManager->KeyDown(e_char);
		m_sucKeyData[e_char] = true;
	}

	void	cGameApp::KeyPress(char e_char)
	{
		//if(!m_sbEnableKeyboardSingnal)
		//	return;
		m_spPhaseManager->KeyPress(e_char);
		m_sucKeyData[e_char] = true;
	}

	void	cGameApp::KeyUp(char e_char)
	{
		//SHORT	l_ = GetAsyncKeyState('A');
		//cGameApp::KeyUp(e_char);
		//std::string	l_sss = UT::ComposeMsgByFormat("KeyValue:%s",&e_char);
		//std::wstring l_ss2 = UT::CharToWchar(l_sss.c_str());
		//OutputDebugString(l_ss2.c_str());
		//OutputDebugString(L"\n");

		//if(!m_sbEnableKeyboardSingnal)
		//	return;
	    if( e_char == 107 )//107'+'
		{
			m_sfDebugValue *= 2.f;
			if( m_sfDebugValue >1073741824 )
				m_sfDebugValue  = 1073741824;
		}
		else
		if( e_char == 109 )//109'-'
		{
			m_sfDebugValue /= 2.f;	
			if( m_sfDebugValue <= 0.00000000001f )
				m_sfDebugValue = 0.00000000001f;
		}
		m_spPhaseManager->KeyUp(e_char);
		if(m_sucKeyData['P'])
			m_sbGamePause = !m_sbGamePause;
		m_sucKeyData[e_char] = false;
	}
//end namespace
}

	//<root FullScreen="0" Resolution="960,640" ViewPort="960,640" DeviceOrietation="0" />
	void	cGameApp::ResoluctionParse(char*e_strFileName)
	{
		cNodeISAX	l_NodeISAX;
		bool	l_bFullScreen = false;
		bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(e_strFileName);
		if(l_b)
		{
			TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
			const WCHAR*l_strFullscreen = l_pTiXmlElement->Attribute(L"FullScreen");
			if( l_strFullscreen )
				m_sbFullScreen = _wtoi(l_strFullscreen)?true:false;
			const WCHAR*l_strResolution = l_pTiXmlElement->Attribute(L"Resolution");
			const WCHAR*l_strViewPort = l_pTiXmlElement->Attribute(L"ViewPort");
			const WCHAR*l_strDeviceOrietation = l_pTiXmlElement->Attribute(L"DeviceOrietation");
			const WCHAR*l_strHideMouseCursor = l_pTiXmlElement->Attribute(L"HideMouseCursor");
			const WCHAR*l_strTexturePowerOfTwo = l_pTiXmlElement->Attribute(L"TexturePowerOfTwo");
			const WCHAR*l_strDevice = l_pTiXmlElement->Attribute(L"Device");
			const WCHAR*l_strDebugFunctionWorking = l_pTiXmlElement->Attribute(L"DebugFunction");
			if( l_strDebugFunctionWorking )
			{
				m_sbDebugFunctionWorking = _wtoi(l_strDebugFunctionWorking)?true:false;
			}
			if( l_strDevice )
				m_sbDeviceExist = _wtoi(l_strDevice)?true:false;
			if( l_strResolution )
			{
				POINT	l_Resolution = GetPoint(l_strResolution);
				if( !l_strViewPort )
				{
					cGameApp::m_svViewPortSize.x = (float)l_Resolution.x;
					cGameApp::m_svViewPortSize.y = (float)l_Resolution.y;
				}
				cGameApp::m_svGameResolution.x = (float)l_Resolution.x;
				cGameApp::m_svGameResolution.y = (float)l_Resolution.y;
				//if(!m_spClickMouseBehavior)
				//	m_spClickMouseBehavior = new cClickMouseBehavior();
				//m_spClickMouseBehavior->SetCollisionRange(Vector4(0,0,m_svGameResolution.x,m_svGameResolution.y));
			}
			if( l_strViewPort )
			{
				POINT	l_Resolution = GetPoint(l_strViewPort);
				cGameApp::m_svViewPortSize.x = (float)l_Resolution.x;
				cGameApp::m_svViewPortSize.y = (float)l_Resolution.y;
			}
			if( l_strDeviceOrietation )
			{
				int	l_iIndex = _wtoi(l_strDeviceOrietation);
				if( l_iIndex <= eDD_LANDSCAPE_RIGHT )
					cGameApp::m_seDeviceDirection = (eDeviceDirection)l_iIndex;
			}
			if( l_strHideMouseCursor )
			{
#ifdef WIN32
				ShowCursor(_wtoi(l_strHideMouseCursor)==0?false:true);
#endif
			}
			if( l_strTexturePowerOfTwo )
			{
				g_bSupportNonPowerOfTwoTexture = _wtoi(l_strTexturePowerOfTwo)?true:false;
			}
		}
	}
	cPuzzleImage*	cGameApp::GetPuzzleImage(const WCHAR*e_strName){ return dynamic_cast<cPuzzleImage*>(m_spImageParser->GetObject(e_strName)); }
	cBaseImage*		cGameApp::GetBaseImage(const WCHAR*e_strName){  return dynamic_cast<cBaseImage*>(m_spImageParser->GetObject(e_strName));  }
	cPuzzleImage*	cGameApp::GetPuzzleImageByFileName(const WCHAR*e_strFileName){ return m_spImageParser->GetPuzzleImageByFileName(e_strFileName); }

	cMPDIList*		cGameApp::GetMPDIList(const WCHAR*e_strName){ return m_spAnimationParser->GetMPDIList(e_strName); }
	cMPDIList*		cGameApp::GetMPDIListByFileName(const WCHAR*e_strFileName){ return m_spAnimationParser->GetMPDIListByFileName(e_strFileName); }
	cMPDIList*		cGameApp::GetMPDIList(const char*e_strFileName,const WCHAR*e_strName){ return m_spAnimationParser->GetMPDIList(e_strFileName,e_strName); }
	cMPDIList*		cGameApp::GetMPDIList(int e_iIndex){ return m_spAnimationParser->GetMPDIList(e_iIndex); }

	cCurveManager*	cGameApp::GetCurveManagerByFileName(const WCHAR*e_strFileName)
	{
		WCHAR*l_strName = UT::GetFileNameWithoutFullPath(e_strFileName);
		cCurveManager*l_pCurveManager = m_spPathFileList->GetObject(l_strName);
		if( l_pCurveManager )
			return l_pCurveManager;
		l_pCurveManager = new cCurveManager();
		if(l_pCurveManager->Parse(UT::WcharToChar(e_strFileName)))
		{
			l_pCurveManager->SetName(l_strName);
			m_spPathFileList->AddObjectNeglectExist(l_pCurveManager);
			l_pCurveManager->DoScale(cGameApp::m_svGameScale);
			return l_pCurveManager;
		}
		delete l_pCurveManager;
		return 0;
	}


	cParticleEmitterGroup*cGameApp::GetPRTG(const char*e_strFileName,const WCHAR*e_strName){ return m_spBehaviorPaticleManager->GetPRTG(e_strFileName,e_strName); }
	cParticleEmitterGroup*cGameApp::GetPRTG(const WCHAR*e_strName){ return m_spBehaviorPaticleManager->GetPRTG(e_strName); }
	cPrtEmitter*cGameApp::GetPrtEmitter(const char*e_strFileName,const WCHAR*e_strName){ return m_spBehaviorPaticleManager->GetPaticleManager()->GetEmitter(e_strFileName,e_strName); }
	cPrtEmitter*cGameApp::GetPrtEmitter(const WCHAR*e_strName){ return m_spBehaviorPaticleManager->GetPaticleManager()->GetObject(e_strName); }

	bool	cGameApp::SoundPlay(const WCHAR*e_strSoundName,bool e_bPlay)
	{
		if( m_spSoundParser )
		{
			cBasicSound*l_pSound = m_spSoundParser->GetObject(e_strSoundName);
			if( !l_pSound )
				return false;
			l_pSound->Play(e_bPlay);
			return true;
		}
		return false;
	}


Vector2		cGameApp::GetViewPortAndGameResolutionScale()
{
	switch(cGameApp::m_seDeviceDirection)
	{
			case eDD_PORTRAIT:
			case eDD_UPSIDE_DOWN:
				return Vector2(m_svGameResolution.x/m_svViewPortSize.x,m_svGameResolution.y/m_svViewPortSize.y);
			case eDD_LANDSCAPE_LEFT:
			case eDD_LANDSCAPE_RIGHT:
				return Vector2(m_svGameResolution.x/m_svViewPortSize.y,m_svGameResolution.y/m_svViewPortSize.x);
	}
	return Vector2(1,1);
}


#ifdef WIN32
void PrintMemoryInfo(WCHAR*e_strName)
//void PrintMemoryInfo( DWORD processID )
{
	DWORD processID = GetCurrentProcessId();
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

 //   // Print the process identifier.
	WCHAR	l_str[MAX_PATH];
    swprintf(l_str,L"\nProcess ID: %u\n", processID );
	OutputDebugString(l_str);
    // Print information about the memory usage of the process.

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                             PROCESS_VM_READ,
                             FALSE, 
                             processID );
    if (NULL == hProcess)
        return;

    if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
    {
		static bool	l_sbFirstTimeIntoHere = false;
		static	int	l_sMemoryUsed = 0;
        //swprintf(l_str,L"\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount );

		//swprintf(l_str,L"\tPageFaultCount: %d\n", pmc.PageFaultCount );
		//OutputDebugString(l_str);
  //      swprintf(l_str,L"\tYour app's PEAK MEMORY CONSUMPTION: %d\n", 
  //                pmc.PeakWorkingSetSize );
		//OutputDebugString(l_str);
  //      swprintf(l_str,L"\tYour app's CURRENT MEMORY CONSUMPTION: %d\n", pmc.WorkingSetSize );
		OutputDebugString(L"Memory Check Name:");
		OutputDebugString(e_strName);
		OutputDebugString(L"\n");
		swprintf(l_str,L"Current Memory:%d\n",pmc.WorkingSetSize );
		OutputDebugString(l_str);
		if( !l_sbFirstTimeIntoHere )
		{
			l_sbFirstTimeIntoHere = true;
			l_sMemoryUsed = pmc.WorkingSetSize;
		}
		else
		{
			int	l_iMemoryChangeSize = pmc.WorkingSetSize-l_sMemoryUsed;
			float	l_fMB = (float)l_iMemoryChangeSize/1024.f/1024.f;
			float	l_fKB = (float)l_iMemoryChangeSize/1024.f;
			swprintf(l_str,L"Memory Change:In MB:%.2f,In KB:%.2f,In Byte:%d\n",l_fMB,l_fKB,l_iMemoryChangeSize );
			OutputDebugString(l_str);
			l_sMemoryUsed = pmc.WorkingSetSize;
		}
		//OutputDebugString(l_str);
  //      swprintf(l_str,L"\tQuotaPeakPagedPoolUsage: %d\n", 
  //                pmc.QuotaPeakPagedPoolUsage );
		//OutputDebugString(l_str);
  //      swprintf(l_str,L"\tQuotaPagedPoolUsage: %d\n", 
  //                pmc.QuotaPagedPoolUsage );
		//OutputDebugString(l_str);
  //      swprintf(l_str,L"\tQuotaPeakNonPagedPoolUsage: %d\n", 
  //                pmc.QuotaPeakNonPagedPoolUsage );
		//OutputDebugString(l_str);
  //      swprintf(l_str,L"\tQuotaNonPagedPoolUsage: %d\n", 
  //                pmc.QuotaNonPagedPoolUsage );
		//OutputDebugString(l_str);
  //      swprintf(l_str,L"\tPagefileUsage: %d\n", pmc.PagefileUsage ); 
		//OutputDebugString(l_str);
  //      swprintf(l_str,L"\tPeakPagefileUsage: %d\n", 
  //                pmc.PeakPagefileUsage );
		//OutputDebugString(l_str);
    }

    //CloseHandle( hProcess );
}
#endif