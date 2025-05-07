//
// Created by Lenovo on 25.04.2025.
//

#ifndef MY_APPLICATION_UI_H
#define MY_APPLICATION_UI_H
#include <glm/glm.hpp>
#include "../graphics/SpriteRenderer.h"

#include  "../graphics/Direction.hpp"
#include "../gameStates.hpp"

struct Rect {
    float x, y, w, h;
    bool contains(float px, float py) const {

        return px >= CollisionData.first.x && px <= CollisionData.first.x + CollisionData.second.x &&
               py >= CollisionData.first.y && py <= CollisionData.first.y + CollisionData.second.y;
    }
    glm::vec2 UVPos;
    glm::vec2 UVScale;
    std::pair<glm::vec2,glm::vec2> CollisionData;
    bool Use = false;
};


class ui {
public:
    ui(const float& Width, const float& Height) {};
   virtual bool click(const glm::vec2& pos);
    void Draw(SpriteRenderer& renderer);
    void resetClick();
protected:

    bool changeColor = true;
   Rect rect;
    Texture *texture;
};

class ui_Arrow : public ui{
public:
    ui_Arrow(const float& WIdth, const float& Height,charDirection direction);
    charDirection& getDirection() { return dir;};
private:
    charDirection dir;

};

class ui_Button : public  ui{
public:
    ui_Button(const float& Width, const float& Height);
    GameState& getState() { return  state;};
protected:
    GameState state;

};



class ui_BackGround : public  ui{
public:
ui_BackGround(const float& Width, const float& Height,Texture* texture);

};

class ui_BackCharacter : public  ui{
public:
    ui_BackCharacter(const float& Width, const float& Height);

};

class ui_Health : public  ui{
public:
    ui_Health(const float& Width, const float& Height);


};
class ui_GO : public  ui_Button{
public:
    ui_GO(const float& Width, const float& Height);

protected:
    GameState state;

};
class ui_START : public  ui_Button{
public:
    ui_START(const float& Width, const float& Height);

};
class ui_Credits : public  ui_Button{
public:
    ui_Credits(const float& Width, const float& Height);

};



#endif //MY_APPLICATION_UI_H
