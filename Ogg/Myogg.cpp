#include "Myogg.h"
#pragma warning( disable : 4996 )
#ifdef WIN32
#include "Windows.h"
#endif
ogg_stream::~ogg_stream()
{
	release();
}

void ogg_stream::open(std::string path)
{
    int result;
    
    if(!(oggFile = fopen(path.c_str(), "rb")))
		throw std::string("Could not open Ogg file.");

    if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
    {
        fclose(oggFile);
        
		throw std::string("Could not open Ogg stream. ") + errorString(result);
    }

    vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);

    if(vorbisInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;
        
        
    alGenBuffers(2, buffers);
    check();
    alGenSources(1, &source);
    check();
    
    alSource3f(source, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(source, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(source, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (source, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (source, AL_SOURCE_RELATIVE, AL_TRUE      );
}




void ogg_stream::release()
{
    alSourceStop(source);
    empty();
    alDeleteSources(1, &source);
    check();
    alDeleteBuffers(1, buffers);
    check();

    ov_clear(&oggStream);
}




void ogg_stream::display()
{
#ifdef WIN32
	wchar_t	l_str[512];
	swprintf(l_str,512,L"verston%d\nchannels%d\nrate (hz) %d\nbitrate upper%d\nbitrate nominal%d\nbitrate lower %d\nbitrate window %d\nvendor%s\0",
		vorbisInfo->version,
		vorbisInfo->channels,
		vorbisInfo->rate,
		vorbisInfo->bitrate_upper,
		vorbisInfo->bitrate_nominal,
		vorbisInfo->bitrate_lower,
		vorbisInfo->bitrate_window,
		vorbisComment->vendor);
	OutputDebugString(l_str);
#else
	char	l_str[512];
	sprintf(l_str,"verston%d\nchannels%d\nrate (hz) %d\nbitrate upper%d\nbitrate nominal%d\nbitrate lower %d\nbitrate window %d\nvendor%s\0",
		vorbisInfo->version,
		vorbisInfo->channels,
		vorbisInfo->rate,
		vorbisInfo->bitrate_upper,
		vorbisInfo->bitrate_nominal,
		vorbisInfo->bitrate_lower,
		vorbisInfo->bitrate_window,
		vorbisComment->vendor);
	printf(l_str);
#endif
    //cout
    //    << "version         " << vorbisInfo->version         << "\n"
    //    << "channels        " << vorbisInfo->channels        << "\n"
    //    << "rate (hz)       " << vorbisInfo->rate            << "\n"
    //    << "bitrate upper   " << vorbisInfo->bitrate_upper   << "\n"
    //    << "bitrate nominal " << vorbisInfo->bitrate_nominal << "\n"
    //    << "bitrate lower   " << vorbisInfo->bitrate_lower   << "\n"
    //    << "bitrate window  " << vorbisInfo->bitrate_window  << "\n"
    //    << "\n"
    //    << "vendor " << vorbisComment->vendor << "\n";
    //    
    //for(int i = 0; i < vorbisComment->comments; i++)
    //    cout << "   " << vorbisComment->user_comments[i] << "\n";
    //    
    //cout << endl;
}




bool ogg_stream::playback()
{
    if(playing())
        return true;
        
    if(!stream(buffers[0]))
        return false;
        
    if(!stream(buffers[1]))
        return false;
    
    alSourceQueueBuffers(source, 2, buffers);
    alSourcePlay(source);
    
    return true;
}



bool ogg_stream::playing()
{
    ALenum state;

    alGetSourcei(source, AL_SOURCE_STATE, &state);
    
    return (state == AL_PLAYING);
}




bool ogg_stream::update()
{
    int processed;
    bool active = true;

    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

    while(processed--)
    {
        ALuint buffer;
        
        alSourceUnqueueBuffers(source, 1, &buffer);
        check();

        active = stream(buffer);

        alSourceQueueBuffers(source, 1, &buffer);
        check();
    }

    return active;
}




bool ogg_stream::stream(ALuint buffer)
{
    char pcm[BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;

    while(size < BUFFER_SIZE)
    {
        result = ov_read(&oggStream, pcm + size, BUFFER_SIZE - size, 0, 2, 1, &section);
    
        if(result > 0)
            size += result;
        else
            if(result < 0)
                throw errorString(result);
            else
                break;
    }
    
    if(size == 0)
        return false;
        
    alBufferData(buffer, format, pcm, size, vorbisInfo->rate);
    check();
    
    return true;
}




void ogg_stream::empty()
{
    int queued;
    
    alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
    
    while(queued--)
    {
        ALuint buffer;
    
        alSourceUnqueueBuffers(source, 1, &buffer);
        check();
    }
}




void ogg_stream::check()
{
	int error = alGetError();

	if(error != AL_NO_ERROR)
		throw std::string("OpenAL error was raised.");
}



std::string ogg_stream::errorString(int code)
{
    switch(code)
    {
        case OV_EREAD:
            return std::string("Read from media.");
        case OV_ENOTVORBIS:
            return std::string("Not Vorbis data.");
        case OV_EVERSION:
            return std::string("Vorbis version mismatch.");
        case OV_EBADHEADER:
            return std::string("Invalid Vorbis header.");
        case OV_EFAULT:
            return std::string("Internal logic fault (bug or heap/stack corruption.");
        default:
            return std::string("Unknown Ogg error.");
    }
}
