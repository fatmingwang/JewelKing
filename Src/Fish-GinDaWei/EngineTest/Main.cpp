/*
	Copyright (C) 2005-2009 Feeling Software Inc.
	Available only to licensees.
	Distribution of this file or its content is strictly prohibited.
*/

// FViewerSample.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
//#include "FViewerSample.h"
#include <FS3dViewer.h>
#include <FSCompositor.h>
#include <FSDefaultObjects.h>
#include <FSRenderer.h>
#include <FSRendererWidget.h>
#include <FSResourceManager.h>
#include <FSResourceRenderTarget.h>
#include <FCDocument/FCDocument.h>
#include <FCDocument/FCDLibrary.h>
#include <FCDocument/FCDSceneNode.h>
#include <FCDocument/FCDSceneNodeTools.h>
#include <FCDocument/FCDTransform.h>

#include "GameApp.h"
//#include "BulletEngine.h"
//#pragma comment(lib, "../../../lib/glew32.lib")
//#pragma comment(lib, "../../../lib/glut32.lib")

#pragma comment(lib, "../../../lib/Devil.lib")
//#pragma comment(lib, "../../../lib/glew.lib")

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib ")
#pragma comment(lib, "ole32.lib ")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "uuid.lib ")
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")
#pragma comment(lib, "../../../lib/Debug/BulletCollision.lib")
#pragma comment(lib, "../../../lib/Debug/BulletDynamics.lib")
#pragma comment(lib, "../../../lib/Debug/LinearMath.lib")

#ifdef _DEBUG

#pragma comment(lib, "../../../lib/debug/Core.lib")
#pragma comment(lib, "../../../lib/Debug/FCollada.lib")
#pragma comment(lib, "../../../lib/Debug/FViewer.lib")
//#pragma comment(lib, "../../../lib/Debug/FRenderFX.lib")
#pragma comment(lib, "../../../lib/Debug/Particle.lib")
//#pragma comment(lib, "../../lib/debug_FixedPipeline/FViewer.lib")
#else
#pragma comment(lib, "../../../lib/Release/Core.lib")
#pragma comment(lib, "../../../lib/Release/Particle.lib")
#pragma comment(lib, "../../../lib/Release/FCollada.lib")
#pragma comment(lib, "../../../lib/Release/FViewer.lib")
//#pragma comment(lib, "../../../lib/Release/FRenderFX.lib")
//#pragma comment(lib, "../../../lib/Release_FixedPipeline/FViewer.lib")
#endif


#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;								// current instance
HDC dc = NULL;									// Window's drawing context
HGLRC glrc = NULL;								// Window's opengl rendering context
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
HWND g_hWnd;
cGameApp*g_pGameApp = 0;
extern Vector2 g_vViewPortSize;
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	// TODO: Place code here.
	MSG msg;


	// Initialize global strings
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_FVIEWERSAMPLE, szWindowClass, MAX_LOADSTRING);
	swprintf(szTitle,MAX_PATH,L"Test\0");
	swprintf(szWindowClass,MAX_PATH,L"Test\0");
	WORD l_dw =	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

//	ShowWindow(hwnd, iCmdShow);
  //  UpdateWindow(hwnd);
	//HACCEL hAccelTable;
	//hAccelTable = LoadAccelerators(hInstance, L"Test");

	FCollada::Initialize();
	g_pGameApp = new cGameApp(g_hWnd);
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	FCollada::Release();

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

	wcex.cbSize = sizeof(wcex);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;//LoadIcon(NULL, IDI_APPLICATION);;
	wcex.hIconSm        = 0;//LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadIcon(NULL, IDI_APPLICATION);//LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);//(HBRUSH) GetStockObject(WHITE_BRUSH);
	wcex.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wcex.lpszClassName	= szWindowClass;
	wcex.lpszMenuName	= NULL;

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
   g_hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}


static uint32 width = 0;
static uint32 height = 0;

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		//SetTimer(hWnd, 1,  USER_TIMER_MINIMUM, NULL);
		break;

	case WM_TIMER: 
		{
			float time = (float) GetTickCount() / 300.0f;
			InvalidateRect(hWnd, NULL, false);
			if (g_pGameApp != NULL)
			{
				hdc = BeginPaint(hWnd, &ps);

				g_pGameApp->Run();
				//CameraUpDateByMouse(e);
				g_pGameApp->ResetCamera();

				EndPaint(hWnd, &ps);
			}
			break; 
		}
	case WM_PAINT:
		if (g_pGameApp != NULL)
		{
			hdc = BeginPaint(hWnd, &ps);

			g_pGameApp->Run();
			//CameraUpDateByMouse(e);
			g_pGameApp->ResetCamera();

			EndPaint(hWnd, &ps);
		}
		break;

	case WM_SIZE: {
		// Retrieve the new size
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		width = clientRect.right - clientRect.left;
		height = clientRect.bottom - clientRect.top;

		if (g_pGameApp == NULL)
		{
			g_pGameApp = new cGameApp(hWnd);
		}
		else
		{
			g_vViewPortSize = Vector2((float)width,(float)height);
			glViewport(0,0,width, height);
			glScissor(0,0,width, height);
		}

		// Set an appropriate camera..
		//fm::pvector<FCDEntityInstance> cameraInstances;
		//FCDSceneNodeTools::FindInstancesByType(cdoc->GetVisualSceneInstance(), FCDEntity::CAMERA, cameraInstances);
		//if (cameraInstances.size() > 0) rendererWidget->SetCurrentCamera(cameraInstances.front());

		break; }

	case WM_DESTROY:
		//KillTimer(hWnd, timerId);
		SAFE_DELETE(g_pGameApp);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}