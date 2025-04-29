//
// Created by Lenovo on 19.04.2025.
//

#ifndef MY_APPLICATION_CRACKS_H
#define MY_APPLICATION_CRACKS_H

#include "Particles.h"
#include "../SpriteRenderer.h"

class CracksGenerator : public ParticleGenerator {
public:
    CracksGenerator(Shader shader, Texture tex, unsigned int amount,const unsigned int frames = 3,const float lifeTime = 0.5f);
    void Update(float dt) override;

    void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f)) override;
    void Draw(SpriteRenderer& renderer);

    void setSpriteScale(glm::vec2 Scale) { Size = Scale;}
private:
    float frames;
    int currFrame;
    glm::vec2 SpriteScale;
    glm::vec2 Size;
};
#endif //MY_APPLICATION_CRACKS_H
