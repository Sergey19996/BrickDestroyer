//
// Created by Lenovo on 20.04.2025.
//

#ifndef MY_APPLICATION_TEXTRENDERER_H
#define MY_APPLICATION_TEXTRENDERER_H

#include <map>

#include "glm/glm.hpp"

#include "../rendering/Texture.h"
#include "../rendering/Shader.h"



struct Character
{
    unsigned int TextureId;
    glm::vec2 Size;
    glm::vec2 Bearing;
    unsigned int Advance; //horizontal offset to next glyph
};


class TextRenderer {
public:
    std::map<char, Character> Characters;

    Shader TextShader;

    TextRenderer(unsigned int width, unsigned int height);

    void Load(std::string font, unsigned int fontSize);

    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:
    unsigned int VAO, VBO;
};


#endif //MY_APPLICATION_TEXTRENDERER_H
