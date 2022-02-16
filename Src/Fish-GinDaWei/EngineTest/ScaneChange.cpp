#include "stdafx.h"
#include "SceneChange.h"
#include "GameApp.h"
#include "MonsterBase.h"
#include "FishProbability.h"
#include "../Monster/FishGroup.h"
#include "../Monster/TurtleBoss.h"

cSceneChange::cSceneChange()
{
	for( int i=0;i<eSSCT_MAX;++i )
	{
		m_fSpecialSceneChangeTime[i] = 15.f;
	}
	m_bStopChangeScene = false;
	m_bSceneIsSpecial = false;
	m_pFishGroup = 0;
	m_pFishGroup2 = 0;
	m_fFishRunAwaySpeed = 2.f;
	m_iTotalProbability = 0;
	m_pBGImage = 0;
	m_CurrentBGName = "";
	m_pPrtEmitter = 0;
	m_pbSceneChange = &cFishApp::m_sbSceneChange;
	m_pWaterMPDI = 0;
	m_pWaterList = 0;
	m_pLastBGImage = 0;
	m_pShowChangeSceneMPDI = 0;
	m_BubbleClamDownTC.SetTargetTime(2.f);
	m_StopGenerateFishTime.SetTargetTime(15.f);
}

cSceneChange::~cSceneChange()
{
	SAFE_DELETE(m_pFishGroup);
	SAFE_DELETE( m_pFishGroup2 );
	SAFE_DELETE(m_pBGImage);
	SAFE_DELETE(m_pLastBGImage);
}

void	cSceneChange::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("NormaleBG")
	{
		ProcessNormalBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("SpecialBG")
	{
		ProcessSpecialBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("BG")
	{
		ProcessBG(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("ShowTime")
	{
		ProcessShowTime(e_pTiXmlElement);
	}
}

void	cSceneChange::ProcessSpecialBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strBGName = e_pTiXmlElement->Attribute(L"Name");
	const WCHAR*l_strProbability = e_pTiXmlElement->Attribute(L"Probability");
	m_SpecialBG.ImageNameList = GetStringListByCommaDivide(l_strBGName);
	m_SpecialBG.iProbability = _wtoi(l_strProbability);
	m_iTotalProbability += m_SpecialBG.iProbability;
}

//<BG ChangeCircle="300,500">
//	<NormaleBG Name="BG1.png,BG2.png,BG3.png" Probability="50" />
//	<SpecialBG Name="BG1.png,BG2.png,BG3.png" Probability="50" />
//</BG>
void	cSceneChange::ProcessNormalBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strBGName = e_pTiXmlElement->Attribute(L"Name");
	const WCHAR*l_strProbability = e_pTiXmlElement->Attribute(L"Probability");
	m_NormalBG.ImageNameList = GetStringListByCommaDivide(l_strBGName);
	m_NormalBG.iProbability = _wtoi(l_strProbability);
	m_iTotalProbability += m_NormalBG.iProbability;
}
//<ShowTime FishGroupTime="15" FishGroupTime2="15" BossTime="15"  />
void	cSceneChange::ProcessShowTime(TiXmlElement*e_pTiXmlElement)
{
//	<ShowTime FishGroupTime="15" FishGroupTime="15" BossTime="15"  />
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("FishGroupTime")
		{
			m_fSpecialSceneChangeTime[eSSCT_FISG_GROUP] = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("FishGroupTime2")
		{
			m_fSpecialSceneChangeTime[eSSCT_FISG_GROUP2] = VALUE_TO_FLOAT;
		}
		else
		COMPARE_NAME("BossTime")
		{
			m_fSpecialSceneChangeTime[eSSCT_BOSS] = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
}

void	cSceneChange::ProcessBG(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strChangeCircle = e_pTiXmlElement->Attribute(L"ChangeCircle");
	m_TimeCircleRestrict = GetVector2(l_strChangeCircle);
	const WCHAR*l_strBubbleMoving = e_pTiXmlElement->Attribute(L"BubbleMoving");
	m_BubbleMovingTC.SetTargetTime((float)_wtof(l_strBubbleMoving));
	const WCHAR*l_strFishRunAwaySpeed = e_pTiXmlElement->Attribute(L"FishRunAwaySpeed");
	if( l_strFishRunAwaySpeed )
	{
		m_fFishRunAwaySpeed = (float)_wtof(l_strFishRunAwaySpeed);
	}
	const WCHAR*l_strFishGroupMovingTime = e_pTiXmlElement->Attribute(L"FishGroupMovingTime");
	if( l_strFishGroupMovingTime )
	{
		m_StopGenerateFishTime.SetTargetTime((float)_wtof(l_strFishRunAwaySpeed));
	}
}

void	cSceneChange::Init()
{
	m_bStopChangeScene = false;
	m_eSceneChangeStatus = eSCS_NONE;
	if(!this->Parse(SCENE_CHANGE_SETUP_FILE))
	{
		UT::ErrorMsg(SCENE_CHANGE_SETUP_FILE,"parse failed!");
	}
	ChangeScene();
	m_ChangeSceneCircle.SetTargetTime(m_TimeCircleRestrict.Rand());
	m_pWaterList = cGameApp::GetMPDIListByFileName(BG_WATER_MPDI);
	if( m_pWaterList )
	{
		m_pWaterMPDI = m_pWaterList->GetObject(0);
		m_pWaterMPDI->Init();
	}
	m_pPrtEmitter = cGameApp::GetPrtEmitter(SCENE_CHANGE_PRT,L"Bobble");
	if( !m_pPrtEmitter )
	{
		UT::ErrorMsg(SCENE_CHANGE_PRT,"parse failed");
	}
	cMPDIList*l_pMPDIList = cGameApp::GetMPDIListByFileName(SCENE_CHANGE_ALERT);
	m_pShowChangeSceneMPDI = l_pMPDIList->GetObject(L"FishTimeShow");
	m_pShowChangeSceneMPDI->Init();
	//
	m_pFishGroup = new cFishGroup(FISH_GROUP_FILE);
	m_pFishGroup2 = new cFishGroup( 2, MINI_GAME_WINNING_2 );
}


void	cSceneChange::UpdateeSCS_NONE(float e_fElpaseTime)
{
	m_ChangeSceneCircle.Update(e_fElpaseTime);
	m_pWaterMPDI->Update(e_fElpaseTime);
	if( m_ChangeSceneCircle.bTragetTimrReached )
	{
		int	l_iValue = rand(0,this->m_iTotalProbability);
		if( this->m_NormalBG.iProbability> l_iValue )
		{
			m_bSceneIsSpecial = false;
			ChangeScene();
		}
		else
		{
			m_bSceneIsSpecial = true;
			ChangeSpecialScene();
		}
		m_ChangeSceneCircle.SetTargetTime(m_TimeCircleRestrict.Rand());
		m_pShowChangeSceneMPDI->Init();
		m_eSceneChangeStatus = eSCS_PLAY_SCENE_CHANGE_MPDI;
	}
}

void	cSceneChange::UpdateeeSCS_PLAY_SCENE_CHANGE_MPDI(float e_fElpaseTime)
{
	if( !m_pShowChangeSceneMPDI->IsDone() )
	{
		m_pShowChangeSceneMPDI->Update(e_fElpaseTime);
		if(m_pShowChangeSceneMPDI->IsDone())
		{
			*m_pbSceneChange = true;
			cFishApp::m_sfSceneChangeSpeed = this->m_fFishRunAwaySpeed;
			m_eSceneChangeStatus = eSCS_WAIT_FISH_LEAVE;
		}
	}
}

void	cSceneChange::UpdateeeSCS_WAIT_FISH_LEAVE(float e_fElpaseTime)
{
	if(cFishApp::m_spMonsterManager->f_GetCountMonsterAlive() == 0)
	{
		m_BubbleMovingTC.Start();
		m_BubbleClamDownTC.Start();
		m_pPrtEmitter->Emit(Vector3(cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y/2.f,0.f));
		m_pPrtEmitter->SetLoop(true);
		m_eSceneChangeStatus = eSCS_BUBBLE_GO;
		cGameApp::SoundPlay(L"30",true);
	}
}

void	cSceneChange::UpdateeeSCS_BUBBLE_GO(float e_fElpaseTime)
{
	m_pPrtEmitter->Update(e_fElpaseTime);
	m_BubbleMovingTC.Update(e_fElpaseTime);
	if( m_BubbleMovingTC.bTragetTimrReached )
	{
		float	l_fUV[4] = {0,0,1,1};
		m_pBGImage->SetUV(l_fUV);
		m_pBGImage->SetImageShowWidth((int)cGameApp::m_svGameResolution.x);
		m_BubbleClamDownTC.Update(e_fElpaseTime);
		m_pPrtEmitter->SetLoop(false);
		m_eSceneChangeStatus = eSCS_WAIT_BUBBLE_DISSAPEAR;
	}
	else
	{
		Vector2	l_vPos = cGameApp::m_svGameResolution;
		float	l_fLerp = m_BubbleMovingTC.GetLERP();
		l_vPos.x = (1-l_fLerp)*l_vPos.x;
		m_pPrtEmitter->SetPos(Vector3(l_vPos.x,l_vPos.y/2.f,0.f));
		if( m_pLastBGImage )
		{
			m_pLastBGImage->SetImageShowWidth((int)l_vPos.x);
			float	l_fUV[4] = {0,0,1-l_fLerp,1};
			m_pLastBGImage->SetUV(l_fUV);
		}
	}
}

void	cSceneChange::UpdateeeSCS_WAIT_BUBBLE_DISSAPEAR(float e_fElpaseTime)
{
	m_pPrtEmitter->Update(e_fElpaseTime);
	m_BubbleClamDownTC.Update(e_fElpaseTime);
	if( m_BubbleClamDownTC.bTragetTimrReached )
	{
		*m_pbSceneChange = false;
		m_eSceneChangeStatus = eSCS_NONE;
		cFishApp::m_sfSceneChangeSpeed = 1.f;
		if( m_bSceneIsSpecial )
		{
			cGameApp::SoundPlay(L"27",false);
			cGameApp::SoundPlay(L"28",false);
			cGameApp::SoundPlay(L"29",true);
			int	l_iValue = rand()%3;
			if(l_iValue==0)
			{
				m_StopGenerateFishTime.SetTargetTime(m_fSpecialSceneChangeTime[eSSCT_FISG_GROUP]);
				m_pFishGroup->FishGroupGoLeftToRight(0,m_StopGenerateFishTime.fTargetTime);
				m_pFishGroup->FishGroupGRightTooLeft(1,m_StopGenerateFishTime.fTargetTime);
			}
			else
			if(l_iValue==1)
			{
				m_StopGenerateFishTime.SetTargetTime(1.f);
				if(cFishApp::m_spMonsterManager->GetBoss()->GetMonsterStatus() == eMS_NONE)
				{
					m_StopGenerateFishTime.SetTargetTime(m_fSpecialSceneChangeTime[eSSCT_BOSS]);
					cFishApp::m_spMonsterManager->BossOut();
					m_StopGenerateFishTime.Start();
				}
			}
			else //±mª÷¤G
			{
				m_StopGenerateFishTime.SetTargetTime(m_fSpecialSceneChangeTime[eSSCT_FISG_GROUP2]);
				m_pFishGroup2->f_PrizeII_Call( m_StopGenerateFishTime.fTargetTime );
			} //end if
			cFishApp::m_spFishProbability->SetPauseGenerateMonster(true);
			m_StopGenerateFishTime.Start();
		}
		else
		{
			if(cGameApp::m_spSoundParser->GetObject(L"27")->IsPlay())
			{
				cGameApp::SoundPlay(L"27",false);
				cGameApp::SoundPlay(L"28",true);
			}
			else
			if(cGameApp::m_spSoundParser->GetObject(L"28")->IsPlay())
			{
				cGameApp::SoundPlay(L"27",true);
				cGameApp::SoundPlay(L"28",false);
			}
			else
			{
				cGameApp::SoundPlay(L"27",true);
			}
		}
	}
}


void	cSceneChange::Update(float e_fElpaseTime)
{
	if( m_bStopChangeScene )
		return;
	if( cFishApp::m_spFishProbability )
	{
		if(cFishApp::m_spFishProbability->IsPauseGenerateMonster())
		{
			if(!m_StopGenerateFishTime.bTragetTimrReached)
			{
				m_StopGenerateFishTime.Update(e_fElpaseTime);
				if(m_StopGenerateFishTime.bTragetTimrReached)
				{
					cFishApp::m_spFishProbability->SetPauseGenerateMonster(false);
					cGameApp::SoundPlay(L"29",false);
					if(rand()%2)
					{
						cGameApp::SoundPlay(L"27",true);
					}
					else
						cGameApp::SoundPlay(L"28",true);
				}
			}
		}
	}
	switch(m_eSceneChangeStatus)
	{
		case 	eSCS_NONE:
				UpdateeSCS_NONE(e_fElpaseTime);
			break;
		case 	eSCS_PLAY_SCENE_CHANGE_MPDI:
				UpdateeeSCS_PLAY_SCENE_CHANGE_MPDI(e_fElpaseTime);
			break;
		case 	eSCS_WAIT_FISH_LEAVE:
				UpdateeeSCS_WAIT_FISH_LEAVE(e_fElpaseTime);
			break;
		case 	eSCS_BUBBLE_GO:
				UpdateeeSCS_BUBBLE_GO(e_fElpaseTime);
			break;
		case 	eSCS_WAIT_BUBBLE_DISSAPEAR:
				UpdateeeSCS_WAIT_BUBBLE_DISSAPEAR(e_fElpaseTime);
			break;
	}
}

void	cSceneChange::Render()
{
	if( eSCS_NONE == m_eSceneChangeStatus )
	{
		m_pBGImage->Render();
	}
	else
	{
		if( eSCS_WAIT_FISH_LEAVE == m_eSceneChangeStatus || eSCS_PLAY_SCENE_CHANGE_MPDI == m_eSceneChangeStatus )
		{
			m_pLastBGImage->Render();
		}
		else
		if( eSCS_BUBBLE_GO == m_eSceneChangeStatus || eSCS_WAIT_BUBBLE_DISSAPEAR == m_eSceneChangeStatus )
		{
			m_pBGImage->Render();
			this->m_pLastBGImage->Render();
			m_pPrtEmitter->Render();
		}
	}
}

void	cSceneChange::LastRender()
{
	if( eSCS_PLAY_SCENE_CHANGE_MPDI == m_eSceneChangeStatus )
		m_pShowChangeSceneMPDI->Render();
	if( eSCS_NONE == m_eSceneChangeStatus )
	{
		m_pWaterMPDI->Render();
	}
}

void	cSceneChange::DebugRender()
{
	if( !*m_pbSceneChange )
	{
		std::wstring	l_str = UT::ComposeMsgByFormat(L"ToChangeScene%.2f",m_ChangeSceneCircle.fRestTime);
		cGameApp::m_spGlyphFontRender->RenderFont(0,600,l_str.c_str());
	}
}

int		IndexOfVector(const char*e_strName,std::vector<std::string>*e_pList)
{
	size_t	l_uiSize = e_pList->size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		if(!strcmp((*e_pList)[i].c_str(),e_strName))
		{
			return i;
		}
	}
	return -1;
}
void	cSceneChange::ChangeScene()
{
	assert(m_NormalBG.ImageNameList.size()>0);
	int	l_iIndex = rand()%m_NormalBG.ImageNameList.size();
	if(IndexOfVector(m_CurrentBGName.c_str(),&m_NormalBG.ImageNameList) == l_iIndex)
	{
		l_iIndex = UT::GetLoopIndex(l_iIndex+1,(int)m_NormalBG.ImageNameList.size());
	}
	SAFE_DELETE(m_pLastBGImage);
	m_pLastBGImage = m_pBGImage;
	m_pBGImage = new cBaseImage(m_NormalBG.ImageNameList[l_iIndex].c_str());
	m_pBGImage->SetImageShowWidth((int)cGameApp::m_svGameResolution.x);
	m_pBGImage->SetImageShowHeight((int)cGameApp::m_svGameResolution.y);
	m_CurrentBGName = m_NormalBG.ImageNameList[l_iIndex];
}

void	cSceneChange::ChangeSpecialScene()
{
	int	l_iIndex = rand()%m_SpecialBG.ImageNameList.size();
	assert(m_SpecialBG.ImageNameList.size()>0);
	if(IndexOfVector(m_CurrentBGName.c_str(),&m_SpecialBG.ImageNameList) == l_iIndex)
	{
		l_iIndex = UT::GetLoopIndex(l_iIndex+1,(int)m_SpecialBG.ImageNameList.size());
	}
	SAFE_DELETE(m_pLastBGImage);
	m_pLastBGImage = m_pBGImage;
	m_pBGImage = new cBaseImage(m_SpecialBG.ImageNameList[l_iIndex].c_str());
	m_pBGImage->SetImageShowWidth((int)cGameApp::m_svGameResolution.x);
	m_pBGImage->SetImageShowHeight((int)cGameApp::m_svGameResolution.y);
	m_CurrentBGName = m_SpecialBG.ImageNameList[l_iIndex];
}

bool	cSceneChange::IsChangeScene()
{
	return m_eSceneChangeStatus == eSCS_NONE?false:true;
}