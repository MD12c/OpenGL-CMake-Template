#include "RBO.h"
#include "Globals.h"

RBO::RBO(int numSamples, int width, int height, GLenum internalformat)
    : numSamples(numSamples)
{
    glGenRenderbuffers(1, &ID);
    glBindRenderbuffer(GL_RENDERBUFFER, ID);

    if (numSamples > 1)
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, numSamples, GL_DEPTH24_STENCIL8, width, height);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

RBO& RBO::operator=(RBO&& other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &ID);

        ID             = other.ID;
        numSamples = other.numSamples;
        other.ID       = 0;
    }

    return *this;
}

RBO::RBO(RBO&& other) noexcept
    : ID(other.ID), numSamples(numSamples)
{
    other.ID = 0;
}

RBO::~RBO()
{
    glDeleteBuffers(1, &ID);
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