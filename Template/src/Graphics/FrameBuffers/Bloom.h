#ifndef BLOOM_CLASS_H
#define BLOOM_CLASS_H

#include "Framebuffer.h"

class Bloom
{
private:
    Framebuffer buffer1;
    Framebuffer buffer2;
    
public:
    Bloom();
    
    GLuint BlurPass(GLuint brightTexture, ShaderIDs shaderID, int numPasses);
    void Resize(int w, int h);
};

#endif