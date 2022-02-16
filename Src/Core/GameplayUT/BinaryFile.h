#ifndef BINARY_FILE_H
#define BINARY_FILE_H
namespace FATMING_CORE
{
	//=======================================
	//
	//signed char: -127 to 127 (note, not -128 to 127; this accommodates 1's-complement platforms)
	//unsigned char: 0 to 255
	//"plain" char: -127 to 127 or 0 to 255 (depends on default char signedness)
	//signed short: -32767 to 32767
	//unsigned short: 0 to 65535
	//signed int: -32767 to 32767
	//unsigned int: 0 to 65535
	//signed long: -2147483647 to 2147483647
	//unsigned long: 0 to 4294967295
	//signed long long: -9223372036854775807 to 9223372036854775807
	//unsigned long long: 0 to 18446744073709551615
	//A C++ (or C) implementation can define the size of a type in bytes sizeof(type) to any value, as long as

	//the expression sizeof(type) * CHAR_BIT evaluates to the number of bits enough to contain required ranges, and
	//the ordering of type is still valid (e.g. sizeof(int) <= sizeof(long)).
	//The actual implementation-specific ranges can be found in <limits.h> header in C, or <climits> in C++ (or even better, templated std::numeric_limits in <limits> header).

	//For example, this is how you will find maximum range for int:

	//C:

	//#include <limits.h>
	//const int min_int = INT_MIN;
	//const int max_int = INT_MAX;
	//C++:

	//#include <limits>
	//const int min_int = std::numeric_limits<int>::min();
	//const int max_int = std::numeric_limits<int>::max();
	//=======================================
	class	cBinaryFile
	{
		FILE*				m_pFile;
		//for big endian and little endian
		int 				D0,D1,D2,D3,D4,D5,D6,D7;
		int 				W0,W1;
		void 				SwapToPc();
		void 				SwapToXBOX();
		VOID*				m_pData;
		//copy data into memory.
		//void				FetchData();
		bool				m_bBigEndian;
	public:
#ifdef WIN32
		HANDLE				m_FileHandle;
#endif
		cBinaryFile(bool e_bBigEndian = true);
		~cBinaryFile();

		VOID 				WriteToFile(int	  val );
		VOID 				WriteToFile(FLOAT val );
		//if u want to write double please use float or int64 instead
		//VOID 				WriteToFile(double val );
		VOID 				WriteToFile(DWORD val );
		//VOID 				WriteToFile(WCHAR val );
		VOID 				WriteToFile(SHORT val );
		VOID 				WriteToFile(WORD val );
		VOID 				WriteToFile(char* val );
		VOID 				WriteToFile(WCHAR* val );
		VOID 				WriteToFile(int64	  val );
		//create file
		//for cet to write for WIN32 to set flag
		bool				Writefile(char*,bool e_bBinary,bool e_bForceToWrite);
		bool				Flush();
		//fclose
		void				CloseFile();
		//fopen
		bool				Openfile(const char*);
		//from m_pFontData
		void*				GetDataFile(UINT e_uiStart);
		int					GetDataFileSize();
		//push data into e_pDestData
		void*				GetDataFileSegment(UINT e_uiStart,UINT e_uiSLength,void*e_pDestData);
		//below for log file
		void 				WriteToFileImmediatelyWithLine(const char* val,bool e_bTimeStamp = true);
		void 				WriteToFileImmediatelyWithLine(const WCHAR* val,bool e_bTimeStamp = true);
		void				CreateTimestamp();
		FILE*				GetFile(){return m_pFile;}
		//pos for byte
		bool				SetFilePos(int e_iPos);
		static	int64		GetInt64(void*e_pData);
		static	float		GetFloat(void*e_pData);
		//static	double		GetDouble(void*e_pData);
		static	int			GetInt(void*e_pData);
		static	char		GetChar(void*e_pData);
	};
}
#endif
