//
// Created by Lenovo on 20.04.2025.
//

#ifndef MY_APPLICATION_ENGINE_H
#define MY_APPLICATION_ENGINE_H
#include "Game.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>

class Engine {
public:
    Engine(android_app *pApp);

    ~Engine();

    void handleInput();

    void render();

private:
    void init();

    Game* BrickDestroyer;
    android_app* app_;


    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;
    EGLint width_;
    EGLint height_;
  //  io  inputController;
};


#endif //MY_APPLICATION_ENGINE_H
