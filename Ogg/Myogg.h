#ifndef MY_OGG_H
#define MY_OGG_H

#include <string>
#include <iostream>

#include "../Include/AL/alut.h"
#include "ogg/ogg.h"
#include "vorbis/vorbisfile.h"



#define BUFFER_SIZE (4096 * 4)



class ogg_stream
{
    public:
		virtual	~ogg_stream();
		void open(std::string path);
        void release();
        void display();
        bool playback();
        bool playing();
        bool update();

    protected:

        bool stream(ALuint buffer);
        void empty();
        void check();
		std::string errorString(int code);

    private:

        FILE*           oggFile;
        OggVorbis_File  oggStream;
        vorbis_info*    vorbisInfo;
        vorbis_comment* vorbisComment;

        ALuint buffers[2];
        ALuint source;
        ALenum format;
};

//class	SoundManager
//{
//	std::vector<ogg_stream*>	SoundList;
//public:
//
//};

#endif // __ogg_h__
