// ParticalEditor.cpp: 主要專案檔。

#include "stdafx.h"
#include "Form1.h"
#pragma comment(lib, "../../../lib/Devil.lib")
#pragma comment(lib, "../../../lib/OpenAL32.lib")
#pragma comment(lib, "../../../lib/alut.lib")
#pragma comment(lib, "../../../lib/ogg.lib")//I have build my self OGG lib here
#pragma comment(lib, "../../../lib/glew32.lib") 
#pragma comment(lib, "Psapi.lib")

#ifdef _DEBUG
	#ifndef OPENGLES_2_X
		//#pragma comment(lib, "../../../lib/DebugFixedPipeline/Core.lib")
	#else
		//#pragma comment(lib, "../../../lib/Debug/Core.lib")
	#endif
	#pragma comment(lib, "../../../lib/Debug/BulletCollision.lib")
	#pragma comment(lib, "../../../lib/Debug/BulletDynamics.lib")
	#pragma comment(lib, "../../../lib/Debug/LinearMath.lib")
#else
	#ifndef OPENGLES_2_X
		#pragma comment(lib, "../../../lib/ReleaseFixedPipeline/Core.lib")
	#else
		#pragma comment(lib, "../../../lib/Release/Core.lib")
	#endif
	#pragma comment(lib, "../../../lib/Release/BulletCollision.lib")
	#pragma comment(lib, "../../../lib/Release/BulletDynamics.lib")
	#pragma comment(lib, "../../../lib/Release/LinearMath.lib")
#endif
using namespace ParticalEditor;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	Application::Run(gcnew Form1());
	NamedTypedObject::DumpUnReleaseInfo();
	_CrtDumpMemoryLeaks();
	return 0;
}
