#ifndef _GAME_ERROR_H_
#define _GAME_ERROR_H_

	class	cGameMoneyLimitError
	{
		bool		m_bError;
		int*		m_piMoneyLimit;
		void		CheckError();
		cMPDI*		m_pMoneyLimitMPDI;
	public:
		cGameMoneyLimitError();
		~cGameMoneyLimitError();
		bool		IsError();
		void		Update(float e_fElpaseTime);
		void		Render();
	};

#endif