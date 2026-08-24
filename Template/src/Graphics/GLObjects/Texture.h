#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <iostream>
#include <string>

#include <glad/glad.h>

#include "..\Shaders\ShaderManager.h"

class Texture
{
public:
    enum TextureType
    {
        NONE,
        DIFFUSE,
        SPECULAR,
        ALBEDO,
        AO,
        METALIC_ROUGHNESS,
        NORMAL,
        DISPLACEMENT,
        CUSTOM
    } type;

    enum class TextureCombineMode
    {
        Add,
        Pack
    };

    GLuint      ID;
    std::string path;

    Texture(GLenum formatL, GLenum formatR, int w, int h, GLenum dataType = GL_UNSIGNED_BYTE, const void* data = nullptr);
    Texture(const std::string& image, TextureType texType);
    Texture(const std::string& image1, const std::string& image2, TextureType texType, TextureCombineMode mode);  // combines the two

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    ~Texture();

    void LoadTexture(const void* data, GLenum formatL, GLenum formatR, int w, int h, GLenum dataType = GL_UNSIGNED_BYTE) const;
    void texUnit(const ShaderIDs shaderID, const std::string& uniform) const;
    void texUnit(const Shader& shader, const std::string& uniform, const GLint unit) const;  // Old
    void Bind(GLint unit) const;
    void Unbind(GLint unit) const;

private:
    bool mipMapGenerated = false;

    std::pair<GLenum, GLenum> getImageType(int numColCh, TextureType texType);
    void                      createGLtexture(const void* data, GLenum formatL, GLenum formatR, int widthImg, int heightImg, GLenum dataType = GL_UNSIGNED_BYTE);
    static void               CombineAdd(unsigned char* dst, unsigned char* src, int w, int h, int ch);
    static void               CombinePack(unsigned char* dst, unsigned char* src, int w, int h, int ch);
};
#endif