#ifndef _FAI_ANIMATION_STATUS_H_
#define _FAI_ANIMATION_STATUS_H_

namespace FATMING_AI
{
	class	cFAIAnimationCharacter:public cFMWorkingObjectChanger<cBaseBehavior>,public cFAICharacterInterface
	{
	public:
		DEFINE_TYPE_INFO();
		cFAIAnimationCharacter(TiXmlElement*e_pElement);
		cFAIAnimationCharacter(cFAIAnimationCharacter*e_pFAIAnimationStatus);
		~cFAIAnimationCharacter();
		CLONE_MYSELF(cFAIAnimationCharacter);
		virtual void*   GetAnimationData();
		virtual	void	Init(){ cFMWorkingObjectChanger<cBaseBehavior>::Init(); }
		virtual	void	Update(float e_fElpaseTime);
		virtual	void	Render();
	};
}

#endif