#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include "Scene.h"
#include "Frustum/Frustum.h"
#include "FrameBuffers/PostProcessSystem.h"
#include "Models/Transform.h"

class Renderer
{
public:
    PostProcessSystem postProcessSystem;
    Frustum           frustum;

    std::shared_ptr<Texture> noTexture;

    Renderer();
    ~Renderer();

    void Render(const Scene& scene);
};

#endif