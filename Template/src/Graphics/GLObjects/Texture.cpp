#include "Texture.h"
#include <stb/stb_image.h>
#include "Globals.h"

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

    if (texType == DISPLACEMENT || texType == NORMAL || texType == SPECULAR)
        return std::pair<GLenum, GLenum>(colorChannels, colorChannels);

    if (colorChannels == GL_RED)
        colorType = GL_SRGB;
    if (colorChannels == GL_RGB)
        colorType = GL_SRGB;
    if (colorChannels == GL_RGBA)
        colorType = GL_SRGB;

    return std::pair<GLenum, GLenum>(colorType, colorChannels);
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

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);

    if (type == DISPLACEMENT)
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

    glTexImage2D(GL_TEXTURE_2D, 0, imageParameters.first, widthImg, heightImg, 0, imageParameters.second, GL_UNSIGNED_BYTE, bytes);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);

    glBindTexture(GL_TEXTURE_2D, 0);
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
