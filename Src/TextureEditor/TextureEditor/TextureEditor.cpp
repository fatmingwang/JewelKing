// TextureEditor.cpp: 主要專案檔。

#include "stdafx.h"
#include "Form1.h"
#include "../../AllLibInclude.h"
#pragma comment(lib, "Vfw32.lib")
//p4 test
//#pragma comment(lib,"OpenGL32.lib")
//#pragma comment(lib,"odbc32.lib")
//#pragma comment(lib,"odbccp32.lib")
//#pragma comment(lib,"User32.lib")
//#pragma comment(lib,"gdi32.lib")
//#pragma comment(lib,"Vfw32.lib")
//#pragma comment(lib,"Glu32.lib")
//#pragma comment(lib, "Psapi.lib")
//#pragma comment(lib, "Vfw32.lib")
//#pragma comment(lib, "../../../lib/Devil.lib")
//#pragma comment(lib, "../../../lib/OpenAL32.lib")
//#pragma comment(lib, "../../../lib/alut.lib")	
//#pragma comment(lib, "../../../lib/ogg.lib")//I have build my self OGG lib here
//#pragma comment(lib, "../../../lib/glew32.lib")
//#ifdef _DEBUG
//#pragma comment(lib, "../../../lib/Debug/BulletCollision.lib")
//#pragma comment(lib, "../../../lib/Debug/BulletDynamics.lib")
//#pragma comment(lib, "../../../lib/Debug/LinearMath.lib")
//#else
//#pragma comment(lib, "../../../lib/Release/BulletCollision.lib")
//#pragma comment(lib, "../../../lib/Release/BulletDynamics.lib")
//#pragma comment(lib, "../../../lib/Release/LinearMath.lib")
//#endif
float	g_fTestValue = 1.f;
bool	g_bCtrlPressed = false;
bool	g_bDataChanged = false;
bool    g_bResolutionChanged = false;
using namespace TextureEditor;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// 建立任何控制項之前，先啟用 Windows XP 視覺化效果
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// 建立主視窗並執行
	Application::Run(gcnew Form1());
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return 0;
}
