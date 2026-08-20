#include "Texture.h"

#include <algorithm>

#include <stb/stb_image.h>
#include "Globals.h"

Texture::Texture(GLuint slot, GLenum formatL, GLenum formatR, int w, int h, GLenum dataType, const void* data)
    : path("Custom texture"), type(CUSTOM), unit(slot)
{
    createGLtexture(data, formatL, formatR, w, h, dataType);
}

Texture::Texture(const std::string& image, TextureType texType, GLuint slot)
    : path(image), type(texType), unit(slot)
{
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* bytes = stbi_load(path.c_str(), &widthImg, &heightImg, &numColCh, 0);

    if (bytes == nullptr)
        throw std::runtime_error("[ERROR] Couldn't Load Texture");

    std::pair<GLenum, GLenum> imageParameters = getImageType(numColCh, texType);
    createGLtexture(bytes, imageParameters.first, imageParameters.second, widthImg, heightImg);
    stbi_image_free(bytes);
}

Texture::Texture(const std::string& image1, const std::string& image2, TextureType texType, GLuint slot, TextureCombineMode mode)
    : path(image1 + image2), type(texType), unit(slot)
{
    stbi_set_flip_vertically_on_load(false);

    int            widthImg1, heightImg1, numColCh1;
    unsigned char* bytes1 = stbi_load(image1.c_str(), &widthImg1, &heightImg1, &numColCh1, 0);
    if (bytes1 == nullptr)
        throw std::runtime_error("[ERROR] Couldn't Load Texture image1");

    int            widthImg2, heightImg2, numColCh2;
    unsigned char* bytes2 = stbi_load(image2.c_str(), &widthImg2, &heightImg2, &numColCh2, 0);
    if (bytes2 == nullptr)
    {
        stbi_image_free(bytes1);
        throw std::runtime_error("[ERROR] Couldn't Load Texture image2");
    }

    if (widthImg1 != widthImg2 || heightImg1 != heightImg2 || numColCh1 != numColCh2)
    {
        stbi_image_free(bytes1);
        stbi_image_free(bytes2);
        throw std::runtime_error("[ERROR] Couldn't Load Texture image dimensions don't match");
    }

    switch (mode)
    {
        case TextureCombineMode::Add:
            CombineAdd(bytes1, bytes2, widthImg1, heightImg1, numColCh1);
            break;
        case TextureCombineMode::Pack:
            CombinePack(bytes1, bytes2, widthImg1, heightImg1, numColCh1);
            break;
    }

    stbi_image_free(bytes2);
    std::pair<GLenum, GLenum> imageParameters = getImageType(numColCh1, texType);
    createGLtexture(bytes1, imageParameters.first, imageParameters.second, widthImg1, heightImg1);
    stbi_image_free(bytes1);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        glDeleteTextures(1, &ID);

        ID         = other.ID;
        type       = other.type;
        unit       = other.unit;
        path       = other.path;
        other.ID   = 0;
        other.type = NONE;
        other.unit = 0;
        other.path = "";
    }

    return *this;
}

Texture::Texture(Texture&& other) noexcept
    : ID(other.ID), type(other.type), unit(other.unit), path(other.path)
{
    other.ID   = 0;
    other.type = NONE;
    other.unit = 0;
    other.path = "";
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

void Texture::createGLtexture(const void* data, GLenum formatL, GLenum formatR, int widthImg, int heightImg, GLenum dataType)
{
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);

    if (1)  // no usecase for now
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

    glTexImage2D(GL_TEXTURE_2D, 0, formatL, widthImg, heightImg, 0, formatR, dataType, data);

    if (data)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        mipMapGenerated = true;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::LoadTexture(const void* data, GLuint slot, GLenum formatL, GLenum formatR, int w, int h, GLenum dataType) const
{
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, formatL, w, h, 0, formatR, dataType, data);
    if (!mipMapGenerated && data)
        glGenerateMipmap(GL_TEXTURE_2D);
}

std::pair<GLenum, GLenum> Texture::getImageType(int numColCh, TextureType texType)
{
    GLenum colorType;
    GLenum colorChannels;

    if (numColCh == 1)
        colorChannels = GL_RED;
    else if (numColCh == 3)
        colorChannels = GL_RGB;
    else if (numColCh == 4)
        colorChannels = GL_RGBA;
    else
        throw std::runtime_error("[ERROR] Couldn't Load Texture, Invalid number of color channels");

    if (texType != DIFFUSE && texType != ALBEDO)
        return std::pair<GLenum, GLenum>(colorChannels, colorChannels);

    if (colorChannels == GL_RED)
        colorType = GL_RED;
    else if (colorChannels == GL_RGB)
        colorType = GL_SRGB8;
    else if (colorChannels == GL_RGBA)
        colorType = GL_SRGB8_ALPHA8;

    return std::pair<GLenum, GLenum>(colorType, colorChannels);
}

void Texture::CombineAdd(unsigned char* dst, unsigned char* src, int w, int h, int ch)
{
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            int idx = (y * w + x) * ch;
            for (int k = 0; k < ch; k++)
            {
                int sum      = static_cast<int>(dst[idx + k]) + static_cast<int>(src[idx + k]);
                dst[idx + k] = static_cast<unsigned char>(std::clamp(sum, 0, 255));
            }
        }
}

void Texture::CombinePack(unsigned char* dst, unsigned char* src, int w, int h, int ch)
{
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
        {
            int idx = (y * w + x) * ch;
            if (ch > 1)
                dst[idx + 1] = src[idx];
            if (ch > 2)
                dst[idx + 2] = src[idx];
        }
}

void Texture::texUnit(const int shaderID, const std::string& uniform) const
{
    ShaderManager::Activate(shaderID);
    glUniform1i(ShaderManager::getLoc(shaderID, uniform), unit);
}

// Old! Dont use if using shader manager
void Texture::texUnit(const Shader& shader, const std::string& uniform, const GLuint unit) const
{
    GLuint texUni = glGetUniformLocation(shader.ID, uniform.c_str());
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind() const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
