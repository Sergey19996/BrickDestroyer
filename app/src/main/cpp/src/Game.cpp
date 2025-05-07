#include "Game.h"
#include "graphics/ResourceManager.h"
#include "graphics/SpriteRenderer.h"
#include "graphics/Effects/Particles.h"

#include "graphics/Direction.hpp"

#include "graphics/text/textRenderer.h"
#include "sstream"

#include "audio/soundSource.h"
#include "audio/soundDevice.h"
#include "audio/SoundBuffer.h"
#include "audio/MusicBuffer.h"
#include "AndroidOut.h"

#include <game-activity/native_app_glue/android_native_app_glue.h>


unsigned int Game::RECORD = 0;
unsigned int Game::POINTS = 0;

glm::vec2 CHAR_SIZE = {0.053f,0.102f };
glm::vec2 BRICK_SIZE = {0.043f,0.092f};
glm::vec2 SQUERE_SIZE = { 0.474f,0.462f };
const glm::vec2 SQUERE_Offset_RATIO = { 0.262f, 0.268f};

character* player;
ParticleGenerator* particles;
TextRenderer* text;
GameLevel* gameLevel;



SoundDevice* soundDevice; //singleton
SoundSource Speaker;
MusicBuffer* music;
uint32_t Sound_BrickDestroy;
//bool::Game::Keys[GLFW_KEY_LAST] = { 0 };  //GLFW_KEY_LAST это константа, которая задаёт максимальное количество клавиш, распознаваемых GLFW.
//bool Game::KeysProcessed[GLFW_KEY_LAST] = { 0 };

// Initial velocity of the player paddle
const glm::vec2 INITIAL_PLAYER_VELOCITY(150.0f,0.0f);
glm::vec2 CURRENT_PLAYER_VELOCITY(0.0f, 0.0f);

const float timeClamp = 0.05f;
const unsigned int TEXTSIZE = 48;


Game::Game(android_app* App_,float width, float height) : pApp(App_), Width(width), Height(height)
{
    Init();
    States[GAME_ACTIVE] = new GameActiveState();
    States[GAME_MENU]  = new GameMenuState();
    States[GAME_WIN]   = new GameWinState();
    State = States[GAME_ACTIVE];

}

Game::~Game()
{
    delete renderer;
    delete waterRenderer;
    delete particles;
    delete player;
    delete gameLevel;
    delete text;
    delete music;
    for (int i = 0; i < 3; ++i) {
        delete States[i];
    }

    CHAR_SIZE = {0.053f,0.102f };
    BRICK_SIZE = {0.043f,0.092f};
    SQUERE_SIZE = { 0.474f,0.462f };




}

void Game::Init()
{
    // 1) Инициализируем наш менеджер ресурсов

    BRICK_SIZE = {BRICK_SIZE.x * Width, BRICK_SIZE.y * Height};
    SQUERE_SIZE = {SQUERE_SIZE.x * Width, SQUERE_SIZE.y * Height};
    offset = { SQUERE_Offset_RATIO.x * Width, SQUERE_Offset_RATIO.y * Height };
    CURRENT_PLAYER_VELOCITY = INITIAL_PLAYER_VELOCITY * 2.0f;

    std::string path = pApp->activity->internalDataPath;
     saveGamePath = path + "/saveGame.bin";
     saveRecordPath = path + "/saveRecord.bin";

// Загружаем прогресс
    if (!loadGame(saveData, saveGamePath)) {
        saveGame(saveData, saveGamePath);  // Если не удалось загрузить, сохраняем
    }

    if (!loadGame(RECORD, saveRecordPath)) {
        saveGame(RECORD, saveRecordPath);  // Если не удалось загрузить, сохраняем
    }

    //Sound init
    ResourceManager::Init(pApp->activity->assetManager);

    // Sound
    soundDevice = SoundDevice::get();
    Sound_BrickDestroy = SoundBuffer::get()->addSoundEffect(ResourceManager::OpenAsset("audio/destroy.wav"), pApp->activity);
    Speaker.generate();


    //Music
    music = new MusicBuffer(ResourceManager::OpenAsset("audio/Main_Music.wav"),pApp->activity);
    music->Play();

    ResourceManager::LoadShader( "shaders/vertexShader.glsl", "shaders/fragmentShader.glsl", "sprite");
    ResourceManager::LoadShader( "shaders/particleVS.glsl", "shaders/particleFS.glsl", "particle");
    ResourceManager::LoadShader( "shaders/waterVS.glsl", "shaders/waterFS.glsl", "water");



    ResourceManager::LoadTexture("textures/Plates_Clear.png", "plates");
    ResourceManager::LoadTexture("textures/Character.png", "Char");
    ResourceManager::LoadTexture("textures/particle.png", "Stones");
    ResourceManager::LoadTexture("textures/Plates_Cracking.png", "Cracks");
    ResourceManager::LoadTexture("textures/BackGround.png", "background");
    ResourceManager::LoadTexture("textures/input.png","input");
    ResourceManager::LoadTexture("textures/Go.png","Go");
    ResourceManager::LoadTexture("textures/MainMenuBackGround.png","MainBack");
    ResourceManager::LoadTexture("textures/MainMenuChar.png", "MainChar");
    ResourceManager::LoadTexture("textures/MainMenuCredits.png","MainCredits");
    ResourceManager::LoadTexture("textures/MainMenuStart.png","MainStart");
    ResourceManager::LoadTexture("textures/CreditsBack.png","Credits");
    ResourceManager::LoadTexture("textures/Water.png", "water");
    ResourceManager::LoadTexture("textures/Heart.png",  "heart");


    renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));  // передаем для плейна, который берёт тайл в 128
    waterRenderer = new SpriteRenderer(ResourceManager::GetShader("water"));


    gameLevel = new GameLevel();
    gameLevel->BrickDestroyed =  [&]() { // созданный слот в gamelevel был инициалищирован в game и будет вызван у себя
        Speaker.Play(Sound_BrickDestroy, 0.25f);
    };


    gameLevel->Generate(&ResourceManager::GetTexture("plates"), BRICK_SIZE, offset, SQUERE_SIZE,saveData.Level);
    particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("Stones"), 100);


    glm::mat4 ortoMatrix = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);


    //configure shaders
    ResourceManager::GetShader("sprite").use().setInt("texture0", 0);
    ResourceManager::GetShader("sprite").setMat4("projection", ortoMatrix);
    ResourceManager::GetShader("particle").use().setInt("texture0", 0);
    ResourceManager::GetShader("particle").setMat4("projection", ortoMatrix);
    ResourceManager::GetShader("water").use().setInt("texture0", 0);
    ResourceManager::GetShader("water").setMat4("projection", ortoMatrix);


    CHAR_SIZE = {CHAR_SIZE.x * Width, CHAR_SIZE.y * Height};

    player = new character(offset-CHAR_SIZE / 4.0f, CHAR_SIZE, CURRENT_PLAYER_VELOCITY , 0, &ResourceManager::GetTexture("Char"),charDirection::Right);

    text = new TextRenderer(this->Width, this->Height);
    text->Load("fonts/PressStart2P-Regular.ttf", TEXTSIZE);


}




void Game::Update(float dt){

    if (this->State == States[GAME_ACTIVE]){

        DoCollisions();

        AnimationTimer(dt);
        player->update(dt, SQUERE_SIZE, offset);
        particles->Update(dt);
        gameLevel->Update(dt);


        if (gameLevel->IsCompleted()) {
            this->ResetLevel();
            this->ResetPlayer();
            SwitchState(GAME_WIN);
            saveData.Level++;

            gameLevel->Generate(&ResourceManager::GetTexture("plates"), BRICK_SIZE, offset, SQUERE_SIZE, saveData.Level);

        }

        if (POINTS > RECORD)
            RECORD = POINTS;

    }
}

void Game::Render(){




    music->UpdateBufferStream();
    music->UpdateVolume();

    ResourceManager::GetShader("sprite").use();
    ResourceManager::GetShader("sprite").setvec2("spriteScale", { 1.0f,1.0f });
    ResourceManager::GetShader("sprite").setvec2("uv", { 0.0f ,0.0f });
    ResourceManager::GetShader("sprite").setvec4("color", {1.0f,1.0f,1.0f,1.0f});
    ResourceManager::GetShader("sprite").setBool("isWhite" , false);
    renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height), 0.0f);

    ResourceManager::GetShader("water").use();
    ResourceManager::GetShader("water").setFloat("time",getTime() * 0.1f);

    gameLevel->Draw(*renderer, *waterRenderer);

    particles->Draw();
    player->Draw(*renderer);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //std::cout << x <<"\t" << y << std::endl;



    if (this->State == States[GAME_MENU]) {
        std::string sMenu = "MENU";
        text->RenderText(sMenu, Width / 2.0f - (sMenu.size() * TEXTSIZE / 2.0f) + 2, Height / 2.0f - sMenu.size() * TEXTSIZE / 2.0f + 2, 1.0f, { 0.0f,0.0f,0.0f });
        text->RenderText(sMenu, Width / 2.0f - (sMenu.size() * TEXTSIZE / 2.0f), Height / 2.0f - sMenu.size() * TEXTSIZE / 2.0f , 1.0f);
    }
    else  if (this->State == States[GAME_WIN]) {
        std::string sWind = "WIN";
        text->RenderText(sWind, Width / 2.0f - sWind.size() * TEXTSIZE / 2.0 + 2, Height / 2.0f - sWind.size() * TEXTSIZE / 2.0 + 2, 1.0f, { 0.0f,0.0f,0.0f });
        text->RenderText(sWind, Width / 2.0f - sWind.size() * TEXTSIZE / 2.0, Height / 2.0f - sWind.size() * TEXTSIZE / 2.0, 1.0f);

    }
    else{
        std::stringstream ss, sp,sr, sh; ss << saveData.Level;
        text->RenderText("Level:" + ss.str(), Width / 2.0f - SQUERE_SIZE.x / 2.0f + 2, Height / 2.0f - SQUERE_SIZE.y / 2.0f - TEXTSIZE * 2.0f + 2, 0.5f, { 0.0f,0.0f,0.0f });
        text->RenderText("Level:" + ss.str(), Width / 2.0f - SQUERE_SIZE.x / 2.0f, Height / 2.0f-SQUERE_SIZE.y / 2.0f - TEXTSIZE * 2.0f, 0.5f);
        sp << POINTS;
        text->RenderText("Points:" + sp.str(), Width / 2.0f + sp.str().size() * TEXTSIZE + 2, Height / 2.0f - SQUERE_SIZE.y / 2.0f - TEXTSIZE * 2.0f + 2, 0.5f, { 0.0f,0.0f,0.0f });
        text->RenderText("Points:" + sp.str(), Width / 2.0f + sp.str().size() * TEXTSIZE, Height / 2.0f - SQUERE_SIZE.y / 2.0f - TEXTSIZE * 2.0f, 0.5f);
        sr << RECORD;
        text->RenderText("RECORD:" + sr.str(), Width / 2.0f + sp.str().size() * TEXTSIZE + 2, Height / 2.0f + SQUERE_SIZE.y / 2.0f + TEXTSIZE * 2.0f + 2, 0.5f, { 0.0f,0.0f,0.0f });
        text->RenderText("RECORD:" + sr.str(), Width / 2.0f + sp.str().size() * TEXTSIZE, Height / 2.0f + SQUERE_SIZE.y / 2.0f + TEXTSIZE * 2.0f, 0.5f);
        sh << saveData.currLife;
        text->RenderText("X " + sh.str(), Width / 2.0f - sh.str().size() * TEXTSIZE + 2, Height / 2.0f - SQUERE_SIZE.y / 2.0f - TEXTSIZE * 2.0f + 2, 0.5f,{0.0f,0.0f,0.0f});
        text->RenderText("X " + sh.str(), Width / 2.0f - sh.str().size() * TEXTSIZE, Height / 2.0f - SQUERE_SIZE.y / 2.0f - TEXTSIZE * 2.0f, 0.5f);




    }







}

void Game::CharDeath(){
    //dead
    if (saveData.currLife > 0) {
        saveData.currLife--;




        unsigned int LoadedRecord = 0;
        if (loadGame(LoadedRecord, saveRecordPath)) // если файл есть то мы подгружаем старый прогресс
        {
            if (RECORD > LoadedRecord) // если текущий рекорд больше подгруженого
            {
                saveGame(RECORD,  saveRecordPath);  // то мы

            }
        }

    }
    else
    {

        saveData.Level = 1;
        saveData.currLife = saveData.MaxLife;
        unsigned int LoadedRecord = 0;

        if (loadGame(LoadedRecord, saveRecordPath)) // если файл есть то мы подгружаем старый прогресс
        {
            if (RECORD > LoadedRecord) // если текущий рекорд больше подгруженого
            {
                saveGame(RECORD,  saveRecordPath);  // то мы

            }
        }


    }

    this->ResetLevel();
    this->ResetPlayer();
    SwitchState(GAME_MENU);
    POINTS = 0;

    gameLevel->Generate(&ResourceManager::GetTexture("plates"), BRICK_SIZE, offset, SQUERE_SIZE, saveData.Level);
}

void Game::ResetLevel()
{
    particles->reset(true);
}

void Game::ResetPlayer()
{
    offset = { Width / 2 - SQUERE_SIZE.x / 2,Height / 2 - SQUERE_SIZE.y / 2 };
    glm::vec2 CharSize = { 48.0f,48.0f };
    player->pos = offset - CharSize / 4.0f;
    player->velocity = { CURRENT_PLAYER_VELOCITY.x * saveData.Level,CURRENT_PLAYER_VELOCITY.y * saveData.Level };
    player->rotate = 0;
    player->setDirection(charDirection::Right, CURRENT_PLAYER_VELOCITY);
}



void Game::AnimationTimer(float dt){


    timer += dt;

    if (timer > timeClamp) {

        timer -= timer;
        player->UVPulse();
        particles->Spawn(*player, 2, glm::vec2(player->size.x / 2.0f - 5, player->size.y / 2.0f - 5));

    }


}

void Game::DoCollisions()
{
    for (GameObject& box : gameLevel->Bricks) {

        bool bCollision = checkCollision(*player, box); // checking on collision with box

        if (!box.bDestroyed) {   // when box not bDestroyed


            if (bCollision == true){



                if (!box.IsSolid) {

                    //box.bDestroyed = true;
                    if (!box.prepared)
                    {

                        gameLevel->PreparateDestroy(&box);
                        POINTS += 100;

                    }
                }

                if (box.IsDanger)
                    CharDeath();


            }





        }
        else
        {
            //dead
            if (bCollision == true) {
                CharDeath();



            }
        }


    }

}

bool Game::checkCollision(character& player, GameObject& box)
{


    //get center point circle first
    glm::vec2 center(player.pos + player.size / 2.0f);
    //Calculate AABB info
    glm::vec2 aabb_half_extens(box.size / 2.0f);
    glm::vec2 aabb_center(box.pos + aabb_half_extens);
    glm::vec2 difference = center - aabb_center;

    glm::vec2 clmaped = glm::clamp(difference, -aabb_half_extens, aabb_half_extens); //nearest point in local space  -aabb, +aabb
    glm::vec2 closest = aabb_center + clmaped; // nearest point in world

    difference = closest - center; // вектор от сентра к ближайшей точк

    if (glm::length(difference) <= 0.01) {
        return true;



    }
    else
    {
        return false;
    }




    return false;
}

void Game::setDirection(charDirection direction) {
    switch (direction) {
        case charDirection::Down:
            player->setDirection(charDirection::Down, CURRENT_PLAYER_VELOCITY);
        break;
        case charDirection::Right:
            player->setDirection(charDirection::Right, CURRENT_PLAYER_VELOCITY);
            break;
        case charDirection::Up:
            player->setDirection(charDirection::Up, CURRENT_PLAYER_VELOCITY);
            break;
        case charDirection::Left:
            player->setDirection(charDirection::Left, CURRENT_PLAYER_VELOCITY);

            break;
        default:
            break;

    }
    //    if (player->pos.x >= 0.0f) {
            //		player->pos.x -= velocity;
  //          player->setDirection(charDirection::Left, CURRENT_PLAYER_VELOCITY);

 //       }



    }

void Game::SwitchState(GameState state) {
    State = States[state];
    onStateChanged(state);
}

void Game::SoundUp() {
music->upSound();
}

void Game::SoundDown() {
music->downSound();
}

double Game::getTime() {
    using namespace std::chrono;
    static auto start = steady_clock::now();
    auto now = steady_clock::now();
    return duration<double>(now - start).count();
}





//  if (this->Keys[GLFW_KEY_D])
   // {
   //     if (player->pos.x <= this->Width - player->size.x)
   //     {
            //	player->pos.x += velocity;
   //         player->setDirection(charDirection::Right, CURRENT_PLAYER_VELOCITY);

   //     }


 //   }

 //   if (this->Keys[GLFW_KEY_W])
 //   {
    //    if (player->pos.y >= 0.0f) {
            //		player->pos.y -= velocity;
   //         player->setDirection(charDirection::Up, CURRENT_PLAYER_VELOCITY);
///
  //      }



 //   }
 //   if (this->Keys[GLFW_KEY_S])
  //  {
    //    if (player->pos.y <= this->Height - player->size.y)
     //   {
            //		player->pos.y += velocity;
     //       player->setDirection(charDirection::Down, CURRENT_PLAYER_VELOCITY);

   //     }


   // }





	