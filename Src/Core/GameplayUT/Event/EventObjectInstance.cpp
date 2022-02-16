#include "../../stdafx.h"
#include "EventObjectInstance.h"
#include "EventVariable.h"
namespace   FATMING_CORE
{
	const wchar_t*     cEventObjectInstance::TypeID( L"cEventObjectInstance" );
	const wchar_t*     cEventObjectStatus::TypeID( L"cEventObjectStatus" );

	cEventObjectStatus::cEventObjectStatus()
	{
		m_pStatusEventVariable = 0;
	}

	cEventObjectStatus::cEventObjectStatus(cEventObjectStatus*e_pEventObjectStatus)
		:cFMWorkingObjectChanger<cObjectAndName<cEventObject> >(e_pEventObjectStatus)
	{
		m_pStatusEventVariable = e_pEventObjectStatus->m_pStatusEventVariable;
	}

	void	cEventObjectStatus::Update(float e_fElpaseTime)
	{
		if( this->m_pStatusEventVariable )
		{
			if( m_pCurrentWorkingObject )
			{
				std::wstring	l_strVariableName = m_pStatusEventVariable->ConvertToString();
				int	l_iIndex = this->GetObjectIndexByName(l_strVariableName.c_str());
				if( l_iIndex != -1 )
				{
					if(wcscmp(m_pCurrentWorkingObject->GetName(),l_strVariableName.c_str()))
					{
						this->SetCurrentWorkingObject(l_iIndex);
					}
				}
			}
			//this->gtobje
		}
		if( m_pCurrentWorkingObject )
			m_pCurrentWorkingObject->Update(e_fElpaseTime); 
	}

	cEventObjectInstance::cEventObjectInstance()
	{
		m_pCurrentEventPageData = 0;
		m_pEventInstanceList = new cClickMouseBehaviorList<cEventInstance>;
		m_pTargetEventObjectStatus = 0;
		m_pEventInstanceList->SetOwner(this);
		m_bAllStatusWorking = false;
	}

	cEventObjectInstance::cEventObjectInstance(cEventObjectInstance*e_pEventObjectStatus):cEventObjectStatus(e_pEventObjectStatus)
	{
		m_pTargetEventObjectStatus = e_pEventObjectStatus->m_pTargetEventObjectStatus;
		int	l_iCount = e_pEventObjectStatus->Count();
		m_pEventInstanceList = dynamic_cast<cClickMouseBehaviorList<cEventInstance>*>(e_pEventObjectStatus->Clone());
		m_pEventInstanceList->SetOwner(this);
		m_bAllStatusWorking = e_pEventObjectStatus->m_bAllStatusWorking;
		//l_iCount = this->Count();
		//for( int i=0;i<l_iCount;++i )
		//{
		//	cObjectAndName<cEventObject>*l_pData = CloneEventObject(e_pEventObjectStatus->GetObject(i));
		//	this->AddObjectNeglectExist(l_pData);
		//}
	}

	cEventObjectInstance::~cEventObjectInstance()
	{
		SAFE_DELETE(m_pEventInstanceList);
	}
	
	bool cEventObjectInstance::InternalCollide(int e_iPosX,int e_iPosY)
	{
		if( m_bAllStatusWorking )
			return true;
		if( !m_pCurrentEventPageData )
			return true;
		return m_pCurrentEventPageData->IsCollided(e_iPosX,e_iPosY);
	}

	bool      cEventObjectInstance::SetCurrentWorkingObject(int e_iIndex,bool e_bRestart)
	{
		if( cEventObjectStatus::SetCurrentWorkingObject(e_iIndex,e_bRestart) )
		{
			m_pCurrentEventPageData = this->m_pCurrentWorkingObject->GetObject();
		}
		return 0;
	}

	void			cEventObjectInstance::MouseMove(int e_iPosX,int e_iPosY)
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->MouseMove(e_iPosX,e_iPosY)) 
		}
		else
		if(this->m_pCurrentEventPageData) 
			m_pCurrentEventPageData->MouseMove(e_iPosX,e_iPosY);
		m_pEventInstanceList->MouseMove(e_iPosX,e_iPosY);
	}

    void			cEventObjectInstance::MouseDown(int e_iPosX,int e_iPosY)
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->MouseDown(e_iPosX,e_iPosY)) 
		}
		else
		if(this->m_pCurrentEventPageData) 
			m_pCurrentEventPageData->MouseDown(e_iPosX,e_iPosY);
		m_pEventInstanceList->MouseDown(e_iPosX,e_iPosY);
	}

    void    		cEventObjectInstance::MouseUp(int e_iPosX,int e_iPosY)
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->MouseUp(e_iPosX,e_iPosY)) 
		}
		else
		if(this->m_pCurrentEventPageData)
			m_pCurrentEventPageData->MouseUp(e_iPosX,e_iPosY);
		m_pEventInstanceList->MouseUp(e_iPosX,e_iPosY);
	}

	void	cEventObjectInstance::Init()
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Init()) 
		}
		else
		{
			this->SetCurrentWorkingObject(0,false);
			cEventObjectStatus::Init(); 
		}
		m_pEventInstanceList->Init();
	}

	void	cEventObjectInstance::Update(float e_fElpaseTime)
	{
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Update(e_fElpaseTime)) 
		}
		else
			cEventObjectStatus::Update(e_fElpaseTime);
		m_pEventInstanceList->Update(e_fElpaseTime);
	}

	void	cEventObjectInstance::Render()
	{
		m_pEventInstanceList->Render();
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Render()) 
		}
		else
			cEventObjectStatus::Render();
	}

	void	cEventObjectInstance::Destroy()
	{
		m_pEventInstanceList->Destroy();
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			this->m_ObjectList[i]->Destroy();
		}
	}

	void	cEventObjectInstance::SetAllStatusWorking(bool e_bAllStatusWorking)
	{
		m_bAllStatusWorking = e_bAllStatusWorking;
		if( m_bAllStatusWorking )
		{
			LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Init());
			//LIST_DO_FOR_ALL_CHILDREN(this,GetObject()->Start());
		}
	}

	void	cEventObjectInstance::AddEventInstance(cEventInstance*e_pEventInstance,bool e_bClone)
	{
		if( e_bClone )
		{
			cEventInstance*l_pEventInstance = e_pEventInstance->Clone();
			m_pEventInstanceList->AddObjectNeglectExist(l_pEventInstance);
		}
		else
		{
			m_pEventInstanceList->AddObjectNeglectExist(e_pEventInstance);
		}
	}

	void	cEventObjectInstance::SetTargetEventObjectStatus(cEventObjectStatus*e_pEventObjectStatus)
	{
		cEventObjectStatus::Destroy();
		ADD_CLONE_DATA_TO_NEW_VECTOR(cObjectAndName<cEventObject>,e_pEventObjectStatus,this);
		m_pTargetEventObjectStatus = e_pEventObjectStatus;
		this->m_pStatusEventVariable = e_pEventObjectStatus->GetStatusEventVariable();
	}

	void	cEventObjectInstance::CollectResourceList(cObjectListByName<NamedTypedObject>*e_pObjectListByName)
	{
		int	l_iCount = Count();
		for( int i=0;i<l_iCount;++i )
		{
			NamedTypedObject*l_pResource = GetObject(i)->GetObject()->GetResourcePointer();
			if( l_pResource )
			{
				e_pObjectListByName->AddObject(l_pResource);
			}
		}
		l_iCount = m_pEventInstanceList->Count();
		for( int i=0;i<l_iCount;++i )
		{
			m_pEventInstanceList->GetObject(i)->CollectResourceList(e_pObjectListByName);
		}
	}
	//<cEventObjectInstance Name="VendorMenu" cEventInstance="VendorMenu" cEventObjectStatus="VendorMenu" EventObjectStatusVariable="VendorMenuSelectedIndex" />
	TiXmlElement*	cEventObjectInstance::ToTiXmlElement()
	{
		TiXmlElement*l_pTiXmlElement = new TiXmlElement(cEventObjectInstance::TypeID);
		l_pTiXmlElement->SetAttribute(NAME,this->GetName());
		if(m_pEventInstanceList&&m_pEventInstanceList->Count())
		{
			l_pTiXmlElement->SetAttribute(cEventInstance::TypeID,m_pEventInstanceList->GetObject(0)->GetName());
		}

		if(this->Count())
		{
			l_pTiXmlElement->SetAttribute(cEventObjectStatus::TypeID,m_pTargetEventObjectStatus->GetName());
		}
		if( m_pTargetEventObjectStatus )
		{
			l_pTiXmlElement->SetAttribute(L"EventObjectStatusVariable",m_pStatusEventVariable->GetName());
		}
		return l_pTiXmlElement;
	}
//end Event
}