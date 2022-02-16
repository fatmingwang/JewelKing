#include "../stdafx.h"
#include "Shader.h"

//gl_Color
//gl_FrontColor
//gl_BackColor

//the value here,1 represent sizeof(float)
int	g_iFVF_DataStride[TOTAL_FVF] = {
									3,			//pos,0
									3,			//normal,1
									4,			//diffuse,2
									3,			//tagent,3
									3,			//bitagent,4
									4,			//influnce skinning weight,5
									4,			//influnce skinning bone indicex,6,4 byte as 1 float
									2,			//UV1,7
									2};			//UV2,8

GLenum	g_iFVF_DataType[TOTAL_FVF] = {	
									GL_FLOAT,			//pos
									GL_FLOAT,			//normal
									GL_FLOAT,			//diffuse
									GL_FLOAT,			//tagent
									GL_FLOAT,			//bitagent
									GL_FLOAT,			//influnce skinning weight
									GL_UNSIGNED_BYTE,	//influnce skinning bone indicex
									GL_FLOAT,			//UV1
									GL_FLOAT};			//UV2

char*g_strShaderAttribution[TOTAL_FVF] = {"VSPosition",		//0
											"VSNormal",				//1
											"VSColor",				//2
											"VSTangent",			//3
											"VSBiTangent",			//4
											"VSWeights",			//5
											"VSInflunceBoneIndices",//6
											"VSTexcoord",			//7
											"VSTexcoord1"};			//8
//http://developer.apple.com/library/ios/#documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/OpenGLESPlatforms/OpenGLESPlatforms.html
//OpenGL ES 2.0 on the PowerVR SGX
//Limits
//
//The maximum 2D or cube map texture size is 2048 x 2048. This is also the maximum renderbuffer size and viewport size.
//You can use up to 8 textures in a fragment shader. You cannot use texture lookups in a vertex shader.
//You can use up to 16 vertex attributes.
//You can use up to 8 varying vectors.
//You can use up to 128 uniform vectors in a vertex shader and up to 64 uniform vectors in a fragment shader.
//Points can range in size from 1.0 to 511.0 pixels.
//Lines can range in width from 1.0 to 16.0 pixels.
namespace FATMING_CORE
{
	//														pos, normal,diffuse,tangent,bitangent,  wight,bone,tex0, tex1
	bool	g_bCommonVSClientState[TOTAL_FVF]			=	{true,false, true,   false,  false,    false,false,true, false};
	bool	g_bCommonVSNoTextureClientState[TOTAL_FVF]	=	{true,false, false,  false,  false,    false,false,false,false};
	bool	g_bMyMeshVSClientState[TOTAL_FVF]			=	{true,true , false,  false,  false,    false,false,true, false};
	bool	g_bMySkinningMeshVSClientState[TOTAL_FVF]	=	{true,true , false,  false,  false,    false,false,true, false};
	//tex,color,pos
	WCHAR*DEFAULT_SHADER = L"Default";
	//pos only
	WCHAR*NO_TEXTURE_SHADER = L"NoTexture";
	//precision mediump is not support in the MACbook Air.....fuck
	//and iphone shader should use lowp instead dsclare precision mediump ,
	//because mesh shader occur crash......fuck.
	char*g_strCommonVS = "									\
	attribute vec3 VSPosition;								\
	attribute vec3 VSNormal;								\
	attribute vec4 VSColor;									\
	attribute vec2 VSTexcoord;								\
	uniform mat4 matVP;										\
	uniform mat4 matW;										\
	varying vec2 PSTexcoord;								\
	varying vec4 PSColor;									\
	void main()												\
	{														\
		gl_Position = matVP*matW*vec4(VSPosition,1);		\
		PSTexcoord = VSTexcoord;							\
		PSColor = VSColor;									\
	}";
#ifdef __IPHONE__
	char*g_strCommonFS = "										\
	uniform sampler2D texSample;								\
	varying lowp vec2 PSTexcoord;								\
	varying lowp vec4 PSColor;									\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;\
	}";
#else
	char*g_strCommonFS = "										\
	uniform sampler2D texSample;								\
	varying vec2 PSTexcoord;									\
	varying vec4 PSColor;										\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;\
	}";
#endif

	char*g_strColorFulFS = "													\
	uniform sampler2D texSample;												\
	varying vec2 PSTexcoord;													\
	varying vec4 PSColor;														\
	uniform vec4 MyColor;														\
																				\
	void main()																	\
	{																			\
		gl_FragColor = texture2D(texSample, PSTexcoord);						\
		gl_FragColor.xyz = gl_FragColor.xyz+(PSColor.xyz-vec3(0.5,0.5,0.5));	\
	}";
//=======================
//
//=======================
	char*g_strCommonVSNoTexture = "							\
	attribute vec3 VSPosition;								\
	attribute vec3 VSNormal;								\
	attribute vec4 VSColor;									\
	attribute vec2 VSTexcoord;								\
	uniform mat4 matVP;										\
	uniform mat4 matW;										\
	void main()												\
	{														\
		gl_Position = matVP*matW*vec4(VSPosition,1);		\
	}";
#ifdef __IPHONE__
	char*g_strCommonFSNoTexture = "							\
	uniform lowp vec4 PSColor;								\
															\
	void main()												\
	{														\
		gl_FragColor = PSColor;								\
	}";
#else
	char*g_strCommonFSNoTexture = "							\
	uniform vec4 PSColor;									\
	void main()												\
	{														\
		gl_FragColor = PSColor;								\
	}";
#endif
//=======================
//for mesh
//=======================
	char*g_strMyMeshVS = "									\
	attribute vec3 VSPosition;								\
	attribute vec3 VSNormal;								\
	attribute vec4 VSColor;									\
	attribute vec2 VSTexcoord;								\
	uniform mat4 matVP;										\
	uniform mat4 matW;										\
	varying vec2 PSTexcoord;								\
	varying vec3 PSNormal;									\
	void main()												\
	{														\
		gl_Position = matVP*matW*vec4(VSPosition,1);		\
		PSTexcoord = VSTexcoord;							\
		PSNormal = VSNormal;								\
	}";

#ifdef __IPHONE__
	char*g_strMyMeshFS = "										\
	uniform sampler2D texSample;								\
	varying lowp vec4 PSColor;									\
	varying lowp vec2 PSTexcoord;								\
	varying lowp vec3 PSNormal;									\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;\
	}";
#else
	char*g_strMyMeshFS = "										\
	uniform sampler2D texSample;								\
	uniform vec4 PSColor;										\
	varying vec2 PSTexcoord;									\
	varying vec3 PSNormal;										\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;\
	}";
#endif
	char*g_strMySkinningMeshVS = "							\
	attribute	vec3	VSPosition;							\
	attribute	vec3	VSNormal;							\
	attribute	vec4	VSColor;							\
	attribute	vec2	VSTexcoord;							\
    attribute	vec4	VSWeights;							\
    attribute	vec4	VSInflunceBoneIndices;				\
	uniform		mat4	matVP;								\
	uniform		mat4	matW;								\
	uniform		mat4	matBones[200];						\
	varying		vec2	PSTexcoord;							\
	varying		vec3	PSNormal;							\
	void main()												\
	{																						\
		gl_Position = vec4(0,0,0,0);														\
		PSNormal = vec3(0,0,0);																\
		vec4 curIndex = VSInflunceBoneIndices;												\
		vec4 curWeight = VSWeights;															\
		for( int i=0;i<4;i++ )																\
		{																					\
			if( curWeight.x>0.000001 )														\
			{																				\
				mat4 m44 = matBones[int(curIndex.x)];										\
				gl_Position += m44 * vec4(VSPosition,1) * curWeight.x;						\
				mat3 m33 = mat3(m44[0].xyz,													\
								m44[1].xyz,													\
								m44[2].xyz);												\
				PSNormal += m33 * VSNormal * curWeight.x;									\
				curIndex = curIndex.yzwx;													\
				curWeight = curWeight.yzwx;													\
			}																				\
		}																					\
		gl_Position = matVP *matW* gl_Position;												\
		PSTexcoord = VSTexcoord;															\
		PSNormal = VSNormal;																\
	}";
#ifdef __IPHONE__
	char*g_strMySkinningMeshFS = "								\
	uniform sampler2D texSample;								\
	varying lowp vec4 PSColor;									\
	varying lowp vec2 PSTexcoord;								\
	varying lowp vec3 PSNormal;									\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;\
	}";
#else
	char*g_strMySkinningMeshFS = "								\
	uniform sampler2D texSample;								\
	uniform vec4 PSColor;										\
	varying vec2 PSTexcoord;									\
	varying vec3 PSNormal;										\
																\
	void main()													\
	{															\
		gl_FragColor = texture2D(texSample, PSTexcoord)*PSColor;\
	}";
#endif
//end namespace FATMING_CORE
}
//end _COMMON_SHADER_H_