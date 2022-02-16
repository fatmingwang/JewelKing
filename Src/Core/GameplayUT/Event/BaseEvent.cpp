#include "../../stdafx.h"
#include "BaseEvent.h"
namespace FATMING_CORE
{
	const wchar_t*     cBaseEvent::TypeID( L"cBaseEvent" );
	cBaseEvent::cBaseEvent(){ m_bSatisfiedCondition = false; }
	cBaseEvent::~cBaseEvent(){}
	//cEventInstance*	m_pEventInstance;
	void	cBaseEvent::Update(float e_fElpasedTime)
	{
		if( m_bSatisfiedCondition )
			return;
		InternalUpdate(e_fElpasedTime);
	}
	void	cBaseEvent::ForceRender()
	{
		InternalRender(); 
	}
	void	cBaseEvent::Render()
	{
		if( m_bSatisfiedCondition )
			return;
		InternalRender();
	}
	void	cBaseEvent::Init()
	{
		m_bSatisfiedCondition = false;
		InternalInit();
	}
//end namespace
}