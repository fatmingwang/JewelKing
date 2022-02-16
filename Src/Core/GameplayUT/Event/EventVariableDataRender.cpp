#include "../../stdafx.h"
#include "EventObject.h"
#include "EventVariable.h"
#include "EventVariableDataRender.h"
#include "EventManager.h"

namespace FATMING_CORE
{
	const wchar_t*     cEventVariableDataRender::TypeID( L"cEventVariableDataRender" );

	cEventVariableDataRender::cEventVariableDataRender(TiXmlElement*e_pTiXmlElement):cEventObject(eEOT_VARIABLE_RENDER,Vector3::Zero)
	{
		Vector3	l_vPos;
		cEventVariable*l_pEventVariable = 0;
		std::wstring	l_strPrefix;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME_WITH_DEFINE(cEventVariable::TypeID)
			{
				m_pEventVariable = g_pCurrentEventManager->m_pEventVariableManager->GetObject(l_strValue);
			}
			else
			COMPARE_NAME("Pos")
			{
			    this->m_vPos = VALUE_TO_VECTOR3;
			}
			else
			COMPARE_NAME("PrefixText")
			{
			    m_strPrefixText = l_strValue;
			}
		PARSE_NAME_VALUE_END
	}

	cEventVariableDataRender::cEventVariableDataRender(cEventVariable*e_pNameAndData,Vector3 e_vPos,const WCHAR*e_strPrefixText):cEventObject(eEOT_VARIABLE_RENDER,e_vPos)
	{
		if( e_strPrefixText&&wcslen(e_strPrefixText)>0 )
			m_strPrefixText = e_strPrefixText;
		m_pEventVariable = e_pNameAndData;
	}

	cEventVariableDataRender::cEventVariableDataRender(cEventVariableDataRender*e_pEventVariableDataRender):cEventObject(e_pEventVariableDataRender)
	{
		m_pEventVariable = e_pEventVariableDataRender->m_pEventVariable;
		m_strPrefixText = e_pEventVariableDataRender->m_strPrefixText;
	}

    cEventVariableDataRender::~cEventVariableDataRender()
	{
	
	}

	NamedTypedObject*           cEventVariableDataRender::GetResourcePointer(){ return 0; }

	void    cEventVariableDataRender::InternalRender()
	{
		if(this->m_pEventVariable)
		{
			if( m_strPrefixText.length() )
			{
				Vector4	l_vDrawRect;
				g_pCurrentEventManager->m_pEventFont->RenderFont(this->m_vPos.x,this->m_vPos.y,m_strPrefixText.c_str(),&l_vDrawRect);
				std::wstring	l_str = m_pEventVariable->ConvertToString();
				g_pCurrentEventManager->m_pEventFont->RenderFont(l_vDrawRect.z,l_vDrawRect.y,l_str.c_str());
			}
			else
			{
				std::wstring	l_str = m_pEventVariable->ConvertToString();
				g_pCurrentEventManager->m_pEventFont->RenderFont(this->m_vPos.x,this->m_vPos.y,l_str.c_str());
			}
		}
		else
		{
			g_pCurrentEventManager->m_pEventFont->RenderFont(this->m_vPos.x,this->m_vPos.y,L"m_pEventVariable not exist");
		}
	}
	TiXmlElement*	cEventVariableDataRender::ToTiXmlElement()
	{
		TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(cEventVariableDataRender::TypeID);
		if( m_pEventVariable )
			l_pDataTiXmlElement->SetAttribute(cEventVariable::TypeID,this->m_pEventVariable->GetName());
		l_pDataTiXmlElement->SetAttribute(L"Pos",ValueToStringW(*this->GetPos()));
		l_pDataTiXmlElement->SetAttribute(L"PrefixText",this->m_strPrefixText.c_str());
		return l_pDataTiXmlElement;

		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"VariableRender");
		l_pEventObjectTiXmlElement->SetAttribute(NAME,this->GetName());
		l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		return l_pEventObjectTiXmlElement;
	}
//end namespace
}