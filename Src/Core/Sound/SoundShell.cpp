#include "../stdafx.h"
#include "SoundShell.h"
#include "SoundManager.h"
#include "OpenAlOgg.h"
#include "OpenAlWavFile.h"
namespace FATMING_CORE
{
	const wchar_t*     cBasicSound::TypeID( L"cBasicSound" );

	ALuint	cBasicSound::m_suiTotalSoundIndex = 0;		//how many sound we have created
	cBasicSound::cBasicSound()
	{
		m_uiIndex = m_suiTotalSoundIndex++;
		m_bLoop = false;
		m_bPause = false;
		m_uiBufferID = m_uiSourceID = 0;
		m_fTimeLength = 0.f;
		m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex = -1;
		m_fVolume = 1.f;

		m_iFormat = -1;
		m_iSize = -1;
		m_iFreq = -1;
		m_iPiority = 0;
	}
	cBasicSound::~cBasicSound()
	{
		Destroy();
	}

	bool	cBasicSound::IsUsing()
	{
		if( m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex == -1 )//have had assign data to sound manager
			return false;
		//assert(cSoundParser::m_psuiSourceUsingIDIndex&&"sound manager is not existed!!!");
		if( cSoundParser::m_psuiSourceUsingIDIndex )
		if( cSoundParser::m_psuiSourceUsingIDIndex[m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex] == m_uiIndex )//the inde is correspond itself Index
			return true;
		return false;
	}


	void	cBasicSound::Rewind()
	{
		if( !this->IsUsing() )
			return;
		/* Rewind a Source (set playback postiton to beginning) */
		alSourceRewind( this->m_uiSourceID);
	}

	void	cBasicSound::AssignBufferData(void*e_pBuffer)
	{
		if( m_uiBufferID == 0 )
			alGenBuffers(1, &m_uiBufferID);
		alBufferData (m_uiBufferID, m_iFormat, e_pBuffer, m_iSize, m_iFreq);
		check();	
	}

	void	cBasicSound::Pause(bool e_bPause)
	{
		if( e_bPause )
		{
			if( !this->IsPlay() )
				return;
			m_bPause = true;
			/* Pause a Source */
			alSourcePause( this->m_uiSourceID );
		}
		else
		{
			if( m_bPause&&this->IsUsing() )
			{
				m_bPause = false;
				this->Play(true);
			}
		}
	}

	void	cBasicSound::Destroy()
	{
		if( m_uiBufferID == 0 )
			return;
		if(IsUsing())
		{
			alSourceStop(m_uiSourceID);
			alSourcei (m_uiSourceID, AL_BUFFER, AL_NONE);
		}
		check();
		alDeleteBuffers(1, &m_uiBufferID);
		m_bLoop = false;
		m_uiBufferID = m_uiSourceID = 0;
		check();
	}

	void cBasicSound::check()
	{
	#ifdef _DEBUG
		int error = alGetError();

		if(error != AL_NO_ERROR)
		{
			WCHAR*l_sterrorCode = 0;
			if( 0xA001 == error )
				l_sterrorCode = L"AL_INVALID_NAME";
			else
			if(error == 0xA002)
				l_sterrorCode = L"AL_INVALID_ENUM  File data type error??";
			else
			if(error == 0xA003)
				l_sterrorCode = L"AL_INVALID_VALUE";
			else
			if(error == 0xA004)
				l_sterrorCode = L"AL_INVALID_OPERATION";
			else
			if(error == 0xA005)
				l_sterrorCode = L"AL_OUT_OF_MEMORY";
			UT::ErrorMsg(this->GetName(),l_sterrorCode);
		}
	#endif
	}

	void	cBasicSound::Play(bool e_bPlay)
	{
		assert(cSoundParser::m_psuiSourceUsingIDIndex&&"forget to create sound manager??");
		if( this->IsUsing() )
		{
			if( !e_bPlay )
				alSourceStop (m_uiSourceID);
			else
			{
				alSourcePlay (m_uiSourceID);
				alSourcef(m_uiSourceID, AL_GAIN, m_fVolume);
			}
			return;
		}

		if( !e_bPlay )//not in using and want to stop the source
			return;

	#ifdef _DEBUG
		//chaeck for all source state,if all paused,the game design is not correct
		int	l_iNumPaused = 0;
		for( int i=0;i<cSoundParser::m_siNumSourceID;++i )
		{
			ALuint	l_uiSourceID = cSoundParser::m_psuiSourceID[i];
			ALenum l_iState;
			alGetSourcei(l_uiSourceID, AL_SOURCE_STATE, &l_iState); 
			if( AL_PAUSED == l_iState )
				l_iNumPaused++;
		}
		assert( l_iNumPaused != cSoundParser::m_siNumSourceID &&"whole source is paused there is no more source could be play" );
	#endif
		for( int i=0;i<cSoundParser::m_siNumSourceID;++i )
		{
			ALuint	l_uiSourceID = cSoundParser::m_psuiSourceID[i];
			ALenum l_iState;
			alGetSourcei(l_uiSourceID, AL_SOURCE_STATE, &l_iState); 
			//check which one is stop,if stop using it
			if( l_iState != AL_PLAYING&& AL_PAUSED != l_iState )
			{
				m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex = i;	//set current using index
				cSoundParser::m_psuiSourceUsingIDIndex[i] = m_uiIndex;		//set manage data for quick sort
				m_uiSourceID = l_uiSourceID;								//store the source ID we are going to using
				alSourcei( m_uiSourceID, AL_LOOPING, m_bLoop?AL_TRUE:AL_FALSE );
				alSourcei (m_uiSourceID, AL_BUFFER, m_uiBufferID);
				if( e_bPlay )
				{
					alSourcePlay (m_uiSourceID);
					alSourcef(m_uiSourceID, AL_GAIN, m_fVolume);
				}
				return;
			}
		}
	}

	void	cBasicSound::SetLoop(bool e_bLoop)
	{
		m_bLoop = e_bLoop;
		//if m_uiIndexFor_cSoundParser_m_psuiSourceUsingIDIndex have had setup, and sound manager data is coorespond
		if( this->IsUsing() )
		{//and it's playing
			ALenum l_iState;
			alGetSourcei(m_uiSourceID, AL_SOURCE_STATE, &l_iState); 	
			if( AL_PLAYING == l_iState )
			{
				alSourcei( m_uiSourceID, AL_LOOPING, m_bLoop?AL_TRUE:AL_FALSE );
			}
		}
	}

	bool	cBasicSound::IsLoop(){ return m_bLoop;  }

	bool	cBasicSound::IsPlay()
	{	
		if(this->IsUsing())
		{
			ALenum l_iState;
			alGetSourcei(m_uiSourceID, AL_SOURCE_STATE, &l_iState); 
			return (l_iState == AL_PLAYING); 
		}
		return false;
	}
	
	void	cBasicSound::SetVolume(float e_fVolume)
	{
		m_fVolume = e_fVolume;
		if(this->IsUsing())
			alSourcef(m_uiSourceID, AL_GAIN, e_fVolume);
	}

	char*   cBasicSound::GetFileNameWithExtension(cBasicSound*e_pBasicSound)
	{
	    std::wstring    l_str = e_pBasicSound->GetName();
	    if(e_pBasicSound->Type() == cOpanalWAV::TypeID )
	    {
	        l_str += L".wav";
	    }
	    else
	    //now it's ogg
	    {
	        l_str += L".ogg";
	    }
	    return UT::WcharToChar(l_str.c_str());
	}

	ALenum	cBasicSound::PerSampleBitAndChannelToOpenALFormat(int e_iBit,int e_iChannel)
	{
		ALenum	l_iFormat = AL_FORMAT_MONO16;
		if (e_iChannel==1)
		{
			l_iFormat = (e_iBit==4?alGetEnumValue("AL_FORMAT_MONO_IMA4"):(e_iBit==8?AL_FORMAT_MONO8:AL_FORMAT_MONO16));
		}
		else 
		if (e_iChannel==2)
		{
			l_iFormat = (e_iBit==4?alGetEnumValue("AL_FORMAT_STEREO_IMA4"):(e_iBit==8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16));
		}
		return l_iFormat;
	}
}