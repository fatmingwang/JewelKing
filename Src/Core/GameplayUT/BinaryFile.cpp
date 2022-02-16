#include "../stdafx.h"
#include "BinaryFile.h"
#ifdef _WIN32
#include <io.h>
#include <time.h>
#endif
//p4 test
namespace FATMING_CORE
{
	cBinaryFile::cBinaryFile(bool e_bBigEndian)
	{
		m_bBigEndian = e_bBigEndian;
		if( e_bBigEndian )
			SwapToPc();
		else
			SwapToXBOX();
		m_pData = 0;
		m_pFile = 0;
#ifdef WIN32
		m_FileHandle = 0;
#endif
	}

	cBinaryFile::~cBinaryFile()
	{
		CloseFile();
	};

	void cBinaryFile::SwapToPc()
	{
		D0 = 0;D1 = 1;D2 = 2;D3 = 3;
		D4 = 4;D5 = 5;D6 = 6;D7 = 7;
		W0 = 0;W1 = 1;
	}

	void cBinaryFile::SwapToXBOX()
	{
		D0 = 7;D1 = 6;D2 = 5;D3 = 4;
		D4 = 3;D5 = 2;D6 = 1;D7 = 0;
		W0 = 1;W1 = 0;
	}

	VOID cBinaryFile::WriteToFile(FLOAT val )
	{
		CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
						 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
		fwrite( data, sizeof(data), 1, m_pFile ); 
	}

	//VOID cBinaryFile::WriteToFile(double val )
	//{
	//	//fwrite( &val, sizeof(double), 1, m_pFile );
	//	WriteToFile((int64) val );
	//}
	//VOID cBinaryFile::WriteToFile(WCHAR val )
	//{
	//	CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
	//	fwrite( data, sizeof(data), 1, m_pFile ); 
	//}

	VOID cBinaryFile::WriteToFile(int	  val )
	{
		CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
						 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
		fwrite( data, sizeof(data), 1, m_pFile );
	}

	VOID cBinaryFile::WriteToFile(DWORD val )
	{
		CHAR data[4] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
						 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3] };
		fwrite( data, sizeof(data), 1, m_pFile ); 
	}


	VOID cBinaryFile::WriteToFile(SHORT val )
	{
		CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
		fwrite( data, sizeof(data), 1, m_pFile ); 
	}


	VOID cBinaryFile::WriteToFile(WORD val )
	{
		CHAR data[2] = { ((CHAR*)&val)[W0], ((CHAR*)&val)[W1] };
		fwrite( data, sizeof(data), 1, m_pFile ); 
	}

	void cBinaryFile::WriteToFileImmediatelyWithLine(const char* val ,bool e_bTimeStamp)
	{
		if( !m_pFile )
			return;
		if( e_bTimeStamp )
			CreateTimestamp();
		int	l_iLength = (int)strlen(val);
		fwrite( val, sizeof(char),l_iLength, m_pFile );
		fwrite( "\n", sizeof(char),1, m_pFile );
		fflush(m_pFile);
	}

	void cBinaryFile::WriteToFileImmediatelyWithLine(const WCHAR* val,bool e_bTimeStamp)
	{
		if( !m_pFile )
			return;
		if( e_bTimeStamp )
			CreateTimestamp();
		int	l_iLength = (int)wcslen(val);
		if( m_bBigEndian )
		{
			fwrite( val, sizeof(WCHAR),l_iLength, m_pFile ); 
		}
		else
		{
			for( int i=0;i<l_iLength;++i )
				WriteToFile(val[i]);
		}
		if( m_bBigEndian )
			fwrite( L"\n", sizeof(WCHAR),1, m_pFile );
		else
			WriteToFile(L"\n");
		fflush(m_pFile);
	}

	VOID cBinaryFile::WriteToFile(char* val)
	{
		if( !m_pFile )
			return;
		int	l_iLength = (int)strlen(val);
		fwrite( val, sizeof(char),l_iLength, m_pFile );
	}

	VOID cBinaryFile::WriteToFile(int64	  val )
	{
		if( !m_pFile )
			return;
		CHAR data[8] = { ((CHAR*)&val)[D0], ((CHAR*)&val)[D1],
						 ((CHAR*)&val)[D2], ((CHAR*)&val)[D3],
						 ((CHAR*)&val)[D4], ((CHAR*)&val)[D5],
						 ((CHAR*)&val)[D6], ((CHAR*)&val)[D7]};
		fwrite( data, sizeof(data), 1, m_pFile );
	}

	VOID cBinaryFile::WriteToFile(WCHAR* val )
	{
		if( !m_pFile )
			return;
		int	l_iLength = (int)wcslen(val);
		if( m_bBigEndian )
		{
			fwrite( val, sizeof(WCHAR),l_iLength, m_pFile ); 
		}
		else
		{
			for( int i=0;i<l_iLength;++i )
				WriteToFile(val[i]);
		}
	}
	bool	cBinaryFile::Flush()
	{
		if(m_pFile)
		{
			fflush(m_pFile);
			return true;
		}
		return false;
	}
	bool	cBinaryFile::Writefile(char*e_str,bool e_bBinary,bool e_bForceToWrite)
	{
#ifdef WIN32
		if( e_bForceToWrite )
		{
			int	l_iFlag = FILE_ATTRIBUTE_NORMAL;
			if( e_bForceToWrite )
				l_iFlag |= (FILE_FLAG_WRITE_THROUGH);
			m_FileHandle = CreateFile(UT::CharToWchar(e_str), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, l_iFlag, NULL);
			if (m_FileHandle != INVALID_HANDLE_VALUE) 
			{
				int file_descriptor = _open_osfhandle((intptr_t)m_FileHandle, 0);
				if (file_descriptor != -1) 
				{
					if( e_bBinary )
						m_pFile = _fdopen(file_descriptor, "w+b");
					else
						this->m_pFile = _fdopen(file_descriptor,"w");
				}
			}
		}
		else
		{
			if( e_bBinary )
				this->m_pFile = fopen(e_str,"w+b");
			else
				this->m_pFile = fopen(e_str,"w");		
		}
#else
		char	l_strFileName[MAX_PATH];
		GetAppleBundelResourcePathByObjectPath(e_str,l_strFileName);
		if( e_bBinary )
			this->m_pFile = fopen(l_strFileName,"w+b");
		else
			this->m_pFile = fopen(l_strFileName,"w");
#endif
		if(m_pFile)
			return true;
		return false;
	}

	void	cBinaryFile::CloseFile()
	{
		if( m_pData )
			free(m_pData);
		m_pData = 0;
#ifdef WIN32
		if( m_FileHandle )
			CloseHandle(m_FileHandle);
		else
		if( m_pFile )
			fclose( m_pFile );
#else
		if( m_pFile )
			fclose( m_pFile );
#endif
		m_pFile = 0;
	}

	bool	cBinaryFile::Openfile(const char*e_strFileName)
	{
#ifndef __IPHONE__
		this->m_pFile = fopen(e_strFileName,"r+b");
#else
		char	l_strFileName[MAX_PATH];
		GetAppleBundelResourcePathByObjectPath(e_str,l_strFileName);
		this->m_pFile = fopen(l_strFileName,"r+b");
#endif

		if(m_pFile)
			return true;
		return false;
	}

	int		cBinaryFile::GetDataFileSize()
	{
		if(  !m_pFile )
			return 0;
		return UT::GetFileSize(this->m_pFile);

	}
	void*	cBinaryFile::GetDataFile(UINT e_uiStart)
	{
		if(!m_pData)
		{
			assert(m_pFile&&"file is not existed");
			if( !m_pFile )
				return 0;
			long l_lFileSize = UT::GetFileSize(this->m_pFile);
			m_pData = malloc(l_lFileSize);
			fread(m_pData,sizeof(char),l_lFileSize,m_pFile);
		}
		return (void*)((BYTE*)m_pData+e_uiStart);
	}

	//push data into e_pDestData
	void*	cBinaryFile::GetDataFileSegment(UINT e_uiStart,UINT e_uiSLength,void*e_pDestData)
	{
	#ifdef WIN32
		fpos_t pos = e_uiStart;
		fsetpos(m_pFile,&pos);
		fread(e_pDestData,sizeof(BYTE),e_uiSLength,m_pFile);
		rewind(m_pFile);
		return e_pDestData;
	#else
			//or just indtead by pos.__pos = e_uiStart;
		//and call as win32
			unsigned short*l_pData = (unsigned short*)GetDataFile(e_uiStart);
			WCHAR*l_Transto2Wchar = (WCHAR*)e_pDestData;        
			for(int i=0;i<e_uiSLength/2;++i )
			{
				l_Transto2Wchar[i] = l_pData[i];
			}
			l_Transto2Wchar[e_uiSLength/2] = L'\0';
		return e_pDestData;
	#endif
	}

	void	cBinaryFile::CreateTimestamp()
	{
	#ifdef WIN32
			char buffer[32];
			time_t ltime; time(&ltime);
			struct tm now; _localtime64_s(&now, &ltime);
			strftime(buffer, 32, "[%H:%M:%S] ", &now);
			int	l_iLength = (int)strlen(buffer);
			fwrite( buffer, sizeof(char),l_iLength, m_pFile );
	#endif //WIN32
	}

	bool	cBinaryFile::SetFilePos(int e_iPos)
	{
		return fseek(m_pFile,e_iPos,SEEK_SET)==0?true:false;
	}

	int64		cBinaryFile::GetInt64(void*e_pData)
	{
		return *(int64*)e_pData;
	}

	float		cBinaryFile::GetFloat(void*e_pData)
	{
		return *(float*)e_pData;
	}

	//double		cBinaryFile::GetDouble(void*e_pData)
	//{
	//	double	l_dbValue = 0.f;
	//	memcpy(&l_dbValue,e_pData,sizeof(double));
	//	//return *(double*)e_pData;
	//	return l_dbValue;
	//}

	int			cBinaryFile::GetInt(void*e_pData)
	{
		return *(int*)e_pData;
	}

	char		cBinaryFile::GetChar(void*e_pData)
	{
		return *(char*)e_pData;
	}

}