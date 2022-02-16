#ifndef _MULTI_CILLISION_FISH_H_
#define _MULTI_CILLISION_FISH_H_

#include "Fish.h"

	//<Monster Type="Normal">
	//	<Animation	MPDIList="xxx.mpdi" MPDI="xxxx" >
	//	<Physical File="xxx.xml">
	//	<AI Moving="xxx.xml">
	//</Monster>
	//it only has 3 AI,random path,fixed path.2 point move path.
	class	cMultiCollisionDataFish:public cNormalFish
	{
		//
		virtual	void			SetTransform();
		//most time is one but some monster are assemble by parts
		struct	sCollisionData
		{
			int						iNumCollisionShape;
			int*					piCollisionTargetIndex;
			cPuzzleImage*			pCollisionPITarget;
			c2DImageCollisionData	CollisionData;
			sCollisionData();
			~sCollisionData();
			sCollisionData*			Clone();
		};
		std::vector<sCollisionData*>	m_CollisionDataList;
		//
		virtual	void			ProcessPhysicalData(TiXmlElement*e_pElement);
	public:
		cMultiCollisionDataFish(TiXmlElement*e_pElement);
		cMultiCollisionDataFish(cMultiCollisionDataFish*e_pMultiCollisionDataFish);
		~cMultiCollisionDataFish();
		CLONE_MYSELF(cMultiCollisionDataFish);
		DEFINE_TYPE_INFO();
		virtual	void			DebugRender();
		virtual	bool			IsCollide(cbtShapeCollision*e_pbtShapeCollision);
		virtual	void			CollisionDataCheck();
	};


#endif