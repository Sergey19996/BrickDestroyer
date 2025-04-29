//
// Created by Lenovo on 20.04.2025.
//

#ifndef MY_APPLICATION_SOUNDBUFFER_H
#define MY_APPLICATION_SOUNDBUFFER_H

#include <AL/al.h>
#include <vector>

class SoundBuffer {
public:
    static SoundBuffer* get();


    ALuint addSoundEffect(const char* filename);
    bool removeSoundEffect(const ALuint& buffer);


private:
    SoundBuffer();
    ~SoundBuffer();
    std::vector<ALuint> p_SoundEffectBuffers;


};


#endif //MY_APPLICATION_SOUNDBUFFER_H
