#include "stdafx.h"
#include "FAIAnimationCharacter.h"
#include "../AllPhysicInclude.h"
//from Frame
//extern const FLOAT    FRAME_DIRTY_WORLD_CACHE;

namespace FATMING_AI
{
	//<FAIAnimationStatus >
	//	<Status Name="" MPDIList="" MPDI="" Loop="" />
	//	<Status Name="" MPDIList="" MPDI="" Loop="" />
	//	<Status Name="" MPDIList="" MPDI="" Loop="" />
	//	<Status Name="" MPDIList="" MPDI="" Loop="" />
	//</FAIAnimationStatus>
	const wchar_t*	cFAIAnimationCharacter::TypeID( L"cFAIAnimationCharacter" );
	cFAIAnimationCharacter::cFAIAnimationCharacter(TiXmlElement*e_pElement):cFAICharacterInterface(e_pElement)
	{
		//assert(wcscmp(e_pElement->Value(),cFAIAnimationCharacter::TypeID) == 0);
		if( e_pElement )
			e_pElement = e_pElement->FirstChildElement();
		cMPDIList*l_pMPDIList = 0;
		cMPDI*l_pMPDI = 0;
		cBaseBehavior*l_pBaseBehavior = 0;
		bool	l_bLoop = false;
		while( e_pElement )
		{
			std::wstring	l_strBehaviorName;
			PARSE_ELEMENT_START(e_pElement)
				COMPARE_NAME("Name")
				{
					l_strBehaviorName = l_strValue;
				}
				else
				COMPARE_NAME("MPDIList")
				{
					l_pMPDIList = cGameApp::GetMPDIListByFileName(l_strValue);
				}
				else
				COMPARE_NAME("MPDI")
				{
					if( l_pMPDIList )
						l_pMPDI = l_pMPDIList->GetObject(l_strValue);
				}
				else
				COMPARE_NAME("Loop")
				{
					l_bLoop = VALUE_TO_BOLLEAN;
				}
			PARSE_NAME_VALUE_END
			assert( l_pMPDIList && l_pMPDI && "failed to get MPDI");
			if( l_pMPDIList && l_pMPDI )
			{
				cMPDI*l_pCloneMPDI = new cMPDI(l_pMPDI);
				l_pBaseBehavior = l_pCloneMPDI;
				l_pBaseBehavior->SetName(l_strBehaviorName.c_str());
				l_pCloneMPDI->SetLoop(l_bLoop);
			}
			if( l_pBaseBehavior )
			{
				bool	l_b = this->AddObject(l_pBaseBehavior);
				assert(l_b);
			}
			e_pElement = e_pElement->NextSiblingElement();
		}
	}

	cFAIAnimationCharacter::cFAIAnimationCharacter(cFAIAnimationCharacter*e_pFAIAnimationStatus)
	:cFAICharacterInterface(e_pFAIAnimationStatus),cFMWorkingObjectChanger<cBaseBehavior>(e_pFAIAnimationStatus)
	{
	
	}

	cFAIAnimationCharacter::~cFAIAnimationCharacter()
	{
	
	}
	void*   cFAIAnimationCharacter::GetAnimationData()
	{
		//return (void*)(cFMWorkingObjectChanger<cBaseBehavior>*)this; 
		return this; 
	}

	void	cFAIAnimationCharacter::Update(float e_fElpaseTime)
	{
		//cFMWorkingObjectChanger<cBaseBehavior>::Update(e_fElpaseTime); 
		//if(this->m_pCurrentWorkingObject && m_CachedWorldTransform._11 == FRAME_DIRTY_WORLD_CACHE )
		//if(this->m_pCurrentWorkingObject)
		//{
		//	cMatrix44	l_mat = this->GetWorldTransform();
		//	if( m_pCurrentWorkingObject->Type() == cMPDI::TypeID )
		//	{
		//		cMPDI*l_pMPDI = dynamic_cast<cMPDI*>(m_pCurrentWorkingObject);
		//		l_pMPDI->SetTransform(l_mat);
		//	}
		//	cbtConcaveShapeList*l_pbtConcaveShapeList = (cbtConcaveShapeList*)this->GetCollisionData();
		//	l_pbtConcaveShapeList->SetTransform(l_mat);
		//}
	}
	void	cFAIAnimationCharacter::Render()
	{
		cFMWorkingObjectChanger<cBaseBehavior>::Render();
	}
}