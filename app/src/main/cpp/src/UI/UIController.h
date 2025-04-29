//
// Created by Lenovo on 25.04.2025.
//

#ifndef MY_APPLICATION_UICONTROLLER_H
#define MY_APPLICATION_UICONTROLLER_H
#include "ui.h"
#include <vector>
#include "../graphics/SpriteRenderer.h"
#include "../gameStates.hpp"
#include  "../graphics/Direction.hpp"
#include "unordered_map"
#include "../Game.h"
class UIController {
public:
    std::function<void(charDirection)> onDirectionChanged;  // slot for change track
    std::function<void(GameState)> onGoButtonPressed; // slot for  switch button

    UIController( Game* state,const float Width, const float Height);

  void render();
  void handleClick(const glm::vec2& pos);

  void ResetClick();
  void SetNewState(GameState state);
;
    ~UIController();
void connectRender(SpriteRenderer* Renderer) {this->renderer = Renderer; };
private:
   // std::vector<ui> UiArrows;
   // std::vector<ui*> currentUI;   // fix in future
    std::vector<ui_Arrow*> arrows;
    std::vector<ui_Button*> buttons;
    std::vector<ui*> staticUi;
    SpriteRenderer* renderer;

std::unordered_map<std::string,ui*> UiMap;
    Game* game;

    glm::vec2 ScreenSize;
};


#endif //MY_APPLICATION_UICONTROLLER_H
