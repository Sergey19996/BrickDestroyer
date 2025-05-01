//
// Created by Lenovo on 19.04.2025.
//

#include "Particles.h"


unsigned int lastUsedParticle = 0;

ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, unsigned int amount, bool IsRandCoord): shader(shader), texture(texture),amount(amount)
{
    this->init(IsRandCoord);
}
ParticleGenerator::~ParticleGenerator() {
    glDeleteVertexArrays(1, &VAO);
    // Если ты сохраняешь VBO и EBO в члены класса — освободи и их:
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void ParticleGenerator::Update(float dt)
{




    for (unsigned int i = 0; i < this->amount; i++) {

        Particle& p = this->particles[i];
        p.Life -= dt;

        if (p.Life > 0) {
            p.Pos -= p.Velocity * dt;
            p.Color.a = p.Life / p.InitialLife;
            p.Scale = {p.InitialScale * p.Color.a,p.InitialScale*p.Color.a};
            p.Velocity = { p.InitialVel.x * p.Color.a ,p.InitialVel.y * p.Color.a };
        }
    }



}

void ParticleGenerator::Draw(){



    // Pixels_Uv_coords в диапозоне от 0 до макс width и height тектсруы




    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();






    glm::vec2 spriteScale = { 64.0f / texture.Width, 64.0f / texture.Height };
    this->shader.setvec2("spriteScale", { spriteScale.x,spriteScale.y }); // 64 - size of sprite
    for (Particle particle : this->particles) {

        if (particle.Life > 0.0f) {
            this->shader.setvec2("offset", particle.Pos);
            this->shader.setvec2("uv", particle.Coord);
            this->shader.setvec4("color", particle.Color);
            this->shader.setFloat("scale", particle.Scale.x);


            glActiveTexture(GL_TEXTURE0);
            this->texture.Bind();



            glBindVertexArray(this->VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 6 indices glDrawArrays - когда без индексов
            glBindVertexArray(0);
        }
    }
    //reset blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Spawn( GameObject& object, unsigned int newParticles,glm::vec2 offset)
{
    for (unsigned int i = 0; i < newParticles; i++) {

        int unusedParticle = this->firstUnusedParticle();
        if (unusedParticle >= 0 )
            this->respawnParticle(this->particles[unusedParticle], object, offset);

    }
}

void ParticleGenerator::init(bool& isRandCoord)
{
    ////  VAO/VBO
    float vertices[] = {
            // vrtx         UV
            1.0f,  1.0f,		1,1,		 // top right    0
            1.0f,  0.0f,		1,0,			// bottom right 1
            0.0f,  1.0f,		0,1,			// top left     2
            // vrtx           UV
            0.0f, 0.0f, 		0,0,			// bottom left  3
    };
    unsigned int indices[] = {
            0,3,2,
            1,3,0
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);


    // // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // layout = 0  // говорим как читать данные с VBO
    glEnableVertexAttribArray(0);
    // Разбиндивание VAO (необязательно, но хорошая практика)
    glBindVertexArray(0);
    //
    float offsetUnit = 0.5f;
    for (unsigned int i = 0; i < this->amount; i++) {


        Particle particle;
        if (isRandCoord)
            particle.Coord = { (rand() % 2) * offsetUnit,(rand() % 2) * offsetUnit };

        this->particles.push_back(particle);

    }


}

int ParticleGenerator::firstUnusedParticle()
{

    for (unsigned int i = lastUsedParticle; i < this->amount; i++) {
        if (this->particles[i].Life <= 0) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (unsigned int i = 0; i < lastUsedParticle; i++) {

        if (this->particles[i].Life <= 0){
            lastUsedParticle = i;
            return i;
        }
    }

    lastUsedParticle = 0;
    return -1;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset){


    float random = ((rand() % 100) - 50) / 10.0f;  // -5 ->> +5
    float random_dot5 = rand() % 5 / 10.0f;

    particle.Pos = object.pos + offset ;
    particle.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    particle.InitialLife = 1 - random_dot5;
    particle.Life = particle.InitialLife;
    particle.InitialVel = { object.velocity.x * (1 - random_dot5)  ,object.velocity.y * (1 - random_dot5) };
    particle.Velocity = particle.InitialVel;
    particle.InitialScale = 1.0f + (rand() % 20) / 10;;
    particle.Scale = {particle.InitialScale,particle.InitialScale};
}

void ParticleGenerator::reset( bool isRandCoord){
    this->particles.clear();
    float offsetUnit = 0.5f;
    for (unsigned int i = 0; i < this->amount; i++) {


        Particle particle;
        if (isRandCoord)
            particle.Coord = { (rand() % 2) * offsetUnit,(rand() % 2) * offsetUnit };

        this->particles.push_back(particle);

    }

}