#include "Skybox.h"

#include "Globals.h"
#include "../Shaders/ShaderManager.h"
#include "../GLObjects/Texture.h"

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

    cubemapTexture = std::make_unique<CubeTexture>(0);

    bool success = true;
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < 6; i++)
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
        irradiancemapTexture = std::make_unique<CubeTexture>(6);
        CubeToIrradiance(32);
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
        cubemapTexture = std::make_unique<CubeTexture>(0);
        HDRtoCube(data, widthImg, heightImg, 2048);

        irradiancemapTexture = std::make_unique<CubeTexture>(6);
        CubeToIrradiance(32);
    }
    else
        std::cout << "Failed to load HDR image." << std::endl;

    stbi_image_free(data);
}

Skybox::~Skybox()
{
}

void Skybox::HDRtoCube(float* data, int widthImg, int heightImg, int resolution)
{
    Texture hdrTexture(0, GL_RGB16F, GL_RGB, widthImg, heightImg, GL_FLOAT, data);

    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    for (unsigned int i = 0; i < 6; ++i)
        cubemapTexture->LoadTexture(i, data, GL_FLOAT, GL_RGB16F, GL_RGB, resolution, resolution);

    hdrTexture.Bind();
    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    glViewport(0, 0, resolution, resolution);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemapTexture->ID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawCaptureCube(ShaderManager::IDs.HDRconverter, captureProjection * captureViews[i], hdrTexture.ID);
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Skybox capture FBO incomplete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);

    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
}

void Skybox::CubeToIrradiance(int resolution)
{
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    for (unsigned int i = 0; i < 6; ++i)
        irradiancemapTexture->AllocTexture(i, GL_FLOAT, GL_RGB16F, GL_RGB, resolution, resolution);

    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    glViewport(0, 0, resolution, resolution);

    ShaderManager::Activate(ShaderManager::IDs.irradiance);
    glUniform1i(ShaderManager::getLoc(ShaderManager::IDs.irradiance, "environmentMap"), 0);
    cubemapTexture->Bind();

    glDisable(GL_CULL_FACE);
    bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        glm::mat4 mvp = captureProjection * captureViews[i];
        glUniformMatrix4fv(ShaderManager::getLoc(ShaderManager::IDs.irradiance, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiancemapTexture->ID, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    unbind();
    glEnable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);

    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
}

void Skybox::DrawCaptureCube(int shaderID, glm::mat4 cameraMatrix, GLuint inputTexture) const
{
    ShaderManager::Activate(shaderID);
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    glUniform1i(ShaderManager::getLoc(shaderID, "equirectangularMap"), 0);

    glDisable(GL_CULL_FACE);
    bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    unbind();
    glEnable(GL_CULL_FACE);
}

void Skybox::Draw(int shaderID, glm::mat4 cameraMatrix) const
{
    ShaderManager::Activate(shaderID);
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    glUniform1i(ShaderManager::getLoc(shaderID, "skybox"), 0);

    bind();
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    if (cubemapTexture)
        cubemapTexture->Draw();
    unbind();
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

void Skybox::ExportUniformsTo(int shaderID) const
{
    ShaderManager::Activate(shaderID);
    irradiancemapTexture->Bind();
    glUniform1i(ShaderManager::getLoc(shaderID, "irradiance0"), 6);
    glUniform1i(ShaderManager::getLoc(shaderID, "useIrradiance"), (irradiancemapTexture != nullptr));
}
