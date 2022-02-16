#ifndef _EVENT_ADD_EVENT_OBJECT_INSTANCE_H_
#define _EVENT_ADD_EVENT_OBJECT_INSTANCE_H_

namespace FATMING_CORE
{
	//<cEventAddEventObjectInstace Name="" cEventObjectInstance="" cEventInstance="" cEventObjectStatus="" AllStatusWorking="0" cEventVariable="" />
	class	cEventAddEventObjectInstace:public cEventBase
	{
		std::wstring					m_strEventObjectInstanceName;
		std::wstring					m_strEventInstanceName;
		std::wstring					m_strEventStatusName;
		std::wstring					m_strEventVariableName;
		bool							m_bAllEventObjectStatusWorking;
		virtual	void					InternalInit();
		virtual	void					InternalUpdate(float e_fElpasedTime);
	public:
		DEFINE_TYPE_INFO();
		cEventAddEventObjectInstace(TiXmlElement*e_pTiXmlElement);
		cEventAddEventObjectInstace(cEventAddEventObjectInstace*e_pEventAddEventObjectInstace);
		EVENT_CLONE_DEFINE(cEventAddEventObjectInstace);
		~cEventAddEventObjectInstace();
		virtual	TiXmlElement*		ToTiXmlElement();
	};
//end FATMING_CORE
}

#endif