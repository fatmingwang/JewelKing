#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "OpenAlWavFile.h"
#include "OpenAlOgg.h"
#include "../XML/XMLLoader.h"
//=============================
//current do not support streaming music
//=============================
//enum	eSoundDataType
//{
//	eSDT_WAV = 0,
//	eSDT_STREAMING,
//	eSDT_NONE
//};
namespace FATMING_CORE
{
	class	cSoundParser:public cObjectListByName<cBasicSound>,public ISAXCallback
	{
	    void    ActiveOpenAL();
	#ifdef __IPHONE__
		ALCdevice*	m_pDevice;
		ALCcontext*	m_pContext;	
	#endif
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
		void	ProcessStreamingData();
		void	ProcessStaticData();
		void	ProcessRootDirectory();
		char	m_strDestDirectory[TEMP_SIZE];
		float	m_fVolume;
	public:
		DEFINE_TYPE_INFO();
		//below for sort source ID,because the source hardearw is limited,so we have to assign the source dymanic
		static	int		m_siNumSourceID;
		static	ALuint*	m_psuiSourceID;				//whole source ID,as many as m_siNumSoundManager
		static	ALuint*	m_psuiSourceUsingIDIndex;	//as many as m_siNumSoundManager
		static	int		m_siNumSoundManager;
		//I have call alutInit (0,0); in the construction while sound manager is first time to create
		cSoundParser();
		~cSoundParser();
		bool	AddStaticSound(char*e_strFileName);
		bool	AddStreamingSound(char*e_strFileName);
		void	Export(char*e_strFileName);
		//for 0 to 1
		void	SetVolume(float e_fVolume);
		float	GetVolume(){ return m_fVolume; }
		void	Pause(bool e_bPause);	//all sound pause
		void	Stop();					//all sound stop
		inline	void	SetSpeed(float e_fSpeed){ alSpeedOfSound(343.3f*e_fSpeed);  }//The default speed of sound value is 343.3.
	};
}
#endif
//
//OpenAL gain means volume. So you have to change the AL_GAIN state to change the volume of a source (you can't change the volume of a buffer as far as I have seen, and it seems strange why you would).
//
//So
//
//// To set the volume of the source to max
//alSourcef(sourceID, AL_GAIN, 1.0f);
//
//// To set it to min
//alSourcef(sourceID, AL_GAIN, 0.0f);
//
//// And halfway 
//alSourcef(sourceID, AL_GAIN, 0.5f);