#ifndef BASE_EVENT_H
#define BASE_EVENT_H

#include "../PhaseControl.h"

namespace FATMING_CORE
{
	class	cBaseEvent:public cSimplePhase
	{
		virtual	void	InternalInit() = 0;
		virtual	void	InternalUpdate(float e_fElpasedTime) = 0;
		virtual	void	InternalRender(){}
	public:
		DEFINE_TYPE_INFO();
		cBaseEvent();
		~cBaseEvent();
		virtual	void	Init();
		virtual	void	Update(float e_fElpasedTime);
		virtual	void	Render();
		//while m_bSatisfiedCondition is true render is skip but ForceRender call internal render
		virtual	void	ForceRender();
		//to get current info
		virtual	void	DebugRender(){}
		virtual	WCHAR*	GetNextPhaseName(){ return 0; }
	};

	//===================================================
	//quick copy sample
	//===================================================
	//class	cEventTest:public cBaseEvent
	//{
	//	virtual	void	InternalInit();
	//	virtual	void	InternalUpdate(float e_fElpasedTime);
	//	virtual	void	InternalRender();
	//public:
	//	DEFINE_TYPE_INFO();
	//	cEventTest(TiXmlDocument*e_pTiXmlDocument);
	//	cEventTest();
	//	cEventTest(cEventTest*e_pEventTest);
	//}
	//const wchar_t*         cEventTest::TypeID( L"cEventTest" );

	//cEventTest::cEventTest(TiXmlDocument*e_pTiXmlDocument)
	//{
	//
	//}
	//cEventTest::cEventTest()
	//{
	//
	//}

	//cEventTest::cEventTest(cEventTest*e_pEventTest)
	//{
	//
	//}

	//void	cEventTest::InternalInit()
	//{

	//}

	//void	cEventTest::InternalUpdate(float e_fElpasedTime)
	//{

	//}

	//void	cEventTest::InternalRender()
	//{

	//}
	
//end FATMING_CORE
}
#endif