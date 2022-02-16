#ifndef GAME_APP_H
#define GAME_APP_H

namespace   FMBOOK
{
	class	cFMBOOKManager;
	class	cFMBookPageDataList;
	class	cFMBookReader;
}

	struct	sFMBookEditorData
	{
		static cEventObjectInstance*					m_spEventObjectInstance;
		static cEventInstance*							m_spEventInstance;
		//for editor current object
		static cEventPackage*							m_spEventPackage;
		static	void									AddEventPackage(cEventPackage*e_pEventPackage);
		static	void									DelEventPackage(WCHAR*e_strName);
		static	void									SaveFile(char*e_strName);
		static	bool									OpenFile(char*e_strName);
		sFMBookEditorData();
		~sFMBookEditorData();
		static	void									SetupResource();
		//while file open call this
		static	void									DestroyResource();
		static	void									DataLegalCheck();
	};

	class	cFMBookApp:public cGameApp
	{
	protected:
		void	Update(float e_fElpaseTime);
		void	Render();
#ifdef WIN32
		virtual	void	OpenglInit(HWND e_Hwnd){}
#endif
	public:
		static cEventManager*							m_spEventManager;
		static cEventPackageRunner*						m_spEventPackageRunner;
#ifdef WIN32
		cFMBookApp(HWND e_Hwnd);
#else
		cFMBookApp();
#endif
		~cFMBookApp();

		void	Init();
		void	Destroy();
        void	MouseDown(int e_iPosX,int e_iPosY);
        void	MouseMove(int e_iPosX,int e_iPosY);
        void	MouseUp(int e_iPosX,int e_iPosY);
		//if true game exit
		bool	m_bLeave;
		void    SwitchMode(bool e_bReader);
	};

	//for editor use
	extern cFMBookApp*g_pFMBookApp;
#endif