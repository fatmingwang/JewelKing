#include "stdafx.h"
#include "CodeInput.h"
#include "math.h"
//#include "GameApp.h"

cCodeInput::cCodeInput()
{
	this->SetName(L"CodeInputPackage");
	m_spGlyphSmallFontRender = new cGlyphFontRender(SmallFontFileName, 2000);

	//Set Key Data
	unsigned char	l_KeyDatas[6] = {'Q','W','I','A','E','R'};
	if(cGameApp::m_sbDeviceExist)
	{
		l_KeyDatas[2] = 129;
		l_KeyDatas[3] = 128;
	}
		

	for( int i=0;i<6;++i )
	{
		m_KeyUpEffect[i].SetKeyData(l_KeyDatas[i]);
		m_KeyUpEffect[i].Init();
	}

	//Set BG
	m_pBG = new cBaseImage("Fish/ControlPanel/Image/CodeInput.PNG");
	m_pBG->SetPos(Vector3(350,150,0));

	//Set Parameter
	for(int i = 0; i < 10; i++) m_iNumber[i] = 0;
	m_iIndex = 0;
	m_fSeconds = 0.0f;
	m_bShineFlag = true;
	m_bCheckFlag = true;
}
cCodeInput::~cCodeInput()
{
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_spGlyphSmallFontRender);
}
void	cCodeInput::Destroy()
{
	
}

void	cCodeInput::Render()
{
	if(m_pBG)
		m_pBG->Render();
	g_pCurrentEventManager->m_pEventFont->RenderFont(510,165,L"请输入打码器内条码");

	g_pCurrentEventManager->m_pEventFont->SetFontColor(Vector4(0,0,0,1));
	for(int i = 0; i < 10; i++)
	{
		
		g_pCurrentEventManager->m_pEventFont->RenderFont(475 + i * 30,370, ValueToStringW(m_iNumber[i]));
		
	}

	if(m_bShineFlag) g_pCurrentEventManager->m_pEventFont->RenderFont(477 + m_iIndex * 30, 380, L"_");
	g_pCurrentEventManager->m_pEventFont->SetFontColor(Vector4::One);

	g_pCurrentEventManager->m_pEventFont->RenderFont(190,150,L"参数调整");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,200,L"解碼调整");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,250,L"查账");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,300,L"报账");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,350,L"系统设置");

	g_pCurrentEventManager->m_pEventFont->RenderFont(165,650,L"【1P上下】改变值");
	g_pCurrentEventManager->m_pEventFont->RenderFont(410,650,L"【1P左右】移动");
	g_pCurrentEventManager->m_pEventFont->RenderFont(630,650,L"【1P发炮键】OK");
	g_pCurrentEventManager->m_pEventFont->RenderFont(850,650,L"【1P加强大炮】离开");

	if(!m_bCheckFlag) g_pCurrentEventManager->m_pEventFont->RenderFont(480,265,L"打码失败, 请输入条码");
}

void	cCodeInput::Update(float e_fElpaseTime)
{
	for( int i=0;i<6;++i )
	{
		m_KeyUpEffect[i].SingnalProcess();
		if( m_KeyUpEffect[i].IsSatisfiedCondition() )
		{
			cEventVariable*l_pCurrentPackageName;
			switch(i) {
				case 1://W - left shift
					m_iIndex++;
					if(m_iIndex > 9) m_iIndex = 0;
					//g_pCurrentEventManager->
				break;
				case 0://Q
					m_iIndex--;
					if(m_iIndex < 0) m_iIndex = 9;
				break;
				case 2://I
					m_iNumber[m_iIndex]--;
					if(m_iNumber[m_iIndex] < 0) m_iNumber[m_iIndex] = 9;
				break;
				case 3://A
					m_iNumber[m_iIndex]++;
					if(m_iNumber[m_iIndex] > 9) m_iNumber[m_iIndex] = 0;
				break;
				case 4://E
					SetupAdjust();
					if(m_bCheckFlag)
					{
						l_pCurrentPackageName = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CurrentPackageName");	
						l_pCurrentPackageName->SetWString(L"VendorMenu_DecodeAdjust_2");
					}
				break;
				case 5://R
					l_pCurrentPackageName = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CurrentPackageName");	
					l_pCurrentPackageName->SetWString(L"VendorMenu_DecodeAdjust_2");
				break;
				default:
				break;
			}
			m_KeyUpEffect[i].Init();
		}
	}	
	

	//Shine Flag
	m_fSeconds += e_fElpaseTime;
	if(m_fSeconds > 0.3 && m_fSeconds < 0.6)
	{
		m_bShineFlag = false;
	} else if(m_fSeconds > 0.6)
	{
		m_bShineFlag = true;
		m_fSeconds = 0.0f;
	}

	
}

void	cCodeInput::Init()
{
	cEventPackage::Init();
	//Init Parameter
	for(int i = 0; i < 10; i++) m_iNumber[i] = 0;
	m_iIndex = 0;
	m_fSeconds = 0.0f;
	m_bShineFlag = true;
	m_bCheckFlag = true;
}

void	cCodeInput::SetupAdjust()
{
	int64 l_iTotal = 0;
	int l_iProbability[] = {98,97,96,94,92};
	int l_iProbability_Hard[] = {91,90,88,86,84};
	int l_iOpenScore[] = {50,100,200,500,1000,2000,10000,50000,100000};
	int l_iExchangeRate[]= {1,2,5,10,20,25,50,100,200,500,1000};
	int l_iTicketRate[]= {1,2,5,10,20,30,40,50,60,70,80,90,100,200,500,1000};
	int l_iMoneyLimit[]= {200000,500000,1000000,5000000,10000000,20000000,50000000,90000000};
	int l_iReportDay[]= {5,7,10,12,14};
	for(int i = 0; i < 10; i++)
	{
		l_iTotal += (int64)m_iNumber[i] * (int64)pow((float)10.0, (float)(9 - i));
	}

	//l_iTotal = 3766484995; //Test Fake Data

	int64 l_iCheckTotal = l_iTotal;
	int l_iCheckStartN = 24;
	bool l_bCheckFlag = true;
	for(int i = 0; i < 6; i++)
	{
		int l_i16N = l_iCheckTotal % 16;
		int l_i2_n1 = l_i16N % 2;
		l_i16N = l_i16N >> 1;
		int l_i2_n2 = l_i16N % 2;
		l_i16N = l_i16N >> 1;
		int l_i2_n3 = l_i16N % 2;
		l_i16N = l_i16N >> 1;
		int l_i2_n4 = l_i16N % 2;
		l_i16N = l_i16N >> 1;

		int64 l_iBeC = l_iTotal / (int)pow((float)2, (float)(l_iCheckStartN + i)) % 2;
		int64 l_iC = (l_i2_n1 ^ l_i2_n2 ^ l_i2_n3 ^ l_i2_n4);

		if(l_iBeC != l_iC)
		{
			l_bCheckFlag = false;
			break;
		}
		l_iCheckTotal = l_iCheckTotal >> 4;
	}

	if(!l_bCheckFlag) 
	{
		m_bCheckFlag = false;
		return;
	}

	cEventVariable*l_pVar= 0;
	//Hard Flag
	int64 l_iTemp = l_iTotal / (int64)pow((float)2, (float)30);
	int64 l_iHardFlag = l_iTemp % 2;
	if(l_iHardFlag < 2)
	{
		l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"HardFlag");	
		l_pVar->SetInt((int)l_iHardFlag);
	}

	//Main Prob
	int64 l_iMainProb = l_iTotal % 8;
	if(l_iMainProb < 5)
	{
		l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Probability");	
		if(l_iHardFlag)
		{
			l_pVar->SetInt((int)l_iProbability_Hard[l_iMainProb]);
		}
		else
		{
			l_pVar->SetInt((int)l_iProbability[l_iMainProb]);
		}
	}

	//OpenScore
	l_iTemp = l_iTotal / 8;
	int64 l_iDecodeOpenScore = l_iTemp % 16;
	if(l_iDecodeOpenScore < 9)
	{
		l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"OpenScore");	
		l_pVar->SetInt(l_iOpenScore[l_iDecodeOpenScore]);
	}

	//ExchangeRate
	l_iTemp = l_iTotal / 128;
	int64 l_iDecodeExchange = l_iTemp % 16;
	if(l_iDecodeExchange < 11)
	{
		l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"ExchangeRate");	
		l_pVar->SetInt(l_iExchangeRate[l_iDecodeExchange]);
	}

	//TicketRate
	l_iTemp = l_iTotal / 2048;
	int64 l_iDecodeTicketRate = l_iTemp % 16;
	if(l_iDecodeTicketRate < 16)
	{
		l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"TicketRate");	
		l_pVar->SetInt(l_iTicketRate[l_iDecodeTicketRate]);
	}

	//HappyTime
	l_iTemp = l_iTotal / 32768;
	int64 l_iHappyTime = l_iTemp % 2;
	if(l_iHappyTime < 2)
	{
		l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"HappyTime");	
		l_pVar->SetInt((int)l_iHappyTime);
		if(l_iHappyTime == 1)
		{
			l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Probability");	
			l_pVar->SetInt(100);
		}
	}

	//MoneyLimit
	l_iTemp = l_iTotal / 65536;
	int64 l_iDecodeMoneyLimit = l_iTemp % 8;
	if(l_iDecodeMoneyLimit < 8)
	{
		l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"MoneyLimit");	
		l_pVar->SetInt64(l_iMoneyLimit[l_iDecodeMoneyLimit]);
	}

	//ReportDay
	l_iTemp = l_iTotal / 524288;
	int64 l_iDecodeReportDay = l_iTemp % 8;
	if(l_iDecodeReportDay < 5)
	{
		l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"ReportDay");	
		l_pVar->SetInt(l_iReportDay[l_iDecodeReportDay]);
	}

	//Area Adjust
	l_iTemp = l_iTotal / 4194304;
	int64 l_iPumpStatus = l_iTemp % 4;
	if(l_iPumpStatus < 3)
	{
		l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"PumpStatus");	
		l_pVar->SetInt((int)l_iPumpStatus);
	}

	//SetupCode
	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"SetupCode");	
	l_pVar->SetInt64(l_iTotal);

	//CheckCode
	int l_iN[10] = {0,0,0,0,0,0,0,0,0,0};
	for(int i = 0; i < 10; i++)
	{
		l_iN[9 - i] = l_iTotal % 10;
		l_iTotal = l_iTotal / 10;
	}
	l_iTemp = 0;
	l_iTemp += (l_iN[9] + l_iN[8]) % 10;
	l_iTemp += ((l_iN[7] + l_iN[6]) % 10) * 10;
	l_iTemp += ((l_iN[5] + l_iN[4] + l_iN[3]) % 10) * 100;
	l_iTemp += ((l_iN[2] + l_iN[1] + l_iN[0]) % 10) * 1000;

	l_pVar = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CheckCode");	
	l_pVar->SetInt64(l_iTemp);
}