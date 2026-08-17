#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "Scene.h"
#include "Cameras/Camera.h"
#include "FrameBuffers/PostProcessSystem.h"

class Renderer
{
private:
    PostProcessSystem postProcessSystem;

public:
    Renderer();
    ~Renderer();

    void Render(const Scene& scene);
};

#endif