#include "stdafx.h"
#include "ReportInput.h"
#include "math.h"
#include "ControlPanel.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/FishProbability.h"
//#include "GameApp.h"
extern cControlPanel*g_pControlPanel;
cReportInput::cReportInput()
{
	this->SetName(L"ReportInputPackage");
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
}
cReportInput::~cReportInput()
{
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_spGlyphSmallFontRender);
}
void	cReportInput::Destroy()
{
	
}

void	cReportInput::Render()
{
	if(m_pBG)
		m_pBG->Render();
	g_pCurrentEventManager->m_pEventFont->RenderFont(540,165,L"请输入打码器内条码");
	g_pCurrentEventManager->m_pEventFont->SetFontColor(Vector4(0,0,0,1));
	for(int i = 0; i < 10; i++)
		g_pCurrentEventManager->m_pEventFont->RenderFont(475 + i * 30,370, ValueToStringW(m_iNumber[i]));

	if(m_bShineFlag) g_pCurrentEventManager->m_pEventFont->RenderFont(477 + m_iIndex * 30, 380, L"_");
	g_pCurrentEventManager->m_pEventFont->SetFontColor(Vector4::One);

	g_pCurrentEventManager->m_pEventFont->RenderFont(190,150,L"参数调整");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,200,L"解碼调整");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,250,L"查账");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,300,L"报账");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,350,L"系统设置");

	g_pCurrentEventManager->m_pEventFont->RenderFont(500,220,L"全部总盈余：");
	g_pCurrentEventManager->m_pEventFont->RenderFont(670,220,ValueToStringW(*m_pi64TotalRevenue));

	g_pCurrentEventManager->m_pEventFont->RenderFont(500,255,L"本期盈余：");
	g_pCurrentEventManager->m_pEventFont->RenderFont(670,255,ValueToStringW(*m_pi64LatestRevenue));

	g_pCurrentEventManager->m_pEventFont->RenderFont(500,290,L"上期盈余：");
	g_pCurrentEventManager->m_pEventFont->RenderFont(670,290,ValueToStringW(*m_pi64PriorRevenue));

	g_pCurrentEventManager->m_pEventFont->RenderFont(500,325,L"校验码：");
	g_pCurrentEventManager->m_pEventFont->RenderFont(670,325,ValueToStringW(*m_piCheckNumber));

	g_pCurrentEventManager->m_pEventFont->RenderFont(165,650,L"【1P上下】改变值");
	g_pCurrentEventManager->m_pEventFont->RenderFont(410,650,L"【1P左右】移动");
	g_pCurrentEventManager->m_pEventFont->RenderFont(630,650,L"【1P发炮键】OK");
	g_pCurrentEventManager->m_pEventFont->RenderFont(850,650,L"【1P加强大炮】离开");

	g_pCurrentEventManager->m_pEventFont->RenderFont(465,435,L"请输入打码器产生的报账条形码。");
}

void	cReportInput::Update(float e_fElpaseTime)
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
					ReportAdjust();
					//l_pCurrentPackageName = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CurrentPackageName");	
					//l_pCurrentPackageName->SetWString(L"VendorMenu_RenderAccount_4_Report");
				break;
				case 5://R
					l_pCurrentPackageName = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CurrentPackageName");	
					l_pCurrentPackageName->SetWString(L"VendorMenu_RenderAccount_4");
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

void	cReportInput::Init()
{
	cEventPackage::Init();
	//Init Parameter
	for(int i = 0; i < 10; i++) m_iNumber[i] = 0;
	m_iIndex = 0;
	m_fSeconds = 0.0f;
	m_bShineFlag = true;

	cEventVariable*l_pHistoryRevenue =  g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"TotalRevenue");
	cEventVariable*l_pLatestRevenue =  g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Latest_Revenue");
	cEventVariable*l_pPriorRevenue =  g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Prior_Revenue");
	cEventVariable*l_pCheckNumber =  g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CheckNumber");
	m_pi64TotalRevenue = l_pHistoryRevenue->GetInt64();
	m_pi64LatestRevenue = l_pLatestRevenue->GetInt64();
	m_pi64PriorRevenue = l_pPriorRevenue->GetInt64();
	m_piCheckNumber = l_pCheckNumber->GetInt();
}

void	cReportInput::ReportAdjust()
{
	//先算出驗證碼
	cEventVariable*l_pTotalRevenue = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"TotalRevenue");
	cEventVariable*l_pCurrentRevenue = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Latest_Revenue");
	cEventVariable*l_pPriorRevenue = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Prior_Revenue");

	cEventVariable*l_pCurrentTotalIn = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Latest_TotalIncome");
	cEventVariable*l_pPriorTotalIn = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Prior_TotalIncome");

	cEventVariable*l_pCurrentTotalOut = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Latest_TotalPay");
	cEventVariable*l_pPriorTotalOut = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"Prior_TotalPay");

	cEventVariable*l_pMachineNumber = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"MachineNumber");
	cEventVariable*l_pCheckNumber = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CheckNumber");
	cEventVariable*l_pReportCount = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"ReportCount");
	cEventVariable*l_pReportFlag = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"ReportFlag");
	
	//Report Time
	//cEventVariable*l_pReportTime = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"ReportTime");

	int64 l_iTotalRevenue = *l_pTotalRevenue->GetInt64(); // A
	int64 l_iCurrentRevenue = *l_pCurrentRevenue->GetInt64(); // B
	int64 l_iPriorRevenue = *l_pPriorRevenue->GetInt64(); // C

	int l_iMachineNumber = *l_pMachineNumber->GetInt(); // C
	int l_iCheckNumber = *l_pCheckNumber->GetInt(); // D
	int l_iReportCount = *l_pReportCount->GetInt(); // 報帳次數

	//fake data
	//=========
	//l_iTotalRevenue = 10000;
	//l_iCurrentRevenue = 100;
	//l_iMachineNumber = 0;
	//l_iReportCount = 60;
	//=========

	//32bit = A ^ B * 2 ^ C * 3 ^ D * 5;

	int64 l_iTemp = 0;
	l_iTemp = l_iTotalRevenue ^ (l_iCurrentRevenue * 2) ^ (l_iPriorRevenue * 3) ^ ((l_iCheckNumber % 10000000) * 5);
	l_iTemp = l_iTemp % (int64)pow((float)2, (float)24);
	l_iTemp *= (int64)pow((float)2, (float)8);

	//ReportCount % 8 * 2 ^ 29
	l_iTemp += (int64)(l_iReportCount % 8) * (int64)pow((float) 2, 5);

	//MachineNumber % 32 * 2 ^ 24
	l_iTemp += (l_iMachineNumber % 32);

	int l_iTotal = 0;
	for(int i = 0; i < 10; i++)
	{
		l_iTotal += m_iNumber[i] * (int)pow((float)10.0, (float)(9 - i));
	}

	cEventVariable*l_pCurrentPackageName;
	if(l_iTotal == l_iTemp)
	{
		g_pControlPanel->m_iReportFlag = 0;

		g_pControlPanel->m_iReportCount++;
		l_pReportCount->SetInt(g_pControlPanel->m_iReportCount);

		g_pControlPanel->m_iReportTime = g_pControlPanel->GetTimeForInt64();
		g_pControlPanel->WriteTimeStamp();

		l_pPriorRevenue->SetInt64(l_iCurrentRevenue);
		l_pPriorTotalIn->SetInt64(*l_pCurrentTotalIn->GetInt64());
		l_pPriorTotalOut->SetInt64(*l_pCurrentTotalOut->GetInt64());

		cFishApp::m_spFishProbability->ClearLatestRecord();

		l_pCurrentPackageName = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CurrentPackageName");	
		l_pCurrentPackageName->SetWString(L"VendorMenu_RenderAccount_4_ReportOK");

		char* l_strFileName = "VariableChanger.xml";
		//檔案存在就砍掉
		if(UT::IsFileExists(l_strFileName))
		{
			RemoveFile(l_strFileName);
		}

		cEventVariable*l_pDelayTimeFlag = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"DelayTimeFlag");
		l_pDelayTimeFlag->SetInt(0);

	}
	else
	{
		l_pCurrentPackageName = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CurrentPackageName");	
		l_pCurrentPackageName->SetWString(L"VendorMenu_RenderAccount_4_ReportRefuse");
	}
}