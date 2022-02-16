#include "stdafx.h"
#include "SlotProbability.h"
#include "SlotTemperatureScore.h"

cSlotProbability::cSlotProbability()
{
	m_SlotGoodTables.ParseWithMyParse("JewelsKing/Probability.xml");
}

cSlotProbability::~cSlotProbability()
{

}

void	cSlotProbability::Update(cBetBehaviorResult*e_pBetBehaviorResult)
{
	m_SlotGoodTables.Update(e_pBetBehaviorResult);
}

void	cSlotProbability::Render()
{
	m_SlotGoodTables.Render();
}