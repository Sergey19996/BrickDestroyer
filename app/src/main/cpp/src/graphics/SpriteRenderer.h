#ifndef ANDROIDGLINVESTIGATIONS_RENDERER_H
#define ANDROIDGLINVESTIGATIONS_RENDERER_H

#include <EGL/egl.h>
#include <memory>

#include "rendering/Shader.h"
#include "rendering/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct android_app;


class SpriteRenderer {
public:

    SpriteRenderer(Shader& );
    ~SpriteRenderer();


    void DrawSprite(Texture& texture, glm::vec2 position,
                    glm::vec2 size = glm::vec2(32.0f, 32.0f), float rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f));




private:

    Shader shader;
    unsigned quadVAO;  // хранит вертексы квадрата


    void initRendererData();



};

#endif //ANDROIDGLINVESTIGATIONS_RENDERER_H