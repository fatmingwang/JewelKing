// FMBookEditor.cpp: 主要專案檔。

#include "stdafx.h"
#include "Form1.h"

#pragma comment(lib, "../../lib/Devil.lib")
#pragma comment(lib, "../../lib/OpenAL32.lib")
#pragma comment(lib, "../../lib/alut.lib")
#pragma comment(lib, "../../lib/ogg.lib")
#pragma comment(lib, "../../lib/glew32.lib")

#pragma comment(lib, "Vfw32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Urlmon.lib")
#ifdef _DEBUG
#ifdef OPENGLES_2_X
//#pragma comment(lib, "../../lib/Debug/Core.lib")
//#pragma comment(lib, "../../lib/Debug/Particle.lib")
#else
//#pragma comment(lib, "../../lib/DebugFixedPipeline/Core.lib")
//#pragma comment(lib, "../../lib/DebugFixedPipeline/Particle.lib")
#endif
#pragma comment(lib, "../../lib/Debug/BulletCollision.lib")
#pragma comment(lib, "../../lib/Debug/BulletDynamics.lib")
#pragma comment(lib, "../../lib/Debug/LinearMath.lib")
#else
//#pragma comment(lib, "../../lib/Release/FCollada.lib")
//#pragma comment(lib, "../../lib/Release/FViewer.lib")
//#pragma comment(lib, "../../lib/Release/FeelingLib.lib")
#ifdef OPENGLES_2_X
//#pragma comment(lib, "../../lib/Release/Core.lib")
//#pragma comment(lib, "../../lib/Release/Particle.lib")
#else
//#pragma comment(lib, "../../lib/ReleaseFixedPipeline/Core.lib")
//#pragma comment(lib, "../../lib/ReleaseFixedPipeline/Particle.lib")
#endif
#pragma comment(lib, "../../lib/Release/BulletCollision.lib")
#pragma comment(lib, "../../lib/Release/BulletDynamics.lib")
#pragma comment(lib, "../../lib/Release/LinearMath.lib")
#endif


using namespace FMBookEditor;

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
