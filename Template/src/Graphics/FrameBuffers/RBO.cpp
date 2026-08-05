#include "RBO.h"
#include "Globals.h"

RBO::RBO(bool useMultisample)
{
    glGenRenderbuffers(1, &ID);
    glBindRenderbuffer(GL_RENDERBUFFER, ID);

    if (useMultisample)
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples, GL_DEPTH24_STENCIL8, width, height);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void RBO::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, ID);
}

void RBO::Unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RBO::Delete()
{
    glDeleteRenderbuffers(1, &ID);
}