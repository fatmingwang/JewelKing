#include "stdafx.h"
#include "SerialCtrl.h"

SerialCtrl::SerialCtrl(void):m_portStatus(FALSE),m_portHandle(NULL)
{
	m_portConfig.ByteSize = 8;            // Byte of the Data.
	m_portConfig.StopBits = ONESTOPBIT;   // Use one bit for stopbit.
	m_portConfig.Parity = NOPARITY;       // No parity bit
	m_portConfig.BaudRate = CBR_9600;     // Buadrate 9600 bit/sec
}

SerialCtrl::~SerialCtrl(void)
{
	m_portHandle = NULL;
}

void SerialCtrl::SetPortStatus(BOOL bOnOff)
{
	m_portStatus=bOnOff;
}

BOOL SerialCtrl::GetPortStatus(void)
{
	return m_portStatus;
}

HANDLE SerialCtrl::GetPortHandle(void)
{
	return m_portHandle;
}

BOOL SerialCtrl::OpenPort(DCB dcb, const WCHAR * portName)
{

	if (m_portStatus == FALSE)  // if port is opened already, not open port again.
	{
		m_portHandle = CreateFile(portName,  // Specify port device: default "COM1"
			GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
			0,                                  // the devide isn't shared.
			NULL,                               // the object gets a default security.
			OPEN_EXISTING,                      // Specify which action to take on file. 
			0,                                  // default.
			NULL);                              // default.

		// Get current configuration of serial communication port.
		if (GetCommState(m_portHandle,&m_portConfig) == 0)
		{
			//AfxMessageBox("Get configuration port has problem.");
			return FALSE;
		}
		// Assign user parameter.
		m_portConfig.BaudRate = dcb.BaudRate;    // Specify buad rate of communicaiton.
		m_portConfig.StopBits = dcb.StopBits;    // Specify stopbit of communication.
		m_portConfig.Parity = dcb.Parity;        // Specify parity of communication.
		m_portConfig.ByteSize = dcb.ByteSize;    // Specify  byte of size of communication.

		// Set current configuration of serial communication port.
		if (SetCommState(m_portHandle,&m_portConfig) == 0)
		{
			//AfxMessageBox("Set configuration port has problem.");
			return FALSE;
		}

		// instance an object of COMMTIMEOUTS.
		COMMTIMEOUTS comTimeOut;                   
		comTimeOut.ReadIntervalTimeout = 3;
		comTimeOut.ReadTotalTimeoutMultiplier = 3;
		comTimeOut.ReadTotalTimeoutConstant = 2;
		comTimeOut.WriteTotalTimeoutMultiplier = 3;
		comTimeOut.WriteTotalTimeoutConstant = 2;
		SetCommTimeouts(m_portHandle,&comTimeOut);		// set the time-out parameter into device control.
		m_portStatus = TRUE; 
		return TRUE;      
	}
	return FALSE;
}

BOOL SerialCtrl::OpenPort(const WCHAR * baudRate, const WCHAR * portName)
{
	DCB configSerial;
	configSerial.ByteSize = 8;
	configSerial.StopBits = ONESTOPBIT;
	configSerial.Parity = NOPARITY;
	switch(_wtoi(baudRate))
	{
	case 110:
		configSerial.BaudRate = CBR_110;
		break;
	case 300:
		configSerial.BaudRate = CBR_300;
		break;
	case 600:
		configSerial.BaudRate = CBR_600;
		break;
	case 1200:
		configSerial.BaudRate = CBR_1200;
		break;
	case 2400:
		configSerial.BaudRate = CBR_2400;
		break;
	case 4800:
		configSerial.BaudRate = CBR_4800;
		break;
	case 9600:
		configSerial.BaudRate = CBR_9600;
		break;
	case 14400:
		configSerial.BaudRate = CBR_14400;
		break;
	case 19200:
		configSerial.BaudRate = CBR_19200;
		break;
	case 38400:
		configSerial.BaudRate = CBR_38400;
		break;
	case 56000:
		configSerial.BaudRate = CBR_56000;
		break;
	case 57600:
		configSerial.BaudRate = CBR_57600;
		break;
	case 115200 :
		configSerial.BaudRate = CBR_115200;
		break;
	case 128000:
		configSerial.BaudRate = CBR_128000;
		break;
	case 256000:
		configSerial.BaudRate = CBR_256000;
		break;
	default:
		break;
	}

	return OpenPort(configSerial,portName);
}
BOOL SerialCtrl::Read(char * inputData, const unsigned int & sizeBuffer, unsigned long & length)
{
	if (ReadFile(m_portHandle,  // handle of file to read
		inputData,               // handle of file to read
		sizeBuffer,              // number of bytes to read
		&length,                 // pointer to number of bytes read
		NULL) == 0)              // pointer to structure for data
	{
		m_strDebugString = L"read failed";
		// AfxMessageBox("Reading of serial communication has problem.");
		return FALSE;
	}

	if (length > 0)
	{
		m_strDebugString = L"read ok";
		m_strDebugString += ValueToStringW((int)length);
		m_strDebugString += L" ok";
		inputData[length] = NULL; // Assign end flag of message.
		return TRUE;  
	}

	m_strDebugString = L"read failed";
	return TRUE;
}

BOOL SerialCtrl::Write(const char * outputData, const unsigned int & sizeBuffer, unsigned long & length)
{
	if (sizeBuffer > 0)
	{
		if (WriteFile(m_portHandle, // handle to file to write to
			outputData,              // pointer to data to write to file
			sizeBuffer,              // number of bytes to write
			&length,NULL) == 0)      // pointer to number of bytes written
		{
			m_strDebugString = L"wrtie failed";
			return FALSE;
		}
		m_strDebugString = L"wrtie ok size::";
		m_strDebugString += ValueToStringW(sizeBuffer);
		return TRUE;
	}
	m_strDebugString  = L"write failed ";
	return FALSE;
}

void	SerialCtrl::DebugRender()
{
	cGameApp::m_spGlyphFontRender->RenderFont(100,200,m_strDebugString.c_str());
}

BOOL SerialCtrl::ClosePort(void)
{
	if (m_portStatus == TRUE)               // Port need to be open before.
	{
		m_portStatus = FALSE;                 // Update status
		if(CloseHandle(m_portHandle) == 0)    // Call this function to close port.
		{
			//AfxMessageBox("Port Closeing isn't successed.");
			return FALSE;
		}    
		return TRUE;
	}
	return FALSE;
}


void	ComportWorkThread(size_t _workParameter, size_t _pUri)
{
	CSerialIO*l_pSerialThread = (CSerialIO*)_workParameter;
	l_pSerialThread->m_serialProcess->Run();
}
void	ComportWorkThreadDone(size_t _workParameter, size_t _pUri)
{
}

SerialThread::SerialThread() :m_serialIO(NULL)
{
	m_bSafeToDoSingnal = false;
	m_fTimeToWrite = 0.f;
	m_fTargetTimeToWrite = 0.05f;
	Spawn(1);
}

SerialThread::~SerialThread()
{
	m_serialIO = NULL;
	WaitForIdle();
	Clear();
}

void SerialThread::Clear()
{
	cFUThreadPool::Clear();
}

BOOL SerialThread::InitInstance()
{
	return TRUE;
}

int SerialThread::Run()
{
	UT::sTimeAndFPS	l_sTimeAndFPS;
	l_sTimeAndFPS.Update();
	// Check signal controlling and status to open serial communication.
	while(1)
	{
		while(m_serialIO->GetProcessActivateValue()==TRUE)
		{
			l_sTimeAndFPS.Update();
			m_fTimeToWrite += l_sTimeAndFPS.fElpaseTime;
			bool	l_bSingnalGo = false;
			if( m_fTimeToWrite >= m_fTargetTimeToWrite )
			{
				m_fTimeToWrite -= m_fTargetTimeToWrite;
				l_bSingnalGo = true;
				m_bSafeToDoSingnal = true;
			}
			if ((serialCtrl().GetPortStatus()==FALSE)&&m_serialIO->GetPortActivateValue()==TRUE)
			{
				if(serialCtrl().OpenPort(m_serialIO->m_DCB,m_serialIO->m_strPortName.c_str())==TRUE)
				{
					m_serialIO->OnEventOpen(TRUE);
				}
				else
				{
					m_serialIO->OnEventOpen(FALSE);
					m_serialIO->SetPortActivate(FALSE);
				}
			}
			if( l_bSingnalGo )
			{
				if ( m_serialIO->GetSendActivateValue()== TRUE )
				{
					static int l_i = 0;
					OutputDebugString(L"Singnal go");
					OutputDebugString(ValueToStringW(l_i));
					OutputDebugString(L"\n");
					++l_i;
					unsigned long nWritten;
					if(serialCtrl().Write(m_serialIO->m_sendBuffer,m_serialIO->m_sendSize,nWritten)==TRUE)
					{
						m_serialIO->OnEventWrite(nWritten);
					}
					else
					{
						m_serialIO->OnEventWrite(-1);
					}
					m_serialIO->SetSendActivate(FALSE);
				}
				else
				if (m_serialIO->GetPortActivateValue()==TRUE)
				{
					char message[MAX_SEND_BUFFER]={0};
					unsigned int lenBuff = MAX_SEND_BUFFER;
					unsigned long lenMessage;
					if(serialCtrl().Read(message,lenBuff,lenMessage)==TRUE)
					{
						  if(lenMessage>0)
						  {
							  m_serialIO->OnEventRead(message,lenMessage);
							  m_bSafeToDoSingnal = false;
						  }
					}
					else
					{
						//m_serialIO->SetProcessActivate(FALSE);
					}

				}
			}
			if (m_serialIO->m_bClosePort==TRUE)
			{
				if (serialCtrl().ClosePort()==TRUE)
				{
					m_serialIO->OnEventClose(TRUE);
				}
				else
				{
					m_serialIO->OnEventClose(FALSE);
				}
				m_serialIO->m_bClosePort=FALSE;
				return 0;
				
			}
		}

	}
	return 0;
}
void SerialThread::ClosePort()
{

	serialCtrl().ClosePort();

}


CSerialIO::CSerialIO():m_serialProcess(NULL),m_strPortName(L"COM1"),m_strBaudRate(L"115200"),m_sendSize(0)
{
	m_serialProcess = new SerialThread();
	m_bProccessActivate=FALSE;
	m_bPortActivate=FALSE;
	m_bSendActivate=FALSE;
	m_bClosePort=FALSE;
	Init();
	m_bPortActivate=TRUE;
	
}
CSerialIO:: ~CSerialIO()
{
	UnInit();
}
BOOL CSerialIO::Init()
{
	m_serialProcess->setOwner(this);

	m_DCB.ByteSize = 8;
	m_DCB.StopBits = ONESTOPBIT;
	m_DCB.Parity = NOPARITY;
	switch(_wtoi(m_strBaudRate.c_str()))
	{
	case 110:
		m_DCB.BaudRate = CBR_110;
		break;
	case 300:
		m_DCB.BaudRate = CBR_300;
		break;
	case 600:
		m_DCB.BaudRate = CBR_600;
		break;
	case 1200:
		m_DCB.BaudRate = CBR_1200;
		break;
	case 2400:
		m_DCB.BaudRate = CBR_2400;
		break;
	case 4800:
		m_DCB.BaudRate = CBR_4800;
		break;
	case 9600:
		m_DCB.BaudRate = CBR_9600;
		break;
	case 14400:
		m_DCB.BaudRate = CBR_14400;
		break;
	case 19200:
		m_DCB.BaudRate = CBR_19200;
		break;
	case 38400:
		m_DCB.BaudRate = CBR_38400;
		break;
	case 56000:
		m_DCB.BaudRate = CBR_56000;
		break;
	case 57600:
		m_DCB.BaudRate = CBR_57600;
		break;
	case 115200 :
		m_DCB.BaudRate = CBR_115200;
		break;
	case 128000:
		m_DCB.BaudRate = CBR_128000;
		break;
	case 256000:
		m_DCB.BaudRate = CBR_256000;
		break;
	default:
		break;
	}
	m_bProccessActivate=TRUE;
	//create thread
	FUStaticFunctor2<size_t, size_t, void>* workFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ComportWorkThread);
	FUStaticFunctor2<size_t, size_t, void>* doneFunctor = new FUStaticFunctor2<size_t, size_t, void>(&ComportWorkThreadDone);
	m_serialProcess->ExecuteWork(workFunctor,doneFunctor,(size_t)this,0);
	return TRUE;

}
void CSerialIO::UnInit()
{
	m_bPortActivate= FALSE;
	if(m_serialProcess)
	{
		m_serialProcess->ClosePort();
		assert(0);
		m_serialProcess->Clear();
	}
}
void CSerialIO::OpenPort(WCHAR*strPortName,WCHAR*strBaudRate)
 {
	 m_strBaudRate=strBaudRate;
	 m_strPortName=strPortName;
	 m_bPortActivate=TRUE;
	 
 }
void CSerialIO::OnEventOpen(BOOL bSuccess)
{
	if(bSuccess)
		printf("port open successfully\n");
	else
		printf("port open failed\n");
}

void CSerialIO::ClosePort()
{
	m_bClosePort=TRUE;
	SetPortActivate(FALSE);
}
void CSerialIO::OnEventClose(BOOL bSuccess)
{
	if(bSuccess)
		printf("port close successfully\n");
	else
		printf("port close failed\n");
	
}
void CSerialIO::OnEventRead(char *inPacket,int inLength)
{
    
	return;
}

void CSerialIO::Write(char *outPacket,int outLength)
{
	if(outLength<=MAX_SEND_BUFFER)
	{
		memcpy(m_sendBuffer,outPacket,outLength);
		m_sendSize=outLength;
		SetSendActivate(TRUE);
	}
	else
	{
		printf("buffer over flow");
		OnEventWrite(-1);
	}
	return ;
}

void CSerialIO::OnEventWrite(int nWritten)
{
	if(nWritten<0)
	{
		printf("write error\n");
	}
	else
	{
		printf("%d bytes send\n",nWritten);
	}
}

BOOL CSerialIO::GetSendActivateValue(void)
{
	return m_bSendActivate;
}

void CSerialIO::SetSendActivate(BOOL bSendActivate)
{
	m_bSendActivate=bSendActivate;
}

BOOL CSerialIO::GetPortActivateValue(void)
{
	return m_bPortActivate;
}

void CSerialIO::SetPortActivate(BOOL bPortActivate)
{
	m_bPortActivate=bPortActivate;
}

BOOL CSerialIO::GetProcessActivateValue(void)
{
	return m_bProccessActivate;
}

void CSerialIO::SetProcessActivate(BOOL bProcessActivate)
{
	m_bProccessActivate=bProcessActivate;
}
