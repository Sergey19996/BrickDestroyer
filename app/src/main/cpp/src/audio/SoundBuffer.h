//
// Created by Lenovo on 20.04.2025.
//

#ifndef MY_APPLICATION_SOUNDBUFFER_H
#define MY_APPLICATION_SOUNDBUFFER_H

#include <AL/al.h>
#include <vector>
#include <sndfile.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <game-activity/GameActivity.h>
class SoundBuffer {
public:
    static SoundBuffer* get();


    ALuint addSoundEffect(AAsset* asset_, GameActivity* activity);
    bool removeSoundEffect(const ALuint& buffer);


private:
    SoundBuffer();
    ~SoundBuffer();
    std::vector<ALuint> p_SoundEffectBuffers;
    GameActivity* activity;
    AAsset* asset = nullptr;
    SF_INFO p_Sfinfo;
};


#endif //MY_APPLICATION_SOUNDBUFFER_H
