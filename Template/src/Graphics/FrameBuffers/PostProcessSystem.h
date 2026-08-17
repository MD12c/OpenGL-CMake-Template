#ifndef POST_PROCESS_SYSTEM_CLASS_H
#define POST_PROCESS_SYSTEM_CLASS_H

#include "MSAABuffer.h"
#include "Framebuffer.h"
#include "Bloom.h"

class PostProcessSystem
{
public:
    MSAAbuffer  antiAlias;
    Framebuffer finalFrameBuffer;
    Bloom       bloom;

    PostProcessSystem();
    ~PostProcessSystem();

    void Begin();
    void End(int postShaderID, int blurShaderID);
};

#endif