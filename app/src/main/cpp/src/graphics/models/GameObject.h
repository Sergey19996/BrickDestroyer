//
// Created by Lenovo on 19.04.2025.
//

#ifndef MY_APPLICATION_GAMEOBJECT_H
#define MY_APPLICATION_GAMEOBJECT_H

#include <glm/glm.hpp>
#include "../SpriteRenderer.h"

class GameObject {
public:

    glm::vec2 pos;
    glm::vec2 size;
    glm::vec2 velocity;
    glm::vec4 color;
    float rotate;
    bool bDestroyed;
    bool IsSolid;
    bool prepared;
    bool IsDanger;

    GameObject();

    GameObject(glm::vec2 pos, glm::vec2 size, float angle, Texture *texture,
               glm::vec4 color = glm::vec4(1.0f), glm::vec2 velocity = glm::vec2{0.0f, 0.0f});

    Texture *texture;


    virtual void Draw(SpriteRenderer &renderer);

};
#endif //MY_APPLICATION_GAMEOBJECT_H
