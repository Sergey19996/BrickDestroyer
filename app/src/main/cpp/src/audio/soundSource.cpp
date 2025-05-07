//
// Created by Lenovo on 20.04.2025.
//
#include "soundSource.h"

SoundSource::SoundSource()
{
}

SoundSource::~SoundSource()
{
    alDeleteSources(1, &p_source);

}

void SoundSource::generate()
{
    alGenSources(1, &p_source);
    alSourcef(p_source, AL_PITCH, p_pitch);
    alSourcef(p_source, AL_PITCH, p_gain);
    alSource3f(p_source, AL_POSITION, p_position[0], p_position[1], p_position[2]);
    alSource3f(p_source, AL_VELOCITY, p_velocity[0], p_velocity[1], p_velocity[2]);
    alSourcei(p_source, AL_LOOPING, p_loopSound);
    alSourcei(p_source, AL_BUFFER, p_buffer);


}

void SoundSource::Play(const ALuint buffer_to_play,float volume){

    if (buffer_to_play != p_buffer) {
        p_buffer = buffer_to_play;
        alSourcei(p_source, AL_BUFFER, (ALint)p_buffer);

    }

    // Устанавливаем громкость
    alSourcef(p_source, AL_GAIN, volume);
    alSourcePlay(p_source);

}
