//
// Created by Lenovo on 25.04.2025.
//

#include "UIController.h"

UIController::UIController(Game* game, const float Width, const float Height) : ScreenSize({Width,Height}),game(game)
{
    UiMap["Arrow_Down"] = new ui_Arrow (ScreenSize.x,ScreenSize.y,charDirection::Down);
    UiMap["Arrow_Right"] = new ui_Arrow (ScreenSize.x,ScreenSize.y,charDirection::Right );
    UiMap["Arrow_Up"] = new ui_Arrow (ScreenSize.x,ScreenSize.y,charDirection::Up );
    UiMap["Arrow_Left"] = new ui_Arrow (ScreenSize.x,ScreenSize.y,charDirection::Left);
    UiMap["Button_Go"] = new ui_GO(ScreenSize.x, ScreenSize.y);
    UiMap["Main_Back"] = new ui_BackGround(ScreenSize.x, ScreenSize.y);
    UiMap["Main_Char"] = new ui_BackCharacter(ScreenSize.x, ScreenSize.y);
    UiMap["Main_BTN_START"] = new ui_START(ScreenSize.x, ScreenSize.y);
    UiMap["Main_BTN_Credits"] = new ui_Credits(ScreenSize.x, ScreenSize.y);
   // UiArrows.push_back(ui_Arrow(ScreenSize.x,ScreenSize.y,charDirection::Down));
   // UiArrows.push_back(ui_Arrow(ScreenSize.x,ScreenSize.y,charDirection::Right));
   // UiArrows.push_back(ui_Arrow(ScreenSize.x,ScreenSize.y,charDirection::Up));
  //  UiArrows.push_back(ui_Arrow(ScreenSize.x,ScreenSize.y,charDirection::Left));

   // UIButtons.push_back(new ui_GO(ScreenSize.x, ScreenSize.y, states));
SetNewState(GameState::GAME_ACTIVE);
}

void UIController::render() {


            for(ui* Element :arrows){
                Element->Draw(*renderer);
            }
            for(ui* staticui : staticUi)
                staticui->Draw(*renderer);

            for(ui* Button : buttons)
                Button->Draw(*renderer);

}

void UIController::SetNewState(GameState state) {
    buttons.clear();
    arrows.clear();
    staticUi.clear();

    switch (state) {
        case GameState::GAME_ACTIVE:
         //   UiArrows.push_back(ui_Arrow(ScreenSize.x,ScreenSize.y,charDirection::Down));
         //   UiArrows.push_back(ui_Arrow(ScreenSize.x,ScreenSize.y,charDirection::Right));
         //   UiArrows.push_back(ui_Arrow(ScreenSize.x,ScreenSize.y,charDirection::Up));
         //   UiArrows.push_back(ui_Arrow(ScreenSize.x,ScreenSize.y,charDirection::Left));
            arrows.push_back(  dynamic_cast<ui_Arrow*>(UiMap["Arrow_Down"]) );
            arrows.push_back(  dynamic_cast<ui_Arrow*>(UiMap["Arrow_Right"]) );
            arrows.push_back(  dynamic_cast<ui_Arrow*>(UiMap["Arrow_Up"]) );
            arrows.push_back( dynamic_cast<ui_Arrow*>(UiMap["Arrow_Left"]) );

        break;
        case GAME_MENU:

            staticUi.push_back(  UiMap["Main_Back"] );
            staticUi.push_back(  UiMap["Main_Char"] );
            buttons.push_back(  dynamic_cast<ui_Button*> (UiMap["Main_BTN_START"]));
            buttons.push_back(  dynamic_cast<ui_Button*>(UiMap["Main_BTN_Credits"]));
            break;
        case GAME_WIN:
            buttons.push_back(  dynamic_cast<ui_Button*>(UiMap["Button_Go"]));
            break;

    }

}

void UIController::ResetClick() {

    for(ui_Arrow* Element : arrows)
        Element->resetClick();

    for(ui* Element : buttons)
        Element->resetClick();
}

UIController::~UIController() {
    for (auto& pair : UiMap) {
        delete pair.second;
    }
    UiMap.clear();

    // Не удаляем currentUI элементы, так как они принадлежат UiMap
    buttons.clear();
    arrows.clear();

}

void UIController::handleClick(const glm::vec2 &pos) {

    for (ui_Arrow* element : arrows) {
        if (element->click(pos)) {
                if (onDirectionChanged)   // in registrated  function true
                    onDirectionChanged(element->getDirection());



        }
    }
    for (ui_Button* element : buttons) {
        if (element->click(pos)) {
            // можно добавить другие события:
                if (onGoButtonPressed)
                    onGoButtonPressed(element->getState());


        }
    }



}
