//
// Created by Lenovo on 20.04.2025.
//
#include "SoundBuffer.h"
#include <sndfile.h>  // для декода  тоолько звука
#include <inttypes.h>  // поддержка  uint8,16,32
#include <AL/alext.h>
#include "../graphics/ResourceManager.h"
SoundBuffer* SoundBuffer::get()
{
    static SoundBuffer* buffer = new SoundBuffer();
    return buffer;
}

ALuint SoundBuffer::addSoundEffect(AAsset* asset_, GameActivity* activity)
{
    this->activity = activity;
    asset = asset_;

    // 1. Open using virtual I/O
    SF_VIRTUAL_IO vio;
    vio.get_filelen = ResourceManager::get_filelen;
    vio.seek = ResourceManager::seek;
    vio.read = ResourceManager::read;
    vio.tell = ResourceManager::tell;

    SNDFILE* sndFile = sf_open_virtual(&vio, SFM_READ, &p_Sfinfo, asset);
    if (!sndFile) {
        fprintf(stderr, "Could not open audio asset: %s\n", sf_strerror(sndFile));
        return 0;
    }

    // 2. Determine format
    ALenum format = AL_NONE;
    if (p_Sfinfo.channels == 1)
        format = AL_FORMAT_MONO16;
    else if (p_Sfinfo.channels == 2)
        format = AL_FORMAT_STEREO16;
    else if (p_Sfinfo.channels == 3 &&
             sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
        format = AL_FORMAT_BFORMAT2D_16;

    if (!format) {
        fprintf(stderr, "Unsupported channel count: %d\n", p_Sfinfo.channels);
        sf_close(sndFile);
        return 0;
    }

    // 3. Read audio data
    sf_count_t numFrames = p_Sfinfo.frames;
    short* membuf = static_cast<short*>(malloc(numFrames * p_Sfinfo.channels * sizeof(short)));
    sf_count_t framesRead = sf_read_short(sndFile, membuf, numFrames * p_Sfinfo.channels);

    if (framesRead < 1) {
        fprintf(stderr, "Failed to read audio data: %" PRId64 "\n", framesRead);
        free(membuf);
        sf_close(sndFile);
        return 0;
    }

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, membuf, framesRead * sizeof(short), p_Sfinfo.samplerate);

    free(membuf);
    sf_close(sndFile);

    // 4. Check OpenAL error
    ALenum err = alGetError();
    if (err != AL_NO_ERROR) {
        fprintf(stderr, "OpenAL error: %s\n", alGetString(err));
        if (buffer && alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        return 0;
    }

    p_SoundEffectBuffers.push_back(buffer);
    return buffer;
}

bool SoundBuffer::removeSoundEffect(const ALuint& buffer)
{
    auto it = p_SoundEffectBuffers.begin();
    while (it != p_SoundEffectBuffers.end())
    {
        if (*it == buffer) {
            alDeleteBuffers(1, &*it);
            it = p_SoundEffectBuffers.erase(it);
            return true;
        }
        else
        {
            ++it;
        }

    }


    return false;
}

SoundBuffer::SoundBuffer()
{
    p_SoundEffectBuffers.clear();
}

SoundBuffer::~SoundBuffer()
{
    alDeleteBuffers(p_SoundEffectBuffers.size(), p_SoundEffectBuffers.data());
    p_SoundEffectBuffers.clear();
}
