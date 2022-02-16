// test.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winsock.h>
#include "../EngineTest/stdafx.h"
#include "targetver.h"
#include "test.h"
#include "../EngineTest/GameApp.h"
#include "Windowsx.h"
#include "../../AllLibInclude.h"
#include "../../Core/GamePlayUT/Event/AllEventInclude.h"
#pragma comment(lib, "../../../lib/FPGAWinXPLib.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "iphlpapi.lib")
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR *szTitle = L"GemKing";
TCHAR *szWindowClass=L"GemKingWondowClass";
HWND g_hWnd;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
HHOOK MouseHook;
bool	g_bLeave = false;

cGameApp*g_pGameApp = 0;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
//http://www.codeproject.com/Articles/7831/Dual-Multiple-Monitor-Support-in-Win32
//int width  = GetSystemMetrics(SM_CXSCREEN);
//int height = GetSystemMetrics(SM_CYSCREEN);
////
//// size of screen in pixels excluding size of taskbar
////
//int width2 = GetSystemMetrics(SM_CXFULLSCREEN);
//int height2 = GetSystemMetrics(SM_CYFULLSCREEN);
////
//// or
////
//RECT rect;
//SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);

	ErrorMsgByFormat(L"%s%d",L"Test",123);

	ATOM	l_atom = MyRegisterClass(hInstance);
	Vector4	l_v = Vector4::One;
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	g_pGameApp = new cEngineTestApp(g_hWnd,cGameApp::m_svGameResolution,cGameApp::m_svViewPortSize);
	if( g_pGameApp  )
		g_pGameApp->Init();
	SetTimer (g_hWnd, 0, 0, NULL) ;
    //MouseHook = SetWindowsHookEx(WH_MOUSE_LL,MouseHookProc,hInstance,0);
	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)&&!g_bLeave)
	//while(1)
	{
		//g_pGameApp->Run();
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//UnhookWindowsHookEx(MouseHook);
	SAFE_DELETE(g_pGameApp);
	_CrtDumpMemoryLeaks();
	//NamedTypedObject::DumpUnReleaseInfo();
	return 0;
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(0,IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(0,IDI_APPLICATION);
	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
	bool	l_bFullScreen = false;
	cNodeISAX	l_NodeISAX;
	cGameApp::m_svViewPortSize.x = 1024.;
	cGameApp::m_svViewPortSize.y = 768.f;
	cGameApp::ResoluctionParse("JewelsKingSetup.xml");

	DWORD	l_dwFlag = WS_OVERLAPPEDWINDOW;
	if(cGameApp::m_sbFullScreen)
		l_dwFlag = WS_VISIBLE | WS_POPUP |	WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	g_hWnd = CreateWindow(szWindowClass, szTitle, l_dwFlag, 0, 0, (int)cGameApp::m_svViewPortSize.x, (int)cGameApp::m_svViewPortSize.y, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		DWORD	l_dw = GetLastError();
	  return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
POINT g_MousePosition;
bool	g_bLeftMouseDown = false;
bool	g_bRightMouseDown = false;
short	g_cMouseWhellDelta = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( !g_pGameApp )
		return DefWindowProc(hWnd, message, wParam, lParam);
	g_cMouseWhellDelta = 0;
	float   l_fScaleX = cGameApp::m_svGameResolution.x/cGameApp::m_svViewPortSize.x;
	float   l_fScaleY = cGameApp::m_svGameResolution.y/cGameApp::m_svViewPortSize.y;	
	switch (message)
	{
	case  WM_SIZE:
		cGameApp::m_svViewPortSize.x = (float)LOWORD(lParam);
		cGameApp::m_svViewPortSize.y = (float)HIWORD(lParam);
		break;
	case WM_TIMER:
		if( !g_bLeave && g_pGameApp )
		{
			g_pGameApp->Run();
		}
		else
			SAFE_DELETE(g_pGameApp);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//case WM_KEYFIRST:
	//	if( g_pGameApp )
	//	{
	//		g_pGameApp->KeyDown(LOWORD(wParam));
	//	}
	//	break;

	case WM_KEYDOWN:
		if( g_pGameApp )
		{
			g_pGameApp->KeyDown((char)LOWORD(wParam));
		}
		switch(LOWORD(wParam))
		{
			case VK_RETURN:
			{

			}
			break;
			case VK_ESCAPE:
				g_bLeave = true;
				break;
			break;
		}
		break;
	case WM_KEYUP:
		if( g_pGameApp )
		{
			g_pGameApp->KeyUp((char)LOWORD(wParam));
		}
		break;
	//case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
		g_cMouseWhellDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	case WM_LBUTTONDOWN:
		g_bLeftMouseDown = true;
		g_bRightMouseDown = false;
		if( g_pGameApp )
			g_pGameApp->MouseDown(g_MousePosition.x,g_MousePosition.y);
			return 0;
	case WM_LBUTTONUP:
		g_bLeftMouseDown = false;
		if( g_pGameApp )
			g_pGameApp->MouseUp(g_MousePosition.x,g_MousePosition.y);
			return 0;
	case WM_RBUTTONDOWN:
		g_bRightMouseDown = true;
		g_bLeftMouseDown = false;
			return 0;
	case WM_RBUTTONUP:
		g_bRightMouseDown = false;
			return 0;
	case WM_MOUSEMOVE:
		g_MousePosition.x = (int)(GET_X_LPARAM(lParam));
		g_MousePosition.y = (int)(GET_Y_LPARAM(lParam));
		g_pGameApp->m_sMousePosition = g_MousePosition;
		if( g_pGameApp )
			g_pGameApp->MouseMove(g_MousePosition.x,g_MousePosition.y);

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}