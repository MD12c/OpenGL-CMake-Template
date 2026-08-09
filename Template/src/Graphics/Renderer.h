#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "Scene.h"
#include "FrameBuffers/MSAABuffer.h"
#include "FrameBuffers/PostProcess.h"
#include "Cameras/Camera.h"

class Renderer
{
private:
    MSAAbuffer  antiAlias;
    Framebuffer postProcess;

    double timePrev = 0;
    double timeCrnt = 0;
    double timeDiff;

public:
    Renderer();
    ~Renderer();

    void Render(const Scene& scene);
};

#endif