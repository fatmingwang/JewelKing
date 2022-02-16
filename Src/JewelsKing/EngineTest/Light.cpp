#include "stdafx.h"
#include "Light.h"

void	cLight::TurnUpperLight(bool e_b)
{
	if( g_pIOSMDriverInterface&&cGameApp::m_sbDeviceExist )
	{
		WCHAR*l_strIOName[] = {BUTTON_LIGHT_LEFT_UP_1,
								BUTTON_LIGHT_LEFT_UP_2,
								BUTTON_LIGHT_LEFT_UP_3,
								BUTTON_LIGHT_LEFT_UP_4,
								BUTTON_LIGHT_LEFT_UP_5};
		for( int i=0;i<5;++i )
		{
			g_pIOSMDriverInterface->GetObject(l_strIOName[i])->WriteIO(e_b);
		}
	}		
}

void	cLight::TurnLight(bool e_b)
{
	if( g_pIOSMDriverInterface&&cGameApp::m_sbDeviceExist )
	{
		WCHAR*l_strIOName[] = {BUTTON_LIGHT_LEFT_UP_1,
								BUTTON_LIGHT_LEFT_UP_2,
								BUTTON_LIGHT_LEFT_UP_3,
								BUTTON_LIGHT_LEFT_UP_4,
								BUTTON_LIGHT_LEFT_UP_5,
								BUTTON_LIGHT_LEFT_DOWN_3,
								BUTTON_LIGHT_LEFT_DOWN_4,
								BUTTON_LIGHT_LEFT_DOWN_6,
								BUTTON_LIGHT_RIGHT};
		for( int i=0;i<BUTTON_LIGHT_COUNT;++i )
		{
			g_pIOSMDriverInterface->GetObject(l_strIOName[i])->WriteIO(e_b);
		}
	}
}

void	cLight::TurnBetPhaseLight(bool e_b)
{
	if( g_pIOSMDriverInterface&&cGameApp::m_sbDeviceExist )
	{
		WCHAR*l_strIOName[] = { BUTTON_LIGHT_LEFT_UP_5,
								BUTTON_LIGHT_LEFT_DOWN_3,
								BUTTON_LIGHT_LEFT_DOWN_4,
								BUTTON_LIGHT_LEFT_DOWN_6,
								BUTTON_LIGHT_RIGHT};
		for( int i=0;i<5;++i )
		{
			g_pIOSMDriverInterface->GetObject(l_strIOName[i])->WriteIO(e_b);
		}
	}
}