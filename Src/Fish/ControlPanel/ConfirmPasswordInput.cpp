#include "stdafx.h"
#include "ConfirmPasswordInput.h"
#include "math.h"
//#include "GameApp.h"

cConfirmPasswordInput::cConfirmPasswordInput()
{
	this->SetName(L"ConfirmPasswordInput");
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
	for(int i = 0; i < 5; i++) m_iNumber[i] = 0;
	m_iIndex = 0;
	m_fSeconds = 0.0f;
	m_bShineFlag = true;
	m_bCheckFlag = true;
}
cConfirmPasswordInput::~cConfirmPasswordInput()
{
	SAFE_DELETE(m_pBG);
	SAFE_DELETE(m_spGlyphSmallFontRender);
}
void	cConfirmPasswordInput::Destroy()
{
	
}

void	cConfirmPasswordInput::Render()
{
	if(m_pBG)
		m_pBG->Render();
	g_pCurrentEventManager->m_pEventFont->RenderFont(550,255,L"确认新后台密码");

	g_pCurrentEventManager->m_pEventFont->SetFontColor(Vector4(0,0,0,1));
	for(int i = 0; i < 5; i++)
	{
		
		g_pCurrentEventManager->m_pEventFont->RenderFont(565 + i * 30,370, ValueToStringW(m_iNumber[i]));
		
	}

	if(m_bShineFlag) g_pCurrentEventManager->m_pEventFont->RenderFont(567 + m_iIndex * 30, 380, L"_");
	g_pCurrentEventManager->m_pEventFont->SetFontColor(Vector4::One);

	g_pCurrentEventManager->m_pEventFont->RenderFont(190,150,L"参数调整");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,200,L"解碼调整");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,250,L"查账");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,300,L"报账");
	g_pCurrentEventManager->m_pEventFont->RenderFont(190,350,L"系统设置");

	g_pCurrentEventManager->m_pEventFont->RenderFont(165,650,L"【1P上下】改变值");
	g_pCurrentEventManager->m_pEventFont->RenderFont(410,650,L"【1P左右】移动");
	g_pCurrentEventManager->m_pEventFont->RenderFont(630,650,L"【1P发炮键】OK");
	//g_pCurrentEventManager->m_pEventFont->RenderFont(850,650,L"【1P加强大炮】离开");

	//if(!m_bCheckFlag) g_pCurrentEventManager->m_pEventFont->RenderFont(515,436,L"密码错误请重新输入");
}

void	cConfirmPasswordInput::Update(float e_fElpaseTime)
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
					if(m_iIndex > 4) m_iIndex = 0;
					//g_pCurrentEventManager->
				break;
				case 0://Q
					m_iIndex--;
					if(m_iIndex < 0) m_iIndex = 4;
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
					CheckPassword();
					if(m_bCheckFlag)
					{
						l_pCurrentPackageName = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CurrentPackageName");	
						l_pCurrentPackageName->SetWString(L"ChangePasswordOK");
					}
					else
					{
						l_pCurrentPackageName = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"CurrentPackageName");	
						l_pCurrentPackageName->SetWString(L"ChangePasswordFailed");
					}
				break;
				case 5://R
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

void	cConfirmPasswordInput::Init()
{
	cEventPackage::Init();
	//Init Parameter
	for(int i = 0; i < 5; i++) m_iNumber[i] = 0;
	m_iIndex = 0;
	m_fSeconds = 0.0f;
	m_bShineFlag = true;
	m_bCheckFlag = true;
}

void	cConfirmPasswordInput::CheckPassword()
{
	int l_iTotal = 0;
	WCHAR   l_str[MAX_PATH];
	swprintf(l_str,MAX_PATH,L"%d%d%d%d%d", m_iNumber[0], m_iNumber[1], m_iNumber[2], m_iNumber[3], m_iNumber[4]);
	cEventVariable*l_pNewPassword = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"NewPassword");

	if(!wcscmp(l_str,l_pNewPassword->GetWString()->c_str()))
	{	
		cEventVariable*l_pVendorPassword = g_pCurrentEventManager->m_pEventVariableManager->GetObject(L"VendorPassword");
		l_pVendorPassword->SetWString(l_str);
		m_bCheckFlag = true;	
	}
	else
	{
		m_bCheckFlag = false;	
	}
}