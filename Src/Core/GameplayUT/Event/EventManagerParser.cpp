#include "../../stdafx.h"
#include "AllEventInclude.h"
namespace FATMING_CORE
{
//		<cEventObjectInstance Name="" EventInstance="" EventObjectStatus="" >

//<EventInstanceList>
//	<EventInstance Name="EventTest" Repeat="true" >
//		<EventTrigger>
//			<cEventButton Name="" BaseImage="test" Pos="0,0,0" />
//		</EventTrigger>
//		<EventActiver>
//			<cEventValueChangeActiver Name="Test" ValueChangeType="Increase" >
//				<cRestrictValue DataType="int" Max="100" Min="0" IncreaseValue="Increase" Value="1" />
//			</cEventValueChangeActiver
//		</EventActiver>
//	</EventInstance>
//	<EventInstance>
//		<EventTrigger>
//			<cEventMultiButton Name="" ButtonSelectionType="One" >
//				<cEventButton Name="" BaseImage="test" Pos="0,0,0" />
//				<cEventButton Name="" PI="test" PIUnit="test" Pos="0,0,0" />
//				<cEventButton Name="" Text="79979 test" BGColor="1,0,0.5,1" Pos="0,0,0" />
//			</cEventMultiButton>
//		</EventTrigger>
//		<EventActiver>
//
//		</EventActiver>
//	</EventInstance>
//</EventInstanceList>

//		<cEventObjectInstance Name="" EventInstance="" EventObjectStatus="" AllStatusWorking="" EventObjectStatusVariable="">
	cEventObjectInstance*	cEventManager::GetEventObjectInstance(TiXmlElement*e_pTiXmlElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventObjectInstance::TypeID);
		std::wstring	l_strEventObjectInstanceName;
		std::wstring	l_strEventInstanceName;
		std::wstring	l_strEventObjectStatusName;
		std::wstring	l_strEventVariableName;
		bool			l_bEventObjectStatusAllWorking = false;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				l_strEventObjectInstanceName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventInstance::TypeID)
			{
				l_strEventInstanceName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(cEventObjectStatus::TypeID)
			{
				l_strEventObjectStatusName = l_strValue;
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_OBJECTINSTANCE_ALL_STATUS_WORJING)
			{
				l_bEventObjectStatusAllWorking = VALUE_TO_BOLLEAN;
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_OBJECT_STATUS_VARIABLE)
			{
				l_strEventVariableName = l_strValue;
			}
		PARSE_NAME_VALUE_END
		return GetEventObjectInstance(l_strEventObjectInstanceName.c_str(),l_strEventInstanceName.c_str(),l_strEventObjectStatusName.c_str(),l_bEventObjectStatusAllWorking,l_strEventVariableName.c_str());
	}

	void		cEventManager::EventInstanceAssignEventList(TiXmlElement*e_pElement,cClickMouseBehaviorList<cEventBase>*e_pList,bool e_bTrigger)
	{
		cEventBase*l_pEventBase = 0;
		while( e_pElement )
		{
			if( e_bTrigger )
				l_pEventBase = GetTriggerEvent(e_pElement);
			else
				l_pEventBase = GetActiverEvent(e_pElement);
			if( l_pEventBase )
				e_pList->AddObjectNeglectExist(l_pEventBase);
			else
			{
				UT::ErrorMsg(e_pElement->Value(),L"unsupport event type element value");
			}
			e_pElement = e_pElement->NextSiblingElement();
		}
	}

	cEventVariable*	cEventManager::GetEventVariable(const WCHAR*e_strName)
	{
		return m_pEventVariableManager->GetObject(e_strName);
	}
	//Event Object Instance
	//<cEventObjectStatus Name="" >
	//	<EventObject Name="" Type="MPDI" >
	//		<Data MPDIList="" MPDI="" StopAtLastFrame=""/>
	//	</EventObject>
	//	<EventObject Name="" Type="Text">
	//		<Data Color=""Size="" />
	//	</EventObject>
	//</EventObject>
	cEventObjectStatus*		cEventManager::GetEventObjectStatus(TiXmlElement*e_pTiXmlElement)
	{
		cEventObjectStatus*l_pEventObjectStatus = new cEventObjectStatus();;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventObjectStatus::TypeID);
		const WCHAR*l_strName = ATTRIBUT_VALUE(e_pTiXmlElement,L"Name");
		if( l_strName )
			l_pEventObjectStatus->SetName(l_strName);
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while(e_pTiXmlElement)
		{
			cEventObject*l_pEventObject = GetEventObject(e_pTiXmlElement);
			if( l_pEventObject )
			{
				l_strName = ATTRIBUT_VALUE(e_pTiXmlElement,L"Name");
				cObjectAndName<cEventObject>*l_pObjectAndName = new cObjectAndName<cEventObject>(l_strName);
				l_pObjectAndName->SetObject(l_pEventObject);
				if(!l_pEventObjectStatus->AddObject(l_pObjectAndName))
				{
					std::wstring	l_str;
					l_str = l_pEventObjectStatus->GetName();
					l_str += L"status ";
					l_str += l_pObjectAndName->GetName();
					l_str += L"exists\n";
					UT::ErrorMsg(l_str.c_str(),L"GetEventObjectStatus");
					delete l_pObjectAndName;
				}
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
		return l_pEventObjectStatus;
	}

	cEventObjectStatus*		cEventManager::GetEventObjectStatus(const WCHAR*e_strName)
	{
		cEventObjectStatus*l_pEventObjectStatus = m_pEventObjectStatusList->GetObject(e_strName);
		if( !l_pEventObjectStatus )
		{
			TiXmlElement*l_pTiXmlElement = this->m_pEventObjectStatusXml->GetChildOfRootXmlElementByAttribueName(cEventObjectStatus::TypeID,e_strName,NAME);
			if( l_pTiXmlElement )
			{
				TiXmlElement l_TiXmlElement(*l_pTiXmlElement);
				l_pEventObjectStatus = GetEventObjectStatus(&l_TiXmlElement);
				m_pEventObjectStatusList->AddObjectNeglectExist(l_pEventObjectStatus);
			}
		}
		return l_pEventObjectStatus;
	}

	cEventInstance*			cEventManager::GetEventInstance(const WCHAR*e_strName)
	{
		cEventInstance*l_pEventInstance = this->m_pEventInstanceList->GetObject(e_strName);
		if( !l_pEventInstance )
		{
			TiXmlElement*l_pTiXmlElement = this->m_pEventInstanceXml->GetChildOfRootXmlElementByAttribueName(cEventInstance::TypeID,e_strName,NAME);
			if( l_pTiXmlElement )
			{//prevent xml element attribute lost
				TiXmlElement l_TiXmlElement(*l_pTiXmlElement);
				l_pEventInstance = GetEventInstance(&l_TiXmlElement);
				m_pEventInstanceList->AddObjectNeglectExist(l_pEventInstance);
			}
			else
			{
				UT::ErrorMsg(L"there is no such EventInstance",e_strName);
			}
		}
		return l_pEventInstance;
	}

	cEventPackage*			cEventManager::GetEventPackage(const WCHAR*e_strName)
	{
		TiXmlElement*l_pTiXmlElement = GetChildOfRootXmlElementByAttribueName(L"EventPackage",e_strName,NAME);
		if( !l_pTiXmlElement )
		{
			cEventPackage*l_pEventPackage = m_pExternalEventPackage->GetObject(e_strName);
			return l_pEventPackage;
		}
		return GetEventPackage(l_pTiXmlElement);
	}

	cEventInstance*			cEventManager::GetEventInstance(TiXmlElement*e_pTiXmlElement)
	{
		cEventInstance*l_pEventInstance = new cEventInstance();
		bool	l_bRepeat = false;
		l_pEventInstance->SetRepeat(l_bRepeat);
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventInstance::TypeID);
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(NAME)
			{
				l_pEventInstance->SetName(l_strValue);
			}
			else
			COMPARE_NAME_WITH_DEFINE(EVENT_REPEAT)
			{
				l_pEventInstance->SetRepeat(VALUE_TO_BOLLEAN);
			}
			else
			COMPARE_NAME("AllTriggetDoneAtSameTime")
			{
				l_pEventInstance->SetTriggerAllDoneAtSameTime(VALUE_TO_BOLLEAN);
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while(e_pTiXmlElement)
		{
			cEventBase*l_pEventBase = 0;
			const WCHAR*	l_strValue = e_pTiXmlElement->Value();
			COMPARE_VALUE_WITH_DEFINE(EVENT_TRIGGER)
			{
				EventInstanceAssignEventList(e_pTiXmlElement->FirstChildElement(),&l_pEventInstance->m_TriggerEventList,true);
			}
			else
			COMPARE_VALUE_WITH_DEFINE(EVENT_ACTIVER)
			{
				EventInstanceAssignEventList(e_pTiXmlElement->FirstChildElement(),&l_pEventInstance->m_ActiveEventList,false);
			}
			else
			{
				UT::ErrorMsg(L"unsupport format",l_strValue);
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
		return l_pEventInstance;
	}
	//there are 4 files

	//variable manager.
	//event instance.
	//event object instance.
	//event packages.

	//event packages parse indicate package and fetch necessary data from variable manager,event instance,event object instance.

	//variable

	//<VariableData>
	//	<Data Name="" Type="int" Value="0" />
	//	<Data Name="" Type="char" Value="37" />
	//	<Data Name="" Type="float" Value="0.1" />
	//	<Data Name="" Type="double" Value="0.3" />
	//	<Data Name="" Type="string" Value="79979" />
	//	<Data Name="" Type="wstring" Value="2266 88 99" />
	//</VariableData>

	//<EventPackage PackageCount="">
	//	<Package Name="" EventInstanceCount="">
	//		<EventInstance Name="">
	//		<EventInstance Name="">
	//		<EventInstance Name="">
	//		<EventInstance Name="">
	//		<EventInstance Name="">
	//	</Package>
	//</EventPackage>
}