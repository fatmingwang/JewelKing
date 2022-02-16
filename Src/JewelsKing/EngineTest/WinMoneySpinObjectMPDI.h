#ifndef _WIN_MONEY_SPIN_OBJECT_MPDI_H_
#define _WIN_MONEY_SPIN_OBJECT_MPDI_H_

#include "BetBehavior.h"
	struct	sLineRule;
	//===========
	//show the object where achieve the condition to win the money
	//===========
	class	cWinMoneySpinObjectMPDI
	{
		cObjectListByName<cMPDI>					m_WinMoneyMPDIList;
		std::vector<int>							m_WinMoneyLineIndex;
		bool*										m_pbWinMoneySpinObject;
	public:
		cWinMoneySpinObjectMPDI();
		~cWinMoneySpinObjectMPDI();
		//setup win money lines and spinobject
		void	Destroy(){ m_WinMoneyMPDIList.Destroy(); }
		void	Init(cBetBehaviorResult::sTableResult*e_pTableResult);
		void	Update(float e_fElpaseTime);
		void	Render();
		bool	IsDone();
		float	GetCurrentProgress();
		//for only show line data
		void	SetupWinMoneySpinObjectByLine(cBetBehaviorResult::sTableResult::sLine*e_pLine);
		//void	UpdateByLine(float e_fElpaseTime);
		//void	RenderByLine();
	};

#endif