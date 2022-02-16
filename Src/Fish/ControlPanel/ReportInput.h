//#include "../../IOSimulationMachine/EventPackageRunner.h"
#ifndef _REPORT_INPUT_H_
#define _REPORT_INPUT_H_

#define SmallFontFileName	"Fish/ControlPanel/EventFont_12"

class cReportInput:public cEventPackage
{
	cGlyphFontRender*			m_spGlyphSmallFontRender;
	cClickMouseBehavior			m_KeyUpEffect[6];//cvbnm
	cBaseImage*					m_pBG;
	int							m_iNumber[10];
	int							m_iIndex;
	float						m_fSeconds;
	bool						m_bShineFlag;
	int64*						m_pi64TotalRevenue;
	int64*						m_pi64LatestRevenue;
	int64*						m_pi64PriorRevenue;
	int*						m_piCheckNumber;
	void						ReportAdjust();
public:
	cReportInput();
	~cReportInput();
	//while page is turned,turn back again someparameter must reset
    virtual void    Init();
    //while page is leave
    virtual void    Destroy();
    virtual void    Update(float e_fElpaseTime);
	virtual void    Render();
};

#endif