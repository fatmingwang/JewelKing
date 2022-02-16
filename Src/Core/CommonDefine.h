#ifndef CommonDefine_H
#define CommonDefine_H

#include "PlafFormDefine.h"

#define	TEMP_SIZE			1024

//do the thing going to happen,if cvall back is assigned
typedef void    (*DoButtonGoal_Callback)(void*e_pData);

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_FREE
#define SAFE_FREE(p)       { if(p) { free (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

#define DELETE_VECTOR(List,T){for( UINT i = 0;i<List.size();++i){	T l_p = List[i];SAFE_DELETE(l_p);}List.clear();}
#define DELETE_POINTER_VECTOR(List,T){for( UINT i = 0;i<List->size();++i){	T l_p = (*List)[i];SAFE_DELETE(l_p);}SAFE_DELETE(List);}
//p for first q for second r for map object
#define DEL_MAP(p,q,r){											\
	std::map<p,q>::iterator i=r.begin();						\
	for(;i!=r.end();++i)										\
	{															\
		q l_Type2=i->second;									\
		SAFE_DELETE(l_Type2);									\
	}r.clear();}

#define RELEASE_MAP(p,q,r){											\
	std::map<p,q>::iterator i=r.begin();						\
	for(;i!=r.end();++i)										\
	{															\
		q l_Type2=i->second;									\
		SAFE_RELEASE(l_Type2);									\
	}r.clear();}

//Type Variable,GetName,SetName
#define GET_SET_DEC( T,V,p,q ) protected:T V;public:T p (){ return V; }void q (T value){ V = value; }
////Type Variable,GetName,SetName
//return pointer
#define GETP_SET_DEC( T,V,p,q ) protected:T V;public:T* p (){ return &V; }void q (T value){ V = value; }
//
#define GETPP_SET_DEC( T,V,p,q ) protected:T V;public:T** p (){ return &V; }void q (T value){ V = value; }
//
#define GET_SET_DEC_QUICK( TYPE, NAME )   TYPE	m_##NAME; void Set##NAME(TYPE e_Data ){m_##NAME = e_Data;} TYPE Get##NAME(){ return m_##NAME; }
//here the limit for string length is TEMP_SIZE = 255
#ifdef WIN32
#define	CHAR_TO_WCHAR( p,q )	 MultiByteToWideChar( CP_ACP, 0, p, -1, q, 512 );//q for return p for convert
#define	WCHAR_TO_CHAR( p,q )  WideCharToMultiByte( CP_ACP, 0, p, -1, q, 256, NULL, NULL );//q for return p for convert
#else
//here should using
//size_t iconv(iconv_t cd, char **restrict inbuf,
//       size_t *restrict inbytesleft, char **restrict outbuf,
//       size_t *restrict outbytesleft); 
//http://www.opengroup.org/onlinepubs/009695399/functions/iconv.html
#define CHAR_TO_WCHAR(char_,wchar_t_){ mbstowcs(wchar_t_,char_,TEMP_SIZE);}
#define WCHAR_TO_CHAR(wchar_t_,char_){wcstombs(char_,wchar_t_,TEMP_SIZE);}
#endif

#define UINT_ARGB(a,r,g,b) ((unsigned int)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define UINT_RGBA(r,g,b,a) UINT_ARGB(a,r,g,b)

//the velocities dont need to be normalised. 
//1.0E-6f means 1.0f * 10^-6, so 0.000001f. It's a tolerance that you need to avoid divides by 0.
#define	EPSIONAL	0.0001f

#define	FM	FATMING_CORE


//#define CORE_DLL
#ifdef CORE_DLL
#ifdef WIN32
// Disable the "private member not available for export" warning,
// because I don't feel like writing interfaces
//#pragma warning(disable:4251) 
#define CORE_EXPORT __declspec(dllexport)
//#elif defined(__IPHONE__) || defined(LINUX)
#else
#define CORE_EXPORT __attribute__((visibility("default")))
//#define FCOLLADA_LOCAL __attribute__((visibility("hidden")))
#endif
#else // FCOLLADA_DLL
//#define FCOLLADA_EXPORT
#endif // FCOLLADA_DLL

#endif