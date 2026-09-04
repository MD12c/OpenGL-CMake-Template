#ifndef BLOOM_CLASS_H
#define BLOOM_CLASS_H

#include "Framebuffer.h"

class Bloom
{
private:
    Framebuffer buffer1;
    Framebuffer buffer2;

public:
    Bloom(int width, int height);

    Texture& BlurPass(const Texture& brightTexture, ShaderID shaderID, int numPasses);
    void     Resize(int w, int h);
};

#endif