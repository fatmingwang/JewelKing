#ifndef UTILITY_H
#define UTILITY_H

#include "NamedTypedObject.h"
#include <map>
#include "Math/Collision.h"
#include "Glh.h"
#ifdef WIN32
#pragma warning( disable : 4996 )
#endif
namespace UT
{
	extern char		g_sstrForStringToStructure[TEMP_SIZE];
	extern WCHAR	g_sstrForStringToStructureW[TEMP_SIZE];
	void	MyGlErrorTest();
	struct sTimeAndFPS
	{
#ifdef __IPHONE__
		static double dbConversion;
#endif
		//calculate	elpase time
		UINT					uiPreviousTime;		//previously frame time ticks
		UINT					uiCurrentTime;		//current frame time ticks
		UINT					uiElpaseTime;		//current frame substract previously frame = elpase time
		float					fElpaseTime;		//uiElpaseTime/1000.f
		//for frame rate
		UINT					uiNumFrame;			//per second frame rate
		float					fTimeFor1Sec;		//detect is time reached 1 sec

		char					strFrameRate[16];

		sTimeAndFPS();
		void					Update();
		char* 					GetFPS();
		char* 					GetDynamicFPS();
	};
	//to calculate the destination time is reached or not,
	struct	sTimeCounter
	{
		bool					bLoop;//do not set this directly,instead by SetLoop
		float					fRestTime;
		float					fTargetTime;
		//while the target time is reached set to true,else false
		bool					bTragetTimrReached;
		sTimeCounter(){ bTragetTimrReached = true; fRestTime = fTargetTime = 1.f; bLoop = false; }
		sTimeCounter(float e_fTargetTime);
		void					Start(float e_fElpaseTime = 0.f);//it could be start by specific time that it have had past
		void					Update(float e_fElpaseTime);
		void					SetTargetTime(float e_fTargetTime);
		void					SetLoop(bool e_bLoop);
		//0 to 1.
		float					GetLERP();
	};
	//=============
	//only active once,if it has been actived,it won't be active until start is called again.
	//=============
	struct	sKeyTimeActive
	{
		bool					bTriggered;
		float					fTargetTime;
		//u can call this to reset active time
		void					Start(float e_fTargetTime){ fTargetTime = e_fTargetTime; bTriggered = false; }
		//ensure u have set dest time already
		void					Start(){ bTriggered = false; }
		inline	bool			IsActive(float e_fCurrentTime)
		{
			if( !bTriggered )
			{
				if( e_fCurrentTime>=fTargetTime ) 
				{
					bTriggered = true;
					return true;
				}
			}
			return false;
		}
	};
#ifdef WIN32
	HGLRC					InitOpenGL(HWND e_pHwnd,bool e_bInitGlewInit,HDC e_HdcMV);
	bool 					InitMultisample(HINSTANCE hInstance,HWND hWnd);
	bool 					IsExtensionSupported( char* szTargetExtension );
#endif
	eDataType				GetDataType(const WCHAR*e_str);
	eDataType				GetDataType(const char*e_str);
	//for loading data to ad root directory(Media/)
	inline char*			StringAddRootDirectory(char* e_str){ static char l_strTemp[TEMP_SIZE]; sprintf(l_strTemp,"Media/%s\0",e_str); return l_strTemp; }
	inline char*			ConstCharToChar(const char* e_str){ static char l_strTemp[TEMP_SIZE]; sprintf(l_strTemp,"%s\0",e_str);return l_strTemp; }
	std::wstring			ComposeMsgByFormat(WCHAR*e_strErrMsg1,...);
	std::string				ComposeMsgByFormat(char*e_strErrMsg1,...);
	void					ErrorMsg(const wchar_t*e_strErrMsg1,const wchar_t*e_strErrMsg2);
	void					ErrorMsg(char*e_strErrMsg1,char*e_strErrMsg2);
	void					ErrorMsg(const char*e_strErrMsg1,char*e_strErrMsg2);
	void					ErrorMsg(char*e_strErrMsg1,const char*e_strErrMsg2);
	void					ErrorMsg(const char*e_strErrMsg1,const char*e_strErrMsg2);	
	void					ErrorMsgByFormat(char*e_strErrMsg1,...);
	void					ErrorMsgByFormat(WCHAR*e_strErrMsg1,...);
	//
	void					SystemErrorCheck();
	WCHAR*					GetFileNameWithoutFullPath(const WCHAR*e_pString,bool e_bStripExtensionFileName = true);
	WCHAR*					GetFileExtensionName(WCHAR*e_pString);
	char*					GetFileNameWithoutFullPath(const char*e_pString,bool e_bStripExtensionFileName = true);
	char*					GetFileExtensionName(char*e_pString);
	char*					GetDirectoryWithoutFileName(const char*e_str);
	bool					IsFileExists( const char* FileName );
	bool					RemoveFile( const char* FileName );
	//ex:  view rang is 100,100,200,200
	//                  4 factor represent x,y,x,y coordinate
	Vector4					ViewRectToOpenGLScissor(Vector4 e_v2DViewRange,Vector2 e_vViewPort,Vector2 e_vGameResolution,eDeviceDirection e_eDeviceDirection);
	Vector4					ViewRectToOpenGLScissor(Vector4 e_v2DViewRange);
	//============
	//only support 4 characters
	int						HexadecimalStringToDecimal(char*e_strValue);
	int						DecimalStringToHexadcimal(char*e_strValue);	
	int						FindChacterIndexFromBackBuffer(char*e_String,char e_char);
	int						FindChacterIndexFromBackBuffer(WCHAR*e_String,WCHAR e_char);
	int						FindChacterIndexFromStartBuffer(WCHAR*e_String,WCHAR e_char,int e_iLookForIndex = 0);
	//
	long					GetFileSize( FILE*e_pFile);
	//if e_bCN is true return chinese format or return western format
	std::wstring			GetSystemTimeForFile(bool e_bCN);
	std::wstring			GetSystemTime(bool e_bCN);
	//====================
	//this is good for immediately to get pointer,but becareful!
	//because here using static variable
	//====================
	inline	void	WcharToChar(const wchar_t *e_strSrc,char*e_strDest)
	{
		assert(e_strDest&&"dest can't be NULL");
		assert(e_strSrc&&"the WCHAR string is NULL");
                memset(e_strDest,0,sizeof(char)*MAX_PATH);
		WCHAR_TO_CHAR(e_strSrc,e_strDest);
	}
	inline	void	WcharToChar(wchar_t *e_strSrc,char*e_strDest)
	{
		assert(e_strDest&&"dest can't be NULL");
		assert(e_strSrc&&"the WCHAR string is NULL");
                memset(e_strDest,0,sizeof(char)*MAX_PATH);
		WCHAR_TO_CHAR(e_strSrc,e_strDest);
	}
	inline	char*	WcharToChar(wchar_t *e_strWchar)
	{
		static char	l_sChar[TEMP_SIZE];
		assert(e_strWchar&&"the WCHAR string is NULL");
                memset(l_sChar,0,sizeof(char)*TEMP_SIZE);
		WCHAR_TO_CHAR(e_strWchar,l_sChar);
		return l_sChar;
	}

	inline	char*	WcharToChar(const wchar_t *e_strWchar)
	{
		static char	l_sChar[TEMP_SIZE];
		assert(e_strWchar&&"the WCHAR string is NULL");
                memset(l_sChar,0,sizeof(char)*TEMP_SIZE);
		WCHAR_TO_CHAR(e_strWchar,l_sChar);
		return l_sChar;
	}
	inline	void	CharToWchar(const char *e_strSrc,WCHAR*e_strDest)
	{
		assert(e_strDest&&"dest can't be NULL");
		assert(e_strSrc&&"the WCHAR string is NULL");
                memset(e_strDest,0,sizeof(WCHAR)*MAX_PATH);
		CHAR_TO_WCHAR(e_strSrc,e_strDest);
	}
	inline	void	CharToWchar(char *e_strSrc,WCHAR*e_strDest)
	{
		assert(e_strDest&&"dest can't be NULL");
		assert(e_strSrc&&"the WCHAR string is NULL");
                memset(e_strDest,0,sizeof(WCHAR)*MAX_PATH);
		CHAR_TO_WCHAR(e_strSrc,e_strDest);
	}
	inline	wchar_t*	CharToWchar(char *e_strChar){static wchar_t	l_sWchar[TEMP_SIZE];assert(e_strChar&&"the WCHAR string is NULL");memset(l_sWchar,0,sizeof(WCHAR)*TEMP_SIZE);CHAR_TO_WCHAR(e_strChar,l_sWchar);	return l_sWchar;}
	inline	wchar_t*	CharToWchar(const char *e_strChar){static wchar_t	l_sWchar[TEMP_SIZE];memset(l_sWchar,0,sizeof(WCHAR)*TEMP_SIZE);assert(e_strChar&&"the WCHAR string is NULL");CHAR_TO_WCHAR(e_strChar,l_sWchar);return l_sWchar;}
	//a vector contain a bunch data,now input a index it is possible bigger than last index or smaller 0,
	//so we have to find the proper one if we want it be loop
	inline	int				GetLoopIndex(int e_iIndex,int e_iTotalCount)
	{
		if( e_iTotalCount == 0 )
			return -1; 
		if( e_iIndex < 0 )
		{
			int	l_iIndex = e_iIndex%e_iTotalCount;
			return e_iTotalCount+l_iIndex;
		}
		if( e_iIndex >= e_iTotalCount )
		{
			int	l_iIndex = e_iIndex%e_iTotalCount;
			return l_iIndex;
		}
		return e_iIndex;
	}
	template <class T>inline	int		GetLoopIndex(int e_iIndex,std::vector<T>*e_pVector)
	{
		return GetLoopIndex(e_iIndex,(int)e_pVector->size());
	}
	//is index legal in the vector range
	template <class T>inline	bool	IsLegalIndexInVector(std::vector<T>*e_pVector,int e_iIndex){if( e_iIndex<0 )return false;if( e_iIndex>=(int)e_pVector->size() )return false;return true;	}
#ifdef WIN32
	inline	POINT			GetScreenResolution()
	{
		POINT	l_ScreenResolution = {GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN)};
		return l_ScreenResolution;

	}
#endif
	//==============================
	//move to destination from a to b with a time and get current move distance.
	//==============================
    template<class T>struct  sMoveToDestTimeCounter
    {
        T    				vCurrentMoveDistance;
        T    				vCurrentPos;
        T    				vEndPos;
		T    				vStartPos;
        UT::sTimeCounter    MoveToDestinationTC;
		void    			Start()
		{
			MoveToDestinationTC.Start();
		}
        void    			Start(T e_vStartPos,T e_vEndPos,float e_fTimeToDestination)
        {
            vStartPos = e_vStartPos;
            vEndPos = e_vEndPos;
			vCurrentPos = e_vStartPos;
			//set it to zero.
            this->vCurrentMoveDistance = e_vStartPos-e_vStartPos;
            this->MoveToDestinationTC.SetTargetTime(e_fTimeToDestination);
        }

        void    			Update(float e_fElpaseTime)
        {
            MoveToDestinationTC.Update(e_fElpaseTime);
            if(!this->MoveToDestinationTC.bTragetTimrReached)
            {
                vCurrentMoveDistance = (e_fElpaseTime*(1.f/MoveToDestinationTC.fTargetTime))*(vEndPos-vStartPos);
                vCurrentPos += vCurrentMoveDistance;
            }
            else
            {
				vCurrentMoveDistance = vEndPos-vCurrentPos;
                vCurrentPos = vEndPos;
            }
        }
		bool	IsDone(){ return MoveToDestinationTC.bTragetTimrReached; }
    };
	//do not support pointer data
	template <class TYPE>inline	void	InvertVectorDateOrder(std::vector<TYPE>*e_pVectorData)
	{
		int l_iLastIndex = (int)e_pVectorData->size()-1;
		//only 1 or 0 data is not necessery to invert
		if( l_iLastIndex <1 )
			return;
		UINT l_iSize = (UINT)e_pVectorData->size();
		std::vector<TYPE> l_OriginalPointList = *e_pVectorData;
		for( UINT i=0;i<l_iSize;++i )
		{
			l_OriginalPointList[l_iLastIndex-i] = (*e_pVectorData)[i];
		}
		*e_pVectorData = l_OriginalPointList;
	}
	//template <class Type>  inline	void	EraseVectorMemberByList(std::vector<Type>*e_pTypeVector,std::list<int> *e_pEraseIndexList)
	//{
	//	e_pEraseIndexList->sort();
	//	int	l_iGoAhead = 0;
	//	for( UINT i=0;i<e_pEraseIndexList->size();++i )
	//	{
	//		e_pTypeVector->erase(e_pTypeVector->begin()+e_pEraseIndexList[i]-l_iGoAhead);
	//		l_iGoAhead++;
	//	}
	//};
	//it dones't support pointer
	template <class Type1,class Type2> map<Type1,Type2>	SwapMapData(Type1 e_iIndex1,Type1 e_iIndex2,map<Type1,Type2> e_pMap)
	{
		Type2	Object1 = e_pMap[e_iIndex1];
		Type2	Object2 = e_pMap[e_iIndex2];
		e_pMap.erase(e_iIndex1);
		e_pMap.erase(e_iIndex2);
		e_pMap.insert(make_pair(e_iIndex1,Object2));
		e_pMap.insert(make_pair(e_iIndex2,Object1));
		return e_pMap;
	}

	//it dones't support pointer
	template <class Type1,class Type2> map<Type1,Type2>	ReplaceMapData(Type1 e_iIndex,Type2 e_Data,map<Type1,Type2> e_pMap)
	{
		e_pMap.erase(e_iIndex);
		e_pMap.insert(make_pair(e_iIndex,e_Data));
		return e_pMap;
	}
	template<class T>	inline	std::vector<int>	IndexListOfVector(T e_Object,std::vector<T>*e_pSrc)
	{
		std::vector<int>l_List;
		size_t	l_uiSize = e_pSrc->size();
		for(size_t i=0;i<l_uiSize;++i  )
		{
			if((*e_pSrc)[i] == e_Object)
				l_List.push_back(i);
		}
		return l_List;
	}
	template<class T>	inline	int		IndexOfVector(T e_Object,std::vector<T>*e_pSrc)
	{
		size_t	l_uiSize = e_pSrc->size();
		for(size_t i=0;i<l_uiSize;++i  )
		{
			if((*e_pSrc)[i] == e_Object)
				return i;
		}
		return -1;
	}
	template<class T>	inline	bool	IsVectorContain(T e_Object,std::vector<T>*e_pSrc)
	{
		size_t	l_uiSize = e_pSrc->size();
		for(size_t i=0;i<l_uiSize;++i  )
		{
			if((*e_pSrc)[i] == e_Object)
				return true;
		}
		return false;
	}
	template<class T>	inline	int	NumVectorContain(T e_Object,std::vector<T>*e_pSrc)
	{
		size_t	l_uiSize = e_pSrc->size();
		int	l_iNum = 0;
		for(size_t i=0;i<l_uiSize;++i  )
		{
			if((*e_pSrc)[i] == e_Object)
				++l_iNum;
		}
		return l_iNum;
	}
//end namespace Utility
}
using namespace UT;
#endif
