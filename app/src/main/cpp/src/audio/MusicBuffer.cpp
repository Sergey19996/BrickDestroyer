//
// Created by Lenovo on 20.04.2025.
//
#include "MusicBuffer.h"
#include <cstddef>
#include <AL/alext.h>
#include <malloc.h>
#include "../graphics/ResourceManager.h"
#include <jni.h>
#include <android/native_activity.h>
#include <android/log.h>
void MusicBuffer::Play()
{
    ALsizei i;

    // clear any al errors
    alGetError();

    /* Rewind the source position and clear the buffer queue */
    alSourceRewind(p_Source);
    alSourcei(p_Source, AL_BUFFER, 0);

    /* Fill the buffer queue */
    for (i = 0; i < NUM_BUFFERS; i++)
    {
        /* Get some data to give it to the buffer */
        sf_count_t slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
        if (slen < 1) break;

        slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
        alBufferData(p_Buffers[i], p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
    }
    if (alGetError() != AL_NO_ERROR)
    {
        throw("Error buffering for playback");
    }

    /* Now queue and start playback! */
    alSourceQueueBuffers(p_Source, i, p_Buffers);
    alSourcePlay(p_Source);
    if (alGetError() != AL_NO_ERROR)
    {
        throw("Error starting playback");
    }


}

//void MusicBuffer::Pause()
//{
//}

//void MusicBuffer::Stop()
//{
//}

void MusicBuffer::UpdateBufferStream()
{
    ALint processed, state;

    // clear error
    alGetError();
    /* Get relevant source info */
    alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
    alGetSourcei(p_Source, AL_BUFFERS_PROCESSED, &processed);
    if (alGetError() != AL_NO_ERROR)
    {
        throw("error checking music source state");
    }

    /* Unqueue and handle each processed buffer */
    while (processed > 0)
    {
        ALuint bufid;
        sf_count_t slen;

        alSourceUnqueueBuffers(p_Source, 1, &bufid);
        processed--;

        /* Read the next chunk of data, refill the buffer, and queue it
         * back on the source */
        slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
        if (slen > 0)
        {
            slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
            alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen,
                         p_Sfinfo.samplerate);
            alSourceQueueBuffers(p_Source, 1, &bufid);
        }
        if (alGetError() != AL_NO_ERROR)
        {
            throw("error buffering music data");
        }
    }

    /* Make sure the source hasn't underrun */
    if (state != AL_PLAYING && state != AL_PAUSED)
    {
        ALint queued;

        /* If no buffers are queued, playback is finished */
        alGetSourcei(p_Source, AL_BUFFERS_QUEUED, &queued);
        if (queued == 0)
            return;

        alSourcePlay(p_Source);
        if (alGetError() != AL_NO_ERROR)
        {
            throw("error restarting music playback");
        }
    }

}

ALint MusicBuffer::getSource()
{
    return p_Source;
}

MusicBuffer::MusicBuffer(AAsset* asset_, GameActivity* activity) {
    alGenSources(1, &p_Source);
    alGenBuffers(NUM_BUFFERS, p_Buffers);

   asset = asset_;

   SF_VIRTUAL_IO vio;
   vio.get_filelen = ResourceManager::get_filelen;
   vio.seek = ResourceManager::seek;
   vio.read = ResourceManager::read;
   vio.tell = ResourceManager::tell;

   p_SndFile = sf_open_virtual(&vio,SFM_READ,&p_Sfinfo,asset);
   if(!p_SndFile)
       throw ("couldn't open virtual music file from assets");

    initialization();
    this->activity = activity;
    SetVoolumeControlToMusic( activity);

}

MusicBuffer::MusicBuffer(const char* filename)
{
    alGenSources(1, &p_Source);
    alGenBuffers(NUM_BUFFERS, p_Buffers);

    std::size_t frame_size;

    p_SndFile = sf_open(filename, SFM_READ, &p_Sfinfo);
    if (!p_SndFile)
    {
        throw("could not open provided music file -- check path");
    }
    initialization();


}

MusicBuffer::~MusicBuffer()
{
    alDeleteSources(1, &p_Source);

    if (p_SndFile)
        sf_close(p_SndFile);

    p_SndFile = nullptr;

    free(p_Membuf);

    alDeleteBuffers(NUM_BUFFERS, p_Buffers);

}

void MusicBuffer::initialization() {
    /* Get the sound format, and figure out the OpenAL format */
    std::size_t frame_size;
    if (p_Sfinfo.channels == 1)
        p_Format = AL_FORMAT_MONO16;
    else if (p_Sfinfo.channels == 2)
        p_Format = AL_FORMAT_STEREO16;
    else if (p_Sfinfo.channels == 3)
    {
        if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            p_Format = AL_FORMAT_BFORMAT2D_16;
    }
    else if (p_Sfinfo.channels == 4)
    {
        if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            p_Format = AL_FORMAT_BFORMAT3D_16;
    }
    if (!p_Format)
    {
        sf_close(p_SndFile);
        p_SndFile = NULL;
        throw("Unsupported channel count from file");
    }

    frame_size = ((size_t)BUFFER_SAMPLES * (size_t)p_Sfinfo.channels) * sizeof(short);
    p_Membuf = static_cast<short*>(malloc(frame_size));

}

void MusicBuffer::SetVoolumeControlToMusic(GameActivity *activity) {
    // 1) Привязать hardware‑кнопки к AudioManager.STREAM_MUSIC
    JNIEnv* env;
    activity->vm->AttachCurrentThread(&env, nullptr);
    jclass actCls = env->GetObjectClass(activity->javaGameActivity);
    jmethodID mid = env->GetMethodID(actCls, "setVolumeControlStream", "(I)V");
    env->CallVoidMethod(activity->javaGameActivity, mid, /*STREAM_MUSIC=*/3);
    activity->vm->DetachCurrentThread();

}

void MusicBuffer::UpdateVolume() {
    JNIEnv* env;
    activity->vm->AttachCurrentThread(&env, nullptr);

    // 1) find AudioManager class
    jclass audioMgrCls = env->FindClass("android/media/AudioManager");
    // 2) getContext.getSystemService("audio")
    jclass ctxCls = env->GetObjectClass(activity->javaGameActivity);
    jmethodID gs = env->GetMethodID(ctxCls, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jstring svc = env->NewStringUTF("audio");
    jobject audioMgr = env->CallObjectMethod(activity->javaGameActivity, gs, svc);

    // 3) getStreamMaxVolume(STREAM_MUSIC)
    jmethodID gsmv = env->GetMethodID(audioMgrCls, "getStreamMaxVolume", "(I)I");
    int maxVol = env->CallIntMethod(audioMgr, gsmv, /*STREAM_MUSIC=*/3);

    // 4) getStreamVolume(STREAM_MUSIC)
    jmethodID gsv = env->GetMethodID(audioMgrCls, "getStreamVolume", "(I)I");
    int currVol = env->CallIntMethod(audioMgr, gsv, /*STREAM_MUSIC=*/3);
    // 5) apply to OpenAL (normalize to 0.0-1.0 range)
    float gain = float(currVol) / float(maxVol);
    alSourcef(p_Source, AL_GAIN, gain);

    __android_log_print(ANDROID_LOG_INFO, "Music", "CurrentVolume=%d/%d, gain=%.2f",
                        currVol, maxVol, gain);

    activity->vm->DetachCurrentThread();
}

void MusicBuffer::upSound() {
    JNIEnv* env;
    activity->vm->AttachCurrentThread(&env, nullptr);

    jclass audioMgrCls = env->FindClass("android/media/AudioManager");
    jclass ctxCls = env->GetObjectClass(activity->javaGameActivity);
    jmethodID gs = env->GetMethodID(ctxCls, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jstring svc = env->NewStringUTF("audio");
    jobject audioMgr = env->CallObjectMethod(activity->javaGameActivity, gs, svc);

    jmethodID getVol = env->GetMethodID(audioMgrCls, "getStreamVolume", "(I)I");
    jmethodID getMax = env->GetMethodID(audioMgrCls, "getStreamMaxVolume", "(I)I");
    jmethodID setVol = env->GetMethodID(audioMgrCls, "setStreamVolume", "(III)V");

    int curr = env->CallIntMethod(audioMgr, getVol, 3);     // STREAM_MUSIC
    int max = env->CallIntMethod(audioMgr, getMax, 3);

    int newVol = std::min(curr + 10, max);
    env->CallVoidMethod(audioMgr, setVol, 3, newVol, 0);

    activity->vm->DetachCurrentThread();
}

void MusicBuffer::downSound() {
    JNIEnv* env;
    activity->vm->AttachCurrentThread(&env, nullptr);

    jclass audioMgrCls = env->FindClass("android/media/AudioManager");
    jclass ctxCls = env->GetObjectClass(activity->javaGameActivity);
    jmethodID gs = env->GetMethodID(ctxCls, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jstring svc = env->NewStringUTF("audio");
    jobject audioMgr = env->CallObjectMethod(activity->javaGameActivity, gs, svc);

    jmethodID getVol = env->GetMethodID(audioMgrCls, "getStreamVolume", "(I)I");
    jmethodID getMax = env->GetMethodID(audioMgrCls, "getStreamMaxVolume", "(I)I");
    jmethodID setVol = env->GetMethodID(audioMgrCls, "setStreamVolume", "(III)V");

    int curr = env->CallIntMethod(audioMgr, getVol, 3);     // STREAM_MUSIC
    int max = env->CallIntMethod(audioMgr, getMax, 3);

    int newVol = std::min(curr - 10, max);
    env->CallVoidMethod(audioMgr, setVol, 3, newVol, 0);

    activity->vm->DetachCurrentThread();
}
