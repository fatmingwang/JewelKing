#ifndef OPENAL_WAV_FILE_H
#define OPENAL_WAV_FILE_H

#include "SoundShell.h"


namespace FATMING_CORE
{
	ALvoid GetWAVData(const ALbyte *file,ALenum *format,ALvoid **data,ALsizei *size,ALsizei *freq, ALboolean *loop);


	class	cOpanalWAV:public cBasicSound
	{
	public:
		DEFINE_TYPE_INFO();
		cOpanalWAV(){}
		cOpanalWAV(char*e_strileName);
		~cOpanalWAV();
		virtual	NamedTypedObject*	Clone(){ return 0; }
		virtual void	OpenFile(char*e_strileName);
	};
}

#endif