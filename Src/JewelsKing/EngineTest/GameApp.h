#ifndef GAME_APP_H
#define GAME_APP_H

#include "PlayerData.h"
#include "BetBehavior.h"

enum	eLanguage
{
	e_TCN = 0,
	e_SCN,
	e_EN
};
	class	cSlotObjectManager;
	class	cSlotRuleData;
	class	cSlotObjectManagerList;
	class	cKing;
	class	cGameNetwork;
	class	cEngineTestApp:public cGameApp
	{
		void	ControlPanelSetup();
		void	ReleaseGame();
		void	PhaseSetup();
		void	LoadResolutionChanger(char*e_strFileName);
		void	LoadIOData(char*e_strFileName);
		void	LoadSound(char*e_strFileName);
		void	LoadGameSetup();
		void	SetupSpinData();
	protected:
		void	Update(float e_fElpaseTime);
		void	Render();
#ifdef WIN32
		virtual	void	OpenglInit(HWND e_Hwnd){}
#endif
	public:
#ifdef WIN32
		cEngineTestApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#else
		cEngineTestApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));
#endif
		~cEngineTestApp();
		void	Init();
		virtual	void	MouseDown(int e_iPosX,int e_iPosY);
		virtual	void	MouseMove(int e_iPosX,int e_iPosY);
		virtual	void	MouseUp(int e_iPosX,int e_iPosY);
		//void	Destroy();

		//void	Run();

		void	KeyDown(char e_char);
		void	KeyUp(char e_char);
		//if true game exit
		bool	m_bLeave;
		//
		cOrthogonalCamera									m_OrthogonalCamera;
		//for global to multiply	win money,cSpinPhase is using it now u could simply change to win more money
		static std::vector<int>*							m_spExtraBonusMoneyRate;
		static std::vector<int>*							m_spRestFreeGameTimes;
		static cSlotObjectManagerList*						m_spSlotLineObjectManagerList;
		static cSlotRuleData*								m_spSlotRuleData;
		static cPlayerData*									m_spPlayerData;
		static cBetBehavior*								m_spBetBehavior;
		static cKing*										m_spKing;
		//
		static  cControlPanel*								m_spControlPanel;
		static bool*										m_spbForceIntoFreeGame;
		static std::wstring*								m_spstrFreeGameTest;
		static cGameNetwork*								m_spNetwork;
		//free to play without and money for bet
		static int											m_siFreeGame;
		static bool											m_sbFreeToPlay;
		static bool											m_sbOpenNetWork;
		static int											m_siNumLineBetable;
		static eLanguage									m_seLanguage;
		static int											m_siExchangeRate;
		static int											m_siErrorMsg;
		static void											WriteDataIntoControlPanel();
		void	LoadGame();
	};
#endif