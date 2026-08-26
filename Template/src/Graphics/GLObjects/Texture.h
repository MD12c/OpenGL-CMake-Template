#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <iostream>
#include <string>
#include <array>

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

    Texture(GLenum formatL, GLenum formatR, int w, int h,
            GLenum      dataType = GL_UNSIGNED_BYTE,
            const void* data     = nullptr,
            GLenum      paramMin = GL_LINEAR_MIPMAP_LINEAR,
            GLenum      paramMax = GL_LINEAR,
            GLenum      wrapType = GL_REPEAT);
    Texture(const std::string& image, TextureType texType);
    Texture(const std::string& image1, const std::string& image2, TextureType texType, TextureCombineMode mode);  // combines the two

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    ~Texture();

    void LoadTexture(const void* data, GLenum formatL, GLenum formatR, int w, int h, GLenum dataType = GL_UNSIGNED_BYTE) const;
    void texUnit(const ShaderID shaderID, const std::string& uniform) const;
    void texUnit(const Shader& shader, const std::string& uniform, const GLint unit) const;  // Old
    void Bind(GLint unit) const;

    static void UnbindAt(ShaderID shaderID, const std::string& uniform);
    static void Unbind(GLint unit);
    static void UnbindAll();

    static void setNoTextureID(GLuint texID) { noTextureID = texID; }

private:
    bool mipMapGenerated = false;

    void createGLtexture(const void* data,
                         GLenum      formatL,
                         GLenum      formatR,
                         int         widthImg,
                         int         heightImg,
                         GLenum      dataType = GL_UNSIGNED_BYTE,
                         GLenum      paramMin = GL_LINEAR_MIPMAP_LINEAR,
                         GLenum      paramMax = GL_LINEAR,
                         GLenum      wrapType = GL_REPEAT);

    inline static GLuint                noTextureID;
    inline static std::array<GLint, 32> boundTextures{};

    static std::pair<GLenum, GLenum> getImageType(int numColCh, TextureType texType);
    static void                      CombineAdd(unsigned char* dst, unsigned char* src, int w, int h, int ch);
    static void                      CombinePack(unsigned char* dst, unsigned char* src, int w, int h, int ch);
};

inline void PrintTextureParams(GLenum target)  // for debug only
{
    GLint minFilter, magFilter, wrapS, wrapT, baseLevel, maxLevel;
    glGetTexParameteriv(target, GL_TEXTURE_MIN_FILTER, &minFilter);
    glGetTexParameteriv(target, GL_TEXTURE_MAG_FILTER, &magFilter);
    glGetTexParameteriv(target, GL_TEXTURE_WRAP_S, &wrapS);
    glGetTexParameteriv(target, GL_TEXTURE_WRAP_T, &wrapT);
    glGetTexParameteriv(target, GL_TEXTURE_BASE_LEVEL, &baseLevel);
    glGetTexParameteriv(target, GL_TEXTURE_MAX_LEVEL, &maxLevel);

    std::cout << "MIN_FILTER: " << std::hex << minFilter << "\n"
              << "MAG_FILTER: " << std::hex << magFilter << "\n"
              << "WRAP_S: " << std::hex << wrapS << "\n"
              << "WRAP_T: " << std::hex << wrapT << "\n"
              << "BASE_LEVEL: " << std::dec << baseLevel << "\n"
              << "MAX_LEVEL: " << std::dec << maxLevel << std::endl;
}
#endif