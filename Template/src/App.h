#ifndef APP_CLASS_H
#define APP_CLASS_H

#include "Graphics/Window.h"
#include "Graphics/Renderer.h"
#include "Graphics/ImguiSetup.h"

class App
{
private:
    Window   window;
    Renderer renderer;
    Scene    scene;

    double timePrev = 0;
    double timeCrnt = 0;
    double timeDiff;

    void Render() { renderer.Render(scene); }
    void Update();

public:
    App();
    ~App();

    void RunFrame();
    bool ShouldClose() { return window.ShouldClose(); }
};

#endif