#ifndef STRING_TO_STRUCT_H
#define STRING_TO_STRUCT_H

#include "../Math/Vector3.h"
#include "../Math/Matrix44.h"
#include "../VS_TO_GNN.h"
namespace FATMING_CORE
{
	//x,y
	//"100,200"
	POINT	GetPoint(char* e_str);
	POINT	GetPoint(const char* e_str);
	POINT	GetPoint(const WCHAR* e_str);

	float	GetFloat(const char* e_str);
	float	GetFloat(const WCHAR* e_str);

	int		GetInt(const char* e_str);
	int		GetInt(const WCHAR* e_str);

	uint64	GetUint64(const WCHAR* e_str);
	uint64	GetUint64(const char* e_str);

	int64	GetInt64(const WCHAR* e_str);
	int64	GetInt64(const char* e_str);
	//left top right bottom
	//"0,0,960,540"
	RECT	GetRectangle(const WCHAR*e_pString);
	RECT	GetRectangle(char*e_pString);
	RECT*	GetRectangle(int e_iNumRect,WCHAR*e_pString);
	Vector4	GetVector4(char*e_strValue);
	Vector3	GetVector3(char*e_strValue);
	Vector2	GetVector2(char*e_strValue);
	Vector4	GetVector4(const WCHAR*e_strValue);
	Vector3	GetVector3(const WCHAR*e_strValue);
	Vector2	GetVector2(const WCHAR*e_strValue);

	//ensure column order or row order
	cMatrix44	GetMatrix(const char*e_str,bool e_bTranspose = false);
	cMatrix44	GetMatrix(const WCHAR*e_str,bool e_bTranspose = false);
	//this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const char*e_str,int e_iSize,bool e_bTranspose = false);
	//this might lost data if parse string again!
	cMatrix44	*GetMatrcies(const WCHAR*e_str,int e_iSize,bool e_bTranspose = false);


	void	GetUV(char*e_pData,float*e_pUVBuffer);
	void	GetUV(const char*e_pData,float*e_pUVBuffer);
	void	GetUV(const WCHAR*e_pData,float*e_pUVBuffer);


	//input size if u know it,it will reserve the size for vector faster
	std::vector<int>	GetIntegerListByCommaDivide(char*,int e_iSize);
	std::vector<float>	GetFloatListByCommaDivide(char*,int e_iSize);
	std::vector<double>	GetDoubleListByCommaDivide(char*,int e_iSize);

	std::vector<int>	GetIntegerListByCommaDivide(const WCHAR*,int e_iSize);
	std::vector<float>	GetFloatListByCommaDivide(const WCHAR*,int e_iSize);
	std::vector<double>	GetDoubleListByCommaDivide(const WCHAR*,int e_iSize);

	std::vector<std::wstring>	GetWStringListByCommaDivide(const WCHAR*e_str);
	std::vector<std::string>	GetStringListByCommaDivide(const WCHAR*e_str);
	//inpput data and size to generation,only support common type(int float char double....not support structure)
	//although it could add a callback function make it better but slow.
	template<class T>T*	ParseDataToGenerateIntPointer(char*e_strData,int e_iSize)
	{
		T	*l_pData = new	T[e_iSize];
		char*	l_str = strtok(e_strData,", ");
		int	l_iStep = 0;
		while(l_str)
		{
			l_pData[l_iStep] = (T)atof(l_str);

			l_str = strtok(0,", ");
			++l_iStep;
		}
		return l_pData;
	}
	//for
	template<class T>T*	ParseDataToGenerateIntPointer(const WCHAR*e_strData,int e_iSize)
	{
		T	*l_pData = new	T[e_iSize];
		WCHAR*	l_str = wcstok((WCHAR*)e_strData,L", ");
		int	l_iStep = 0;
		while(l_str)
		{
			l_pData[l_iStep] = (T)_wtof(l_str);
			l_str = wcstok(0,L", ");
			++l_iStep;
		}
		//avoid size is not match
		assert(e_iSize == l_iStep);
		return l_pData;
	}
	//performance is not good enough...by sometimes we just need it
	template<class T>std::vector<T>	GetValueListByCommaDivide(const WCHAR*e_str)
	{
		std::vector<T>	l_NumerialList;
		WCHAR*	l_str = wcstok((WCHAR*)e_str,L", ");
		while(l_str)
		{
	#ifdef _DEBUG
			T	l_i = (T)_wtof(l_str);
			l_NumerialList.push_back(l_i);
	#else
			l_NumerialList.push_back((T)_wtof(l_str));
	#endif
			l_str = wcstok(0,L", ");
		}
		return l_NumerialList;
	}
	//
	WCHAR*	ValueToStringW(eDataType e_eDataType);
	WCHAR*	ValueToStringW(std::wstring	e_strValue);
	WCHAR*	ValueToStringW(std::string	e_strValue);
	WCHAR*	ValueToStringW(const WCHAR*e_strValue);
	WCHAR*	ValueToStringW(char e_cValue);
	WCHAR*	ValueToStringW(const char*e_strValue);
	WCHAR*	ValueToStringW(int e_iValue);
	WCHAR*	ValueToStringW(size_t e_iValue);
	WCHAR*	ValueToStringW(float e_fValue);
	WCHAR*	ValueToStringW(double e_fValue);
	WCHAR*	ValueToStringW(Vector2 e_vValue);
	WCHAR*	ValueToStringW(Vector3 e_vValue);
	WCHAR*	ValueToStringW(Vector4 e_vValue);
	WCHAR*	ValueToStringW(POINT e_Value);
	WCHAR*	ValueToStringW(RECT e_Rect);
	WCHAR*	ValueToStringW(cMatrix44 e_mat);
	WCHAR*	ValueToStringW(int64 e_uiValue );
	WCHAR*	ValueToStringW(uint64 e_uiValue );

	char*	ValueToString(std::string e_strValue);
	char*	ValueToString(const WCHAR*e_strValue);
	char*	ValueToString(const char*e_strValue);
	char*	ValueToString(char e_cValue);
	char*	ValueToString(int	e_iValue);
	char*	ValueToString(size_t e_iValue);
	char*	ValueToString(float e_fValue);
	char*	ValueToString(double e_fValue);
	char*	ValueToString(Vector2 e_vValue);
	char*	ValueToString(Vector3 e_vValue);
	char*	ValueToString(Vector4 e_vValue);
	char*	ValueToString(POINT e_Value);
	char*	ValueToString(RECT e_Rect);
	char*	ValueToString(cMatrix44 e_mat);
	char*	ValueToString(uint64 e_uiValue );
	char*	ValueToString(int64 e_uiValue );

	char*	UVToString(float*e_pfUV);

	char*	MatrixToString(cMatrix44 e_mat);
	char*	MatrixToStringWithIndex(cMatrix44 e_mat);

	std::string	Vector3VectorToString(std::vector<Vector3>* e_pvPosVector);
	std::string	Vector2VectorToString(std::vector<Vector2>* e_pvPosVector);
	std::vector<Vector2>	StringToVector2Vector(WCHAR*e_str,int e_iSize = 0);
	std::vector<Vector3>	StringToVector3Vector(WCHAR*e_str,int e_iSize = 0);
	std::vector<Vector2>	StringToVector2Vector(char*e_str,int e_iSize = 0);
	std::vector<Vector3>	StringToVector3Vector(char*e_str,int e_iSize = 0);

	enum	eShapeType
	{
		eST_RECT = 0,
		eST_CIRCLE,
		eST_MAX,
	};
	enum	eAnimationType
	{
		eAT_NONE = 0,
		eAT_MDPI_LIST,
		eAT_MDPI,
		eAT_IP,
		eAT_ISIS,
		eAT_MAX,
	};
	eShapeType	StringToShapeType(const char*e_str);
	eAnimationType	StringToAnimationType(const char*e_str);
}
#endif