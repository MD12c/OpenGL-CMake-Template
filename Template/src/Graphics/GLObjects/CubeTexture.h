#ifndef CUBE_TEXTURE_CLASS_H
#define CUBE_TEXTURE_CLASS_H

#include "glad/glad.h"

class CubeTexture
{
public:
    GLuint ID;
    GLuint unit;

    CubeTexture(GLuint unit);
    CubeTexture(const CubeTexture&)            = delete;
    CubeTexture& operator=(const CubeTexture&) = delete;
    CubeTexture& operator=(CubeTexture&& other) noexcept;
    CubeTexture(CubeTexture&& other) noexcept;
    ~CubeTexture();

    void LoadTexture(unsigned int index, const void* data, GLenum type, GLenum formatL, GLenum formatR, int w, int h) const;
    void AllocTexture(unsigned int index, GLenum type, GLenum formatL, GLenum formatR, int w, int h) const;
    void Draw() const;
    void Bind() const;
    void Unbind() const;
};

#endif