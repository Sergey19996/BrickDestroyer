//
// Created by Lenovo on 19.04.2025.
//

#include "GameObject.h"
GameObject::GameObject() : pos(0.0,0.0),size(32.0f,32.0f),rotate(0),texture(nullptr),IsSolid(false),bDestroyed(false),color(1.0f),velocity(0.0f,0.0f),prepared (false),IsDanger(false)
{

}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, float angle, Texture* texture,glm::vec4 Color,glm::vec2 Velocity) : pos(pos), size(size),rotate(angle), texture(texture),IsSolid(false),bDestroyed(false),color(Color),velocity(Velocity),prepared(false),IsDanger(false)
{

}

void GameObject::Draw(SpriteRenderer& renderer)
{
    renderer.DrawSprite(*texture, pos, size, rotate);
}
