#ifndef ALL_CORE_INCLUDE
#define	ALL_CORE_INCLUDE

#ifdef WIN32
#else
#ifdef LINUX
#else
#ifndef __IPHONE__
#define __IPHONE__
#endif
#endif
#endif


#ifdef WIN32
#pragma warning( disable : 4996 )
#include "windows.h"
#include <math.h>
#include "XML/AtgXmlWriter.h"
#endif
#ifdef WIN32
#include "../../Include/glew.h"
#include "../../Include/glext.h"
#else
#ifdef __IPHONE__
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#else
#include "glew.h"
#include "glext.h"
#endif
#endif
//<string.h> is for c, <string> is for c++
#include <string>
#include <vector>
#include "GameplayUT/GameUTDefine.h"
#include "CommonDefine.h"
#include "NamedTypedObject.h"
#include "SmartPointer.h"
#include "TemplateClass.h"
#include "TwoDimensionDataContainer.h"
#include "MapCellDataContainer.h"
#include "AllMathInclude.h"
#include "XML/StringToStructure.h"
#include "Utility.h"
#include "Math/MathUtility.h"
#include "XML/XMLLoader.h"
#include "Camera/SimpleCamera.h"
#include "Camera/MouseCamera.h"
#include "Camera/OrthogonalCamera.h"
#include "Camera/SplitScreenCamera.h"
#include "AllImageInclude.h"
#include "AllMathInclude.h"
//#ifndef __IPHONE__
#include "Sound/SoundManager.h"
//#endif
//#ifndef __IPHONE__
//#include "AVIPlayer/AVIPlayer.h"
//#endif
//
#ifdef  WIN32
#include "GLSL/GLSLUiniform.h"
#endif
#ifdef  OPENGLES_2_X//iphone 3g only support openGLES 1.5,android up to 2.0
#include "GLSL/GLSLUiniform.h"
#endif
#include "GLSL/Shader.h"
#include "GameplayUT/AllGamePlayUTInclude.h"
//#include "AllPhysicInclude.h"
//#include "Particle/AllParticleInclude.h"
using namespace FATMING_CORE;

#endif