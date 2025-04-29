//
// Created by Lenovo on 25.04.2025.
//

#include "ui.h"
#include "../graphics/ResourceManager.h"




void ui::Draw(SpriteRenderer &renderer) {
    Shader& Shader = ResourceManager::GetShader("sprite");
    float TexWidth = texture ->Width;
    float TexHeight = texture ->Height;
    Shader.use();






        Shader.setvec2("spriteScale", { rect.UVScale.x / TexWidth,rect.UVScale.y / TexHeight });
// переводим позицию с пикселей в диапозон от 0 до 1
        Shader.setvec2("uv", { rect.UVPos.x / TexWidth,rect.UVPos.y / TexHeight });
        //UVCoords
        if(changeColor)
        Shader.setvec4("color",  {rect.Use,rect.Use,rect.Use,1.0f});
        else
            Shader.setvec4("color",  {1.0f,1.0f,1.0f,1.0f});

        renderer.DrawSprite(*texture, {rect.x,rect.y}, {rect.w,rect.h}, 0);




}

bool ui::click(const glm::vec2 &pos) {

    return  rect.Use = rect.contains(pos.x,pos.y) ?  true : false;
}

void ui::resetClick() {
rect.Use = false;
}



ui_Arrow::ui_Arrow(const float & Width, const float &Height, charDirection direction): ui(Width,Height) {

    dir = direction;
    switch (direction) {
        case charDirection::Down:
           rect =  { 0.049f * Width, 0.570f * Height,
                     0.157f * Width, 0.169f * Height ,
                     {140.0f,152.0f},{293.0f,140.0f},
                     {{0.0f,0.515f * Height},{0.25f * Width,0.485f * Height}}
                        };
            break;
        case charDirection::Right:
           rect = { 0.901f * Width, 0.364f * Height,
                    0.169f * Height, 0.157f * Width,
                    {439.0f,0.0f},{140.0f,292.0f},
                    {{0.875f * Width,0.25f * Height},{0.125f* Width,0.75f * Height}}
           };
            break;
        case charDirection::Up:
          rect = { 0.049f * Width, 0.294f * Height,
                   0.157f * Width, 0.169f * Height  ,
                   {140.0f,0.0f},{293.0f,140.0f},
                  {{0,0},{0.25f * Width,0.485f * Height}}
          };
            break;
        case charDirection::Left:
         rect = {   0.771f * Width, 0.364f * Height,
                    0.169f * Height , 0.157f * Width ,
                    {0.0f,0.0f},{140.0f,292.0f},
                 {{0.75f * Width,0.25f * Height},{0.125f* Width,0.75f * Height}}

         };

            break;
        default:
            break;

    }

   texture = &ResourceManager::GetTexture("input");
}



ui_BackGround::ui_BackGround(const float &Width, const float &Height) : ui(Width, Height) {

    texture = &ResourceManager::GetTexture("MainBack");
    float texW = texture->Width / Width * 0.5f ;
//float texH = texture->Height / Height * 0.5f;
    rect = {   0.5f * Width - texW * Width, 0.0f,
               0.5f * Width ,  Height ,
               {0.0f,0.0f},
               {texture->Width,texture->Height},
               {{0.0f,0.0f},{0.0f,0.0f}},
               true
    };






}

ui_BackCharacter::ui_BackCharacter(const float &Width, const float &Height) : ui(Width, Height) {
    texture = &ResourceManager::GetTexture("MainChar");
    float texW = texture->Width / Width * 0.5f ;
float texH = texture->Height / Height * 0.5f;

    Texture* texture2 = &ResourceManager::GetTexture("MainBack");
    float texW2 = texture2->Width / Width * 0.5f ;
    texW2 =  0.5f * Width - texW2 * Width;




    rect = {   texW2 + 0.106f * Width , 0.319f * Height,
               0.241f * Width ,  0.556f * Height ,
               {0.0f,0.0f},
               {texture->Width,texture->Height},
               {{0.0f,0.0f},{0.0f,0.0f}},
               true
    };


}

ui_START::ui_START(const float &Width, const float &Height) : ui_Button(Width, Height) {
    texture = &ResourceManager::GetTexture("MainStart");
    float texW = texture->Width / Width * 0.5f ;
    float texH = texture->Height / Height * 0.5f;


    Texture* texture2 = &ResourceManager::GetTexture("MainBack");
    float texW2 = texture2->Width / Width * 0.5f ;
    texW2 =  0.5f * Width - texW2 * Width;



    rect = {   texW2 + 0.123f * Width  , 0.738f * Height,
               0.218f * Width , 0.174f * Height ,
               {0.0f,0.0f},
               {texture->Width,texture->Height},
               {{0.4f * Width,0.75f * Height},{0.2f * Width,0.2f * Height}},
               true
    };
changeColor = false;

}

ui_Credits::ui_Credits(const float &Width, const float &Height) : ui_Button(Width, Height){
    texture = &ResourceManager::GetTexture("MainCredits");
    float texW = texture->Width / Width * 0.5f ;
    float texH = texture->Height / Height * 0.5f;


    Texture* texture2 = &ResourceManager::GetTexture("MainBack");
    float texW2 = texture2->Width / Width * 0.5f ;
    texW2 =  0.5f * Width - texW2 * Width;



    rect = {   texW2 + 0.332f * Width  , 0.0f,
               0.157f * Width , 0.097f * Height ,
               {0.0f,0.0f},
               {texture->Width,texture->Height},
               {{texW2 + 0.332f * Width,0.0f},{0.2f * Width,0.174f * Height}},
               true
    };
    changeColor = false;
    state = GameState::GAME_WIN;
}

ui_GO::ui_GO(const float &Width, const float &Height) : ui_Button(Width,Height) {
    rect = {   0.4f * Width, 0.75f * Height,
               0.2f * Width , 0.2f * Height ,
               {0.0f,0.0f},
               {232.0f,124.0f},
               {{0.4f * Width,0.75f * Height},{0.2f * Width,0.2f * Height}}
    };
    texture = &ResourceManager::GetTexture("Go");
}

ui_Button::ui_Button(const float &Width, const float &Height) : ui(Width, Height),state(GameState::GAME_ACTIVE)  {

}
