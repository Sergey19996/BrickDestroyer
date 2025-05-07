//
// Created by Lenovo on 20.04.2025.
//

#ifndef MY_APPLICATION_SOUNDSOURCE_H
#define MY_APPLICATION_SOUNDSOURCE_H
#include "AL/al.h"
class SoundSource {

public:
    SoundSource();
    ~SoundSource();
    void generate();
    void Play(const ALuint buffer_to_play, float volume);


private:
    ALuint p_source;
    float p_pitch = 1.0f;
    float p_gain = 1.0f;
    float p_position[3] = { 0,0,0 };
    float p_velocity[3] = { 0,0,0 };
    bool p_loopSound = false;
    ALuint p_buffer = 0;

};

#endif //MY_APPLICATION_SOUNDSOURCE_H
