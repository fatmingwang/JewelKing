#include "stdafx.h"
#include "GameApp.h"
#include "CoinDevice.h"
#include "King.h"
#include "GameMessage.h"

cDeviceBehaviorBase::cDeviceBehaviorBase(unsigned char e_ucKey)
{
	m_Singnal.SetKeyData(e_ucKey);
	m_Singnal.Init();
}

void	cDeviceBehaviorBase::Update(float _fElpaseTime)
{
	m_Singnal.SingnalProcess();
	if(m_Singnal.IsSatisfiedCondition())
	{
		SingnalOn();
		m_Singnal.Init();
	}
}



cExchangeMoneyDevice::cExchangeMoneyDevice(int e_iMoney,unsigned char e_ucKey,cEventVariable*e_pExhcangeVariable,cEventVariable*e_pExhcangeVariable1,cEventVariable*e_pExhcangeVariable2)
:cDeviceBehaviorBase(e_ucKey)
{
	m_pExhcangeVariable1 = e_pExhcangeVariable1;
	m_pExhcangeVariable = e_pExhcangeVariable;
	m_pExhcangeVariable2 = e_pExhcangeVariable2;
	m_iMoney = e_iMoney;
}

void	cExchangeMoneyDevice::SingnalOn()
{
	int	l_iMoneyToExchange = m_iMoney*cEngineTestApp::m_siExchangeRate;
	if( l_iMoneyToExchange<0 )
	{
		if(cEngineTestApp::m_spPlayerData->m_iMoney < -l_iMoneyToExchange )
			return;
	}
	if( m_pExhcangeVariable )
	{
		int	l_iMoney = abs(m_iMoney);
		m_pExhcangeVariable->SetInt(*m_pExhcangeVariable->GetInt()+l_iMoney);
		m_pExhcangeVariable1->SetInt(*m_pExhcangeVariable1->GetInt()+l_iMoney);
		m_pExhcangeVariable2->SetInt(*m_pExhcangeVariable2->GetInt()+l_iMoney);
	}
	cEngineTestApp::m_spPlayerData->m_iMoney += l_iMoneyToExchange;
	if(cEngineTestApp::m_spPlayerData->m_iMoney == 0)
	{
		cEngineTestApp::m_spKing->GetGameMessage()->SetInsertCoin();
	}
}



cToastCoinDevice::cToastCoinDevice(int e_iMoney,unsigned char e_ucKey,cEventVariable*e_pToastVariable,cEventVariable*e_pToastVariable2,cEventVariable*e_pToastVariable3):cDeviceBehaviorBase(e_ucKey)
{
	m_bToast = false;
	m_pToastVariable = e_pToastVariable;
	m_pToastVariable2 = e_pToastVariable2;
	m_pToastVariable3 = e_pToastVariable3;
	m_iCoinForPlayerMoney = e_iMoney;
}

void	cToastCoinDevice::SingnalOn()
{
	if(cEngineTestApp::m_spKing->GetGameMessage()->GetGameMessage() == eGM_INSERT_COIN )
	{
		cEngineTestApp::m_spKing->GetGameMessage()->SetIdle();
	}
	m_bToast = true;
	cEngineTestApp::m_spPlayerData->m_iMoney += (m_iCoinForPlayerMoney*cEngineTestApp::m_siExchangeRate);
	if( m_pToastVariable )
	{
		m_pToastVariable->SetInt(*m_pToastVariable->GetInt()+m_iCoinForPlayerMoney);	
		m_pToastVariable2->SetInt(*m_pToastVariable2->GetInt()+m_iCoinForPlayerMoney);
		m_pToastVariable3->SetInt(*m_pToastVariable3->GetInt()+m_iCoinForPlayerMoney);
	}
}


cRefundingCoinDevice::cRefundingCoinDevice(unsigned char e_ucRefundKeyData,unsigned char e_ucCoinRefundKeyData,cIOSMIOData*e_pMotorData,cToastCoinDevice*e_pToastCoinDevice,cEventVariable*e_pRefuncdCoinVariable,cEventVariable*e_pRefuncdCoinVariable1,cEventVariable*e_pRefuncdCoinVariable2)
:cDeviceBehaviorBase(e_ucRefundKeyData)
{
	m_bNoCoinError = false;
	m_pRefuncdCoinVariable1 = e_pRefuncdCoinVariable1;
	m_pRefuncdCoinVariable = e_pRefuncdCoinVariable;
	m_pRefuncdCoinVariable2 = e_pRefuncdCoinVariable2;
	m_MoneyRefundSingnal.SetKeyData(e_ucCoinRefundKeyData);
	m_MoneyRefundSingnal.Init();
	m_pToastCoinDevice = e_pToastCoinDevice;
	m_pMotorData = e_pMotorData;
	m_iNoCoinWaitingTime.SetTargetTime(5.f);
	m_iOffsetForCoinOut.SetTargetTime(0.1f);
	m_bMoneyOutSingal = false;
	m_bRefound = false;
	m_pRefundMoneyFile = 0;
	Init();
}

cRefundingCoinDevice::~cRefundingCoinDevice()
{
	SAFE_DELETE(m_pRefundMoneyFile);
}

void	cRefundingCoinDevice::Update(float e_fElpaseTime)
{
	if( m_bNoCoinError )
		m_pMoneyNotEnoughMPDI->Update(e_fElpaseTime);
	if( cEngineTestApp::m_spPlayerData->m_iMoney == 0 )
		m_bNoCoinError = false;
	cDeviceBehaviorBase::Update(e_fElpaseTime);
	if( !m_bRefound )
		return;
	int	l_iMoneyForOut = m_pToastCoinDevice->m_iCoinForPlayerMoney*cEngineTestApp::m_siExchangeRate;
	if( cEngineTestApp::m_spPlayerData->m_iMoney >= l_iMoneyForOut )
	{
		m_iNoCoinWaitingTime.Update(e_fElpaseTime);
		if( m_iNoCoinWaitingTime.bTragetTimrReached )
		{//no coin to refund
			cEngineTestApp::m_siErrorMsg = ERROR_CODE_NO_ERROR;
			m_pMotorData->WriteIO(false);
			m_bRefound = false;
			SAFE_DELETE(m_pRefundMoneyFile);
			RemoveFile(REFUND_MONEY_FILE_NAME);
			m_bNoCoinError = true;
			return;
		}
		m_MoneyRefundSingnal.SingnalProcess();
		if(cGameApp::m_sucKeyData[m_MoneyRefundSingnal.GetKeyData()])
		{
			m_iOffsetForCoinOut.Update(e_fElpaseTime);
			if( m_iOffsetForCoinOut.bTragetTimrReached )
			{
				m_iOffsetForCoinOut.Start();
				m_pMotorData->WriteIO(false);
			}
		}
		else
		{
			m_pMotorData->WriteIO(true);
			m_iOffsetForCoinOut.Start();
		}
		if( m_MoneyRefundSingnal.IsSatisfiedCondition() )
		{
			m_iNoCoinWaitingTime.Start();
			m_iOffsetForCoinOut.Start();
			m_MoneyRefundSingnal.Init();
			cEngineTestApp::m_spPlayerData->m_iMoney -= l_iMoneyForOut;
			MYPRINTF("money minus:%d\n",l_iMoneyForOut);
			MYPRINTF("player money:%d\n",cEngineTestApp::m_spPlayerData->m_iMoney);
			//m_pRefuncdCoinVariable->SetInt(*m_pRefuncdCoinVariable->GetInt()+l_iMoneyForOut);
			//m_pRefuncdCoinVariable1->SetInt(*m_pRefuncdCoinVariable1->GetInt()+l_iMoneyForOut);
			//cEngineTestApp::m_spPlayerData->WritePlayerMoney();
			m_pRefundMoneyFile->WriteToFile(cEngineTestApp::m_spPlayerData->m_iMoney);
			FILE*l_pFILE = m_pRefundMoneyFile->GetFile();
			fflush(l_pFILE);
			//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(ValueToString(cEngineTestApp::m_spPlayerData->m_iMoney));
			//l_pFILE = cGameApp::m_spLogFile->GetFile();
			//fflush(l_pFILE);
		}
	}
	else
	{//refound score is not enough
		m_pMotorData->WriteIO(false);
		m_bRefound = false;
		SAFE_DELETE(m_pRefundMoneyFile);
		RemoveFile(REFUND_MONEY_FILE_NAME);
		if(cEngineTestApp::m_spPlayerData->m_iMoney == 0)
		{
			cEngineTestApp::m_spKing->GetGameMessage()->SetInsertCoin();
		}
	}
}

bool	cRefundingCoinDevice::IsWorking()
{
	return m_bRefound;
}

void	cRefundingCoinDevice::SingnalOn()
{
	int	l_iMoneyForOut = m_pToastCoinDevice->m_iCoinForPlayerMoney*cEngineTestApp::m_siExchangeRate;
	if( cEngineTestApp::m_spPlayerData->m_iMoney < l_iMoneyForOut )
		return;
	//for no device
	if(!cGameApp::m_sbDeviceExist)
	{
		m_pRefuncdCoinVariable->SetInt(*m_pRefuncdCoinVariable->GetInt()+cEngineTestApp::m_spPlayerData->m_iMoney);
		m_pRefuncdCoinVariable1->SetInt(*m_pRefuncdCoinVariable1->GetInt()+cEngineTestApp::m_spPlayerData->m_iMoney);
		m_pRefuncdCoinVariable2->SetInt(*m_pRefuncdCoinVariable2->GetInt()+cEngineTestApp::m_spPlayerData->m_iMoney);
		cEngineTestApp::m_spPlayerData->m_iMoney = 0;
		return;
	}
	if( m_bRefound )
		return;
	if( cEngineTestApp::m_siErrorMsg != ERROR_CODE_NO_ERROR && !m_bRefound )
		return;
	SAFE_DELETE(m_pRefundMoneyFile);
	m_pRefundMoneyFile = new cBinaryFile();
	m_pRefundMoneyFile->Writefile(REFUND_MONEY_FILE_NAME,true,cEngineTestApp::m_sbDeviceExist?true:false);
	m_pRefundMoneyFile->WriteToFile(cEngineTestApp::m_spPlayerData->m_iMoney);
	FILE*l_pFILE = m_pRefundMoneyFile->GetFile();
	m_iNoCoinWaitingTime.Start();
	m_iOffsetForCoinOut.Start();
	m_bRefound = true;
	m_pMotorData->WriteIO(true);
	m_bNoCoinError = false;
}

int		cRefundingCoinDevice::GetRefuncdFileMoney()
{
	int	l_iMoney = -1;
	cBinaryFile*l_pResundMoneyFile = new cBinaryFile();
	int	l_iLength = 0;
	if(!l_pResundMoneyFile->Openfile(REFUND_MONEY_FILE_NAME))
	{
		goto END;
	}
	l_pResundMoneyFile->GetDataFile(0);
	l_iLength = l_pResundMoneyFile->GetDataFileSize();
	if( l_iLength<4 )
	{
		goto END;
	}
	l_iMoney = *(int*)l_pResundMoneyFile->GetDataFile(l_iLength-sizeof(int));
END:
	SAFE_DELETE(l_pResundMoneyFile);
	if( l_iMoney != -1 )
		UT::RemoveFile(REFUND_MONEY_FILE_NAME);
	return l_iMoney;
}

void	cRefundingCoinDevice::Init()
{
	cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	m_pMoneyNotEnoughMPDI = l_pMPDIList->GetObject(L"MoneyNotEnough");
	m_pMoneyNotEnoughMPDI->Init();
	m_bNoCoinError = false;
}

void	cRefundingCoinDevice::Render()
{
	if( m_bNoCoinError )
	{
		m_pMoneyNotEnoughMPDI->Render();
	}
}