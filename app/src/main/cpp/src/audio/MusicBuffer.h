//
// Created by Lenovo on 20.04.2025.
//

#ifndef MY_APPLICATION_MUSICBUFFER_H
#define MY_APPLICATION_MUSICBUFFER_H
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "AL/al.h"
#include <sndfile.h>
class MusicBuffer
{
public:
    void Play();
    //void Pause();
    //void Stop();

    void UpdateBufferStream();

    ALint getSource();

    MusicBuffer(const char* filename);
    MusicBuffer(AAsset* asset);

    ~MusicBuffer();
private:

    void initialization();

    ALuint p_Source;
    static const int BUFFER_SAMPLES = 8192;
    static const int NUM_BUFFERS = 4;
    ALuint p_Buffers[NUM_BUFFERS];
    SNDFILE* p_SndFile;
    SF_INFO p_Sfinfo;
    short* p_Membuf;
    ALenum p_Format;

    AAsset* asset = nullptr;

    MusicBuffer() = delete;
};



#endif //MY_APPLICATION_MUSICBUFFER_H
