#include "Skybox.h"

#include "Globals.h"
#include "../Shaders/ShaderManager.h"

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
    cubemapTexture = new GLuint();
    genCube(cubemapTexture);

    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < 6; i++)
    {
        int            widthImg, heightImg, numColCh;
        unsigned char* bytes = stbi_load(facesCubemap[i].c_str(), &widthImg, &heightImg, &numColCh, 0);

        if (bytes)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
        else
            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;

        irradiancemapTexture = new GLuint();
        genCube(irradiancemapTexture);
        CubeToIrradiance(*cubemapTexture, 32, *irradiancemapTexture);

        stbi_image_free(bytes);
    }
}

Skybox::Skybox(std::string HDRimage)
    : skyboxVAO(),
      skyboxVBO(skyboxVertices, sizeof(skyboxVertices)),
      skyboxEBO(skyboxIndices, sizeof(skyboxIndices))
{
    stbi_set_flip_vertically_on_load(true);
    int    widthImg, heightImg, nrComponents;
    float* data = stbi_loadf(HDRimage.c_str(), &widthImg, &heightImg, &nrComponents, 0);

    if (data)
    {
        cubemapTexture = new GLuint();
        genCube(cubemapTexture);
        HDRtoCube(data, widthImg, heightImg, 2048, *cubemapTexture);
        irradiancemapTexture = new GLuint();
        genCube(irradiancemapTexture);
        CubeToIrradiance(*cubemapTexture, 32, *irradiancemapTexture);
    }
    else
        std::cout << "Failed to load HDR image." << std::endl;

    stbi_image_free(data);
}

void Skybox::HDRtoCube(float* data, int widthImg, int heightImg, int resolution, GLuint cubeTexture)
{
    unsigned int hdrTexture;
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, widthImg, heightImg, 0, GL_RGB, GL_FLOAT, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);

    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    glViewport(0, 0, resolution, resolution);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeTexture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawCaptureCube(ShaderManager::IDs.HDRconverter, captureProjection * captureViews[i], hdrTexture);
    }
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Skybox capture FBO incomplete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);

    glDeleteFramebuffers(1, &captureFBO);
    glDeleteRenderbuffers(1, &captureRBO);
    glDeleteTextures(1, &hdrTexture);
}

void Skybox::CubeToIrradiance(GLuint srcCubemap, int resolution, GLuint dstCubemap)
{
    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    glBindTexture(GL_TEXTURE_CUBE_MAP, dstCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);

    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    glViewport(0, 0, resolution, resolution);

    ShaderManager::Activate(ShaderManager::IDs.irradiance);
    glUniform1i(ShaderManager::getLoc(ShaderManager::IDs.irradiance, "environmentMap"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, srcCubemap);

    glDisable(GL_CULL_FACE);
    bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        glm::mat4 mvp = captureProjection * captureViews[i];
        glUniformMatrix4fv(ShaderManager::getLoc(ShaderManager::IDs.irradiance, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(mvp));

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dstCubemap, 0);
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

void Skybox::genCube(GLuint* cube)
{
    bind();
    skyboxVAO.LinkAttrib(skyboxVBO, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0);
    unbind();

    glGenTextures(1, cube);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *cube);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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

    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    bind();
    if (cubemapTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *cubemapTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    unbind();
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

void Skybox::ExportUniformsTo(int shaderID) const
{
    ShaderManager::Activate(shaderID);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *irradiancemapTexture);
    glUniform1i(ShaderManager::getLoc(shaderID, "irradiance0"), 6);
    glUniform1i(ShaderManager::getLoc(shaderID, "useIrradiance"), (irradiancemapTexture != nullptr));
}

Skybox::~Skybox()
{
    glDeleteTextures(1, cubemapTexture);
    glDeleteTextures(1, irradiancemapTexture);
    delete cubemapTexture;
    delete irradiancemapTexture;
}