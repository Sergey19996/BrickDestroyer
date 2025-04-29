//
// Created by Lenovo on 19.04.2025.
//

#ifndef MY_APPLICATION_GAMELEVEL_H
#define MY_APPLICATION_GAMELEVEL_H

#include "../models/GameObject.h"
#include <vector>
#include "../ResourceManager.h"
#include "../../algorithms/indicesGenerator.h"

class GameLevel {
public:
    std::vector<GameObject> Bricks;
    GameLevel();
    ~GameLevel();
    void Generate(Texture* texture,const glm::vec2& Brick_Size,const glm::vec2& offset, const glm::ivec2& Squere_Size, unsigned int WaterRoots);

    void Draw(SpriteRenderer& reder);
    void Update(float dt);

    bool IsCompleted();

    void PreparateDestroy(GameObject* Brick);

private:
    void init(std::vector<uint8_t>& indices,const int& row, const int& line, Texture* texture,const glm::ivec2& offset,const glm::vec2& BRICKSIZE);


    std::vector<glm::vec2> Pixels_UV_Coords;

    std::vector<GameObject*>MarkedBricks;
};


#endif //MY_APPLICATION_GAMELEVEL_H
