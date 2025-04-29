//
// Created by Lenovo on 19.04.2025.
//
#include "Character.h"
#include "../ResourceManager.h"
#include "../Direction.hpp"


character::character() : GameObject(),charDirection(0)
{
}

character::character(glm::vec2 pos, glm::vec2 size,glm::vec2 velocity, float angle, Texture* texture, uint8_t charDirection) : GameObject(pos, size, angle, texture,glm::vec4(1.0f),velocity), charDirection(charDirection)
{
    Calc_UV_Direction(charDirection, velocity);
}

void character::setDirection(uint8_t direction, const glm::vec2& INITIAL_PLAYER_VELOCITY)
{
    charDirection = direction;

    Calc_UV_Direction(direction,INITIAL_PLAYER_VELOCITY);
}

void character::Draw(SpriteRenderer& renderer){

    Shader& Shader = ResourceManager::GetShader("sprite");
    float TexWidth = ResourceManager::GetTexture("Char").Width;
    float TexHeight = ResourceManager::GetTexture("Char").Height;
    Shader.use();

    Shader.setvec2("spriteScale", { 128.0f / TexWidth,128.0f / TexHeight });


    // переводим позицию с пикселей в диапозон от 0 до 1
    Shader.setvec2("uv", { UVCoords.x / TexWidth,UVCoords.y / TexHeight });
    //UVCoords
    Shader.setvec4("color", color);

    renderer.DrawSprite(*texture, pos, size, rotate);


}

void character::UVPulse()
{
    UVCoords.y = (UVCoords.y + 128.0f > ResourceManager::GetTexture("Char").Height) ? 0 : UVCoords.y + 128;

}


void character::update(float dt, const glm::ivec2& Squere, const glm::ivec2& offset)
{


    this->pos += velocity * dt;

    glm::vec2 Correction = { size.x / 4, size.y / 4 };

    if (this->pos.x <= offset.x - Correction.x) {
        this->pos.x = offset.x - Correction.x;
    }
    else if (this->pos.x + this->size.x - Correction.x >= offset.x + Squere.x)
    {
        this->pos.x = offset.x + Squere.x - this->size.x + Correction.x;
    }


    if (this->pos.y <= offset.y - Correction.y) {
        this->pos.y = offset.y - Correction.y;
    }
    else if (this->pos.y + this->size.y - Correction.y >= offset.y + Squere.y)
    {
        this->pos.y = offset.y + Squere.y - this->size.y + Correction.y;

    }

}

void character::Calc_UV_Direction(const uint8_t& direction, const glm::vec2& initialVelocity)
{
    switch (direction)
    {
        case Down:

            UVCoords.x = 0.0f;

            velocity.y = initialVelocity.x;
            velocity.x = initialVelocity.y;
            break;
        case Right:
            UVCoords.x = 128.0f;

            velocity.x = initialVelocity.x;
            velocity.y = initialVelocity.y;
            break;
        case Up:
            UVCoords.x = 128.0f * 2;

            velocity.y = -initialVelocity.x;
            velocity.x = initialVelocity.y;
            break;
            break;
        case Left:
            UVCoords.x = 128.0f * 3;

            velocity.x = -initialVelocity.x;
            velocity.y = initialVelocity.y;
            break;
        default:
            std::cout << "Error with Character::CalcUV "<<std::endl;
            break;
    }









}
