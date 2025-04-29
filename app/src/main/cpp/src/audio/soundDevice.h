//
// Created by Lenovo on 20.04.2025.
//

#ifndef MY_APPLICATION_SOUNDDEVICE_H
#define MY_APPLICATION_SOUNDDEVICE_H
#include "AL/alc.h"

class SoundDevice {

public:
    static SoundDevice* get();
    static void init();
private:
    SoundDevice();
    ~SoundDevice();

    ALCdevice* p_ALCDevice;
    ALCcontext* p_ALCContext;


};


#endif //MY_APPLICATION_SOUNDDEVICE_H
