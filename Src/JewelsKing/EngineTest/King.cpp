#include "stdafx.h"
#include "DownScreen.h"
#include "UpperScreen.h"
#include "King.h"
#include "GameApp.h"


WCHAR*g_strKingStatusName[] = {L"Idle",L"FreeGame",L"Money1-500",
L"Money501-1000",L"Money1001-5000",L"Money5000"};

WCHAR*g_strJewelStatusName[] = {L"0",L"1",L"2",L"3"};


WCHAR*g_strLingStatusMPDIName[] = {L"kingwaitA",L"kinglook",L"king1-500",
L"king501-1000",L"king1001-5000",L"king5000"};

WCHAR*g_strKingSound[] = {
KING_IDLE_SOUND_NAME,
PLAY_FREE_GAME_SOUND_NAME,
//KING_WALKING_SOUND_NAME,
PRIZE_1_500_SOUND_NAME,
PRIZE_1_500_SOUND_NAME,
PRIZE_501_1000_SOUND_NAME,
PRIZE_501_1000_SOUND_NAME,
//KING_WALKING_SOUND_NAME
};


#define	NUMERIC_IMAGE_NAME	L"BonusNum_0"

sFreeGameData::sFreeGameData()
{
	pPrizeMPDI = 0;
	pPrizeAwardMPDI = 0;
	cMPDIList*	l_pFreegametimesMPDIList = cGameApp::GetMPDIListByFileName(FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME);
	pPrizeAward_NumerialMPDI = l_pFreegametimesMPDIList->GetObject(L"FreeTimeNum");
	//m_pPrizeAwardMPDI = l_pFreegametimesMPDIList->GetObject(L"freegame");
	pPrizeAwardMPDI = l_pFreegametimesMPDIList->GetObject(L"FreeTimeDiamond");
	iTimeForPickupGems = 0;
}
sFreeGameData::~sFreeGameData()
{
	SAFE_DELETE(pPrizeMPDI);
}


void	sFreeGameData::SetupRestFreeSpinTimes(int e_iPrize)
{
	//SAFE_DELETE(pPrizeMPDI);
	//pPrizeMPDI = new cMPDI(pPrizeAwardMPDI);
	//cMPDIList*	l_pFreegametimesMPDIList = cGameApp::GetMPDIListByFileName(FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME);
	//int	l_iNum = 1;
	//if( e_iPrize>99 )
	//{
	//	pPrizeAward_NumerialMPDI = l_pFreegametimesMPDIList->GetObject(L"NunIn3");
	//	l_iNum = 3;
	//}
	//else
	//if( e_iPrize>9 )
	//{
	//	pPrizeAward_NumerialMPDI = l_pFreegametimesMPDIList->GetObject(L"NunIn2");
	//	l_iNum = 2;
	//}
	//else
	//{
	//	pPrizeAward_NumerialMPDI = l_pFreegametimesMPDIList->GetObject(L"NunIn1");
	//	l_iNum = 1;
	//}
	//int	l_iValue[3];
	//l_iValue[0] = e_iPrize/100;
	//l_iValue[1] = (e_iPrize-l_iValue[0]*100)/10;
	//l_iValue[2] = (e_iPrize-l_iValue[0]*100-l_iValue[1]*10);
	//int*l_pStartValue = &l_iValue[0];
	//if( l_iNum == 3 )
	//	l_pStartValue = &l_iValue[0];
	//else
	//if( l_iNum == 2 )
	//	l_pStartValue = &l_iValue[1];
	//if( l_iNum == 1 )
	//	l_pStartValue = &l_iValue[2];

	//for( int i=0;i<l_iNum;++i )
	//{
	//	std::wstring	l_strNumName = NUMERIC_IMAGE_NAME;
	//	cSubMPDI*l_pSubMPDI = pPrizeAward_NumerialMPDI->GetObject(i);
	//	{
	//		l_strNumName += ValueToStringW(*l_pStartValue);
	//		int	l_iImageIndex = l_pSubMPDI->GetPuzzleImage()->GetObjectIndexByName(l_strNumName.c_str());
	//		l_pSubMPDI->ChangeAllImageByImageIndex(l_iImageIndex);
	//		cSubMPDI*l_pSubMPDIClone = new cSubMPDI(l_pSubMPDI);
	//		pPrizeMPDI->AddObjectNeglectExist(l_pSubMPDIClone);
	//		++l_pStartValue;
	//	}
	//}
	//pPrizeMPDI->Start();
}

void	sFreeGameData::Update(float e_fElpaseTime)
{
	if( pPrizeMPDI )
	{
		pPrizeMPDI->Update(e_fElpaseTime);
		if(pPrizeMPDI->IsDone())
			SAFE_DELETE(pPrizeMPDI);
	}		
}
void	sFreeGameData::Render()
{
	if( pPrizeMPDI )
	{
		if( iTimeForPickupGems >0 )
			pPrizeMPDI->Render();
	}
}

void	sFreeGameData::RestIntoFreeGameTimes()
{
	SAFE_DELETE(pPrizeMPDI);
	if( iTimeForPickupGems == 0 )
		return;
	pPrizeMPDI = new cMPDI(pPrizeAwardMPDI);
	int	l_iIndex = iTimeForPickupGems;
	if( l_iIndex>9 )
		l_iIndex = 9;
	cSubMPDI*l_pSubMPDI = new cSubMPDI(pPrizeAward_NumerialMPDI->GetObject(l_iIndex));
	pPrizeMPDI->AddObjectNeglectExist(l_pSubMPDI);
	pPrizeMPDI->SetLoop(true);
	pPrizeMPDI->Init();
}

cKing::cKing()
{
	m_bRenderKing = true;
	m_bRenderDownScreenBG = true;
	m_bRenderUpperScreenBG = true;
	m_bRenderPayRateHint = true;
	m_pUpperScreen = 0;
	m_pDownScreenEffect = 0;
	m_pDimond = 0;
	m_pNumberForFreeGamePayRate = 0;
	m_pFreeGameData = 0;
	for( int i=0;i<eKS_MAX;++i )
	{
		m_pKingSound[i] = 0;
	}
	{
		for( int i=0;i<eKS_MAX;++i )
		{
			m_pKingSound[i] = cGameApp::m_spSoundParser->GetObject(g_strKingSound[i]);
		}
	}
}

cKing::~cKing()
{
	SAFE_DELETE(m_pFreeGameData);
	SAFE_DELETE(m_pUpperScreen);
	SAFE_DELETE(m_pDownScreenEffect);
}

void	cKing::InitStatus()
{
	this->SetCurrentWorkingObject(eKS_IDLE);
	if(cEngineTestApp::m_sbFreeToPlay)
	{
		//m_pFreeGameData->SetupRestFreeSpinTimes(cEngineTestApp::m_siFreeGame+1);
	}
	this->m_pFreeGameData->RestIntoFreeGameTimes();
	SetFreeGamePayRate(0);
}

bool     cKing::SetCurrentWorkingObject(int e_iIndex,bool e_bRestart)
{
	int	l_iIndex = this->GetCurrentWorkingObjectIndex();
	if( l_iIndex != -1 )
	{
		if( m_pKingSound[l_iIndex] )
			m_pKingSound[l_iIndex]->Play(false);
	}
	if(cStatusObjectChanger(cMPDI)::SetCurrentWorkingObject(e_iIndex,e_bRestart))
	{
		l_iIndex = this->GetCurrentWorkingObjectIndex();
		if( eKS_FREE_GAME == l_iIndex )
		{
			this->m_pDownScreenEffect->GetGameMessage()->SetFreeTime(0);
		}
		if( eKS_MONEY5000 != l_iIndex )
		{
			SetFreeGamePayRate(0);
		}
		if(m_pKingSound[l_iIndex])
		{
			m_pKingSound[l_iIndex]->Play(true);
		}
		return true;
	}
	SetFreeGamePayRate(0);
	return false;
}

void	cKing::Init()
{
	cMPDIList*	l_pMPDIList = 0;
	cMPDIList*	l_pKingMPDIList = cGameApp::GetMPDIListByFileName(KING_MPDI_MPDILIST_FILE_NAME);
	for( int i=0;i<eKS_MAX;++i )
	{
		cMPDI*l_pMPDI = l_pKingMPDIList->GetObject(g_strLingStatusMPDIName[i]);

			cObjectAndName<cMPDI>*l_pObjectAndName = 0;
			l_pObjectAndName = new cObjectAndName<cMPDI>(g_strKingStatusName[i],l_pMPDI);
			this->AddObjectNeglectExist(l_pObjectAndName);
	}

	//this->SetCurrentWorkingObject(0);
	m_iMoneyToChangeStatus.push_back(1);
	m_iMoneyToChangeStatus.push_back(500);
	m_iMoneyToChangeStatus.push_back(1000);
	m_iMoneyToChangeStatus.push_back(5000);

	m_iMoneyToChangeJewelStatus.push_back(1);
	m_iMoneyToChangeJewelStatus.push_back(501);
	m_iMoneyToChangeJewelStatus.push_back(1001);
	m_iMoneyToChangeJewelStatus.push_back(5001);

	m_pUpperScreen = new cUpperScreen();
	m_pDownScreenEffect = new cDownScreenEffect();
	m_pDimond = cGameApp::m_spPaticleManager->GetObject(L"Dimond");
	if( !m_pFreeGameData )
	{
		m_pFreeGameData = new sFreeGameData();
	}
}

void	cKing::Update(float e_fElpaseTime)
{
	if( m_pDimond )
		m_pDimond->Update(e_fElpaseTime);
	if( m_pUpperScreen && m_bRenderUpperScreenBG )
		m_pUpperScreen->Update(e_fElpaseTime);
	if( m_pDownScreenEffect && m_bRenderDownScreenBG )
		m_pDownScreenEffect->Update(e_fElpaseTime);
	cStatusObjectChanger(cMPDI)::Update(e_fElpaseTime);
	if(this->GetCurrentWorkingObject())
	{
		cObjectAndName<cMPDI>*l_pMPDI = dynamic_cast<cObjectAndName<cMPDI>*>(this->GetCurrentWorkingObject());
		if( l_pMPDI->GetObject()->IsDone() )
		{
			if( m_pNumberForFreeGamePayRate )
				m_pNumberForFreeGamePayRate->Update(e_fElpaseTime);		
		}
	}
	if( m_pCurrentWorkingObject )
	{
		cObjectAndName<cMPDI>*l_pData = (cObjectAndName<cMPDI>*)m_pCurrentWorkingObject;
		cMPDI*l_pMPDI = l_pData->GetObject();
		if(l_pMPDI->IsHitHintPoint())
		{
			if( m_pDimond )
				m_pDimond->Emit(Vector3(cGameApp::m_svGameResolution.x/2.f,cGameApp::m_svGameResolution.y/2.f/4.f*3.f,0.f));
		}
		if( cEngineTestApp::m_sbFreeToPlay )
		{
			if( l_pMPDI->IsDone())
			{
				if( !l_pMPDI->IsStopAtLastFrame() )
				{
					//this->SetCurrentWorkingObject(L"Idle");
				}
			}
		}
		else
		{
			//if(this->GetCurrentWorkingObjectIndex() != eKS_FREE_GAME_WALK_AWAY  && eKS_FREE_GAME_BACK != this->GetCurrentWorkingObjectIndex())
			//{
			//	if( l_pMPDI->IsDone())
			//	{
			//		if( !l_pMPDI->IsStopAtLastFrame() )
			//		{
			//			//this->SetCurrentWorkingObject(L"Idle");
			//		}
			//	}
			//}
		}
	}
	if( m_pFreeGameData )
		m_pFreeGameData->Update(e_fElpaseTime);
}

void	cKing::RenderUpperScreen()
{
	m_pUpperScreen->Render();
}

void	cKing::Render()
{
	if( m_bRenderUpperScreenBG )
		RenderUpperScreen();
	if( m_bRenderKing )
		cStatusObjectChanger(cMPDI)::Render();
	if( m_pDimond )
		m_pDimond->Render();
	if( m_pNumberForFreeGamePayRate )
		m_pNumberForFreeGamePayRate->Render();
	m_pFreeGameData->Render();
}

void	cKing::RenderDownScreenEffect()
{
	if( m_pDownScreenEffect )
		m_pDownScreenEffect->Render();
}

void	cKing::RenderPerLineBetMoney()
{
	if( m_pDownScreenEffect )
		m_pDownScreenEffect->DrawPerLineBetMoney();
}

void	cKing::RenderGameMessage()
{
	m_pDownScreenEffect->GetGameMessage()->Render();
}

void	cKing::ChangeStatusByMoney(int e_iMoney,bool e_bShowBigWin)
{
	if( e_bShowBigWin )
		m_pDownScreenEffect->ChangeStatusByMoney(e_iMoney);
	size_t	l_uiSize = m_iMoneyToChangeJewelStatus.size();
	bool	l_bJelwel = false;
	int i = (int)l_uiSize-1;
	l_uiSize = m_iMoneyToChangeStatus.size();
	i = (int)l_uiSize-1;
	for( ; i>-1; --i )
	{
		if(e_iMoney>=m_iMoneyToChangeStatus[i])
		{
			//if( eKS_MONEY1+i == eKS_MONEY1001 )
			//	m_pDimond->Start(Vector3(cGameApp::m_svGameResolution.x/2.f,cGameApp::m_svGameResolution.y/2.f/4.f*3.f,0.f));
			int	l_iIndex = eKS_MONEY1+i;
			//if( this->GetCurrentWorkingObjectIndex() != l_iIndex )
			{
				this->SetCurrentWorkingObject(l_iIndex);
			}
			//else
			//{
			//	if(m_pKingSound[l_iIndex])
			//	{
			//		m_pKingSound[l_iIndex]->Play(true);
			//	}			
			//}
			return;
		}
	}	
}



bool			cKing::IsBigWinDone()
{
	if(this->m_pDownScreenEffect)
		return m_pDownScreenEffect->IsBigWinDone();
	return true;
}


cGameMessage*	cKing::GetGameMessage(){ return m_pDownScreenEffect->GetGameMessage(); }

void	cKing::SetFreeGamePayRate(int e_iPayRate)
{
	if( e_iPayRate == 0 )
	{
		if( m_pNumberForFreeGamePayRate )
		{
			m_pNumberForFreeGamePayRate->Stop();
			m_pNumberForFreeGamePayRate->SetStopAtLastFrame(false);
		}
		return;
	}
	cMPDIList*	l_pKingMPDIList = cGameApp::GetMPDIListByFileName(UI_MPDILIST_FILE_NAME);
	cMPDI*l_pMPDI = 0;
	std::wstring	l_strNumName = NUMERIC_IMAGE_NAME;
	int	l_iValue = 0;
	int	l_iImageIndex = 0;
	#define	PI_NAME	L"UI"
	#define	BONUSNUM1_IMAGE_SUB_MPDI_NAME L"BonusNum1"
	#define	BONUSNUM2_IMAGE_SUB_MPDI_NAME L"BonusNum2"
	if( e_iPayRate>9 )
	{
		l_pMPDI = l_pKingMPDIList->GetObject(L"NunIn2");
		l_iValue = e_iPayRate/10;
		l_strNumName += ValueToStringW(l_iValue);
		l_iImageIndex = l_pMPDI->GetPuzzleImage(PI_NAME)->GetObjectIndexByName(l_strNumName.c_str());
		l_pMPDI->GetObject(BONUSNUM2_IMAGE_SUB_MPDI_NAME)->ChangeAllImageByImageIndex(l_iImageIndex);
	}
	else
	{
		l_pMPDI = l_pKingMPDIList->GetObject(L"NunIn1");
	}
	if( !l_pMPDI )
	{
		assert(0&&"can not get king pay rate MPDI number");
	}
	l_iValue = e_iPayRate%10;
	l_strNumName = NUMERIC_IMAGE_NAME;
	l_strNumName += ValueToStringW(l_iValue);
	l_iImageIndex = l_pMPDI->GetPuzzleImage(PI_NAME)->GetObjectIndexByName(l_strNumName.c_str());
	if( l_iImageIndex == -1 )
	{
		assert(0&&"l_iImageIndex = l_pMPDI->GetPuzzleImage(PI_NAME)->GetObjectIndexByName(l_strNumName.c_str());");
	}

	if( e_iPayRate>9 )
		l_pMPDI->GetObject(BONUSNUM1_IMAGE_SUB_MPDI_NAME)->ChangeAllImageByImageIndex(l_iImageIndex);
	else
		l_pMPDI->GetObject(BONUSNUM1_IMAGE_SUB_MPDI_NAME)->ChangeAllImageByImageIndex(l_iImageIndex);
	m_pNumberForFreeGamePayRate = l_pMPDI;
	m_pNumberForFreeGamePayRate->Init();
	m_pNumberForFreeGamePayRate->SetStopAtLastFrame(true);
}

void	cKing::RenderPayRateHint()
{
	if( m_bRenderPayRateHint && m_pUpperScreen )
		m_pUpperScreen->RenderPayRateHint();
}

void	cKing::RenderExplosionCombo()
{
	m_pUpperScreen->RenderReel();
}