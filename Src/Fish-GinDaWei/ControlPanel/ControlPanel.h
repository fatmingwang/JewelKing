#ifndef _CONTROL_PANEL_INCLUDE_H_
#define _CONTROL_PANEL_INCLUDE_H_

//#include "CodeInput.h"

struct	sIntoMenuData;

#define SmallFontFileName	"Fish/ControlPanel/EventFont_12"

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
	cBaseImage*					m_pPasswordFormBG;

	cGlyphFontRender*			m_spGlyphSmallFontRender;

	void						RenderAuditRecord(int e_iMenuIndex);

	
	int64						m_iTimeStamp;
	int							m_iReportDay;
	float						m_fReportRoutine;
	int							m_iSaveTime;
	
	//int64						m_iRunningTime;
	
	void						CheckReport(); //Check Report Count & Date

	int64*						m_pi64TotalInMoney;
	int64*						m_pi64TotalOutMoney;
	int64*						m_pi64TotalRevenue;
	int64*						m_pi64TotalExchangeIn;
	int64*						m_pi64TotalExchangeOut;
	int64*						m_pi64TotalCoinIn;
	int64*						m_pi64TotalCoinOut;

	int64*						m_pi64LatestInMoney;
	int64*						m_pi64LatestOutMoney;
	int64*						m_pi64LatestRevenue;
	int64*						m_pi64LatestExchangeIn;
	int64*						m_pi64LatestExchangeOut;
	int64*						m_pi64LatestCoinIn;
	int64*						m_pi64LatestCoinOut;

	int64*						m_pi64PriorInMoney;
	int64*						m_pi64PriorOutMoney;
	int64*						m_pi64PriorRevenue;
	int64*						m_pi64PriorExchangeIn;
	int64*						m_pi64PriorExchangeOut;
	int64*						m_pi64PriorCoinIn;
	int64*						m_pi64PriorCoinOut;

	void						ProcessVendorAskMachineStartStatusData(char* e_strFileName);
	void						ProcessVendorInControlPanelModelStatusData(char* e_strFileName);
	void						CheckCheatFile();

	void						GetTimeStamp();
	
	void						GetVersion();
	

public:
	cControlPanel(char*e_strPackagesFileName);
	~cControlPanel();
	void	AddMenuData(unsigned char e_ucKey,const WCHAR*e_strName);
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
	int							m_fProbability;
	//
	int							m_iPlayerCount;
	//bullet
	int							m_iBulletPayRateLimit;
	int							m_iBulletPayRateAddPoint;
	int							m_iCoinToScore;//one coin for how many credit
	int							m_iExchangeIn;
	int							m_iExchangeOut;
	int							m_iExchange_Ticket;
	int64						m_i64MoneyLimit;
	bool						m_bLuckyTime;//??

	float						m_fJPShareScorePercentage;
	void						SaveData();
	void						SetGameReload(bool e_b){ m_bGameReload = e_b; }
	//while control panel mode is on,store the old data
	//and compare necessary data if control panel mode is off
	void						StoreOldData();

	int							m_iRenderAuditMenu;

	int64						m_iReportTime;
	int							m_iReportFlag;
	int							m_iReportCount;

	int64						GetTimeForInt64();
	WCHAR*						GetTime(int64 e_iTime);

	int							m_iRefundCoinOrTicket;	
	int							m_iBulletPayRateMinPoint;
	void						WriteTimeStamp();

	std::wstring				m_wchVersion;
	
};

#endif