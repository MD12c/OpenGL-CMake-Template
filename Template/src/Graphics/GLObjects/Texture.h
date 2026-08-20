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
        DISPLACEMENT
    } type;

    enum class TextureCombineMode
    {
        Add,
        Pack
    };

    GLuint      ID;
    std::string path;
    GLuint      unit;

    Texture(const std::string& image, TextureType texType, GLuint slot);
    Texture(const std::string& image1, const std::string& image2, TextureType texType, GLuint slot, TextureCombineMode mode);  // combines the two

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    ~Texture();

    void texUnit(const int shaderID, const std::string& uniform) const;
    void texUnit(const Shader& shader, const std::string& uniform, const GLuint unit) const;  // Old
    void Bind() const;
    void Unbind() const;

private:
    std::pair<GLenum, GLenum> getImageType(int numColCh, TextureType texType);
    void                      loadGLtexture(unsigned char* bytes, int numColCh, TextureType texType, int widthImg, int heightImg);
    static void               CombineAdd(unsigned char* dst, unsigned char* src, int w, int h, int ch);
    static void               CombinePack(unsigned char* dst, unsigned char* src, int w, int h, int ch);
};
#endif