#ifndef APP_CLASS_H
#define APP_CLASS_H

#include "Globals.h"
#include "Graphics/Window.h"
#include "Graphics/Renderer.h"

class Camera;

class App
{
private:
    glfwPointers glfwPtr;
    Window       window;
    Renderer     renderer;
    Scene        scene;

    double timePrev = 0;
    double timeCrnt = 0;
    double timeDiff;

    void Render() { renderer.Render(scene); }
    void Update();
    void setResizePointers();

public:
    App();
    ~App();

    void RunFrame();
    bool ShouldClose() { return window.ShouldClose(); }
};

#endif