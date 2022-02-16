#ifndef _CONTROL_PANEL_INCLUDE_H_
#define _CONTROL_PANEL_INCLUDE_H_


struct	sIntoMenuData
{
	sIntoMenuData(unsigned char e_ucKey,const WCHAR*e_strName)
	{
		KeyToMenu.SetKeyData(e_ucKey);
		strMenuName = e_strName;
	}
	std::wstring			strMenuName;
	cClickMouseBehavior		KeyToMenu;
	void					Update(){ KeyToMenu.SingnalProcess(); }
	bool					IsSatisfied(){ return KeyToMenu.IsSatisfiedCondition(); }
	void					Init(){ KeyToMenu.Init(); }
};

class	cControlPanel
{
	//while data changed reload game
	bool						m_bGameReload;
	//
	cClickMouseBehavior			m_KeyUpEffect[5];//cvbnm
	void						SoundCheck();
	//
	void						InternalReset();
	//
	cEventManager*				m_pEventManager;
	cEventPackageRunner*		m_pEventPackageRunner;
	std::vector<sIntoMenuData*>	m_IntoMenuDataList;
	bool						m_bInControlPanelMode;
	cBaseImage*					m_pBG;
public:
	cControlPanel(char*e_strPackagesFileName);
	void	AddMenuData(unsigned char e_ucKey,const WCHAR*e_strName);
	~cControlPanel();
	void	Update(float e_fElpaseTime);
	void	Render();
	//if language or bet rule is changed.
	bool	IsReloadGame();
	bool	IsInControlPanelMode();
	void	MouseDown(int e_iPosX,int e_iPosY);
	void	MouseMove(int e_iPosX,int e_iPosY);
	void	MouseUp(int e_iPosX,int e_iPosY);
	cEventManager*				GetEventManager(){return m_pEventManager;}

	//for new game setup
	std::wstring				m_strAuditPassword;
	std::wstring				m_strVendorPassword;
	int							m_iSound;
	int							m_iLineBetable;
	std::wstring				m_strLanguage;
	float						m_fProbability;
	int							m_iCoinToScore;//one coin for how many credit
	int							m_iExchangeIn;
	int							m_iExchangeOut;
	int							m_iExchange_Ticket;
	bool						m_bLuckyTime;//??
	int							m_iMoneyLimit;
	int							m_iLeastJP;
	float						m_fJPShareScorePercentage;
	void						SaveData();
	void						SetGameReload(bool e_b){ m_bGameReload = e_b; }
	//while control panel mode is on,store the old data
	//and compare necessary data if control panel mode is off
	void						StoreOldData();

};

#endif