#include "stdafx.h"
#include "RefundCoinDevice.h"
#include "PlayerBehaviorBase.h"
#include "GameApp.h"
#include "FishProbability.h"
#include "../../IOSimulationMachine/8051/GH8051.h"

cRefoundCoinDevice::cRefoundCoinDevice(
									   cPlayerBehaviorBase*e_pPlayer,//player ID
									   cIOSMIOPackData*e_pInsertCoin,//insert coin count
									   cIOSMIOPackData*e_pHopperValue,//refund coin device
									   cIOSMIOPackData*e_pReHopperValue,//
									   cIOSMIOPackData*e_pTicketValue,//
									   cIOSMIOPackData*e_pReTicketValue,//
									   cIOSMIOData*	e_pResetWantRefundCountCoinDevice,
									   cIOSMIOData*	e_pResetWantRefundCountTicketDevice,
									   int*e_piCoinToScore,//
									   unsigned char e_ucReturnKey,//
									   unsigned char e_ucExchangeInKey,
									   unsigned char e_ucExchangeOutKey
									   )
{
	m_bRefundCoinStart = false;
	m_i64RestCoinToRefund = 0;
	m_bRefundCoin = false;
	m_piExchangeIn = &cFishApp::m_spControlPanel->m_iExchangeIn;
	m_pPlayer = e_pPlayer;
	m_pInsertCoin = e_pInsertCoin;
	bool	l_bCoinDevice = true;
	if( cFishApp::m_spControlPanel )
	{
		cEventVariable*l_pRefundCoinOrTicket = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"RefundCoinOrTicket");
		l_bCoinDevice = *l_pRefundCoinOrTicket->GetInt()==1?false:true;
	}
	if( l_bCoinDevice )
	{
		m_pWantRefundCountDevice = e_pHopperValue;
		m_pCurrentRefundCountDevice = e_pReHopperValue;
		m_pResetWantRefundCountDevice = e_pResetWantRefundCountCoinDevice;
	}
	else
	{
		m_pWantRefundCountDevice = e_pTicketValue;
		m_pCurrentRefundCountDevice = e_pReTicketValue;	
		m_pResetWantRefundCountDevice = e_pResetWantRefundCountTicketDevice;
	}
	assert(m_pWantRefundCountDevice);
	assert(m_pCurrentRefundCountDevice);
	assert(m_pResetWantRefundCountDevice);
	m_piCoinToScore = e_piCoinToScore;
	m_RefundKey.SetKeyData(e_ucReturnKey);
	m_ExchangeInKey.SetKeyData(e_ucExchangeInKey);
	m_ExchangeOutKey.SetKeyData(e_ucExchangeOutKey);
	m_bWorking = false;
	m_pBinaryFile = 0;
	m_RefundToCheckCoinTC.SetTargetTime(6.f);

	//m_Wait8051SingnalToActive.SetTargetTime(REFRESH_8051_IO_TIME_OFFSET);
}
cRefoundCoinDevice::~cRefoundCoinDevice() 
{
	 
}

void	cRefoundCoinDevice::ExchangeBehaviod(float e_fElpaseTime)
{
	m_ExchangeInKey.SingnalProcess();
	m_ExchangeInKey.Update(e_fElpaseTime);
	if(m_ExchangeInKey.IsSatisfiedCondition())
	{	
		this->m_pPlayer->m_i64Score += *m_piExchangeIn;
		m_ExchangeInKey.Init();
		cFishApp::m_spFishProbability->ExchangeIn(*m_piExchangeIn);
		cGameApp::SoundPlay(L"Button_Bet",true);
	}

	m_ExchangeOutKey.SingnalProcess();
	m_ExchangeOutKey.Update(e_fElpaseTime);
	if(m_ExchangeOutKey.IsSatisfiedCondition())
	{		
		cFishApp::m_spFishProbability->ExchangeOut(this->m_pPlayer->m_i64Score);
		this->m_pPlayer->m_i64Score = 0;
		m_ExchangeOutKey.Init();
	}
}

void	cRefoundCoinDevice::CoinRefund()
{
	this->m_pPlayer->m_bNoCoin = false;
	m_RefundKey.Init();
	//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(L"Refund key start");
	if(cGameApp::m_sbDeviceExist)
	{
		if(this->m_pPlayer->m_i64Score >= *m_piCoinToScore)//money is enough to refund.
		{
			m_RefundToCheckCoinTC.Start();
			m_bRefundCoin = true;
			m_i64RestCoinToRefund = this->m_pPlayer->m_i64Score/(*m_piCoinToScore);

			if(m_i64RestCoinToRefund>SAFE_REFUND_COIN_COUNT)
			{
				m_i64RestCoinToRefund = SAFE_REFUND_COIN_COUNT;
			}
			this->m_pWantRefundCountDevice->SetPackDataByByte((unsigned char) m_i64RestCoinToRefund);
			m_bRefundCoinStart = false;
			//void*	l_pData = this->m_pCurrentRefundCountDevice->GetData();
			//*(unsigned char*)l_pData = (unsigned char)m_i64RestCoinToRefund;
		}
		else//not enough money.
		{
			m_bRefundCoin = false;
		}
	}
	else
	{
		m_bRefundCoin = false;
		cFishApp::m_spFishProbability->RefundCoin(this->m_pPlayer->m_i64Score);
		this->m_pPlayer->m_i64Score = 0;
	}
}

void	cRefoundCoinDevice::CoinBehavior(float e_fElpaseTime)
{
	//toast coin
	if(cGameApp::m_sbDeviceExist)
	{
		int64 l_i64ToastCoin = *m_piCoinToScore * this->m_pInsertCoin->GetPackDataByByte();
		void*l_pData = this->m_pInsertCoin->GetData();
		*(unsigned char*)l_pData = 0;
		this->m_pPlayer->m_i64Score += l_i64ToastCoin;
		cFishApp::m_spFishProbability->ToastCoin(l_i64ToastCoin);
	}
	else
	{//detect toast coin key
		if( m_pInsertCoin->GetKeyData()  )
		{
			//m_cToastCoin = *m_piCoinToScore;
		}
	}
	//refunding
	this->m_pPlayer->m_bReturnCoin = m_bRefundCoin;
	if( m_bRefundCoin )
	{
		int64	l_i64RestCoin = this->m_pCurrentRefundCountDevice->GetPackDataByByte();
		if( !m_bRefundCoinStart )
		{
			if( l_i64RestCoin != 0 )
			{
				m_bRefundCoinStart = true;
			}
		}
		if( l_i64RestCoin != m_i64RestCoinToRefund && m_bRefundCoinStart )
		{
			int64 l_i64RefundCoin = *m_piCoinToScore * (m_i64RestCoinToRefund-l_i64RestCoin);
			m_i64RestCoinToRefund = l_i64RestCoin;
			cFishApp::m_spFishProbability->RefundCoin(l_i64RefundCoin);
			this->m_pPlayer->m_i64Score -= l_i64RefundCoin;//
			m_RefundToCheckCoinTC.Start();
			//more coin to refund?
			if( m_i64RestCoinToRefund <= 0 )
			{
				CoinRefund();
			}
		}
		//motor spin too long,no coin
		if( m_RefundToCheckCoinTC.bTragetTimrReached )
		{
			m_bRefundCoin = false;
			this->m_pPlayer->m_bNoCoin = true;
			m_pResetWantRefundCountDevice->WriteIO(true);
		}
	}
	else
	{//check refund key pressed
		m_RefundKey.SingnalProcess();
		m_RefundKey.Update(e_fElpaseTime);
		if(m_RefundKey.IsSatisfiedCondition())
		{
			m_bRefundCoinStart = false;
			//m_Wait8051SingnalToActive.
			CoinRefund();
		}
	}
}

void cRefoundCoinDevice::Update(float e_fElpaseTime)
{
	if(g_pIOSMDriverInterface->IsIOError())
		return;
	CoinBehavior(e_fElpaseTime);
	ExchangeBehaviod(e_fElpaseTime);
	if( m_bRefundCoin )
	{
		m_RefundToCheckCoinTC.Update(e_fElpaseTime);
	}
}

void cRefoundCoinDevice::Render()
{
//	std::wstring	l_str = UT::ComposeMsgByFormat(L"DebugState:%d  %d",m_iDebugState,m_iDebugState1);
//	cGameApp::m_spGlyphFontRender->RenderFont(200 , 400+m_pPlayer->m_iPlayerID*50, l_str.c_str());
	//swprintf(m_strReceive, MAX_PATH, L"Refund State = %d",m_iDebugState);
	//cGameApp::m_spGlyphFontRender->RenderFont(800 , 800, m_strReceive);
}


cRefoundCoinDeviceManager::cRefoundCoinDeviceManager()
{
}

cRefoundCoinDeviceManager::~cRefoundCoinDeviceManager()
{

}

void cRefoundCoinDeviceManager::Init(const char *filename)
{
	if(!cGameApp::m_sbDeviceExist)
	{
	//	std::string	l_str = UT::ComposeMsgByFormat("InitXML - fail");
	//	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		//return;
	}
	if( !g_pIOSMDriverInterface||!cFishApp::m_spPlayerManager )
		return;
	//ceate all player data
	//cFishApp::m_siPlayerCount;
	//std::string	l_str = UT::ComposeMsgByFormat("InitXML - OK");
	//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
	this->Destroy();
	cNodeISAX	l_NodeISAX;
	bool l_b = l_NodeISAX.ParseDataIntoXMLNode(filename);
	TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
	l_pElement = l_pElement->FirstChildElement();
	//rate of coin to score
	int*	l_pCoinToScore = cFishApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CoinToScore")->GetInt();
	while(l_pElement)
	{		//player ID
			const WCHAR*l_strID = l_pElement->Attribute(L"ID");
			if( _wtoi(l_strID) >= cFishApp::m_spControlPanel->m_iPlayerCount )
				return;
			cFishApp::m_spPlayerManager->GetObject(_wtoi(l_strID));
			//coin Insert value
			const WCHAR*l_strInsertCoin = l_pElement->Attribute(L"Insert");
			cIOSMIOPackData*l_strInsertCoinIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_strInsertCoin);
			//Hopper value
			const WCHAR*l_strHopperValue = l_pElement->Attribute(L"Hopper");
			cIOSMIOPackData*l_pHopperValueIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_strHopperValue);
			//ReHopper value
			const WCHAR*l_strReHopperValue = l_pElement->Attribute(L"ReHopper");
			cIOSMIOPackData*l_pReHopperValueIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_strReHopperValue);
			//Ticket value
			const WCHAR*l_strTicketValue = l_pElement->Attribute(L"Ticket");
			cIOSMIOPackData*l_pTicketValueIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_strTicketValue);
			//ReHopper value
			const WCHAR*l_strReTicketValue = l_pElement->Attribute(L"ReTicket");
			cIOSMIOPackData*l_pReTicketValueIOSMIOPackData = (cIOSMIOPackData*)g_pIOSMDriverInterface->GetObject(l_strReTicketValue);
			//return key
			const WCHAR*l_strReturnKey = l_pElement->Attribute(L"Return");
			cIOSMIOData*l_pReturnKey = g_pIOSMDriverInterface->GetObject(l_strReturnKey);
			//open key
			const WCHAR*l_strOpenKey = l_pElement->Attribute(L"Open");
			cIOSMIOData*l_pOpenKey = g_pIOSMDriverInterface->GetObject(l_strOpenKey);
			//wash key
			const WCHAR*l_strWashKey = l_pElement->Attribute(L"Wash");
			cIOSMIOData*l_pWashKey = g_pIOSMDriverInterface->GetObject(l_strWashKey);			
			cPlayerBehaviorBase*l_pPlayerBehaviorBase = cFishApp::m_spPlayerManager->GetObject(_wtoi(l_strID));

			std::wstring	l_str = UT::ComposeMsgByFormat(L"CoinReset_%d",l_pPlayerBehaviorBase->m_iPlayerID);
			cIOSMIOData*l_pCoinResetIOSMIOData = g_pIOSMDriverInterface->GetObject(l_str.c_str());
			l_str = UT::ComposeMsgByFormat(L"TicketReset_%d",l_pPlayerBehaviorBase->m_iPlayerID);
			cIOSMIOData*l_pTicketResetIOSMIOData = g_pIOSMDriverInterface->GetObject(l_str.c_str());
			cRefoundCoinDevice*l_pPlayer = new cRefoundCoinDevice(  l_pPlayerBehaviorBase,//player ID
																	l_strInsertCoinIOSMIOPackData,//Insert Coin Value
																	l_pHopperValueIOSMIOPackData,//Hopper value
																	l_pReHopperValueIOSMIOPackData,//ReHopper value
																	l_pTicketValueIOSMIOPackData,//Ticket value
																	l_pReTicketValueIOSMIOPackData,//ReTicket value
																	l_pCoinResetIOSMIOData,
																	l_pTicketResetIOSMIOData,
																	l_pCoinToScore,//rate of coin to score
																	*l_pReturnKey->GetKeyData(),//return key
																	*l_pOpenKey->GetKeyData(),//Open key
																	*l_pWashKey->GetKeyData()//Wash key
																	);//coin Insert value
			this->AddObjectNeglectExist(l_pPlayer);
			//std::string	l_str = UT::ComposeMsgByFormat("ADD Object OK");
			//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
			l_pElement = l_pElement->NextSiblingElement();
	}
}

void	cRefoundCoinDeviceManager::Update(float e_fElpaseTime)
{
		//std::string	l_str = UT::ComposeMsgByFormat("Have device");
		//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());	
		//g_pRefoundCoinDevice->Update(e_fElpaseTime);
		int	l_iCount = this->Count();		
		for( int i=0;i<l_iCount;++i )
		//for(int i=0;i<cFishApp::m_spPlayerManager->Count();++i)
		//for(int i=0;i<8;++i)
		{
			//std::string	l_str = UT::ComposeMsgByFormat("Refund-Index:%d  OK",i);
			//cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
			this->m_ObjectList[i]->Update(e_fElpaseTime);
		}				
}

void	cRefoundCoinDeviceManager::Render()
{
	int	l_iCount = this->Count();	
	for( int i=0;i<l_iCount;++i )
		this->m_ObjectList[i]->Render();
	//for(int i=0;i<cFishApp::m_spPlayerManager->Count();++i)
	//for(int i=0;i<8;++i)
		
}

void	cRefoundCoinDeviceManager::Destroy()
{

}
