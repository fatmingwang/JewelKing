//#include "../../IOSimulationMachine/EventPackageRunner.h"
#ifndef _NEW_PASSWD_INPUT_H_
#define _NEW_PASSWD_INPUT_H_

#define SmallFontFileName	"Fish/ControlPanel/EventFont_12"

class cNewPasswordInput:public cEventPackage
{
	cGlyphFontRender*			m_spGlyphSmallFontRender;
	cClickMouseBehavior			m_KeyUpEffect[6];//cvbnm
	cBaseImage*					m_pBG;
	int							m_iNumber[5];
	int							m_iIndex;
	float						m_fSeconds;
	bool						m_bShineFlag;
	bool						m_bCheckFlag;
	void						CheckPassword();
public:
	cNewPasswordInput();
	~cNewPasswordInput();
	//while page is turned,turn back again someparameter must reset
    virtual void    Init();
    //while page is leave
    virtual void    Destroy();
    virtual void    Update(float e_fElpaseTime);
	virtual void    Render();
};

#endif