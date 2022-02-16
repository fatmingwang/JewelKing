#include "stdafx.h"
#include "GameError.h"
#include "GameApp.h"
#include "PlayerData.h"


cGameMoneyLimitError::cGameMoneyLimitError()
{
	cEventVariable*l_pEventVariable = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"MoneyLimit");
	if( l_pEventVariable )
		m_piMoneyLimit = l_pEventVariable->GetInt();
	m_bError = false;
	cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	m_pMoneyLimitMPDI = l_pMPDIList->GetObject(L"GameError");
	m_pMoneyLimitMPDI->Init();
}

cGameMoneyLimitError::~cGameMoneyLimitError()
{

}

void		cGameMoneyLimitError::CheckError()
{
	cEventVariable*l_pEventVariable = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"MoneyLimit");
	if( l_pEventVariable )
		m_piMoneyLimit = l_pEventVariable->GetInt();
	int	l_iPlayMoney = cEngineTestApp::m_spPlayerData->m_iMoney;
	int	l_iLimitMoney = m_piMoneyLimit?*m_piMoneyLimit:2500000;
	m_bError = l_iLimitMoney < l_iPlayMoney?true:false;
}

bool		cGameMoneyLimitError::IsError()
{
	return m_bError;
}

void		cGameMoneyLimitError::Update(float e_fElpaseTime)
{
	CheckError();
	if(m_bError)
	{
		m_pMoneyLimitMPDI->Update(e_fElpaseTime);
	}
}

void		cGameMoneyLimitError::Render()
{
	if(m_bError)
	{
		m_pMoneyLimitMPDI->Render();
	}
}