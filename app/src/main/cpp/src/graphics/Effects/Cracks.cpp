//
// Created by Lenovo on 19.04.2025.
//

#include "Cracks.h"

CracksGenerator::CracksGenerator(Shader shader, Texture tex, unsigned int amount, const unsigned int frames, const float lifeTime) : ParticleGenerator(shader,tex,amount,false)
{
    this->frames = lifeTime / frames;
    SpriteScale = { 128.0f / tex.Width,128.0f / tex.Height };
}

void CracksGenerator::Update(float dt){

    for (unsigned int i = 0; i < this->amount; i++) {

        Particle& p = this->particles[i];
        p.Life -= dt;

        if (p.Life > 0) {

            currFrame = (p.InitialLife - p.Life) / frames; // 0-->1 / 0.33
            p.Coord.x = (currFrame * 128.0f) / texture.Width; // кол0во кадро - 2

        }
    }
}

void CracksGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
    particle.Pos = object.pos + offset;
    particle.InitialLife = 0.5;
    particle.Life = particle.InitialLife;
    particle.Coord = { 0.0f,0.0f };
    particle.Scale = Size;


}

void CracksGenerator::Draw(SpriteRenderer& renderer)
{
    this->shader.use();


    for (Particle particle : this->particles) {

        if (particle.Life > 0.0f) {
            this->shader.setvec2("offset", particle.Pos);
            this->shader.setvec2("uv", particle.Coord);
            this->shader.setvec2("spriteScale", SpriteScale);

            renderer.DrawSprite(texture, particle.Pos, particle.Scale, 0 );

            //glActiveTexture(GL_TEXTURE0);
            //this->texture.Bind();



            //glBindVertexArray(this->VAO);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 6 indices glDrawArrays - когда без индексов
            //glBindVertexArray(0);
        }
    }
}
