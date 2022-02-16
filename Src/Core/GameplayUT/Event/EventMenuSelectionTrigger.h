#ifndef _EVENT_MENU_SELECTION_ACTIVER_H_
#define _EVENT_MENU_SELECTION_ACTIVER_H_
//==========================
//for game menu selection
//==========================
namespace FATMING_CORE
{
	class	cEventMenuSelectionTrigger:public cEventBase
	{

		virtual	void	InternalInit();
		virtual	void	InternalUpdate(float e_fElpasedTime);
		virtual	void	InternalRender();
	public:
		DEFINE_TYPE_INFO();
		cEventMenuSelectionTrigger(TiXmlElement*e_pTiXmlElement);
		cEventMenuSelectionTrigger();
		cEventMenuSelectionTrigger(cEventMenuSelectionTrigger*e_pEventTest);
		EVENT_CLONE_DEFINE(cEventMenuSelectionTrigger);
		~cEventMenuSelectionTrigger();
	};
//end namespace FATMING_CORE
}