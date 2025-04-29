//
// Created by Lenovo on 25.04.2025.
//

#ifndef MY_APPLICATION_GAMESTATES_HPP
#define MY_APPLICATION_GAMESTATES_HPP

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class IGameState{
public:
    IGameState() {};
    virtual  void Handle() = 0;
    virtual ~ IGameState() = default;
    GameState getState() const { return state; }
protected:
GameState state;
};

class GameActiveState: public  IGameState{
public:
    GameActiveState()   { state = GameState::GAME_ACTIVE;}
    void Handle() override{


    }

};

class GameMenuState: public  IGameState{
public:
    GameMenuState() { state = GameState::GAME_MENU;}
    void Handle() override{


    }

};
class GameWinState: public  IGameState{
public:
    GameWinState() { state = GameState::GAME_WIN;}
    void Handle() override{


    }

};
#endif //MY_APPLICATION_GAMESTATES_HPP
