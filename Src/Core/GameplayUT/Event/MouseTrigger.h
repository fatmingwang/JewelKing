#ifndef _MOUSE_TRIGGER_H_
#define _MOUSE_TRIGGER_H_
#include "BaseEvent.h"
namespace FATMING_CORE
{
	enum	eGestureEnum
	{
		eGE_CLICK = 0,		//click with collision range
		eGE_HORVER,			//stay at same position for a while
		eGE_DOUBLE_CLICK,	//click twice with collision range at a short time//DOUBLE_CLICK_TIME_SPAN
		eGE_SLIDE_LEFT,		//
		eGE_SLIDE_UP,		//
		eGE_SLIDE_RIGHT,	//
		eGE_SLIDE_DOWN,		//
		eGE_ZOOM_IN,		//
		eGE_ZOOM_OUT,		//
		eGE_MAX,
	};
	//it also combine key data
	class	cMouseTrigger:public cBaseEvent
	{
		GET_SET_DEC(eGestureEnum,m_eGestureEnum,GetGestureEnum,SetGestureEnum);
		virtual	void	InternalInit();
		virtual	void	InternalUpdate(float e_fElpasedTime);
		virtual	void	InternalRender();
	public:
		DEFINE_TYPE_INFO();
		cMouseTrigger(TiXmlDocument*e_pTiXmlDocument);
		cMouseTrigger();
		cMouseTrigger(cMouseTrigger*e_pEventTest);
	};
//end namespace FATMING_CORE
}
#endif