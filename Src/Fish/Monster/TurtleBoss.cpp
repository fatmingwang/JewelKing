/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "TurtleBoss.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/FishProbability.h"
#include "../../Core/GamePlayUT/Event/AllEventInclude.h"
#include "../ControlPanel/AllControlPanelInclude.h"
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
sProbabilityAeverage::sProbabilityAeverage()
{
	iTotalProbability = 0;
}


/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
int	sProbabilityAeverage::GetRandomIndex()
{
	assert(iTotalProbability);
	int	l_iValue = rand(0,iTotalProbability);
	size_t	l_uiSize = TotalProbabilityList.size();
	int	l_iCurrentValue = 0;
	for( size_t i=0;i<l_uiSize;++i )
	{
		l_iCurrentValue += TotalProbabilityList[i];
		if( l_iCurrentValue>=l_iValue )
		{
			return i;
		}
	}
	return 0;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cTurtleBoss::cTurtleBoss()
{
	m_eMonsterStatus = eMS_NONE;
	m_iCurrentLeastBiteMoney = 0;
	m_pHittedBullet = 0;
	m_pHittedPlayer = 0;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cTurtleBoss::~cTurtleBoss()
{

}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strValue = e_pTiXmlElement->Value();
	COMPARE_VALUE("Status")
	{
		ProcessStatusData(e_pTiXmlElement);
	}
	else
	COMPARE_VALUE("Boss")
	{
		ProcessBossData(e_pTiXmlElement);
	}
}

//	<Status Name="Defence" MonsterName="GoldKenmuDefence"  />
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::ProcessStatusData(TiXmlElement*e_pTiXmlElement)
{
	std::wstring	l_strStatusName;
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("Name")
		{
			l_strStatusName = l_strValue;
		}
		else
		COMPARE_NAME("MonsterName")
		{
			cNormalFish*l_pMultiCollisionDataFish = dynamic_cast<cNormalFish*>(cFishApp::m_spMonsterManager->GetObject(l_strValue));
			if( !l_pMultiCollisionDataFish )
			{
				UT::ErrorMsg(L"MonsterName not found",l_strValue);
			}
			else
			{
				cNormalFish*l_pBoss = dynamic_cast<cNormalFish*>(l_pMultiCollisionDataFish->Clone());
				l_pBoss->SetName(l_strStatusName.c_str());
				this->AddObjectNeglectExist(l_pBoss);
			}
		}
		else
		COMPARE_NAME("Probability")
		{
			int	l_iValue = VALUE_TO_INT;
			m_ProbabilityAeverage.iTotalProbability += l_iValue;
			m_ProbabilityAeverage.TotalProbabilityList.push_back(l_iValue);
		}
	PARSE_NAME_VALUE_END
}

//	<Boss LeaestBiteMoney="10000" AliveTime="100,300" />
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::ProcessBossData(TiXmlElement*e_pTiXmlElement)
{
	PARSE_ELEMENT_START(e_pTiXmlElement)
		COMPARE_NAME("LeaestBiteMoney")
		{
			m_LeastBiteMoneyRestrict = VALUE_TO_VECTOR2;
		}
		else
		COMPARE_NAME("AliveTime")
		{
			m_TimeRestrict = VALUE_TO_VECTOR2;
		}
	PARSE_NAME_VALUE_END
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::Init()
{
	m_eMonsterStatus = eMS_NONE;
	if( this->Count() == 0 )
	{
		this->Parse(BOSS_SETUP_FILE);
	}
	this->SetCurrentWorkingObject(eTBS_MOVING);
	Vector3	l_vPos = cFishApp::m_spFishProbability->GetRandomPos(this->GetCurrentWorkingObject());
	this->GetCurrentWorkingObject()->SetPos(l_vPos);
	m_eTurtleBossStatus = eTBS_MOVING;
	m_iLeastBiteMoney = (int)m_LeastBiteMoneyRestrict.Rand()*cFishApp::m_spControlPanel->m_iCoinToScore;
	m_TimeToLeave.SetTargetTime(m_TimeRestrict.Rand());
	m_bSpecialMoving = false;
	m_iCurrentLeastBiteMoney = 0;
	m_pHittedBullet = 0;
	m_pHittedPlayer = 0;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::ChangeStatus()
{
	cNormalFish*l_pMultiCollisionDataFish = this->GetCurrentWorkingObject();
	if( !m_bSpecialMoving&&m_eTurtleBossStatus == eTBS_MOVING )
	{
		if( l_pMultiCollisionDataFish->IsVisible() )
		{
			Vector3	l_vPos = l_pMultiCollisionDataFish->GetAIMachine()->GetSrcCharacter()->GetWorldPosition();
			if( l_vPos.x<300 || l_vPos.x>900 || l_vPos.y<250 || l_vPos.y>600 )
				return;
			float	l_fValue = frand(0,1);
			if( 0.5f<=l_fValue )
			{
				return;
			}
			m_bSpecialMoving = true;
			int	l_iValue = rand()%3;
			eTurtleBossStatus l_eTurtleBossStatus;
			if( l_iValue == 0 )
				l_eTurtleBossStatus = eTBS_DEFENCE;
			else
			if( l_iValue == 1 )
				l_eTurtleBossStatus = eTBS_RANDOMLY_ROTATION;
			else
				l_eTurtleBossStatus = eTBS_MOVING;
			m_eTurtleBossStatus = l_eTurtleBossStatus;
			if( l_iValue != 2 )
			{//go as defence
				this->SetCurrentWorkingObject(l_eTurtleBossStatus,false);
				this->GetCurrentWorkingObject()->SetPos(l_pMultiCollisionDataFish->GetPos());
				this->GetCurrentWorkingObject()->Init();
				this->GetCurrentWorkingObject()->SetAngle(l_pMultiCollisionDataFish->GetAngle());
			}
		}
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::MonsterLeaveCheck(float e_fElpaseTime)
{
	m_TimeToLeave.Update(e_fElpaseTime);
	if( m_TimeToLeave.bTragetTimrReached )
	{
		if( m_eTurtleBossStatus == eTBS_MOVING )
		{
			cNormalFish*l_pMultiCollisionDataFish = this->GetCurrentWorkingObject();
			if( !l_pMultiCollisionDataFish->IsVisible() )
			{
				m_eMonsterStatus = eMS_NONE;
			}
		}
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::Update(float e_fElpaseTime)
{
	if( m_eMonsterStatus == eMS_NONE )
		return;
	m_pHittedBullet = 0;
	m_pHittedPlayer = 0;
	cNormalFish*l_pMultiCollisionDataFish = this->GetCurrentWorkingObject();
	if( !l_pMultiCollisionDataFish )
	{
		int a=0;
	}
	cFMWorkingObjectChanger::Update(e_fElpaseTime);
	MonsterLeaveCheck(e_fElpaseTime);
	ChangeStatus();
	if(!l_pMultiCollisionDataFish)
	{
		this->SetCurrentWorkingObject(eTBS_MOVING);
	}
	else
	{
		cFAIMachine*l_pAIMachine = l_pMultiCollisionDataFish->GetAIMachine();
		if( l_pAIMachine->IsDone() )
		{
			this->SetCurrentWorkingObject(eTBS_MOVING,false);
			if( m_eTurtleBossStatus != eTBS_MOVING )
			{
				this->GetCurrentWorkingObject()->SetPos(l_pMultiCollisionDataFish->GetPos());
				l_pMultiCollisionDataFish->Init();
				this->GetCurrentWorkingObject()->GetAIMachine()->SetCurrentWorkingObject(L"Leave");
			}
			else
			{
				this->SetCurrentWorkingObject(eTBS_MOVING,false);
				l_pMultiCollisionDataFish = this->GetCurrentWorkingObject();
				Vector3	l_vPos = cFishApp::m_spFishProbability->GetRandomPos(l_pMultiCollisionDataFish);
				this->GetCurrentWorkingObject()->SetPos(l_vPos);
				l_pMultiCollisionDataFish->Init();
				this->GetCurrentWorkingObject()->SetAngle(l_pMultiCollisionDataFish->GetAngle());
				if( IsBiteEnoughMoney() )
				{
					//m_eMonsterStatus = eMS_NONE;
				}
			}
			m_bSpecialMoving = false;
			l_pMultiCollisionDataFish->Update(0.f);
			m_eTurtleBossStatus = eTBS_MOVING;
		}
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::Render()
{
	if( m_eMonsterStatus == eMS_NONE )
		return;
	cNormalFish*l_pMultiCollisionDataFish = this->GetCurrentWorkingObject();
	if(!l_pMultiCollisionDataFish)
	{

	}
	else
	{
		l_pMultiCollisionDataFish->Render();
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
bool	cTurtleBoss::IsCollide(cbtShapeCollision*e_pbtShapeCollision)
{
	cNormalFish*l_pMultiCollisionDataFish = this->GetCurrentWorkingObject();
	if( l_pMultiCollisionDataFish )
		return l_pMultiCollisionDataFish->IsCollide(e_pbtShapeCollision);
	return false;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
bool	cTurtleBoss::IsVisible()
{
	cNormalFish*l_pMultiCollisionDataFish = this->GetCurrentWorkingObject();
	if( l_pMultiCollisionDataFish )
	{
		return l_pMultiCollisionDataFish->IsVisible();
	}
	return false;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::DebugRender()
{
	if( m_eMonsterStatus == eMS_NONE )
		return;
	cNormalFish*l_pMultiCollisionDataFish = this->GetCurrentWorkingObject();
	if( l_pMultiCollisionDataFish )
	{
		l_pMultiCollisionDataFish->DebugRender();
		Vector3	l_vPos = l_pMultiCollisionDataFish->GetAIMachine()->GetSrcCharacter()->GetWorldPosition();
		std::wstring	l_str = UT::ComposeMsgByFormat(L"Name:%s,Pos:%s\nAliveTime:%.2f",l_pMultiCollisionDataFish->GetName(),ValueToStringW(l_vPos),m_TimeToLeave.fRestTime);
		if( l_pMultiCollisionDataFish->GetAIMachine()->GetCurrentWorkingObject() )
		{
			l_str += l_pMultiCollisionDataFish->GetAIMachine()->GetCurrentWorkingObject()->GetName();
		}
		cGameApp::m_spGlyphFontRender->RenderFont(0,600,l_str.c_str());
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cTurtleBoss::SetMonsterStatus(eMonsterStatus e_eMonsterStatus)
{
	m_eMonsterStatus = e_eMonsterStatus;
	cNormalFish*l_pMultiCollisionDataFish = this->GetCurrentWorkingObject();
	if( l_pMultiCollisionDataFish )
	{
		l_pMultiCollisionDataFish->SetMonsterStatus(e_eMonsterStatus);
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
bool	cTurtleBoss::IsBiteEnoughMoney()
{
	if( m_eMonsterStatus == eMS_NONE )
		return false;
	return  m_iCurrentLeastBiteMoney >= m_iLeastBiteMoney;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
int		cTurtleBoss::GetPrize()
{
	cNormalFish*l_pMultiCollisionDataFish = this->GetObject(eTBS_MOVING);
	return l_pMultiCollisionDataFish->GetPrize();
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */

