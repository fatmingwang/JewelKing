#include "stdafx.h"
#include "Fish.h"
#include "MultiCollisionFish.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/FishProbability.h"
#include "FishGroup.h"
cFAIBehaviorParser*cMonsterManager::m_spFAIBehaviorParser = 0;
const float	g_fMonsterHittedHintTime = 0.5f;

eBodyType	GetBodyType(const WCHAR*e_str)
{
	if( !wcscmp(e_str,L"Small") )
	{
		return eBT_SMALLE;
	}
	else
	if( !wcscmp(e_str,L"Medium") )
	{
		return eBT_MIDIUM;
	}
	else
	if( !wcscmp(e_str,L"Big") )
	{
		return eBT_BIG;
	}
	UT::ErrorMsg(e_str,L"ErrorBodyType");
	return eBT_MAX;
}

cMonster::cMonster()
{
	m_fScale = 1.f;
	m_fAliveTime = 0.f;
	m_iCurrentFrameBiteMoney = 0;
	m_iTotalBiteMoney = 0;
	m_vHittedHintColor = Vector4::Zero;
	m_eBodyType = eBT_MAX;
	m_bFakeMiniGameMonster = false;
	m_bMiniGameMonster = false;
	m_pHittedPlayer = 0;
	m_pBullet = 0;
	m_fRadius = 100.f;
	m_eMonsterStatus = eMS_NONE;
	m_eMonsterType = eMT_MAX;

	m_piPrize = 0;
	m_piFloatingPrize = 0;

	m_fProbabilityToDied = 0.5f;
	m_fRestProbabilityValueToDied = 1.f;
	m_HittedHintTC.SetTargetTime(g_fMonsterHittedHintTime);
	m_HittedHintTC.bTragetTimrReached = true;
	m_iID = -1;
}

cMonster::cMonster(cMonster*e_pMonster)
{
	m_fScale = e_pMonster->m_fScale;
	m_fAliveTime = 0.f;
	m_iCurrentFrameBiteMoney = 0;
	m_iTotalBiteMoney = 0;
	m_vHittedHintColor = Vector4::Zero;
	m_eBodyType = e_pMonster->m_eBodyType;
	m_bFakeMiniGameMonster = false;
	m_bMiniGameMonster = false;
	SetName(e_pMonster->GetName());
	m_pBullet = 0;
	m_pHittedPlayer = 0;
	m_iID = e_pMonster->m_iID;
	m_eMonsterStatus = eMS_NONE;
	m_fRadius = e_pMonster->m_fRadius;
	m_eMonsterType = e_pMonster->m_eMonsterType;
	m_piPrize = 0;
	m_piFloatingPrize = 0;

	if( e_pMonster->m_piPrize )
	{
		m_piPrize = new int;
		*m_piPrize = *e_pMonster->m_piPrize;
	}
	else
	{
		m_piFloatingPrize = new sMinMaxData<int>;
		*m_piFloatingPrize = *e_pMonster->m_piFloatingPrize;
	}

	m_fProbabilityToDied = e_pMonster->m_fProbabilityToDied;
	m_fRestProbabilityValueToDied = 1.f;
	m_HittedHintTC.SetTargetTime(e_pMonster->m_HittedHintTC.fTargetTime);
	m_HittedHintTC.bTragetTimrReached = true;
}

cMonster::~cMonster()
{
	SAFE_DELETE(m_piPrize);
	SAFE_DELETE(m_piFloatingPrize);
}

void	cMonster::Init()
{
	m_bFakeMiniGameMonster = false;
	m_bMiniGameMonster = false;
	m_fAliveTime = 0.f;
	m_pHittedPlayer = 0;
	m_iTotalBiteMoney = 0;
	m_iCurrentFrameBiteMoney = 0;
	m_eMonsterStatus = eMS_ALIVE;
	m_fRestProbabilityValueToDied = 1.f;
	m_pBullet = 0;
	InternalInit();
}

unsigned int	g_iCurrentHittedColorIndex = 0;
Vector4	g_vHittedHintColor[4] = {Vector4(1.f,0.f ,0.f ,1.f),
								 Vector4(1.f,0.1f,0.2f,1.f),
								 Vector4(1.f,0.2f,0.1f,1.f),
								 Vector4(1.f,0.1f,0.2f,1.f)};

void	cMonster::SetMiniGameMonster(bool e_b,WCHAR*e_strName)
{
	m_bMiniGameMonster = e_b;
	m_MiniGameName = e_strName;
}

bool	cMonster::IsMiniGameMonster()
{
	return m_bMiniGameMonster;
}

void		cMonster::Update(float e_fElpaseTime)
{
	cFishApp::m_sui64CurrentStep = 101;
	if( this->m_eMonsterStatus == eMS_CONTROL_BY_MINI_GAME || m_eMonsterStatus == eMS_WAITING_FOR_CLEAN )
		return;
	cFishApp::m_sui64CurrentStep = 102;
	m_fAliveTime += e_fElpaseTime;
	if( m_eMonsterStatus == eMS_HITTED )
	{
		if( !m_HittedHintTC.bTragetTimrReached )
		{
			m_HittedHintTC.Update(e_fElpaseTime);
			++g_iCurrentHittedColorIndex;
			if( g_iCurrentHittedColorIndex>4 )
				g_iCurrentHittedColorIndex = 0;
			m_vHittedHintColor = g_vHittedHintColor[g_iCurrentHittedColorIndex];
		}
		else
		{
			m_vHittedHintColor = Vector4::One;
			m_eMonsterStatus = eMS_ALIVE;
		}
	}
	m_iTotalBiteMoney += m_iCurrentFrameBiteMoney;
	m_iCurrentFrameBiteMoney = 0;
	cFishApp::m_sui64CurrentStep = 103;
	InternalUpdate(e_fElpaseTime);
	cFishApp::m_sui64CurrentStep = 104;
}

void	cMonster::AddBiteMoney(int e_iMoney)
{
	m_iCurrentFrameBiteMoney += e_iMoney;
}

int		cMonster::GetPrize()
{
	if( m_piPrize )
		return *m_piPrize;
	return m_piFloatingPrize->Rand();
}

bool	cMonster::IsStatusAllowToDied()
{
	if( m_eMonsterStatus != eMS_WAITING_FOR_CLEAN && m_eMonsterStatus != eMS_CONTROL_BY_MINI_GAME && m_eMonsterStatus != eMS_DIED_SHOW
		&& m_eMonsterStatus != eMS_NONE )
		return true;
	return false;
}

void	cMonster::SetMonsterStatus(eMonsterStatus e_eMonsterStatus)
{
	m_eMonsterStatus = e_eMonsterStatus;
	if( e_eMonsterStatus == eMS_HITTED )
	{
		m_HittedHintTC.Start();
	}
}

void	cMonster::ProcessMonsterData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Prize")
		{
			m_piPrize = new int;
			*m_piPrize = VALUE_TO_INT;
		}
		else
		COMPARE_NAME("FloatingPrize")
		{
			m_piFloatingPrize = new sMinMaxData<int>;
			*m_piFloatingPrize = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("Probability")
		{
			m_fProbabilityToDied = VALUE_TO_FLOAT;
		}
	PARSE_NAME_VALUE_END
}