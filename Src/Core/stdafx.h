#ifndef StdAfx_H
#define StdAfx_H

#ifdef WIN32
#define USE_STANDARD_GL
#else
#ifdef LINUX
#else
#ifndef __IPHONE__
#define __IPHONE__
#endif
#endif
#endif
//#define LINUX

#ifdef WIN32
#pragma warning( disable : 4996 )
#include "windows.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <math.h>
#include <wchar.h>

#ifdef WIN32
#include "../../Include/glew.h"
#include "../../Include/glext.h"
#else
#ifdef __IPHONE__
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include "glew.h"
#include "glext.h"
#endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <assert.h>
#include "GameplayUT/GameUTDefine.h"
#include "CommonDefine.h"
#include "NamedTypedObject.h"
#include "TemplateClass.h"
#include "XML/StringToStructure.h"
#include "AllMathInclude.h"
#include "Utility.h"
//#include "Sound/SoundManager.h"

#endif
