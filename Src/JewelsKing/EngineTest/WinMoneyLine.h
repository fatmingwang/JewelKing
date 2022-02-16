#ifndef _WIN_MONEY_LINE_H_
#define _WIN_MONEY_LINE_H_

#include "BetBehavior.h"

	class	cWinMoneySpinObjectMPDI;
	class	cWinmoneyLines
	{
		cBetBehaviorResult::sTableResult*	m_pTableResult;
		cMPDIList*							m_pResultLinesMPDIList;
		//line now I just give it 100 lines,ifit's not enough add it
		cMPDI*								m_pLineMPDI[TOTAL_LINE_RESULT];
		cWinMoneySpinObjectMPDI*			m_pWinMoneySpinObjectMPDI;
		cNumerialImage*						m_pNumerialImage;
	public:
		std::vector<int>		m_iWinMoneyLines;
		cWinmoneyLines(const WCHAR*e_strMPDIListName);
		~cWinmoneyLines();
		void	Stop();
		void	Init(cBetBehaviorResult::sTableResult*e_pTableResult);
		//if one of win money lines was done return true 
		bool	UpdateWinMoneyLinesByStep(float e_fElpaseTime);
		void	Update(float e_fElpaseTime);
		void	Render(int e_iWinMoney);
		bool	IsDone();
		//for bet phase
		void	StartLines(int e_iToIndex);
		void	UpdateLines(int e_iToIndex,float e_fElpaseTime)
		{
			int	l_iSize = e_iToIndex;
			for( int i=0;i<l_iSize;++i )
			{
				this->m_pLineMPDI[i]->Update(e_fElpaseTime);
			}		
		}
		void	RenderLines2(int e_iToIndex)
		{
			int	l_iSize = e_iToIndex;
			for( int i=0;i<l_iSize;++i )
			{
				this->m_pLineMPDI[i]->Render();
			}		
		}
		float	GetCurrentProgress();
		void	SetDone();
		void	ChangeShowLineTime(float e_fTime);
	};

#endif