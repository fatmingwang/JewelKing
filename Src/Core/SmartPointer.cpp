#include "stdafx.h"
#include "SmartPointer.h"

namespace FATMING_CORE
{
	cSmartPointer::cSmartPointer(NamedTypedObject*e_pNamedTypedObject)
	{
		m_pRefernceObjectList = new std::vector<NamedTypedObject*>;
		m_pRefernceObjectList->push_back(e_pNamedTypedObject);
	}

	cSmartPointer::~cSmartPointer()
	{
		if(m_pRefernceObjectList->size() != 0)
		{
			std::wstring	l_strName;
			size_t	l_uiSize = m_pRefernceObjectList->size();
			for( size_t i=0;i<l_uiSize;++i)
			{
				if((*m_pRefernceObjectList)[i])
				{
					l_strName += L"Name:";
					l_strName += (*m_pRefernceObjectList)[i]->GetName();
					l_strName += L"	Type:";
					l_strName += (*m_pRefernceObjectList)[i]->Type();
					l_strName += L"\n";
				}
			}
			UT::ErrorMsg(l_strName.c_str(),L"has no release object");
		}
	}

	int	cSmartPointer::AddRef(NamedTypedObject*e_pNamedTypedObject)
	{
#ifdef _DEBUG
		size_t	l_uiSize = m_pRefernceObjectList->size();
		for( size_t i=0;i<l_uiSize;++i)
		{
			if( (*m_pRefernceObjectList)[i] == e_pNamedTypedObject )
			{
				assert(0);
			}
		}
#endif
		m_pRefernceObjectList->push_back(e_pNamedTypedObject);
		return m_pRefernceObjectList->size();
	}

	int	cSmartPointer::Release(NamedTypedObject*e_pNamedTypedObject)
	{
		size_t	l_uiSize = m_pRefernceObjectList->size();
		for( size_t i=0;i<l_uiSize;++i)
		{
			if( (*m_pRefernceObjectList)[i] == e_pNamedTypedObject )
			{
				m_pRefernceObjectList->erase(m_pRefernceObjectList->begin()+i);
				break;
			}
		}
		return m_pRefernceObjectList->size();
	}
}