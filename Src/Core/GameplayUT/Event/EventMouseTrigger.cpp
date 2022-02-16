#include "../../stdafx.h"
#include "EventMouseTrigger.h"
#include "EventPackageRunner.h"
#include "EventManager.h"
namespace FATMING_CORE
{
	const wchar_t*         cEventMouseTrigger::TypeID( L"cEventMouseTrigger" );
	const wchar_t*         cEventObjectMouseUpDectet::TypeID( L"cEventObjectMouseUpDectet" );

	cEventMouseTrigger::cEventMouseTrigger(TiXmlElement*e_pTiXmlElement)
	{
		m_fMoveDistance = 0.f;
		m_eGestureEnum = eGE_MAX;
		m_eCurrentGestureEnum = eGE_MAX;
		if( !e_pTiXmlElement )
			return;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventMouseTrigger::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME("Distance")
			{
				m_fMoveDistance = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("Gesture")
			{
				m_eGestureEnum = FATMING_CORE::GetGestureEnum(l_strValue);
			}
		PARSE_NAME_VALUE_END
	}

	cEventMouseTrigger::cEventMouseTrigger(cEventMouseTrigger*e_pEventTest)
	{
		m_eGestureEnum = e_pEventTest->m_eGestureEnum;
		m_eCurrentGestureEnum = eGE_MAX;
		m_fMoveDistance = e_pEventTest->m_fMoveDistance;
	}

	cEventMouseTrigger::~cEventMouseTrigger()
	{
	
	}

	void	cEventMouseTrigger::InternalInit()
	{
		cEventBase::Init();
		m_eCurrentGestureEnum = eGE_MAX;
	}

    void	cEventMouseTrigger::InternalMouseMove(int e_iPosX,int e_iPosY)
	{
		m_eCurrentGestureEnum = eGE_HORVER;
	}

    void	cEventMouseTrigger::InternalMouseDown(int e_iPosX,int e_iPosY)
	{
		m_eCurrentGestureEnum = eGE_CLICK_DOWN;
	}

    void    cEventMouseTrigger::InternalMouseUp(int e_iPosX,int e_iPosY)
	{
		m_eCurrentGestureEnum = eGE_CLICK_UP;
		if( m_fMoveDistance != 0.f )
		{
			Vector2	l_vDirection;
			float	l_fDis = this->m_MouseMoveData.DownUpDistance(&l_vDirection);
			if(this->m_fMoveDistance>l_fDis)
			{
				if( fabs(l_vDirection.x)>fabs(l_vDirection.y) )
				{
					if(l_vDirection.x>0.f)
						m_eCurrentGestureEnum = eGE_SLIDE_RIGHT;
					else
						m_eCurrentGestureEnum = eGE_SLIDE_LEFT;
				}
				else
				{
					if(l_vDirection.y>0.f)
						m_eCurrentGestureEnum = eGE_SLIDE_UP;
					else
						m_eCurrentGestureEnum = eGE_SLIDE_DOWN;				
				}
			}
		}
	}

    void    cEventMouseTrigger::InternalMouseDoubleClickUp(int e_iPosX,int e_iPosY)
	{
		m_eCurrentGestureEnum = eGE_DOUBLE_CLICK;
	}

	void	cEventMouseTrigger::InternalUpdate(float e_fElpasedTime)
	{
		if(m_eCurrentGestureEnum == m_eGestureEnum)
		{
			this->m_bSatisfiedCondition = true;
		}
	}

	void	cEventMouseTrigger::InternalRender()
	{

	}

	TiXmlElement*		cEventMouseTrigger::ToTiXmlElement()
	{
		TiXmlElement*l_pTiXmlElement = cEventBase::ToTiXmlElement();
		l_pTiXmlElement->SetAttribute(L"Gesture",ValueToStringW(this->m_eCurrentGestureEnum));
		l_pTiXmlElement->SetAttribute(L"Distance",ValueToStringW(this->m_fMoveDistance));
		return l_pTiXmlElement;
	}

	cEventObjectMouseUpDectet::cEventObjectMouseUpDectet()
	{
		m_pParentEventObjectInstance = 0;
		m_bWaitForPlayDone = false;
		m_bEventObjectInstanceMouseUp = false;
	}

	cEventObjectMouseUpDectet::~cEventObjectMouseUpDectet()
	{
	
	}

	void		cEventObjectMouseUpDectet::InternalInit()
	{
		m_bEventObjectInstanceMouseUp = false;
	}

	void		cEventObjectMouseUpDectet::InternalUpdate(float e_fElpasedTime)
	{
		if(!m_pParentEventObjectInstance)
		{			
			m_pParentEventObjectInstance = g_pCurrentEventManager->m_pEventPackageRunner->GetCurrentEventPackage()->GetObject(m_strEventObjectInstanceName.c_str());
		}
		if( !m_pParentEventObjectInstance )
			return;
		if(m_pParentEventObjectInstance->GetCurrentWorkingObject())
		{
			if(!m_bEventObjectInstanceMouseUp)
			{
				if(m_pParentEventObjectInstance->GetCurrentWorkingObject()->GetObject()->IsMouseUp())
				{
					m_bEventObjectInstanceMouseUp = true;
					if( !m_bWaitForPlayDone )
						this->m_bSatisfiedCondition = true;
				}
			}
			else
			{
				if(m_pParentEventObjectInstance->GetCurrentWorkingObject()->GetObject()->IsPlayDone())
				{
					this->m_bSatisfiedCondition = true;
				}
			}
		}
	}

	TiXmlElement*		cEventObjectMouseUpDectet::ToTiXmlElement()
	{
		TiXmlElement*l_pEvent = this->GetMyTiXmlElement();
		//l_pEvent->SetAttribute(NAME,this->GetName());
		//l_pEvent->SetAttribute(BUTTON_SELECTION_TYPE,ValueToString(this->m_eButtonSelectionType));
		//l_pEvent->SetAttribute(L"StayAtLastSelect",this->m_bStayAtLastSelect?L"1":L"0");
		//if(this->m_iClickCount != 0 )
		//{
		//	l_pEvent->SetAttribute(L"ClickCount",this->m_iClickCount);
		//}
		//if(m_pPrefixButton)
		//{
		//	l_pEvent->SetAttribute(L"PrefixButtonName",m_pPrefixButton->GetName());
		//	cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(m_pPrefixButton);
		//	l_pEvent->LinkEndChild(l_pEventButton->ToTiXmlElement());
		//}
		//int	l_iCount = this->Count();
		//for( int i=0;i<l_iCount;++i )
		//{
		//	cEventButton*l_pEventButton = dynamic_cast<cEventButton*>(this->m_ObjectList[i]);
		//	l_pEvent->LinkEndChild(l_pEventButton->ToTiXmlElement());
		//}
		//return l_pEvent;
		return 0;
	}
//end namespace FATMING_CORE
}