#ifndef POST_PROCESS_SYSTEM_CLASS_H
#define POST_PROCESS_SYSTEM_CLASS_H

#include "MSAABuffer.h"
#include "Framebuffer.h"
#include "Bloom.h"
#include "LUT.h"

class PostProcessSystem
{
public:
    MSAAbuffer  antiAlias;
    Framebuffer finalFrameBuffer;
    Bloom       bloom;
    LUT         lut;

    PostProcessSystem();
    ~PostProcessSystem();

    void Begin();
    void End(int postShaderID, int blurShaderID);
};

#endif