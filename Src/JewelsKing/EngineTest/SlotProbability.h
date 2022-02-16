#ifndef _SLOT_PROBABILITY_H_
#define _SLOT_PROBABILITY_H_

#include "PlayerData.h"
#include "SlotProbabilityData.h"
#include "SlotGoodTables.h"

class	cSlotProbability
{
public:
	cSlotGoodTables	m_SlotGoodTables;
	cSlotProbability();
	~cSlotProbability();
	void	Update(cBetBehaviorResult*e_pBetBehaviorResult);
	void	Render();
};



#endif