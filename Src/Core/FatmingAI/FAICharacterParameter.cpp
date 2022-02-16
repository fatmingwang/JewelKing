#include "stdafx.h"
#include "FAICharacterParameter.h"
#include "../AllPhysicInclude.h"
namespace FATMING_AI
{
	eCharacterAttribute	GetCharacterAttribute(const WCHAR*e_str)
	{
		for( int i=0;i<eCA_UNKNOW;++i )
		{
			if( !wcscmp(e_str,g_strCharacterAttribute[i]) )
			{
				return (eCharacterAttribute)i;
			}
		}
		return eCA_UNKNOW;
	}
//<Monkey	Count="3">
//	<Data Attribute="HP" Value="10"  Posiion="0,0" />
//	<Data Attribute="Agile" Value="10"  Posiion="0,100" />
//	<Data Attribute="Defence" Value="10"  Posiion="0,110" />
//</Monkey>
	std::vector<sCharacterAttributeAndUIInfo>	ParseAnimalStatusAndUIInfo(char*e_strFileName)
	{
		std::vector<sCharacterAttributeAndUIInfo>l_StatueAndUIInfoVector;
		cNodeISAX	l_NodeISAX;
		if(l_NodeISAX.ParseDataIntoXMLNode(e_strFileName))
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strAttribute = l_pElement->Attribute(L"Attribute");
				const WCHAR*l_strValue = l_pElement->Attribute(L"Value");
				const WCHAR*l_strPosiion = l_pElement->Attribute(L"Posiion");
				sCharacterAttributeAndUIInfo l_sStatueAndUIInfo;
				if( l_strAttribute )
					l_sStatueAndUIInfo.CharacterAttribute = GetCharacterAttribute(l_strAttribute);
				if( l_strValue )
					l_sStatueAndUIInfo.fValue = (float)_wtof(l_strValue);
				if( l_strPosiion )
					l_sStatueAndUIInfo.vUIPos = GetVector2(l_strPosiion);
				l_StatueAndUIInfoVector.push_back(l_sStatueAndUIInfo);
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
		return l_StatueAndUIInfoVector;
	}

	sCharacterAttributeAndUIInfo*	GetCharacterAttributeAndUIInfo(std::vector<sCharacterAttributeAndUIInfo>*e_pCharacterAttributeAndUIInfoVector,
		eCharacterAttribute	e_CharacterAttribute,int*e_piReceivedIndex)
	{
		size_t l_uiSize = e_pCharacterAttributeAndUIInfoVector->size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			sCharacterAttributeAndUIInfo*	l_pCharacterAttributeAndUIInfo = &(*e_pCharacterAttributeAndUIInfoVector)[i];
			if(l_pCharacterAttributeAndUIInfo->CharacterAttribute == e_CharacterAttribute)
			{
				if( e_piReceivedIndex )
					*e_piReceivedIndex = i;
				return l_pCharacterAttributeAndUIInfo;
			}
		}
		return 0;
	}
    const wchar_t*	cFAICharacterStatus::TypeID( L"cFAICharacterStatus" );
	const wchar_t*	cFAICharacterInterface::TypeID( L"cFAICharacterInterface" );
    //const wchar_t*cFAICharacterInterface::TypeID( L"cFAICharacterInterface" );

	cFAICharacterInterface::cFAICharacterInterface(cFAICharacterInterface*e_pAICharacterInterface)
	{
		m_pBound = 0;
		m_fAnimationSpeed = e_pAICharacterInterface->m_fAnimationSpeed;
		if( e_pAICharacterInterface->m_pBound )
		{
			m_pBound = new cBound(*e_pAICharacterInterface->m_pBound);
		}
		m_v2DAngle = Vector3::Zero;
		m_fSpeed = e_pAICharacterInterface->m_fSpeed;
		m_fRunAwaySpeed = e_pAICharacterInterface->m_fRunAwaySpeed;
		this->m_pAICharacterStatus = (cFAICharacterStatus*)e_pAICharacterInterface->m_pAICharacterStatus->Clone();
	}

    cFAICharacterInterface::cFAICharacterInterface(TiXmlElement*e_pElement)
    {
		m_pBound = 0;
		m_v2DAngle = Vector3::Zero;
		m_fSpeed = 10.f;
		m_fAnimationSpeed = 1.f;
		m_pAICharacterStatus = 0;
		if( e_pElement )
		{
			const WCHAR*l_strSpeed = e_pElement->Attribute(L"Speed");
			if( l_strSpeed )
				m_fSpeed = (float)_wtof(l_strSpeed);
			const WCHAR*l_strRunAwaySpeed = e_pElement->Attribute(L"RunAwaySpeed");
			if( l_strRunAwaySpeed )
				m_fRunAwaySpeed = (float)_wtof(l_strRunAwaySpeed);
			else
				m_fRunAwaySpeed = m_fSpeed;
		}
    }

	cFAICharacterInterface::~cFAICharacterInterface()
	{
		SAFE_DELETE(m_pBound);
		SAFE_DELETE(m_pAICharacterStatus);
	}

    Vector3* cFAICharacterInterface::GetPosition()
    {
        return (Vector3*)(size_t)(m_LocalTransform.m[MyMath::W]);
    }

	void	cFAICharacterInterface::DebugRender()
	{
		//NamedTypedObject*l_pNamedTypedObject = (NamedTypedObject*)this->GetCollisionData();
		//if( l_pNamedTypedObject )
		//{
		//	if( l_pNamedTypedObject->Type() == cbtConcaveShapeList::TypeID )
		//	{
		//		cbtConcaveShapeList*l_pbtConcaveShapeList = (cbtConcaveShapeList*)this->GetCollisionData();
		//		l_pbtConcaveShapeList->Render();
		//	}
		//}
	}

	void	 cFAICharacterInterface::SetBound(cBound*e_pData)
	{
		if(m_pBound)
		{
			*m_pBound = *e_pData;
		}
		else
			m_pBound = new cBound(*e_pData);
	}

    cFAICharacterStatus::cFAICharacterStatus(TiXmlElement*e_pElement)
    {
		m_iDexterous = m_iHP = m_iDefence = m_iStrgeth = m_iLucky = m_iAgile = 100;
		TiXmlAttribute*l_pTiXmlAttribute = e_pElement->FirstAttribute();
        const WCHAR*l_strName = 0;
        const WCHAR*l_strValue = 0;
        while( l_pTiXmlAttribute )
        {
            l_strValue = l_pTiXmlAttribute->Value();
            l_strName = l_pTiXmlAttribute->Name();
            COMPARE_NAME("Name")
            {
                this->SetName(l_strValue);
            }
            else
            COMPARE_NAME("HP")
            {
                this->m_iHP = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Strength")
            {
                this->m_iStrgeth = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Defence")
            {
                this->m_iDefence = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Agile")
            {
                this->m_iAgile = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Dexterous")
            {
                this->m_iDexterous = VALUE_TO_INT;
            }
            else
            COMPARE_NAME("Lucky")
            {
                this->m_iLucky = VALUE_TO_INT;
            }                        
            l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
        }
    }		
}