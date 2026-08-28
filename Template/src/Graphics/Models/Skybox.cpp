#include "Skybox.h"

#include "Globals.h"
#include "..\Shaders\ShaderManager.h"
#include "../GLObjects/Texture.h"

#include "stb/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

static const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
static const glm::mat4 captureViews[]    = {
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
};

Skybox::Skybox(std::string facesCubemap[6])
    : skyboxVAO(),
      skyboxVBO(skyboxVertices, sizeof(skyboxVertices)),
      skyboxEBO(skyboxIndices, sizeof(skyboxIndices))
{
    bind();
    skyboxVAO.LinkAttrib(skyboxVBO, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0);
    unbind();

    cubemapTexture = std::make_unique<CubeTexture>();

    bool success = true;
    stbi_set_flip_vertically_on_load(false);
    for (GLuint i = 0; i < 6; i++)
    {
        int            widthImg, heightImg, numColCh;
        unsigned char* bytes = stbi_load(facesCubemap[i].c_str(), &widthImg, &heightImg, &numColCh, 0);

        if (bytes)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
        else
        {
            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
            success = false;
        }

        stbi_image_free(bytes);
    }

    if (success)
    {
        irradiancemapTexture = std::make_unique<CubeTexture>();
        CubeToIrradiance(32);
        CubeToPrefiltered(2048);
    }
}

Skybox::Skybox(std::string HDRimage)
    : skyboxVAO(),
      skyboxVBO(skyboxVertices, sizeof(skyboxVertices)),
      skyboxEBO(skyboxIndices, sizeof(skyboxIndices))
{
    bind();
    skyboxVAO.LinkAttrib(skyboxVBO, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0);
    unbind();

    stbi_set_flip_vertically_on_load(true);
    int    widthImg, heightImg, nrComponents;
    float* data = stbi_loadf(HDRimage.c_str(), &widthImg, &heightImg, &nrComponents, 0);

    if (data)
    {
        cubemapTexture = std::make_unique<CubeTexture>();
        HDRtoCube(data, widthImg, heightImg, 2048);

        irradiancemapTexture = std::make_unique<CubeTexture>();
        CubeToIrradiance(32);
        CubeToPrefiltered(2048);
    }
    else
        std::cout << "Failed to load HDR image." << std::endl;

    stbi_image_free(data);
}

void Skybox::HDRtoCube(float* data, int widthImg, int heightImg, int resolution)
{
    GLint prevFramebuffer, prevIsCull;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);
    glGetIntegerv(GL_CULL_FACE, &prevIsCull);

    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    cubemapTexture->Bind(0);
    for (GLint i = 0; i < 6; ++i)
        cubemapTexture->LoadTexture(i, data, GL_FLOAT, GL_RGB16F, GL_RGB, resolution, resolution);

    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    glViewport(0, 0, resolution, resolution);

    Texture hdrTexture(GL_RGB16F, GL_RGB, widthImg, heightImg, GL_FLOAT, data);
    for (GLint i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemapTexture->ID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawCaptureCube(ShaderID::HDR_CONVERTER, captureProjection * captureViews[i], hdrTexture);
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "HDRtoCube capture FBO incomplete!" << std::endl;

    if (prevIsCull) glEnable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
}

void Skybox::CubeToIrradiance(int resolution)
{
    GLint prevFramebuffer, prevIsCull;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);
    glGetIntegerv(GL_CULL_FACE, &prevIsCull);

    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    irradiancemapTexture->Bind(0);
    for (GLuint i = 0; i < 6; ++i)
        irradiancemapTexture->AllocTexture(i, GL_FLOAT, GL_RGB16F, GL_RGB, resolution, resolution);

    cubemapTexture->texUnit(ShaderID::IRRADIANCE, "environmentMap");

    glDisable(GL_CULL_FACE);
    glViewport(0, 0, resolution, resolution);
    bind();
    for (GLuint i = 0; i < 6; ++i)
    {
        glm::mat4 mvp = captureProjection * captureViews[i];
        glUniformMatrix4fv(ShaderManager::getLoc(ShaderID::IRRADIANCE, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiancemapTexture->ID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    unbind();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Irradiance capture FBO incomplete!" << std::endl;

    if (prevIsCull) glEnable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);
    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
}

void Skybox::DrawCaptureCube(ShaderID shaderID, glm::mat4 cameraMatrix, Texture& inputTexture) const
{
    ShaderManager::Activate(shaderID);
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));

    glDisable(GL_CULL_FACE);
    bind();
    inputTexture.texUnit(ShaderID::HDR_CONVERTER, "equirectangularMap");
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    unbind();
    glEnable(GL_CULL_FACE);
}

void Skybox::CubeToPrefiltered(int baseResolution)
{
    GLint prevFramebuffer, prevIsCull;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFramebuffer);
    glGetIntegerv(GL_CULL_FACE, &prevIsCull);

    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

    prefilteredmapTexture = std::make_unique<CubeTexture>();
    prefilteredmapTexture->Bind(0);
    for (GLuint i = 0; i < 6; ++i)
        prefilteredmapTexture->AllocTexture(i, GL_FLOAT, GL_RGB16F, GL_RGB, baseResolution, baseResolution);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    cubemapTexture->texUnit(ShaderID::PREFILTER, "environmentMap");

    glDisable(GL_CULL_FACE);
    bind();

    GLuint maxMipLevels = 5;
    for (GLuint mip = 0; mip < maxMipLevels; ++mip)
    {
        GLuint mipRes = static_cast<GLuint>(baseResolution * std::pow(0.5, mip));
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipRes, mipRes);
        glViewport(0, 0, mipRes, mipRes);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        glUniform1f(ShaderManager::getLoc(ShaderID::PREFILTER, "roughness"), roughness);

        for (GLuint face = 0; face < 6; ++face)
        {
            glm::mat4 mvp = captureProjection * captureViews[face];
            glUniformMatrix4fv(ShaderManager::getLoc(ShaderID::PREFILTER, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, prefilteredmapTexture->ID, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Prefilter capture FBO incomplete!" << std::endl;

    unbind();
    if (prevIsCull) glEnable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, prevFramebuffer);
    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
}

void Skybox::Draw(ShaderID shaderID, glm::mat4 cameraMatrix) const
{
    GPUInstrumentationTimer timerGPU("Skybox");
    ShaderManager::Activate(shaderID);
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    cubemapTexture->texUnit(shaderID, "skybox");

    bind();
    if (cubemapTexture)
        cubemapTexture->Draw(ShaderManager::getUnit(shaderID, "skybox"));
    unbind();
}

void Skybox::ExportUniformsTo(ShaderID shaderID) const
{
    ShaderManager::Activate(shaderID);
    irradiancemapTexture->texUnit(shaderID, "irradiance0");
    prefilteredmapTexture->texUnit(shaderID, "prefilteredmap0");
    glUniform1i(ShaderManager::getLoc(shaderID, "useIBL"), (irradiancemapTexture != nullptr && prefilteredmapTexture != nullptr));
}
