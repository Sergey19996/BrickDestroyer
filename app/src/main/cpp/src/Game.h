//
// Created by Lenovo on 19.04.2025.
//

#ifndef MY_APPLICATION_GAME_H
#define MY_APPLICATION_GAME_H
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include "graphics/Level/GameLevel.h"
#include "graphics/models/Character.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "gameStates.hpp"
#include "graphics/Direction.hpp"

#include <jni.h>
#include <android/native_activity.h>
struct SaveData
{


    unsigned int MaxLife;
    unsigned int currLife;
    unsigned int Level;
    SaveData() : MaxLife(3), currLife(3),Level(1) {
        std::cout << "Hello" << std::endl;
    };

};


template<typename T>
void saveGame(const T& Data, const std::string& fileName) {
    std::ofstream outFile(fileName, std::ios::binary);
    if (outFile) {
        outFile.write(reinterpret_cast<const char*>(&Data), sizeof(T));
        outFile.close();
        std::cout << "GAME::SAVED " << std::endl;
    }
    else
    {
        std::cout << "ERROR::SAVED:can't find file for save " << std::endl;
    }


}


template<typename T>
bool loadGame(T& Data, const std::string& fileName) {
    std::ifstream inFile(fileName, std::ios::binary);
    if (inFile) {
        inFile.read(reinterpret_cast<char*>(&Data), sizeof(T));
        inFile.close();

        std::cout << "PROGRESS LOADED" << std::endl;
        return true;

    }
    else
    {
        std::cout << "ERROR::LOAD: can't open file for reading" << std::endl;
    }



    return false;
}


class Game {
public:

    std::function<void(GameState)> onStateChanged;
    Game( android_app*,float width, float height);

    ~Game();



    void Init();
    void Update(float dt);
    void Render();


    void CharDeath();

    void ResetLevel();
    void ResetPlayer();

    void setDirection(charDirection dir);

    SaveData& getSaveData() { return saveData; };
    IGameState* getStatePtr() { return State; };
    void SwitchState( GameState state);
    SpriteRenderer* getSpriteRenderer() { return  renderer; };


    void SoundUp();
    void SoundDown();

    double getTime();
private:

    const float Width, Height;
    glm::vec2 offset;

    float timer = 0.0f;
    SaveData saveData;

    android_app* pApp;
    static unsigned int RECORD;
    static unsigned int POINTS;
    //GameState State;
    IGameState* State;
   // UIController* ui;
    IGameState* States [3] ;
    void AnimationTimer(float dt);
    void DoCollisions();
    bool checkCollision(character& player, GameObject& box);
    SpriteRenderer* renderer;
    SpriteRenderer* waterRenderer;

    std::string saveGamePath = "/saveGame.bin";
    std::string saveRecordPath =  "/saveRecord.bin";

};


#endif //MY_APPLICATION_GAME_H
