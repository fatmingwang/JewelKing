#ifndef _EVENT_VARIABLE_DATA_RENDER_H_
#define _EVENT_VARIABLE_DATA_RENDER_H_

namespace FATMING_CORE
{
	class	cEventVariable;
    class   cEventVariableDataRender:public cEventObject
    {
		std::wstring						m_strPrefixText;
        virtual NamedTypedObject*           GetResourcePointer();
		virtual void    InternalRender();
		cEventVariable*	m_pEventVariable;
    public:
        DEFINE_TYPE_INFO();
		cEventVariableDataRender(TiXmlElement*e_pTiXmlElement);
        cEventVariableDataRender(cEventVariable*e_pNameAndData,Vector3 e_vPos,const WCHAR*e_strPrefixText);
        cEventVariableDataRender(cEventVariableDataRender*e_pEventVariableDataRender);
        ~cEventVariableDataRender();
		EVENT_CLONE_DEFINE(cEventVariableDataRender);
		virtual	TiXmlElement*				ToTiXmlElement();
		void								SetPrefixText(const WCHAR*e_str){ m_strPrefixText = e_str; }
		void								SetEventVariable(cEventVariable*e_pEventVariable){ m_pEventVariable = e_pEventVariable; }
    };

}

#endif