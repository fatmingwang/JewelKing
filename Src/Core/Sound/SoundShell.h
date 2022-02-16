#ifndef SOUND_SHELL_H
#define SOUND_SHELL_H
#ifdef WIN32
#include "../../../Include/AL/alut.h"
#else
#ifdef __IPHONE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "alut.h"
#endif
#endif
namespace FATMING_CORE
{
	//========================
	//whole sound have implement this,because we need quickly to sort,the sound buffer nad source data
	//it's quick tightly to combine with cSoundParser's static members
	//
	//========================
	class	cBasicSound:public NamedTypedObject
	{
		static	ALuint	m_suiTotalSoundIndex;		//how many sound we have created
		ALuint	m_uiIndex;							//to check the index is not same as destination
	protected:
		//sound data
		GET_SET_DEC(ALenum,m_iFormat,GetFormat,SetFormat);
		GET_SET_DEC(ALsizei,m_iSize,GetSize,SetSize);
		GET_SET_DEC(ALsizei,m_iFreq,GetFreq,SetFreq);
		//
		ALuint	m_uiBufferID;	//unique ID
		ALuint	m_uiSourceID;	//it could be dynamic
		ALuint	m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex;
		ALuint	m_uiUsingSourceIDIndex;	//while play is called,set source ID Index and set up value fo m_uiSourceID,it's related to ALuint*	cSoundParser::m_psuiSourceUsingIDIndex
		bool	m_bLoop;		//
		float	m_fTimeLength;	//
		bool	m_bPause;
		float	m_fVolume;
		GET_SET_DEC(int,m_iPiority,GetPiority,SetPiority);
	public:
		DEFINE_TYPE_INFO();
		cBasicSound();
		~cBasicSound();
		bool	IsUsing();//check is it still on using(occupied source ID)
		virtual	void	Pause(bool e_bPause = true);
		virtual	void	Rewind();
		virtual	void	AssignBufferData(void*e_pBuffer);
		virtual	void	Destroy();
		virtual	void	OpenFile(char*e_strileName){}
		virtual	void	Play(bool e_bPlay);
		virtual	bool	IsPlay();
		virtual	void	SetLoop(bool e_bLoop);
		virtual	bool	IsLoop();
		//iphone support?
		virtual	void	SetVolume(float e_fVolume);
		float			GetVolume(){return m_fVolume;}
		static char*    GetFileNameWithExtension(cBasicSound*e_pBasicSound);
		ALenum			PerSampleBitAndChannelToOpenALFormat(int e_iBit,int e_iChannel);
		void check();
	};
}
#endif