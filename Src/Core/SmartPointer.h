#ifndef _SMART_POINTER_H_
#define _SMART_POINTER_H_


#include "NamedTypedObject.h"

namespace FATMING_CORE
{
	class	cSmartPointer
	{
		std::vector<NamedTypedObject*>*	m_pRefernceObjectList;
	public:
		cSmartPointer(NamedTypedObject*e_pNamedTypedObject);
		~cSmartPointer();
		int	AddRef(NamedTypedObject*e_pNamedTypedObject);
		int	Release(NamedTypedObject*e_pNamedTypedObject);
	};
//END NAMESPACE
}

#endif