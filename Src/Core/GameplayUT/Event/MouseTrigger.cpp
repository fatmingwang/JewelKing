#include "../../stdafx.h"
#include "MouseTrigger.h"
namespace FATMING_CORE
{
	const wchar_t*         cMouseTrigger::TypeID( L"cMouseTrigger" );

	cMouseTrigger::cMouseTrigger(TiXmlDocument*e_pTiXmlDocument)
	{
		m_eGestureEnum = eGE_MAX;
	}
	cMouseTrigger::cMouseTrigger()
	{

	}

	cMouseTrigger::cMouseTrigger(cMouseTrigger*e_pEventTest)
	{

	}

	void	cMouseTrigger::InternalInit()
	{
		this->Init();
	}

	void	cMouseTrigger::InternalUpdate(float e_fElpasedTime)
	{
		eGestureEnum	l_eGestureEnum = eGE_MAX;
		if( this->m_bCollided )
		{
			eGE_CLICK;
			eGE_HORVER;
			eGE_DOUBLE_CLICK;
			eGE_SLIDE_LEFT;
			eGE_SLIDE_UP;
			eGE_SLIDE_RIGHT;
			eGE_SLIDE_DOWN;
			eGE_ZOOM_IN;
			eGE_ZOOM_OUT;
		}

		if(l_eGestureEnum == m_eGestureEnum)
		{
			this->m_bSatisfiedCondition = true;
		}
	}

	void	cMouseTrigger::InternalRender()
	{

	}

//end namespace FATMING_CORE
}