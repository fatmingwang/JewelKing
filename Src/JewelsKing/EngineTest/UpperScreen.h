#ifndef _UPPER_SCREEN_H_
#define _UPPER_SCREEN_H_


	class	cUpperScreen
	{
		cMPDIList*		m_pUpperUIMPDIList;
		cMPDI*			m_pUpperUIMPDI;

		cMPDI*			m_pUpperUIPayRateHint;

		cMPDI*			m_pReelMPDI;
		cMPDIList*		m_pNGMPDIList;
	public:
		cUpperScreen();
		~cUpperScreen();
		void	Update(float e_fElpaseTime);
		void	Render();
		void	RenderReel();

		void	RenderPayRateHint();
	};


#endif