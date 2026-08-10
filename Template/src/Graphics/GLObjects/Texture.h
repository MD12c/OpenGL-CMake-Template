#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <iostream>
#include <string>

#include <glad/glad.h>

#include "..\Shaders\ShaderManager.h"

class Texture
{
public:
    GLuint      ID;
    std::string type;
    std::string path;
    GLuint      unit;

    Texture(const std::string& image, const std::string& texType, GLuint slot);

    void texUnit(const unsigned int shaderID, const std::string& uniform) const;
    void texUnit(const Shader& shader, const std::string& uniform, const GLuint unit) const;  // Old
    void Bind() const;
    void Unbind() const;
    void Delete() const;
};
#endif