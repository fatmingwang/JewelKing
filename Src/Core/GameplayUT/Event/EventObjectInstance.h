#ifndef _EVENT_PATGE_OBJECT_STATUS_H_
#define _EVENT_PATGE_OBJECT_STATUS_H_

#include "EventObject.h"
#include "EventInstance.h"
//page object status
namespace FATMING_CORE
{
	//<EventObjectStatusList WorkgPath="TenJewelData\ControlPanel\">
	//	<cEventObjectStatus Name="Generator">
	//		<cEventObject Type="MPDI" Name="Hi">
	//			<cEventMPDI RelativePath="MPDIFile" MPDIList="Generator" MPDI="Hurt" StopAtLastFrame="1" ClickToPlay="1" />
	//		</cEventObject>
	//		<cEventObject Type="MPDI" Name="Hello">
	//			<cEventMPDI RelativePath="MPDIFile" MPDIList="Generator" MPDI="Generating" StopAtLastFrame="1" ClickToPlay="1"/>
	//		</cEventObject>
	//		<cEventObject Type="MPDI" Name="World">
	//			<cEventMPDI RelativePath="MPDIFile" MPDIList="Generator" MPDI="Idle" StopAtLastFrame="1" ClickToPlay="1"/>
	//		</cEventObject>
	//	</cEventObjectStatus>
	//	<cEventObjectStatus Name="ShowLinePlayable">
	//		<cEventObject Type="VariableRender" Name="Hi">
	//			<cEventVariableDataRender Pos="200,180,0" cEventVariable="LinePlayable" />
	//		</cEventObject>
	//	</cEventObjectStatus>
	//</EventObjectStatusList>
	//cEventObjectStatus could contain event instance list or only event instance list without any event object
	//render object with event instance
	class	cEventVariable;
	class	cEventObjectStatus:public cFMWorkingObjectChanger<cObjectAndName<cEventObject> >
	{
	protected:
		//keep to check StatusEventVariable's data and convert it to string to change eventobject's status
		GET_SET_DEC(cEventVariable*,m_pStatusEventVariable,GetStatusEventVariable,SetStatusEventVariable);
	public:
		DEFINE_TYPE_INFO();
		cEventObjectStatus();
		cEventObjectStatus(cEventObjectStatus*e_pEventObjectStatus);
		CLONE_MYSELF(cEventObjectStatus);
		~cEventObjectStatus(){}
		virtual	void	Update(float e_fElpaseTime);
		//cEventObjectStatus(cEventObjectStatus*e_pEventObjectStatus);
		virtual	TiXmlElement*	ToTiXmlElement();
	};

	//=========================
	//cEventObjectStatus and cEventInstance please call	SetTargetEventObjectStatus,AddEventInstance 
	//because all data must be clone from g_pCurrentEventManager so we could save resource as fine.
	//=========================
	//<cEventObjectInstance Name="VendorMenu" cEventInstance="VendorMenu" cEventObjectStatus="VendorMenu" EventObjectStatusVariable="VendorMenuSelectedIndex" />
	//<cEventObjectInstance Name="VendorMenu" cEventObjectStatus="GamePrizeText" cEventInstance="BackToVendorMenu"  AllStatusWorking="1" />
	//<cEventObjectInstance Name="VendorMenu_SelctionDown" cEventInstance="VendorMenu_SelctionDown" />
	//why I combine cEventInstance and cEventObjectStatus is because I wanna let cEventInstance could manioulate cEventObjectStatus.
	class   cEventObjectInstance:public cEventObjectStatus,public cClickMouseBehavior
	{
		cEventObject*m_pCurrentEventPageData;
		virtual bool InternalCollide(int e_iPosX,int e_iPosY);
		//if u call set ensure memory control problem
		GET_SET_DEC(cClickMouseBehaviorList<cEventInstance>*,m_pEventInstanceList,GetEventInstanceList,SetEventInstanceList);
		//from EventManager
		cEventObjectStatus*	m_pTargetEventObjectStatus;
		//if true all status will update and render at same time
		bool	m_bAllStatusWorking;
	public:
		DEFINE_TYPE_INFO();
		//ensure memory control
		cEventObjectInstance();
		cEventObjectInstance(cEventObjectInstance*e_pEventObjectStatus);
		CLONE_MYSELF(cEventObjectInstance);
		~cEventObjectInstance();
		virtual	bool    SetCurrentWorkingObject(int e_iIndex,bool e_bRestart = true);
		virtual void	MouseMove(int e_iPosX,int e_iPosY);
        virtual void	MouseDown(int e_iPosX,int e_iPosY);
        virtual void    MouseUp(int e_iPosX,int e_iPosY);
		virtual	void	Init();
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
		virtual	void	Destroy();
		bool			IsAllStatusWorking(){return m_bAllStatusWorking;}
		void			SetAllStatusWorking(bool e_bAllStatusWorking);

		cEventObjectStatus*GetEventObjectStatus(){return  m_pTargetEventObjectStatus;}
		void	SetTargetEventObjectStatus(cEventObjectStatus*e_pEventObjectStatus);
		//if not clone ensure data maintain
		void	AddEventInstance(cEventInstance*e_pEventInstance,bool e_bClone = true);
		virtual	void	CollectResourceList(cObjectListByName<NamedTypedObject>*e_pObjectListByName);
		virtual	TiXmlElement*	ToTiXmlElement();
	};

	////typedef cFMWorkingObjectChanger<cObjectAndName<cEventObject>>	cEventObjecttStatus;
	////page object status with mouse
	//class	cEventPageObjectStatus:public cEventObjectInstance
	//{
	//public:
	//	cEventPageObjectStatus();
	//	cEventPageObjectStatus(cEventPageObjectStatus*e_pEventPageObjectStatus);
	//	cEventPageObjectStatus(TiXmlElement*e_pTiXmlElement);
	//	~cEventPageObjectStatus();
	//};
	////
//eend namespace Event
}
#endif