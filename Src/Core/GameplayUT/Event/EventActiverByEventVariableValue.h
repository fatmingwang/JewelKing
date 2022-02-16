#ifndef _EVENT_ACTIVER_BY_EVENT_VARIABLE__H_
#define _EVENT_ACTIVER_BY_EVENT_VARIABLE__H_

namespace FATMING_CORE
{

//event tirgger could be satisfied but result could be failed(ex:input pw but not same)
//so we have to compare variable to detect same or not
//
//<cEventVariableCheckYesAndNoActiver SrcEventVariable="InputPassword" CompareResultVariable="VendorPassword" >
//	<Yes>
//		<cEventValueChangeActiver Name="Test" cEventVariable="CurrentPackageName" TargetValue="VendorMenu" />
//	</Yes>
//	<NO>
//
//	</NO>
//</cEventVariableCheckYesAndNoActiver>
//
//
//<cEventVariableCheckYesAndNoActiver SrcEventVariable="" Result="79979"  >
//	<Yes>
//
//	</Yes>
//	<NO>
//
//	</NO>
//</cEventVariableCheckYesAndNoActiver>

	class	cEventVariableCheckYesAndNoActiver:public cEventBase
	{
		cEventVariable*	m_pSrcEventVariable;
		cEventVariable*	m_pCompareResultVariable;
		std::wstring*	m_pstrResult;
		cDoAllMouseBehaviorList<cEventBase>	m_YesEventList;
		cDoAllMouseBehaviorList<cEventBase>	m_NoEventList;
		virtual	void					InternalInit();
		virtual	void					InternalUpdate(float e_fElpasedTime);
		virtual	void					InternalRender();
	public:
		DEFINE_TYPE_INFO();
		cEventVariableCheckYesAndNoActiver(TiXmlElement*e_pTiXmlElement);
		cEventVariableCheckYesAndNoActiver(cEventVariableCheckYesAndNoActiver*e_pEventVariableCheckYesAndNoActiver);
		EVENT_CLONE_DEFINE(cEventVariableCheckYesAndNoActiver);
		~cEventVariableCheckYesAndNoActiver();
		virtual	TiXmlElement*		ToTiXmlElement();
	};
//end namespace FATMING_CORE
}

#endif