#include "../stdafx.h"
#include "SoundManager.h"
#include "../XML/AtgXmlWriter.h"
namespace FATMING_CORE
{
	const wchar_t*     cSoundParser::TypeID( L"cSoundParser" );

	ALuint*	cSoundParser::m_psuiSourceID = 0;//whole source ID
	ALuint*	cSoundParser::m_psuiSourceUsingIDIndex = 0;	//as many as m_siNumSoundManager
	int		cSoundParser::m_siNumSourceID = 0;
	int		cSoundParser::m_siNumSoundManager = 0;
	void    cSoundParser::ActiveOpenAL()
	{
#ifndef __IPHONE__ 
		alutInit (0,0);
#endif

#ifdef __IPHONE__
		// Initialization
		m_pDevice = alcOpenDevice(NULL); // select the "preferred device"
		assert(m_pDevice);
		if (m_pDevice)
		{
			// use the device to make a context
			m_pContext = alcCreateContext( m_pDevice,NULL);
			// set my context to the currently active one
			alcMakeContextCurrent(m_pContext);
		}
#endif
		std::vector<ALuint>	AllSourceID;
		bool	l_bGo = true;
		while(l_bGo)
		{
			ALuint	l_SourceID = 0;
			alGenSources (1, &l_SourceID);
			if( alGetError() != AL_NO_ERROR )
			{
				l_bGo = false;
			}
			else
			{
				AllSourceID.push_back(l_SourceID);
			}
			if( AllSourceID.size()>128 )//assume maxiuma sound source is 128(depend on hardware)
			{
				l_bGo = false;
			}
		}
		UINT	l_uiNum = (UINT)AllSourceID.size();
		m_psuiSourceID = new ALuint[l_uiNum];
		m_psuiSourceUsingIDIndex = new ALuint[l_uiNum];
		m_siNumSourceID = l_uiNum;
		for( UINT i=0;i<l_uiNum;++i )
		{
			m_psuiSourceID[i] = AllSourceID[i];
			m_psuiSourceUsingIDIndex[i] = -1;
		}
		alGetError();	
	}
	cSoundParser::cSoundParser()
	{
		m_siNumSoundManager++;
		m_fVolume = 1.f;
	#ifdef __IPHONE__
		assert(!m_psuiSourceID&&"iPhone openal just need one sound manager,because I am lazy to do more");
	#endif
		if( !m_psuiSourceID )
		{
            ActiveOpenAL();
		}
	}

	cSoundParser::~cSoundParser()
	{
		this->Destroy();
		m_siNumSoundManager--;
		if( m_siNumSoundManager == 0 )
		{
			for( int i=0;i<m_siNumSourceID;++i )
			{
				alSourceStop(m_psuiSourceID[i]);
				alDeleteSources(1, &m_psuiSourceID[i]);
				int error = alGetError();
				if(error != AL_NO_ERROR)
				{
					assert(0);
				}
			}
			SAFE_DELETE_ARRAY(m_psuiSourceID);
			SAFE_DELETE_ARRAY(m_psuiSourceUsingIDIndex);
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
				this->m_strErrorMsg += l_sterrorCode;
			}
	#endif
	#ifdef __IPHONE__
			// destroy the context
			alcDestroyContext(m_pContext);
			// close the device
			alcCloseDevice(m_pDevice);
	#else//win32,linux
			alutExit();
	#endif
		}
	}

	void	cSoundParser::SetVolume(float e_fVolume)
	{
		if(e_fVolume>1.f)
			e_fVolume = 1.f;
		if( e_fVolume<0.f )
			e_fVolume = 0.f;
		if(m_fVolume != e_fVolume)
		{
			alListenerf(AL_GAIN,e_fVolume);
			m_fVolume = e_fVolume;
			//for( int i=0;i<m_siNumSourceID;++i )
			//{
			//	alSourcef(m_psuiSourceID[i], AL_GAIN, e_fVolume);
			//}
		}
	}

	void	cSoundParser::Pause(bool e_bPause)
	{
		for( int i=0;i<this->Count();++i )
		{
			(*this)[i]->Pause(e_bPause);
		}
	}

	void	cSoundParser::Stop()
	{
		for( int i=0;i<this->Count();++i )
		{
			(*this)[i]->Play(false);
		}
	}


	bool	cSoundParser::AddStreamingSound(char*e_strFileName)
	{
		return true;
	}

	bool	cSoundParser::AddStaticSound(char*e_strFileName)
	{
		char*	l_extensionName = UT::GetFileExtensionName(e_strFileName);
		if(this->GetObject(UT::CharToWchar(UT::GetFileNameWithoutFullPath(e_strFileName))))
		    return false;
		if( !memcmp(l_extensionName,"wav",sizeof(char)*3) )
		{
	#ifdef __IPHONE__
			char	l_strApplePath[MAX_PATH];
			GetAppleBundelResourcePathByObjectPath(e_strFileName,l_strApplePath);
			cOpanalWAV*	l_pOpanalWAV = new cOpanalWAV(l_strApplePath);
	#else
			cOpanalWAV*	l_pOpanalWAV = new cOpanalWAV(e_strFileName);
	#endif
			if( wcslen(l_pOpanalWAV->GetName()) != 0 )
				if(!this->AddObject(l_pOpanalWAV))//such sound already existed
				{
					SAFE_DELETE(l_pOpanalWAV);
				}
				else
					return true;
		}
		else
		if( !memcmp(l_extensionName,"ogg",sizeof(char)*3) )
		{
	#ifdef __IPHONE__
			char	l_strApplePath[MAX_PATH];
			GetAppleBundelResourcePathByObjectPath(e_strFileName,l_strApplePath);
			cOpanalOgg*	l_pOpanalOgg = new cOpanalOgg(l_strApplePath);
	#else
			cOpanalOgg*	l_pOpanalOgg = new cOpanalOgg(e_strFileName);
	#endif
			if( wcslen(l_pOpanalOgg->GetName()) != 0 )
				if(!this->AddObject(l_pOpanalOgg))//such sound already existed
				{
					SAFE_DELETE(l_pOpanalOgg);
				}
				else
					return true;
		}
		else
		{
			this->m_strErrorMsg += UT::CharToWchar(e_strFileName);
			this->m_strErrorMsg += L" not support such format\n";
		}
		return false;
	}

	void	cSoundParser::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const WCHAR*	l_strName = e_pTiXmlElement->Value();
		if( !wcslen(l_strName) )
			return;
		COMPARE_NAME("Static")
			ProcessStaticData();
		else
		COMPARE_NAME("Streaming")
			ProcessStreamingData();
		else
		COMPARE_NAME("SoundData")
			ProcessRootDirectory();
	}

	void	cSoundParser::ProcessStreamingData()
	{
		PARSE_NAME_VALUE_START
			COMPARE_NAME("Name")
			{

			}
			else
			COMPARE_NAME("Loop")
			{

			}
		PARSE_NAME_VALUE_END
	}

	void	cSoundParser::ProcessRootDirectory()
	{
		PARSE_NAME_VALUE_START
			COMPARE_NAME("Root")
			{
				sprintf(this->m_strDestDirectory,"%s\0",UT::WcharToChar(l_strValue));
				//sprintf(this->m_strCurrentDirectory,"%s\0",l_strValue);
			}
		PARSE_NAME_VALUE_END
	}

	void	cSoundParser::ProcessStaticData()
	{
		char	l_strSoundName[TEMP_SIZE];
		bool	l_bLoop = false;
		int		l_iPiority = 0;
		PARSE_NAME_VALUE_START
			COMPARE_NAME("Name")
			{
				sprintf(l_strSoundName,"%s%s\0",this->m_strDestDirectory,UT::WcharToChar(l_strValue));
			}
			else
			COMPARE_NAME("Loop")
			{
				l_bLoop = _wtoi(l_strValue)?true:false;
			}
			else
			COMPARE_NAME("Piority")
			{
				l_iPiority = _wtoi(l_strValue);
			}
		PARSE_NAME_VALUE_END
		if(this->AddStaticSound(l_strSoundName))
		{
			if( l_bLoop )
				this->m_ObjectList[m_ObjectList.size()-1]->SetLoop(true);
			this->m_ObjectList[m_ObjectList.size()-1]->SetPiority(l_iPiority);
		}
	}


	void	cSoundParser::Export(char*e_strFileName)
	{
	#ifdef WIN32
		int	l_iNum = Count();
		ATG::XMLWriter	l_XMLWriter(e_strFileName);
		l_XMLWriter.StartElement("SoundData");
		l_XMLWriter.AddAttribute("Root","Sound/");
			for( int i=0;i<l_iNum;++i )
			{
				cBasicSound*l_pBasicSound = (*this)[i];
				const WCHAR*	l_strName = l_pBasicSound->GetName();
				bool	l_bLoop = l_pBasicSound->IsLoop();
				char	l_strFinalName[TEMP_SIZE];
				if(!wcscmp(l_pBasicSound->Type(),cOpanalOgg::TypeID))
				{
					sprintf(l_strFinalName,"%s.ogg",UT::WcharToChar(l_strName));
					//cOpanalOgg*l_pOpanalOgg = (cOpanalOgg*)l_pBasicSound;			
				}
				else
				if(!wcscmp(l_pBasicSound->Type(),cOpanalWAV::TypeID))
				{
					sprintf(l_strFinalName,"%s.wav",UT::WcharToChar(l_strName));
					//cOpanalWAV*l_pOpanalWAV = (cOpanalWAV*)l_pBasicSound;
				}
				l_XMLWriter.StartElement("Static");
				l_XMLWriter.AddAttribute("Name",l_strFinalName);
				l_XMLWriter.AddAttribute("Loop",l_bLoop);
				l_XMLWriter.EndElement();
			}
		//end SoundData
		l_XMLWriter.EndElement();
	#endif
	}
}