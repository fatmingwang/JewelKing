#ifndef _MPDI_BUTTON_H_
#define _MPDI_BUTTON_H_
#include "../AllImageInclude.h"

	//animation could be cMPDI or cSubMPDI or else.....
	//
	//collide rang usually set by Idle animation,but we could simpley change it
	//
	class	cMPDIButton:public cClickMouseBehavior
	{
	public:
		cMPDIButton(NamedTypedObject*e_pIdelAnimation,NamedTypedObject*e_pHorverAnimation,NamedTypedObject*e_pActiveAnimation);
		~cMPDIButton();

	};

#endif