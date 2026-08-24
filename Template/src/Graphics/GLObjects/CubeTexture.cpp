#include "CubeTexture.h"

#include <iostream>

#include "../Shaders/ShaderManager.h"

CubeTexture::CubeTexture()
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubeTexture& CubeTexture::operator=(CubeTexture&& other) noexcept
{
    if (this != &other)
    {
        glDeleteTextures(1, &ID);

        ID       = other.ID;
        other.ID = 0;
    }

    return *this;
}

CubeTexture::CubeTexture(CubeTexture&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

CubeTexture::~CubeTexture()
{
    glDeleteTextures(1, &ID);
}

void CubeTexture::Draw(GLuint unit) const
{
    Bind(unit);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    Unbind(unit);
}

void CubeTexture::LoadTexture(int index, const void* data, GLenum type, GLenum formatL, GLenum formatR, int w, int h) const
{
    if (data)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, formatL, w, h, 0, formatR, type, data);
    else
        std::cout << "Failed to load texture at: " << index << std::endl;
}

void CubeTexture::AllocTexture(int index, GLenum type, GLenum formatL, GLenum formatR, int w, int h) const
{
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, formatL, w, h, 0, formatR, type, nullptr);
}

void CubeTexture::Bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void CubeTexture::Unbind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeTexture::texUnit(ShaderIDs shaderID, const std::string& uniform) const
{
    ShaderManager::Activate(shaderID);
    GLint unit = ShaderManager::getUnit(shaderID, uniform);
    Bind(unit);
    glUniform1i(ShaderManager::getLoc(shaderID, uniform), unit);
}