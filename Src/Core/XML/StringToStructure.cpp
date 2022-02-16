#include "../stdafx.h"
#include "StringToStructure.h"
#include "../Utility.h"

#ifdef WIN32
#pragma warning( disable : 4996 )
#endif
namespace FATMING_CORE
{
	POINT	GetPoint(const WCHAR* e_str)
	{
		swprintf(g_sstrForStringToStructureW,TEMP_SIZE,L"%ls\0",e_str);
		POINT	l_Point;
		WCHAR*	l_str = wcstok(g_sstrForStringToStructureW,L",: ");
		assert(l_str&&"parse POINT data error");
		l_Point.x = _wtol(l_str);
		l_str = wcstok(0,L",: ");
		assert(l_str&&"parse POINT data error");
		l_Point.y = _wtol(l_str);
		return l_Point;
	}

	POINT	GetPoint(const char* e_str)
	{
		return GetPoint((char*)e_str);
	}

	POINT	GetPoint(char* e_str)
	{
		sprintf(g_sstrForStringToStructure,"%s\0",e_str);
		POINT	l_Point;
		char*	l_str = strtok(g_sstrForStringToStructure,",: ");
		assert(l_str&&"parse POINT data error");
		l_Point.x = atol(l_str);
		l_str = strtok(0,",: ");
		assert(l_str&&"parse POINT data error");
		l_Point.y = atol(l_str);
		return l_Point;
	}

	float	GetFloat(const char* e_str)
	{
		return (float)atof(e_str);
	}

	float	GetFloat(const WCHAR* e_str)
	{
		return (float)_wtof(e_str);
	}

	int		GetInt(const char* e_str)
	{
		return atoi(e_str);
	}

	int		GetInt(const WCHAR* e_str)
	{
		return _wtoi(e_str);
	}

	uint64	GetUint64(const WCHAR* e_str)
	{
		return static_cast<uint64>(_atoi64(UT::WcharToChar(e_str)));
	}

	uint64	GetUint64(const char* e_str)
	{
		return static_cast<uint64>(_atoi64(e_str));
	}

	int64	GetInt64(const WCHAR* e_str)
	{
		return _atoi64(UT::WcharToChar(e_str));
	}

	int64	GetInt64(const char* e_str)
	{
		return _atoi64(e_str);
	}

	RECT	GetRectangle(char*e_pString)
	{
		return GetRectangle(CharToWchar(e_pString));
	}

	RECT	GetRectangle(const WCHAR*e_pString)
	{
		RECT	l_rect;
		WCHAR*	l_pNext = 0;
	    WCHAR    l_str[MAX_PATH];
	    swprintf(l_str,TEMP_SIZE,L"%s\0",e_pString);
		WCHAR*	l_pCurrent = wcstok(l_str,L",:");
		int	l_i = 0;
		//0left 1top 2right 3bottom
		while( l_i != 4 )
		{
			if( l_i == 0 )
			{
				l_rect.left = wcstol(l_pCurrent,&l_pNext,10);
			}
			else
			if( l_i == 2 )
			{
				l_rect.right = wcstol(l_pCurrent,&l_pNext,10);

			}
			else
			if( l_i == 3 )
			{
				l_rect.bottom = wcstol(l_pCurrent,&l_pNext,10);
			}
			else
			if( l_i == 1 )
			{
				l_rect.top = wcstol(l_pCurrent,&l_pNext,10);
			}
			l_i++;
			if( l_i !=4 )
			{
				l_pCurrent = wcstok(0,L":,");
				assert(l_pCurrent&&"rect data is wrong in the TextureLoad");
			}
		}
		return l_rect;
	}
	//L,R,D,T  left right down top
	//							  here is secomd
	//L:0,0,R:1,0,D:0,1,T:1,1,L:0,0,R:1,0,D:0,1,T:1,1
	RECT*	GetRectangle(int e_iNumRect,WCHAR*e_pString)
	{
		WCHAR*  l_strTemp = 0;
		WCHAR*	l_pCurrent = wcstok(e_pString,L",:");
		RECT*	l_pRect = new RECT[e_iNumRect];
		for(int i=0;i<e_iNumRect;++i)
		{
			RECT	l_rect;
			int	l_i = 0;
			while( l_i != 4 )
			{
				if( !wcscmp(l_pCurrent,L"L") )
				{
					l_rect.left = wcstol(l_pCurrent,&l_strTemp,10);
				}
				else
				if( !wcscmp(l_pCurrent,L"R") )
				{
					l_rect.right = wcstol(l_pCurrent,&l_strTemp,10);
				}
				else
				if( !wcscmp(l_pCurrent,L"D") )
				{
					l_rect.bottom = wcstol(l_pCurrent,&l_strTemp,10);
				}
				else
				if( !wcscmp(l_pCurrent,L"T") )
				{
					l_rect.top = wcstol(l_pCurrent,&l_strTemp,10);
				}
				l_i++;
				l_pCurrent = wcstok(0,L",;");
			}
			l_pRect[i] = l_rect;
		}
		assert(!l_pCurrent&&"rect data is wrong in the TextureLoad");
		return l_pRect;
	}


	Vector4	GetVector4(char*e_strValue)
	{
	    char    l_strValue[MAX_PATH];
	    sprintf(l_strValue,"%s\0",e_strValue);
		Vector4	l_Vector4;
		char*	l_str = strtok(l_strValue,",: ");
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.x = (float)atof(l_str);
		l_str = strtok(0,",: ");
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.y = (float)atof(l_str);
		l_str = strtok(0,",:" );
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.z = (float)atof(l_str);
		l_str = strtok(0,",:" );
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.w = (float)atof(l_str);
		return l_Vector4;
	}

	Vector3	GetVector3(char*e_str)
	{
	    char    l_strValue[MAX_PATH];
	    sprintf(l_strValue,"%s\0",e_str);
		Vector3	l_Vector3(0.f,0.f,0.f);
		char*	l_str = strtok(l_strValue,",: ");
		assert(l_str&&"parse Vector3 data error");
		l_Vector3.x = (float)atof(l_str);
		l_str = strtok(0,",: ");
		assert(l_str&&"parse Vector3 data error");
		l_Vector3.y = (float)atof(l_str);
		l_str = strtok(0,",:" );
		if( l_str )//because we may only need xy no z
		{
			assert(l_str&&"parse Vector3 data error");
			l_Vector3.z = (float)atof(l_str);
		}
		return l_Vector3;
	}

	Vector2	GetVector2(char*e_strValue)
	{
	    char    l_strValue[MAX_PATH];
	    sprintf(l_strValue,"%s\0",e_strValue);
		Vector2	l_Vector2;
		char*	l_str = strtok(l_strValue,",: ");
		assert(l_str&&"parse Vector2 data error");
		l_Vector2.x = (float)atof(l_str);
		l_str = strtok(0,",: ");
		assert(l_str&&"parse Vector2 data error");
		l_Vector2.y = (float)atof(l_str);
		return l_Vector2;
	}


	Vector4	GetVector4(const WCHAR*e_strValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%ls\0",e_strValue);
		Vector4	l_Vector4;
		WCHAR*	l_str = wcstok(g_sstrForStringToStructureW,L",: ");
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.x = (float)_wtof(l_str);
		l_str = wcstok(0,L",: ");
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.y = (float)_wtof(l_str);
		l_str = wcstok(0,L",:" );
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.z = (float)_wtof(l_str);
		l_str = wcstok(0,L",:" );
		assert(l_str&&"parse Vector4 data error");
		l_Vector4.w = (float)_wtof(l_str);
		return l_Vector4;
	}

	Vector3	GetVector3(const WCHAR*e_strValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%ls\0",e_strValue);
		Vector3	l_Vector3(0.f,0.f,0.f);
		WCHAR*	l_str = wcstok(g_sstrForStringToStructureW,L",: ");
		assert(l_str&&"parse Vector3 data error");
		l_Vector3.x = (float)_wtof(l_str);
		l_str = wcstok(0,L",: ");
		assert(l_str&&"parse Vector3 data error");
		l_Vector3.y = (float)_wtof(l_str);
		l_str = wcstok(0,L",:" );
		if( l_str )//because we may only need xy no z
		{
			assert(l_str&&"parse Vector3 data error");
			l_Vector3.z = (float)_wtof(l_str);
		}
		return l_Vector3;
	}

	Vector2	GetVector2(const WCHAR*e_strValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%ls\0",e_strValue);	
		Vector2	l_Vector2;
		WCHAR*	l_str = wcstok(g_sstrForStringToStructureW,L",: ");
		assert(l_str&&"parse Vector2 data error");
		l_Vector2.x = (float)_wtof(l_str);
		l_str = wcstok(0,L",: ");
		assert(l_str&&"parse Vector2 data error");
		l_Vector2.y = (float)_wtof(l_str);
		return l_Vector2;
	}

	cMatrix44	GetMatrix(const WCHAR*e_str,bool e_bTranspose)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%ls\0",e_str);
		cMatrix44	l_mat;
		WCHAR*	l_str = wcstok(g_sstrForStringToStructureW,L",: ");
		for( int i=0;i<4;++i )
		{
				for( int j=0;j<4;++j )
				{
						l_mat.m[j][i] = (float)_wtof(l_str);
						l_str = wcstok(0,L",: ");
				}
		}
		assert(!l_str);
		if( e_bTranspose )
				return l_mat.Transposed();
		return l_mat;
	}

	cMatrix44	GetMatrix(const char*e_str,bool e_bTranspose)
	{
	    char    l_strValue[MAX_PATH];
	    sprintf(l_strValue,"%s\0",e_str);	
		cMatrix44	l_mat;
		char*	l_str = strtok((char*)l_strValue,",: ");
		for( int i=0;i<4;++i )
		{
			for( int j=0;j<4;++j )
			{
				l_mat.m[j][i] = (float)atof(l_str);
				l_str = strtok(0,",: ");
			}
		}
		assert(!l_str);
		if( e_bTranspose )
			return l_mat.Transposed();
		return l_mat;
	}
    //this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const WCHAR*e_str,int e_iSize,bool e_bTranspose)
	{
		cMatrix44*l_pMat = new cMatrix44[e_iSize];
		WCHAR*	l_str = wcstok((WCHAR*)e_str,L",: ");
		for( int i=0;i<e_iSize;++i )
		{
			for( int j=0;j<4;++j )
			{
				for( int k=0;k<4;++k )
				{
					(l_pMat[i]).m[k][j] = (float)_wtof(l_str);
					l_str = wcstok(0,L",: ");
				}
			}
			if(e_bTranspose)
				(l_pMat[i]) = (l_pMat[i]).Transposed();
		}
		assert(!l_str);
		return l_pMat;
	}
    //this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const char*e_str,int e_iSize,bool e_bTranspose)
	{
		cMatrix44*l_pMat = new cMatrix44[e_iSize];
		char*	l_str = strtok((char*)e_str,",: ");
		for( int i=0;i<e_iSize;++i )
		{
			for( int j=0;j<4;++j )
			{
				for( int k=0;k<4;++k )
				{
					(l_pMat[i]).m[k][j] = (float)atof(l_str);
					l_str = strtok(0,",: ");
				}
			}
			if(e_bTranspose)
				(l_pMat[i]) = (l_pMat[i]).Transposed();
		}
		assert(!l_str);
		return l_pMat;
	}

	void	GetUV(const WCHAR*e_pData,float*e_pUVBuffer)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%ls\0",e_pData);
		WCHAR*	l_str = wcstok(g_sstrForStringToStructureW,L",: ");
		for( int i=0;i<4;++i )
		{
			e_pUVBuffer[i] = (float)_wtof(l_str);
			l_str = wcstok(0,L",: ");
		}
	}

	void	GetUV(char*e_pData,float*e_pUVBuffer)
	{
		sprintf(g_sstrForStringToStructure,"%s\0",e_pData);
		char*	l_str = strtok(g_sstrForStringToStructure,",: ");
		for( int i=0;i<4;++i )
		{
			e_pUVBuffer[i] = (float)atof(l_str);
			l_str = strtok(0,",: ");
		}
	}

	void	GetUV(const char*e_pData,float*e_pUVBuffer)
	{
		GetUV((char*)e_pData,e_pUVBuffer);
	}

	std::vector<int>	GetIntegerListByCommaDivide(const WCHAR*e_str,int e_iSize)
	{
		std::vector<int>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		WCHAR*	l_str = wcstok((WCHAR*)e_str,L", ");
		while(l_str)
		{
	#ifdef _DEBUG
			int	l_i = _wtoi(l_str);
			l_NumerialList.push_back(l_i);
	#else
			l_NumerialList.push_back(_wtoi(l_str));
	#endif
			l_str = wcstok(0,L", ");
		}
		return l_NumerialList;	
	}


	std::vector<float>	GetFloatListByCommaDivide(const WCHAR*e_str,int e_iSize)
	{
		std::vector<float>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		WCHAR*	l_str = wcstok((WCHAR*)e_str,L", ");
		while(l_str)
		{
	#ifdef _DEBUG
			float	l_f = (float)_wtof(l_str);
			l_NumerialList.push_back(l_f);
	#else
			l_NumerialList.push_back((float)_wtof(l_str));
	#endif
			l_str = wcstok(0,L", ");
		}
#ifdef _DEBUG
		if( e_iSize )
		{
			assert(l_NumerialList.size()==e_iSize);
		}
#endif
		return l_NumerialList;
	}

	std::vector<double>	GetDoubleListByCommaDivide(const WCHAR*e_str,int e_iSize)
	{
		std::vector<double>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		WCHAR*	l_str = wcstok((WCHAR*)e_str,L", ");
		while(l_str)
		{
	#ifdef _DEBUG
			double	l_db = _wtof(l_str);
			l_NumerialList.push_back(l_db);
	#else
			l_NumerialList.push_back(_wtof(l_str));
	#endif
			l_str = wcstok(0,L", ");
		}
		return l_NumerialList;
	}

	std::vector<int>	GetIntegerListByCommaDivide(char*e_str,int e_iSize)
	{
		std::vector<int>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
	#ifdef _DEBUG
			int	l_i = atoi(l_str);
			l_NumerialList.push_back(l_i);
	#else
			l_NumerialList.push_back(atoi(l_str));
	#endif
			l_str = strtok(0,", ");
		}
		return l_NumerialList;
	}

	std::vector<double>	GetDoubleListByCommaDivide(char*e_str,int e_iSize)
	{
		std::vector<double>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
	#ifdef _DEBUG
			double	l_db = atof(l_str);
			l_NumerialList.push_back(l_db);
	#else
			l_NumerialList.push_back(atof(l_str));
	#endif
			l_str = strtok(0,", ");
		}
		return l_NumerialList;
	}


	std::vector<float>	GetFloatListByCommaDivide(char*e_str,int e_iSize)
	{
		std::vector<float>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
	#ifdef _DEBUG
			float	l_f = (float)atof(l_str);
			l_NumerialList.push_back(l_f);
	#else
			l_NumerialList.push_back((float)atof(l_str));
	#endif
			l_str = strtok(0,", ");
		}
		return l_NumerialList;
	}

	std::vector<std::wstring>	GetWStringListByCommaDivide(const WCHAR*e_str)
	{
		std::vector<std::wstring>	l_List;
		WCHAR*	l_str = wcstok((WCHAR*)e_str,L", ");
		while(l_str)
		{
			l_List.push_back(l_str);
			l_str = wcstok(0,L", ");
		}
		return l_List;	
	}

	std::vector<std::string>	GetStringListByCommaDivide(const WCHAR*e_str)
	{
		std::vector<std::string>	l_List;
		WCHAR*	l_str = wcstok((WCHAR*)e_str,L", ");
		while(l_str)
		{
			l_List.push_back(UT::WcharToChar(l_str));
			l_str = wcstok(0,L", ");
		}
		return l_List;
	}

	WCHAR*	ValueToStringW(eDataType e_eDataType)
	{
		if( e_eDataType == eDT_INT )
			return L"int";
		if( e_eDataType == eDT_FLOAT )
			return L"float";
		if( e_eDataType == eDT_BYTE )
			return L"byte";
		if( e_eDataType == eDT_DOUBLE  )
			return L"double";
		if( e_eDataType == eDT_WSTRING )
			return L"wstring";
		if( e_eDataType == eDT_STRING )
			return L"string";
		if( e_eDataType == eDT_VOID )
			return L"void";
		if( e_eDataType == eDT_POINT )
			return L"Point";
		if( e_eDataType == eDT_VECTOR2 )
			return L"Vector2";
		if( e_eDataType == eDT_VECTOR3 )
			return L"Vector3";
		if( e_eDataType == eDT_VECTOR4 )
			return L"Vector4";
		if( e_eDataType == eDT_INT64 )
			return L"int64";
		return L"unknow";
	}

	WCHAR*	ValueToStringW(std::wstring	e_strValue)
	{
		return ValueToStringW(e_strValue.c_str());
	}
	WCHAR*	ValueToStringW(const WCHAR*e_strValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%ls\0",e_strValue);
		return g_sstrForStringToStructureW;		
	}
	WCHAR*	ValueToStringW(std::string	e_strValue)
	{
		return ValueToStringW(e_strValue.c_str());
	}

	WCHAR*	ValueToStringW(const char*e_strValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%ls\0",UT::CharToWchar(e_strValue));
		return g_sstrForStringToStructureW;	
	}

	WCHAR*	ValueToStringW(const char e_cValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%d\0",(int)e_cValue);
		return g_sstrForStringToStructureW;	
	}

	WCHAR*	ValueToStringW(int e_iValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%d\0",e_iValue);
		return g_sstrForStringToStructureW;
	}

	//WCHAR*	ValueToStringW(size_t e_iValue)
	//{
	//	swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%d\0",e_iValue);
	//	return g_sstrForStringToStructureW;	
	//}

	WCHAR*	ValueToStringW(float e_fValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%.2f\0",e_fValue);
		return g_sstrForStringToStructureW;		
	}

	WCHAR*	ValueToStringW(double e_fValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%.2f\0",(double)e_fValue);
		return g_sstrForStringToStructureW;
	}
	WCHAR*	ValueToStringW(Vector2 e_vValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%.2f,%.2f\0",e_vValue.x,e_vValue.y);
		return g_sstrForStringToStructureW;
	}

	WCHAR*	ValueToStringW(Vector3 e_vValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%.2f,%.2f,%.2f\0",e_vValue.x,e_vValue.y,e_vValue.z);
		return g_sstrForStringToStructureW;
	}

	WCHAR*	ValueToStringW(Vector4 e_vValue)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%.2f,%.2f,%.2f,%.2f\0",e_vValue.x,e_vValue.y,e_vValue.z,e_vValue.w);
		return g_sstrForStringToStructureW;	
	}

	WCHAR*	ValueToStringW(POINT e_Value)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%d,%d\0",(int)e_Value.x,(int)e_Value.y);
		return g_sstrForStringToStructureW;	
	}

	WCHAR*	ValueToStringW(RECT e_Rect)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%d,%d,%d,%d\0",e_Rect.left,e_Rect.top,e_Rect.right,e_Rect.bottom);
		return g_sstrForStringToStructureW;
	}

	WCHAR*	Vector2ToStringW(Vector2 e_vPos)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%.2f,%.2f\0",e_vPos.x,e_vPos.y);
		return g_sstrForStringToStructureW;
	}

	WCHAR*	Vector3ToStringW(Vector3 e_vPos)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%.2f,%.2f,%.3f\0",e_vPos.x,e_vPos.y,e_vPos.z);
		return g_sstrForStringToStructureW;
	}

	WCHAR*	ValueToStringW(cMatrix44 e_mat)
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\0",
			e_mat.m[0][0],e_mat.m[1][0],e_mat.m[2][0],e_mat.m[3][0],
			e_mat.m[0][1],e_mat.m[1][1],e_mat.m[2][1],e_mat.m[3][1],
			e_mat.m[0][2],e_mat.m[1][2],e_mat.m[2][2],e_mat.m[3][2],
			e_mat.m[0][3],e_mat.m[1][3],e_mat.m[2][3],e_mat.m[3][3]);
		return g_sstrForStringToStructureW;	
	}

	WCHAR*	ValueToStringW(int64 e_uiValue )
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%I64d",e_uiValue);
		return g_sstrForStringToStructureW;	
	}

	WCHAR*	ValueToStringW(uint64 e_uiValue )
	{
		swprintf(g_sstrForStringToStructureW,MAX_PATH,L"%I64u",e_uiValue);
		return g_sstrForStringToStructureW;
	}

	char*	ValueToString(std::string e_strValue)
	{
		return ValueToString(e_strValue.c_str());
	}

	char*	ValueToString(const char*e_strValue)
	{
		sprintf(g_sstrForStringToStructure,"%s\0",e_strValue);
		return g_sstrForStringToStructure;		
	}
	char*	ValueToString(const char e_cValue)
	{
		sprintf(g_sstrForStringToStructure,"%d\0",(int)e_cValue);
		return g_sstrForStringToStructure;	
	}

	char*	ValueToString(int e_iValue)
	{
		sprintf(g_sstrForStringToStructure,"%d\0",e_iValue);
		return g_sstrForStringToStructure;
	}

	char*	ValueToString(size_t e_iValue)
	{
		sprintf(g_sstrForStringToStructure,"%d\0",e_iValue);
		return g_sstrForStringToStructure;	
	}

	char*	ValueToString(float e_fValue)
	{
		sprintf(g_sstrForStringToStructure,"%.2f\0",e_fValue);
		return g_sstrForStringToStructure;
	}

	char*	ValueToString(double e_fValue)
	{
		sprintf(g_sstrForStringToStructure,"%.2f\0",(float)e_fValue);
		return g_sstrForStringToStructure;	
	}

	char*	ValueToString(const WCHAR*e_strValue)
	{
		sprintf(g_sstrForStringToStructure,"%s\0",UT::WcharToChar(e_strValue));
		return g_sstrForStringToStructure;
	}

	char*	ValueToString(char*e_strValue)
	{
		sprintf(g_sstrForStringToStructure,"%s\0",e_strValue);
		return g_sstrForStringToStructure;	
	}

	char*	ValueToString(Vector2 e_vValue)
	{
		sprintf(g_sstrForStringToStructure,"%.2f,%.2f\0",e_vValue.x,e_vValue.y);
		return g_sstrForStringToStructure;
	}

	char*	ValueToString(Vector3 e_vValue)
	{
		sprintf(g_sstrForStringToStructure,"%.2f,%.2f,%.2f\0",e_vValue.x,e_vValue.y,e_vValue.z);
		return g_sstrForStringToStructure;
	}

	char*	ValueToString(Vector4 e_vValue)
	{
		sprintf(g_sstrForStringToStructure,"%.2f,%.2f,%.2f,%.2f\0",e_vValue.x,e_vValue.y,e_vValue.z,e_vValue.w);
		return g_sstrForStringToStructure;	
	}

	char*	ValueToString(POINT e_Pos)
	{
		sprintf(g_sstrForStringToStructure,"%d,%d\0",(int)e_Pos.x,(int)e_Pos.y);
		return g_sstrForStringToStructure;
	}
	char*	ValueToString(RECT e_Rect)
	{
		sprintf(g_sstrForStringToStructure,"%d,%d,%d,%d\0",e_Rect.left,e_Rect.top,e_Rect.right,e_Rect.bottom);
		return g_sstrForStringToStructure;
	}

	char*	RectangleToString(RECT e_Rect)
	{
		sprintf(g_sstrForStringToStructure,"%d,%d,%d,%d\0",e_Rect.left,e_Rect.top,e_Rect.right,e_Rect.bottom);
		return g_sstrForStringToStructure;
	}
	char*	Vector2ToString(Vector2 e_vPos)
	{
		sprintf(g_sstrForStringToStructure,"%.2f,%.2f\0",e_vPos.x,e_vPos.y);
		return g_sstrForStringToStructure;
	}
	char*	Vector3ToString(Vector3 e_vPos)
	{
		sprintf(g_sstrForStringToStructure,"%.2f,%.2f,%.2f\0",e_vPos.x,e_vPos.y,e_vPos.z);
		return g_sstrForStringToStructure;
	}
	char*	Vector4ToString(Vector4 e_vColor)
	{
		sprintf(g_sstrForStringToStructure,"%.3f,%.3f,%.3f,%.3f\0",e_vColor.x,e_vColor.y,e_vColor.z,e_vColor.w);
		return g_sstrForStringToStructure;
	}

	char*	ValueToString(cMatrix44 e_mat)
	{
		sprintf(g_sstrForStringToStructure,"%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f,%.5f\0",
			e_mat.m[0][0],e_mat.m[1][0],e_mat.m[2][0],e_mat.m[3][0],
			e_mat.m[0][1],e_mat.m[1][1],e_mat.m[2][1],e_mat.m[3][1],
			e_mat.m[0][2],e_mat.m[1][2],e_mat.m[2][2],e_mat.m[3][2],
			e_mat.m[0][3],e_mat.m[1][3],e_mat.m[2][3],e_mat.m[3][3]);
		return g_sstrForStringToStructure;
	}

	char*	ValueToString(int64 e_uiValue )
	{
		//%x is 16.
		//%d is 10.
		//%u is unsigned int
		sprintf(g_sstrForStringToStructure,"%I64d",e_uiValue);
		return g_sstrForStringToStructure;		
	}

	//char*	ValueToString(uint64 e_uiValue )
	//{
	//	//%x is 16.
	//	//%d is 10.
	//	//%u is unsigned int
	//	sprintf(g_sstrForStringToStructure,"%I64u",e_uiValue);
	//	return g_sstrForStringToStructure;	
	//}

	char*	UVToString(float*e_pfUV)
	{
		sprintf(g_sstrForStringToStructure,"%.7f,%.7f,%.7f,%.7f\0",e_pfUV[0],e_pfUV[1],e_pfUV[2],e_pfUV[3]);
		return g_sstrForStringToStructure;
	}

	char*	MatrixToStringWithIndex(cMatrix44 e_mat)
	{
		sprintf(g_sstrForStringToStructure,"0:%.5f,1:%.5f,2:%.5f,3:%.5f,4:%.5f,5:%.5f,6:%.5f,7:%.5f,8:%.5f,9:%.5f,10:%.5f,11:%.5f,12:%.5f,13:%.5f,14:%.5f,15:%.5f\0",
			e_mat.m[0][0],e_mat.m[0][1],e_mat.m[0][2],e_mat.m[0][3],
			e_mat.m[1][0],e_mat.m[1][1],e_mat.m[1][2],e_mat.m[1][3],
			e_mat.m[2][0],e_mat.m[2][1],e_mat.m[2][2],e_mat.m[2][3],
			e_mat.m[3][0],e_mat.m[3][1],e_mat.m[3][2],e_mat.m[3][3]);
		return g_sstrForStringToStructure;
	}

	std::string	Vector3VectorToString(std::vector<Vector3> *e_pvPosVector)
	{
		std::string	l_str;
		UINT	l_uiSize = e_pvPosVector->size();
		for( UINT i=0;i<l_uiSize;++i )
		{
			l_str+=Vector3ToString((*e_pvPosVector)[i]);
			if( i+1 != l_uiSize )
				l_str+=",";
		}
		return l_str;
	}

	std::string	Vector2VectorToString(std::vector<Vector2> *e_pvPosVector,int e_iSize)
	{
		std::string	l_str;
		UINT	l_uiSize = e_pvPosVector->size();
		for( UINT i=0;i<l_uiSize;++i )
		{
			l_str+=Vector2ToString((*e_pvPosVector)[i]);
			if( i+1 != l_uiSize )
				l_str+=",";
		}
		return l_str;	
	}

	std::vector<Vector2>	StringToVector2Vector(WCHAR*e_str,int e_iSize)
	{
		std::vector<Vector2>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		WCHAR*	l_str = wcstok(e_str,L", ");
		while(l_str)
		{
			Vector2	l_v;
			l_v.x = (float)_wtof(l_str);
			l_str = wcstok(0,L", ");
			l_v.y = (float)_wtof(l_str);
			l_NumerialList.push_back(l_v);
			l_str = wcstok(0,L", ");
		}
		return l_NumerialList;
	}

	std::vector<Vector3>	StringToVector3Vector(WCHAR*e_str,int e_iSize)
	{
		std::vector<Vector3>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		WCHAR*	l_str = wcstok(e_str,L", ");
		while(l_str)
		{
			Vector3	l_v;
			l_v.x = (float)_wtof(l_str);
			l_str = wcstok(0,L", ");
			l_v.y = (float)_wtof(l_str);
			l_str = wcstok(0,L", ");
			l_v.z = (float)_wtof(l_str);
			l_NumerialList.push_back(l_v);
			l_str = wcstok(0,L", ");
		}
		return l_NumerialList;
	}

	std::vector<Vector2>	StringToVector2Vector(char*e_str,int e_iSize)
	{
		std::vector<Vector2>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
			Vector2	l_v;
			l_v.x = (float)atof(l_str);
			l_str = strtok(0,", ");
			l_v.y = (float)atof(l_str);
			l_NumerialList.push_back(l_v);
			l_str = strtok(0,", ");
		}
		return l_NumerialList;	
	}

	std::vector<Vector3>	StringToVector3Vector(char*e_str,int e_iSize)
	{
		std::vector<Vector3>	l_NumerialList;
		if( e_iSize )
			l_NumerialList.reserve(e_iSize);
		char*	l_str = strtok(e_str,", ");
		while(l_str)
		{
			Vector3	l_v;
			l_v.x = (float)atof(l_str);
			l_str = strtok(0,", ");
			l_v.y = (float)atof(l_str);
			l_str = strtok(0,", ");
			l_v.z = (float)atof(l_str);
			l_NumerialList.push_back(l_v);
			l_str = strtok(0,", ");
		}
		return l_NumerialList;	
	}


	eShapeType	StringToShapeType(const char*e_str)
	{
		if( !strcmp(e_str,"RECT") )
		{
			return eST_RECT;
		}
		else
		if( !strcmp(e_str,"CIRCLE") )
		{
			return eST_CIRCLE;
		}
		return eST_MAX;
	}

	eAnimationType	StringToAnimationType(const char*e_str)
	{
		if( !strcmp(e_str,"IP") )
		{
			return eAT_IP;
		}
		else
		if( !strcmp(e_str,"SA") )
		{
			return eAT_ISIS;
		}
		else
		if( !strcmp(e_str,"MPDI") )
		{
			return eAT_MDPI;
		}
		else
		if( !strcmp(e_str,"MPDIList") )
		{
			return eAT_MDPI_LIST;
		}
		return eAT_NONE;
		return eAT_MAX;
	}
}