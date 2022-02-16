#include "stdafx.h"
#include "BetPhase.h"
#include "SlotObject.h"
#include "GameApp.h"
#include "SlotRule.h"
#include "King.h"
#include "WinMoneyLine.h"
#include "GameMessage.h"
#include "GameNetwork.h"
#include "GameError.h"
#include "CoinDevice.h"
#include "Light.h"
extern cGameMoneyLimitError*g_pGameMoneyLimitError;
cBetPhase::cBetPhase()
{
	this->SetName(BET_PHASE_NAME);
	m_pConstantUI = 0;
	m_pBetDataButtonManaget = 0;
	m_bAutoSpin = false;
	m_pMenu = 0;
	m_iReadMeProgress = 0;
	m_ExchangeRateVector.push_back(1);
	m_iCurrentFocusExchangeRate = 0;
	m_pGameMoneyLimitError = new cGameMoneyLimitError();
	g_pGameMoneyLimitError = m_pGameMoneyLimitError;
}

cBetPhase::~cBetPhase()
{
	SAFE_DELETE(m_pConstantUI);
	SAFE_DELETE(m_pBetDataButtonManaget);
	SAFE_DELETE(m_pGameMoneyLimitError);
}
extern cRefundingCoinDevice*g_pRefundingCoinDevice;
bool	cBetPhase::ChecMoneyEncoughToBet(bool e_bMinusPlayerMoney)
{
	if( m_pGameMoneyLimitError->IsError() || g_pRefundingCoinDevice->IsWorking() )
		return false;
	if( cEngineTestApp::m_spPlayerData->m_iMoney >= cEngineTestApp::m_spBetBehavior->m_iTotalBet )
	{
		//if( e_bMinusPlayerMoney )
		//	cEngineTestApp::m_spPlayerData->m_iMoney -= cEngineTestApp::m_spBetBehavior->m_iTotalBet;
		return true;
	}
	//play error sound.
	return false;
}

void	cBetPhase::Init()
{
	if( !m_pConstantUI )
	{
		cNodeISAX	l_NodeISAX;
		bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(SPIN_PHASE_DATA);
		if( l_b )
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			const WCHAR*l_strUIMPDIFileName = l_pElement->Attribute(L"UIMPDIFileName");
			const WCHAR*l_strUIMPDIName = l_pElement->Attribute(L"UIMPDIName");
			if( l_strUIMPDIFileName )
			{
				//cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(l_strUIMPDIFileName);
				cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
				cMPDI*l_pMPDI = l_pMPDIList->GetObject(UPPER_UI);
				if( !l_pMPDI )
				{
					UT::ErrorMsg(L"UI"	,L"fetch  MPDI error");
				}
				m_pConstantUI = new cMPDI(l_pMPDI);
				m_pConstantUI->RefreshTotalPlayTime();
				m_pConstantUI->Init();
				m_pConstantUI->Update(EPSIONAL);
				m_pBetDataButtonManaget = new cBetDataButtonManaget(this);
			}
		}
		l_b = l_NodeISAX.ParseDataIntoXMLNode(BET_PHASE_DATA);
		if( l_b )
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			const WCHAR*l_strShowLineTime = l_pElement->Attribute(L"ShowLineTime");
			const WCHAR*l_strCoinExchangeRate = l_pElement->Attribute(L"CoinExchangeRate");
			if( l_strShowLineTime )
			{
				float	l_fTime = (float)_wtof(l_strShowLineTime);
				cEngineTestApp::m_spBetBehavior->m_pWinmoneyLines->ChangeShowLineTime(l_fTime);
			}
			if( l_strCoinExchangeRate )
			{
				m_ExchangeRateVector.clear();
				//l_strName = wcstok((WCHAR*)l_strCoinExchangeRate,L",");
				//while(l_strName)
				//{
				//	float	l_fValue =  (float)_wtof(l_strName);
				//	m_ExchangeRateVector.push_back(l_fValue);
				//	l_strName = wcstok(0,L",");
				//}
				m_ExchangeRateVector = GetValueListByCommaDivide<int>(l_strCoinExchangeRate);
			}
		}
	}
	cEngineTestApp::m_spSlotLineObjectManagerList->Init();
	if( cEngineTestApp::m_siFreeGame > 0 )
	{
		int	l_iValue = (*cEngineTestApp::m_spRestFreeGameTimes)[0];
		cEngineTestApp::m_spKing->GetGameMessage()->SetFreeTime(l_iValue);
		l_iValue = (*cEngineTestApp::m_spRestFreeGameTimes)[0]-1;
		if( l_iValue>0 )
		{
			(*cEngineTestApp::m_spRestFreeGameTimes)[0] = l_iValue;
		}
		else
			(*cEngineTestApp::m_spRestFreeGameTimes).erase(cEngineTestApp::m_spRestFreeGameTimes->begin());
		this->m_bSatisfiedCondition = true;
		--cEngineTestApp::m_siFreeGame;
		cEngineTestApp::m_sbFreeToPlay = true;

	}
	else
	{
		cEngineTestApp::m_spBetBehavior->m_iBonusMoneyRate = 1;
		if( !cGameApp::m_spSoundParser->GetObject(NORMAL_BGM_SOUND_NAME)->IsPlay() )
		{
			cGameApp::m_spSoundParser->GetObject(FREE_GAME_BGM_SOUND_NAME)->Play(false);
			cGameApp::m_spSoundParser->GetObject(NORMAL_BGM_SOUND_NAME)->Play(true);
		}
		cEngineTestApp::m_spKing->SetRenderPayRateHint(true);
		cEngineTestApp::m_sbFreeToPlay = false;
		if( !m_bAutoSpin )
			cEngineTestApp::m_spKing->GetGameMessage()->SetIdle();
		if(cEngineTestApp::m_spPlayerData->m_iMoney == 0)
		{
			cEngineTestApp::m_spKing->GetGameMessage()->SetInsertCoin();
		}
	}
	if( m_bAutoSpin )
	{
		if(ChecMoneyEncoughToBet(true))
			this->m_bSatisfiedCondition = true;
		else
			m_bAutoSpin = false;
	}

	m_pBetDataButtonManaget->Init();
	cEngineTestApp::m_spBetBehavior->m_iWinMoney = 0;
	cEngineTestApp::m_spKing->InitStatus();
	cEngineTestApp::m_spBetBehavior->m_pWinmoneyLines->Stop();
	cEventVariable*l_pLeastJP = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"JPLeastMoney");
	if( l_pLeastJP )
	{
		int	l_iMoney = *l_pLeastJP->GetInt();
		int*l_piJPMoney = cEngineTestApp::m_spPlayerData->GetJP(eJPT_NETWORK);
		if( *l_piJPMoney<l_iMoney )
			*l_piJPMoney = l_iMoney;
	}
}

void	cBetPhase::SetupResult()
{
	if(cEngineTestApp::m_spExtraBonusMoneyRate->size())
	{
		cEngineTestApp::m_spBetBehavior->m_iBonusMoneyRate = (*cEngineTestApp::m_spExtraBonusMoneyRate)[0];
		cEngineTestApp::m_spExtraBonusMoneyRate->erase(cEngineTestApp::m_spExtraBonusMoneyRate->begin());
	}
	cEngineTestApp::m_spBetBehavior->CalculateScoreWithRound();
	cPuzzleImage*l_pPI = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI();
	int	l_iCount = cEngineTestApp::m_spSlotLineObjectManagerList->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cSlotObjectManager*l_pSlotObjectManager = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(i);
		int	l_iCount = cEngineTestApp::m_spSlotRuleData->m_TableCount.y;
		std::vector<std::wstring>l_strResult;
		for( int j=0;j<cEngineTestApp::m_spSlotRuleData->m_TableCount.y;++j )
		{
			std::wstring	l_str;
			int	l_iIndex = i+cEngineTestApp::m_spSlotRuleData->m_TableCount.x*j;
			int	l_iResult;
			l_iResult = cEngineTestApp::m_spBetBehavior->m_pBetBehaviorResult->m_TableResultList[0]->SpinObjectResultList[l_iIndex];
			l_str = cEngineTestApp::m_spSlotRuleData->m_SpineObjectPrizeTableList.GetObject(l_iResult)->GetName();
			//while( !IsVaildSpinObjectForResult(l_str) )
			//{
			//	assert(0&&"while( !IsVaildSpinObjectForResult(l_str) )");
			//	l_iResult = rand()%cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(0)->GetAllSpinObjectPI()->Count();
			//	l_str = l_pPI->GetObject(l_iResult)->GetName();			
			//}
			l_strResult.push_back(l_str);
		}
		l_pSlotObjectManager->Start(&l_strResult);
	}
	if( !cEngineTestApp::m_sbFreeToPlay )
		cEngineTestApp::m_spPlayerData->m_iMoney -= cEngineTestApp::m_spBetBehavior->m_iTotalBet;

	float	l_fJPShareScorePercentage = 0.5f;
	if( cEngineTestApp::m_spControlPanel )
	{
		cEventVariable*l_pEventVariable = cEngineTestApp::m_spControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"JPShareScorePercentage");
		l_fJPShareScorePercentage = *l_pEventVariable->GetFloat();
	}
	int	l_iJPBonus = (int)(cEngineTestApp::m_spBetBehavior->m_iTotalBet*l_fJPShareScorePercentage);
	if( l_iJPBonus > 0 && cEngineTestApp::m_spNetwork->IsConnected() && !cEngineTestApp::m_sbFreeToPlay )
	{
		sGamePacket	l_GamePacket;
		l_GamePacket.iMessageID = eGMI_TO_SERVER_JP_SCORE;
		l_GamePacket.iJPScore = l_iJPBonus;
		cEngineTestApp::m_spNetwork->SendData(&l_GamePacket);
	}
	//assign bet money
	cEngineTestApp::m_spBetBehavior->m_pBetBehaviorResult->AssignTableData(cEngineTestApp::m_sbFreeToPlay?0:cEngineTestApp::m_spBetBehavior->m_iTotalBet);
}

void	cBetPhase::Update(float e_fElpaseTime)
{
	if( cEngineTestApp::m_spPlayerData->m_iMoney != 0 )
	{
		m_pBetDataButtonManaget->SingnalProcess();
		m_pBetDataButtonManaget->Update(e_fElpaseTime);
		if( m_pBetDataButtonManaget->GetCurrentWorkingObject() )
			m_pBetDataButtonManaget->Init();
		m_pBetDataButtonManaget->SetLightOn();
	}
	else
	{
		m_pBetDataButtonManaget->SetLightOff();
	}
	//if( m_iReadMeProgress != 0 )
	//	return;
	//cEngineTestApp::m_spSlotLineObjectManagerList->Update(e_fElpaseTime);
	if( this->m_bSatisfiedCondition == true )
	{
		m_pBetDataButtonManaget->SetLightOff();
		//if(!this->m_bAutoSpin)
		//	cGameApp::SoundPlay(SPIN_SOUND_NAME,true);
		SetupResult();
	}
	cEngineTestApp::m_spBetBehavior->UpdateSelectedLines(e_fElpaseTime);
	
}

void	cBetPhase::Render()
{
	cEngineTestApp::m_spKing->Render();
	m_pConstantUI->Render();
	m_pBetDataButtonManaget->Render();
	cEngineTestApp::m_spBetBehavior->Render();
	cEngineTestApp::m_spSlotLineObjectManagerList->Render();
	cEngineTestApp::m_spBetBehavior->RenderSelectedLines();
}

void	cBetPhase::DebugRender()
{
	//m_pBetDataButtonManaget->DebugRender();
}

void	cBetPhase::MouseDown(int e_iX,int e_iY)
{
	m_pBetDataButtonManaget->MouseDown(e_iX,e_iY);
}

void	cBetPhase::MouseUp(int e_iX,int e_iY)
{
	m_pBetDataButtonManaget->MouseUp(e_iX,e_iY);
}

void	cBetPhase::MouseMove(int e_iX,int e_iY)
{
	m_pBetDataButtonManaget->MouseMove(e_iX,e_iY);
}

void	cBetPhase::KeyDown(char e_cKey)
{
	//m_pBetDataButtonManaget->SingnalProcess(e_cKey);
}

void	cBetPhase::KeyPress(char e_cKey)
{
	//m_pBetDataButtonManaget->SingnalProcess(e_cKey);
}

void	cBetPhase::KeyUp(char e_cKey)
{
	//m_pBetDataButtonManaget->SingnalProcess(e_cKey);
}

void	cBetPhase::RenderReadMeMenu()
{
	if( m_pMenu )
		m_pMenu->Render();
}

void	cBetPhase::SetLightOn()
{
	m_pBetDataButtonManaget->SetLightOn();
}

const WCHAR*	cBetPhase::GetNextPhaseName()
{
	cLight::TurnBetPhaseLight(false);
#ifdef _DEBUG
//	OutputDebugString(L"leave cBetPhase\n");
#endif
	return SPIN_PHASE_NAME;
}

cImageButton*g_pAutoSpinButton = 0;
cClickMouseBehavior*g_pExchangeRateButton = 0;
cBetDataButtonManaget::cBetDataButtonManaget(cBetPhase*e_pBetPhase)
{
	cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	cMPDI*l_pButtonClickMPDI = l_pMPDIList->GetObject(L"but1");
	cMPDI*l_pButtonPressedMPDI = l_pMPDIList->GetObject(L"but2");
	unsigned char	l_ucKeyDataList[] = { 'G','A','L','J','I','K','S','R','E' };
	m_pBetPhase = e_pBetPhase;
	WCHAR*l_strButtonName[] = {	BET_MAX_BUTTON_NAME,
								AUTO_SPIN_BUTTON_NAME,
								LINE_ADD_BUTTON_NAME,
								LINE_MNUS_BUTTON_NAME,
								ADD_BET_BUTTON_NAME,
								MINES_BET_BUTTON_NAME,
								SPIN_BUTTON_NAME,
								MENU_BUTTON_NAME,
								EXCHANGE_BUTTON_NAME};
	for( int i=0;i<eBT_MAX;++i )
	{
		cSubMPDI*l_pSubMPDI = l_pButtonClickMPDI->GetObject(l_strButtonName[i]);
		cSubMPDI*l_pPressedSubMPDI = l_pButtonPressedMPDI->GetObject(l_strButtonName[i]);
		cImageButton*l_pButton = new cImageButton(l_pSubMPDI);
		l_pButton->SetPressedImage(l_pPressedSubMPDI);
		if( i == eBT_AutoSpinButton )
		{
			m_pAutoSpinYesImageButton = new cImageButton(l_pPressedSubMPDI);
			m_pAutoSpinYesImageButton->SetPressedImage(l_pSubMPDI);
			m_pAutoSpinYesImageButton->SetKeyData(l_ucKeyDataList[i]);
		}

		l_pButton->SetKeyData(l_ucKeyDataList[i]);
		if( i != eBT_SpinButton && i != eBT_MenuButton && i != eBT_ExchangeButton && i != eBT_BetMaxButton && i != eBT_AutoSpinButton )
		{
			l_pButton->SetPressedRepeatMouseUp(true);
		}
		this->AddObjectNeglectExist(l_pButton);
	}
	//g_pAutoSpinButton = this->GetObject(eBT_AutoSpinButton);
	g_pAutoSpinButton = m_pAutoSpinYesImageButton;
	g_pExchangeRateButton = this->GetObject(eBT_ExchangeButton);
	if( g_pIOSMDriverInterface&&cGameApp::m_sbDeviceExist )
	{
		int	l_iCount = g_pIOSMDriverInterface->Count();
		bool	l_b = g_pIOSMDriverInterface->Parse("JewelsKing/LightIO.xml");
		if( l_b && l_iCount != g_pIOSMDriverInterface->Count() )
		{
			int	l_iAddedCount = g_pIOSMDriverInterface->Count()-l_iCount;
			m_LightIO.SetFromResource(true);
			for( int i=l_iCount;i<l_iAddedCount+l_iCount;++i )
			{
				m_LightIO.AddObject(g_pIOSMDriverInterface->GetObject(i));
			}
		}
	}
}

cBetDataButtonManaget::~cBetDataButtonManaget()
{
	SAFE_DELETE(m_pAutoSpinYesImageButton);
}

void	cBetDataButtonManaget::ProcessButtEvent(eButtonList e_eButtonList)
{
	if( m_pBetPhase->m_iReadMeProgress  != 0  && e_eButtonList != eBT_MenuButton )
	{
		return;
	}
	switch(e_eButtonList)
	{
		case eBT_MenuButton:
			{
				cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
				m_pBetPhase->m_iReadMeProgress++;
				if( m_pBetPhase->m_iReadMeProgress>3 )
					m_pBetPhase->m_iReadMeProgress = 0;
				if( m_pBetPhase->m_iReadMeProgress == 1 )
					m_pBetPhase->m_pMenu = l_pMPDIList->GetObject(L"Menu1");
				else
				if( m_pBetPhase->m_iReadMeProgress == 2 )
					m_pBetPhase->m_pMenu = l_pMPDIList->GetObject(L"Menu2");
				else
				if( m_pBetPhase->m_iReadMeProgress == 3 )
					m_pBetPhase->m_pMenu = l_pMPDIList->GetObject(L"Menu3");
				else
					m_pBetPhase->m_pMenu = 0;
				if( m_pBetPhase->m_pMenu )
					m_pBetPhase->m_pMenu->Init();
			}
			break;
	
		case eBT_BetMaxButton:
			cEngineTestApp::m_spBetBehavior->BetMax();
			break;
		case eBT_AutoSpinButton:
			if(m_pBetPhase->ChecMoneyEncoughToBet(true))
			{
				m_pBetPhase->m_bAutoSpin = true;
				m_pBetPhase->m_bSatisfiedCondition = true;
				//cGameApp::SoundPlay(AUTO_SPIN_SOUND_NAME,true);
			}
			break;
		case eBT_AddLineBetButton:
			cEngineTestApp::m_spBetBehavior->AddLineBet();
			break;
		case eBT_MinusLineBetButton:
			cEngineTestApp::m_spBetBehavior->MinusLineBet();
			break;
		case eBT_AddBetMoneyButton:
			cEngineTestApp::m_spBetBehavior->AddBetMoney();
			break;
		case eBT_MinusBetMoneyButton:
			cEngineTestApp::m_spBetBehavior->MinusBetMoney();
			break;
		case eBT_SpinButton:
			if(m_pBetPhase->ChecMoneyEncoughToBet(true))
				this->m_pBetPhase->m_bSatisfiedCondition = true;
		break;
		case eBT_ExchangeButton:
		{
			int	l_iOriginalRate = m_pBetPhase->m_ExchangeRateVector[m_pBetPhase->m_iCurrentFocusExchangeRate];
			cMPDIList*l_pMPDIList = cGameApp::m_spAnimationParser->GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
			cMPDI*l_pButtonPressedMPDI = l_pMPDIList->GetObject(L"but2");
			cSubMPDI*l_pPressedSubMPDI = l_pButtonPressedMPDI->GetObject(EXCHANGE_BUTTON_NAME);
			
			++this->m_pBetPhase->m_iCurrentFocusExchangeRate;
			if( this->m_pBetPhase->m_iCurrentFocusExchangeRate >= (int)this->m_pBetPhase->m_ExchangeRateVector.size() )
			{
				this->m_pBetPhase->m_iCurrentFocusExchangeRate = 0;
			}
			int	l_iNewRate = m_pBetPhase->m_ExchangeRateVector[m_pBetPhase->m_iCurrentFocusExchangeRate];
			sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = l_pPressedSubMPDI->GetPointData(this->m_pBetPhase->m_iCurrentFocusExchangeRate);
			float	*l_pfUV = l_pTexBehaviorDataWithImageIndexData->pPI->GetObject(l_pTexBehaviorDataWithImageIndexData->iImageIndex)->GetUV();
			cImageButton*l_pImageButton = (cImageButton*)this->GetObject(eBT_ExchangeButton);
			l_pImageButton->SetUV(l_pfUV);
			l_pPressedSubMPDI = dynamic_cast<cSubMPDI*>(l_pImageButton->GetPressedImage());
			l_pPressedSubMPDI->Init();
			l_pPressedSubMPDI->Update(l_pPressedSubMPDI->GetEndTime()/l_pPressedSubMPDI->GetPointDataList()->size()*this->m_pBetPhase->m_iCurrentFocusExchangeRate);
			
			cEngineTestApp::m_spBetBehavior->ExchangeRateChange(l_iOriginalRate,l_iNewRate);
		}
			break;
	}
}

void	cBetDataButtonManaget::Init()
{
	g_pAutoSpinButton->Init();
	cClickMouseBehaviorList<cClickMouseBehavior>::Init();
	SetLightOn();
}

void	cBetDataButtonManaget::SetLightOn()
{
	if( g_pIOSMDriverInterface )
	{
		if(!cEngineTestApp::m_spControlPanel->IsInControlPanelMode())
		if(!this->m_pBetPhase->m_bAutoSpin && cEngineTestApp::m_siFreeGame == 0 )
		{
			//int	l_iCount = m_LightIO.Count();
			//if( l_iCount )
			{
				cLight::TurnBetPhaseLight(true);
			}
		}
	}
}

void	cBetDataButtonManaget::SetLightOff()
{
	if( g_pIOSMDriverInterface )
	{
		//int	l_iCount = m_LightIO.Count();
		//if( l_iCount )
		{
			cLight::TurnBetPhaseLight(false);
		}
	}
}

void    cBetDataButtonManaget::Update(float e_fElpaseTime)
{
    int l_iCount = this->Count();
    for( int i=0;i<l_iCount;++i )
    {
        this->m_ObjectList[i]->Update(e_fElpaseTime);
        if( this->m_ObjectList[i]->IsSatisfiedCondition() )
        {
			ProcessButtEvent((eButtonList)i);
			cImageButton*l_pImageButton = (cImageButton*)m_ObjectList[i];
			l_pImageButton->Init();
			if( l_pImageButton->IsPressedRepeatMouseUp() )
				l_pImageButton->InitRepeat();
        }
    }
}