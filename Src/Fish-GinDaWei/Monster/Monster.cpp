#include "stdafx.h"
#include "Fish.h"
#include "MultiCollisionFish.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/FishProbability.h"
#include "FishGroup.h"
cFAIBehaviorParser*cMonsterManager::m_spFAIBehaviorParser = 0;
const float	g_fMonsterHittedHintTime = 0.5f;
cMonster::cMonster()
{
	m_fRadius = 100.f;
	m_eMonsterStatus = eMS_NONE;
	m_eMonsterType = eMT_MAX;

	m_piPrize = 0;
	m_piFloatingPrize = 0;

	m_fProbabilityToDied = 0.1f;
	m_fRestProbabilityValueToDied = 1.f;
	m_HittedHintTC.SetTargetTime(g_fMonsterHittedHintTime);
	m_HittedHintTC.bTragetTimrReached = true;
	m_iID = -1;
}

cMonster::cMonster(cMonster*e_pMonster)
{
	this->SetName(e_pMonster->GetName());
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

void		cMonster::Update(float e_fElpaseTime)
{
	if( this->m_eMonsterStatus == eMS_CONTROL_BY_MINI_GAME || m_eMonsterStatus == eMS_WAITING_FOR_CLEAN )
		return;
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
	InternalUpdate(e_fElpaseTime);
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
	if( m_eMonsterStatus != eMS_WAITING_FOR_CLEAN && m_eMonsterStatus != eMS_CONTROL_BY_MINI_GAME )
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