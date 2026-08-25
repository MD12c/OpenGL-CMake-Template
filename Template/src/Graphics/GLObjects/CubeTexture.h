#ifndef CUBE_TEXTURE_CLASS_H
#define CUBE_TEXTURE_CLASS_H

#include "glad/glad.h"

#include "Globals.h"

class CubeTexture
{
public:
    GLuint ID;

    CubeTexture();
    CubeTexture(const CubeTexture&)            = delete;
    CubeTexture& operator=(const CubeTexture&) = delete;
    CubeTexture& operator=(CubeTexture&& other) noexcept;
    CubeTexture(CubeTexture&& other) noexcept;
    ~CubeTexture();

    void LoadTexture(int index, const void* data, GLenum type, GLenum formatL, GLenum formatR, int w, int h) const;
    void AllocTexture(int index, GLenum type, GLenum formatL, GLenum formatR, int w, int h) const;
    void Draw(GLuint unit) const;
    void Bind(GLuint unit) const;
    void Unbind(GLuint unit) const;
    void texUnit(ShaderID shaderID, const std::string& uniform) const;
};

#endif