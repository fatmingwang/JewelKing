#include "stdafx.h"
#include "FreeGameForPickGemsGame.h"
#include "GameApp.h"
#include "SlotObject.h"
#include "SlotRule.h"
#include "King.h"
#include "Light.h"
cPickupObjectForFreeGameAndExtraBonusRate::cPickupObjectForFreeGameAndExtraBonusRate(cSubMPDI*e_pClickButton,cSubMPDI*e_pPickupGemMPDI,int e_iMoney,sGemsBonusData e_sGemsBonusData)
:cImageButton(e_pClickButton)
{
	m_GemsBonusData = e_sGemsBonusData;
	m_iMoney = e_iMoney;
	m_pPickUpGemMPDI = e_pPickupGemMPDI;
	m_pPickUpGemMPDI->Init();
	//m_pPickUpGemMPDI->SetPos(e_vPos);
	//m_pPickUpGemMPDI->SetPos(e_vPos);
	m_pPickUpGemMPDI->SetAnimationLoop(false);
}

cPickupObjectForFreeGameAndExtraBonusRate::~cPickupObjectForFreeGameAndExtraBonusRate()
{
	//SAFE_DELETE(m_pPickUpGemMPDI);
}

bool	cPickupObjectForFreeGameAndExtraBonusRate::InternalCollide(int e_iPosX,int e_iPosY)
{
	if(this->m_TC.bTragetTimrReached)
		return false;
	return cImageButton::InternalCollide(e_iPosX,e_iPosY);
}

bool    cPickupObjectForFreeGameAndExtraBonusRate::IsSatisfiedCondition()
{
	return m_pPickUpGemMPDI->IsAnimationDone();
}

void	cPickupObjectForFreeGameAndExtraBonusRate::Update(float e_fElpaseTime)
{
	if(!cImageButton::IsSatisfiedCondition())
	{
		cImageButton::Update(e_fElpaseTime);
		if(cImageButton::IsSatisfiedCondition())
		{
			DoPickupGem();
		}
	}
	else
	{
		m_pPickUpGemMPDI->Update(e_fElpaseTime);
	}
}

void	cPickupObjectForFreeGameAndExtraBonusRate::DoPickupGem()
{
	cEngineTestApp::m_siFreeGame += m_GemsBonusData.iFreeGameTime;
	cEngineTestApp::m_spRestFreeGameTimes->push_back(m_GemsBonusData.iFreeGameTime);
	cDoAllMouseBehaviorList<cPickupObjectForFreeGameAndExtraBonusRate>*l_pImageButtonList = dynamic_cast<cDoAllMouseBehaviorList<cPickupObjectForFreeGameAndExtraBonusRate>*>(this->GetOwner());
	int	l_iIndex = l_pImageButtonList->GetObjectIndexByPointer(this);
	cEngineTestApp::m_spLogFile->WriteToFileImmediatelyWithLine("!!Into FreeGame!!");
	for( int i=0;i<m_GemsBonusData.iFreeGameTime;++i )
	{
		if( m_GemsBonusData.BonusRateList.size() )
		{
			int	l_iTarget = rand()%m_GemsBonusData.BonusRateList.size();
			cEngineTestApp::m_spExtraBonusMoneyRate->push_back( (int)m_GemsBonusData.BonusRateList[l_iTarget] );
		}
	}
	cGameApp::m_spSoundParser->GetObject(FREE_GAME_WIN_MONEY_SOUND_NAME)->Play(true);
}

void	cPickupObjectForFreeGameAndExtraBonusRate::Render()
{
	cImageButton::Render();
	if(cImageButton::IsSatisfiedCondition())
	{
		m_pPickUpGemMPDI->Render();
	}
	//this->DebugRender();
}

bool	cPickupObjectForFreeGameAndExtraBonusRate::IsPrizeGrabed()
{
	return cImageButton::IsSatisfiedCondition();
}

const wchar_t*         cFreeGameForPickGemsGame::TypeID( L"cFreeGameForPickGemsGame" );

cFreeGameForPickGemsGame::cFreeGameForPickGemsGame(const WCHAR*e_strConditionName,int e_iConditionCount,TiXmlElement*e_pGemsBonusData,cMPDI*e_pConstantUI)
:cBaseExtraGame(e_strConditionName,e_iConditionCount)
{
	cEngineTestApp::m_spSlotRuleData->m_iNumFreeGameToAchevied = e_iConditionCount;
	m_pFreeGameSpinObjectShow = 0;
	m_pUpperScreenUI = 0;
	m_pConstantUI = e_pConstantUI;
	m_pConstantUI->Init();
	m_pConstantUI->Update(0.0001f);
	TiXmlElement*l_pTiXmlElement = e_pGemsBonusData->FirstChildElement();
	while( l_pTiXmlElement )
	{
		sGemsBonusData	l_sGemsBonusData;
		l_sGemsBonusData.iFreeGameTime = _wtoi(l_pTiXmlElement->Attribute(L"FreeTimes"));
		const WCHAR*l_strBonusRate = l_pTiXmlElement->Attribute(L"BonusRate");
		l_sGemsBonusData.BonusRateList = GetValueListByCommaDivide<float>(l_strBonusRate);
		m_GemsBonusDataList.push_back(l_sGemsBonusData);
		l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
	}
	m_TimeForNoActionToLeave.SetTargetTime(50.f);
	m_eFreeGameList = eFGL_PICK_ROCK_BONUS;
	m_strImageName = L"Free";
	m_bPickRockToWinMoneyDone = false;
	m_iRockShowRowIndex = 1;
	m_pEnteringMPDI = 0;
	m_pLeavingMPDI = 0;
	m_pMPDIList = 0;
	m_pSelectedObject = 0;
}

cFreeGameForPickGemsGame::~cFreeGameForPickGemsGame()
{

}

void	cFreeGameForPickGemsGame::MouseMove(int e_iPosX,int e_iPosY)
{
	if( m_pEnteringMPDI->IsDone() )
	{
		if(!IsObjectPickUpDone() && !m_ImageButtonList.GetCurrentMouseDownCollidedObject() )
			m_ImageButtonList.MouseMove(e_iPosX,e_iPosY);
	}
}

void	cFreeGameForPickGemsGame::MouseDown(int e_iPosX,int e_iPosY)
{
	if( m_pEnteringMPDI->IsDone() )
	{
		if(!IsObjectPickUpDone() && !m_ImageButtonList.GetCurrentMouseDownCollidedObject() )
			m_ImageButtonList.MouseDown(e_iPosX,e_iPosY);
	}
}

void    cFreeGameForPickGemsGame::MouseUp(int e_iPosX,int e_iPosY)
{
	if( m_pEnteringMPDI->IsDone() )
	{
		if(!IsObjectPickUpDone() && !m_ImageButtonList.GetCurrentMouseDownCollidedObject() )
			m_ImageButtonList.MouseUp(e_iPosX,e_iPosY);
	}
}
//return true if the condition of bonus is satisfied.
//bool	cFreeGameForPickGemsGame::InternalConditionCheck()
//{
//	if( cBaseExtraGame::IsConditionOk() )
//		return true;
//	return false;
//}


void	cFreeGameForPickGemsGame::SetupResult()
{
	WCHAR*l_strClickMPDI = L"diamain";
	WCHAR*l_strPickupMPDI = L"diamain2";
	int	l_iMoneyToGive[] = {500,0,1000,0,0};
	cSlotObjectManager*l_pSlotObjectManager = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(0);
	cPuzzleImage*l_pPI = l_pSlotObjectManager->GetAllSpinObjectPI();
	cPuzzleImageUnit*l_pImage = l_pPI->GetObject(m_strImageName.c_str());		
	vector<int>	l_uiAllMagicIDVector1 = UT::GenerateRandomTable(5,5);
	//for( int i=0;i<cEngineTestApp::m_spSlotRuleData->m_TableCount.x;++i )
	char	l_KeyData[] = {'C','V','B','N','M'};
	for( int i=0;i<cEngineTestApp::m_spSlotRuleData->m_TableCount.x;++i )
	{
		cSlotObjectManager*l_pSlotObjectManager = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(i);
		Vector3	l_vPos = l_pSlotObjectManager->IndexToResultShowPos(m_iRockShowRowIndex);
		l_vPos.y -= 683.f;
		int	l_iMoneyToWin = l_iMoneyToGive[l_uiAllMagicIDVector1[i]];
		cMPDI*l_pPickupMPDI = m_pMPDIList->GetObject(l_strPickupMPDI);
		cMPDI*l_pClickMPDI = m_pMPDIList->GetObject(l_strClickMPDI);

		cPickupObjectForFreeGameAndExtraBonusRate*l_pPickupObjectToWinMoney = new cPickupObjectForFreeGameAndExtraBonusRate(l_pClickMPDI->GetObject(i),l_pPickupMPDI->GetObject(i),l_iMoneyToWin,this->m_GemsBonusDataList[i]);
		l_pPickupObjectToWinMoney->SetKeyData(l_KeyData[i]);
		m_ImageButtonList.AddObjectNeglectExist(l_pPickupObjectToWinMoney);
	}
	m_ImageButtonList.Init();
}

bool	cFreeGameForPickGemsGame::InternalConditionCheck()
{
	int	l_iTotalCount = cEngineTestApp::m_spSlotRuleData->m_TableCount.x*cEngineTestApp::m_spSlotRuleData->m_TableCount.y;
	int	l_iSame = 0;
	for( int i = 0;i<l_iTotalCount;++i)
	{
		std::wstring	l_strTargetName = GetSpinObjectNameByIndex(i);
		if(!wcscmp(this->m_strConditionName.c_str(),l_strTargetName.c_str()))
		{
			++l_iSame;
			if(this->m_iConditionCount<=l_iSame)
			{
				return true;
			}
		}
	}

	//int	l_iCount = cEngineTestApp::m_spSlotLineObjectManagerList->Count();
	//int	l_iSize = (int)cEngineTestApp::m_spSlotRuleData->m_LineRuleList.size();
	//for( int i = 0;i<l_iSize;++i)
	//{
	//	sLineRule*l_pLineRule = cEngineTestApp::m_spSlotRuleData->m_LineRuleList[i];
	//	char	l_cCount = l_pLineRule->cOneLineCount;
	//	int	l_iSame = 0;
	//	for( char j = 0;j<l_cCount;++j)
	//	{
	//		std::wstring	l_strTargetName = GetSpinObjectNameByIndex(l_pLineRule->pcLineObjectIndex[j]);
	//		if(!wcscmp(this->m_strConditionName.c_str(),l_strTargetName.c_str()))
	//		{
	//			++l_iSame;
	//		}
	//		else
	//			break;
	//	}
	//	if(this->m_iConditionCount<=l_iSame)
	//	{
	//		cGameApp::m_sfDebugValue = 1.f;
	//		return true;
	//	}
	//}
	return false;
}

void	cFreeGameForPickGemsGame::LoadResource()
{
	cGameApp::m_spSoundParser->GetObject(FREE_GAME_CONDITION_OK)->Play(true);
	if( !m_pMPDIList )
	{
		m_pMPDIList = cGameApp::GetMPDIListByFileName(FREE_GAME_BONUS_MPDILIST_FILE_NAME);
		m_pEnteringMPDI = m_pMPDIList->GetObject(L"INTO");
		m_pLeavingMPDI = m_pMPDIList->GetObject(L"LEAVE");
		m_pAwardListMPDI = m_pMPDIList->GetObject(L"plate");
		//m_pUpperScreenUI = m_pMPDIList->GetObject(L"UIsb");
		m_pEnteringMPDI->SetLoop(false);
		m_pLeavingMPDI->SetLoop(false);
		m_pAwardListMPDI->SetLoop(false);
		m_pAwardListMPDI->Init();
	}
	if( !m_pFreeGameSpinObjectShow )
	{
		cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(WIN_MONEY_SPIN_OBJECT_MPDI_MPDILIST_FILE_NAME);
		m_pFreeGameSpinObjectShow = l_pMPDIList->GetObject(L"Free");
	}
	m_pFreeGameSpinObjectShow->Init();
	if( m_pUpperScreenUI )
		m_pUpperScreenUI->Init();
	m_bPickRockToWinMoneyDone = false;
	m_pEnteringMPDI->Init();
	m_pLeavingMPDI->Init();
	SetupResult();
	m_TimeForNoActionToLeave.Start();
	cEngineTestApp::m_spKing->SetCurrentWorkingObject(-1);
	if( g_pIOSMDriverInterface&&cGameApp::m_sbDeviceExist )
	{
		//dont ask me why because I am fucking lazy to do this
		g_pIOSMDriverInterface->WriteOutputData(LIGHT_FLASH_ID,true);
	}

	cGameApp::m_spSoundParser->GetObject(NORMAL_BGM_SOUND_NAME)->Play(false);
	cGameApp::m_spSoundParser->GetObject(FREE_GAME_BGM_SOUND_NAME)->Play(true);

	cEngineTestApp::m_spKing->SetFreeGamePayRate(0);
	cEngineTestApp::m_spKing->SetRenderPayRateHint(false);
	cEngineTestApp::m_spKing->SetRenderUpperScreenBG(false);
	m_pSelectedObject = 0;
}

void	cFreeGameForPickGemsGame::Destroy()
{
	if( m_pMPDIList )
	{
		//cGameApp::m_spAnimationParser->RemoveObject(m_pMPDIList->GetName());
		//m_pMPDIList = 0;
	}
	m_ImageButtonList.Destroy();
	if( g_pIOSMDriverInterface&&cGameApp::m_sbDeviceExist )
	{
		cLight::TurnUpperLight(false);
		//dont ask me why because I am fucking lazy to do this
		g_pIOSMDriverInterface->WriteOutputData(LIGHT_FLASH_ID,false);
	}
	cGameApp::m_spSoundParser->GetObject(FREE_GAME_WIN_MONEY_SOUND_NAME)->Play(false);
	cEngineTestApp::m_spKing->SetRenderUpperScreenBG(true);
	m_ImageButtonList.SetCurrentMouseDownCollidedObject(0);
#ifdef _DEBUG
//	OutputDebugString(L"leave cFreeGameForPickGemsGame\n");
#endif
}

bool	cFreeGameForPickGemsGame::IsObjectPickUpDone()
{
	int	l_iCount = m_ImageButtonList.Count();
	for( int i=0;i<l_iCount;++i )
	{
		if( m_ImageButtonList[i]->IsPrizeGrabed() )
		{
			return true;
		}
	}
	return false;
}

void	cFreeGameForPickGemsGame::DoSelected()
{
	m_bPickRockToWinMoneyDone = true;
	m_TimeForNoActionToLeave.Start();
	cEngineTestApp::m_spKing->SetCurrentWorkingObject(-1);
	if( g_pIOSMDriverInterface&&cGameApp::m_sbDeviceExist )
	{
		cLight::TurnUpperLight(false);
		//dont ask me why because I am fucking lazy to do this
		g_pIOSMDriverInterface->WriteOutputData(LIGHT_FLASH_ID,false);
	}
}

void	cFreeGameForPickGemsGame::Update(float e_fElpaseTime)
{
	if( m_pUpperScreenUI )
		m_pUpperScreenUI->Update(e_fElpaseTime);

	if( !m_pFreeGameSpinObjectShow->IsDone() )
	{
		m_pFreeGameSpinObjectShow->Update(e_fElpaseTime);
		return;
	}
	//if(cEngineTestApp::m_spKing->GetCurrentWorkingObjectIndex() == eKS_FREE_GAME_WALK_AWAY)
	//{
	//	cObjectAndName<cMPDI>*l_pMPDI = (cObjectAndName<cMPDI>*)cEngineTestApp::m_spKing->GetCurrentWorkingObject();
	//	if( !l_pMPDI->GetObject()->IsDone() )
	//		return;
	//}
	//else
	//if(cEngineTestApp::m_spKing->GetCurrentWorkingObjectIndex() == eKS_FREE_GAME_BACK)
	//{
	//	cObjectAndName<cMPDI>*l_pMPDI = (cObjectAndName<cMPDI>*)cEngineTestApp::m_spKing->GetCurrentWorkingObject();
	//	if( l_pMPDI->GetObject()->IsDone() )
	//	{
	//		this->m_bDone = true;
	//		cEngineTestApp::m_spKing->SetRenderBG(true);
	//		cEngineTestApp::m_spKing->SetCurrentWorkingObject(eKS_IDLE);
	//	}
	//	return;
	//}
	if( !m_TimeForNoActionToLeave.bTragetTimrReached )
	{
		m_TimeForNoActionToLeave.Update(e_fElpaseTime);
		if(m_TimeForNoActionToLeave.bTragetTimrReached )
		{
			DoSelected();
			cPickupObjectForFreeGameAndExtraBonusRate*l_pPickupObjectToWinMoney = (cPickupObjectForFreeGameAndExtraBonusRate*)m_ImageButtonList[0];
			l_pPickupObjectToWinMoney->DoPickupGem();
		}
	}
	if( m_bPickRockToWinMoneyDone )
	{
		if( !m_pLeavingMPDI->IsDone() )
		{
			m_pLeavingMPDI->Update(e_fElpaseTime);
			if( m_pLeavingMPDI->IsDone() )
			{
				cEngineTestApp::m_spKing->SetCurrentWorkingObject(eKS_IDLE);
				this->m_bDone = true;
			}
		}
	}
	else
	{
		if( m_pEnteringMPDI->IsDone() )
		{
			if( !m_bPickRockToWinMoneyDone )
			{
				if( !m_pSelectedObject )
				{
					m_ImageButtonList.Update(e_fElpaseTime);
					if(!IsObjectPickUpDone())
						m_ImageButtonList.SingnalProcess();
					int	l_iCount = m_ImageButtonList.Count();
					if( !m_pSelectedObject )
					{
						for( int i=0;i<l_iCount;++i )
						{
							if(m_ImageButtonList[i]->GetMouseBehavior() != eOMB_NONE  && !m_pSelectedObject)
							{
								m_pSelectedObject = m_ImageButtonList[i];
								break;
							}
						}
					}
				}
				else
				{
					if( !m_pSelectedObject->IsSatisfiedCondition() )
					{
						m_pSelectedObject->Update(e_fElpaseTime);
						m_pSelectedObject->SingnalProcess();
						m_pSelectedObject->ButtonUp();
					}
					else
					{
						DoSelected();
					}
				}
			}
		}
		else
		{
			m_pEnteringMPDI->Update(e_fElpaseTime);
			if( m_pEnteringMPDI->IsDone() )
			{
				cEngineTestApp::m_spKing->SetCurrentWorkingObject(eKS_FREE_GAME);
				cLight::TurnUpperLight(true);
			}
		}
	}
}

void	cFreeGameForPickGemsGame::Render()
{
	if( m_pFreeGameSpinObjectShow->IsDone() )
	{
		if( m_bPickRockToWinMoneyDone )
		{
			if( m_pLeavingMPDI->IsDone() )
			{
				
			}
			else
			{
			}
			m_pLeavingMPDI->Render();
			if(m_pUpperScreenUI)
				m_pUpperScreenUI->Render();
			cEngineTestApp::m_spKing->Render();
		}
		else
		{
			m_pEnteringMPDI->Render();
			if(m_pUpperScreenUI)
				m_pUpperScreenUI->Render();
			cEngineTestApp::m_spKing->Render();
			if( m_pEnteringMPDI->IsDone() )
			{
				m_ImageButtonList.Render();
				//m_pAwardListMPDI->Render();
			}
			else
			{
				//if( m_pEnteringMPDI->GetCurrentProgress() >0.5f )
				{
					//m_ImageButtonList.Render();
				}
			}
		}
		m_pConstantUI->Render();
		if( m_pEnteringMPDI->IsDone() )
		{
			m_pAwardListMPDI->Render();
		}
		cEngineTestApp::m_spSlotLineObjectManagerList->Render();
	}
	else
	{
		m_pEnteringMPDI->Render();
		m_pConstantUI->Render();
		cEngineTestApp::m_spSlotLineObjectManagerList->Render();	
		cPuzzleImage*l_pPI = cGameApp::m_spAnimationParser->GetAllBaseImageList()->GetPuzzleImage(SPIN_OBJECT_PI_NAME);
		cPuzzleImageUnit*l_pPIUnit = l_pPI->GetObject(0);
		float	l_fImageHalfWidth = *l_pPIUnit->GetImageShowWidth()/2.f;
		float	l_fImageHalfHeight = *l_pPIUnit->GetImageShowHeight()/2.f;
		int	l_iTotalCount = cEngineTestApp::m_spSlotRuleData->m_TableCount.x*cEngineTestApp::m_spSlotRuleData->m_TableCount.y;
		for( int i = 0;i<l_iTotalCount;++i)
		{
			std::wstring	l_strTargetName = GetSpinObjectNameByIndex(i);
			if(!wcscmp(this->m_strConditionName.c_str(),l_strTargetName.c_str()))
			{
				sLoopMovingObject*l_pLoopMovingObject = GetLoopMovingObjectByIndex(i);
				Vector3	l_vPos = l_pLoopMovingObject->vCurrentPos;
				l_vPos.x += l_fImageHalfWidth;
				l_vPos.y += l_fImageHalfHeight;
				m_pFreeGameSpinObjectShow->SetPos(l_vPos);
				m_pFreeGameSpinObjectShow->Render();
			}
		}
	}
}