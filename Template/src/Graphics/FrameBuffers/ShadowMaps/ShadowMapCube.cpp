#include "ShadowMapCube.h"
#include "../../Shaders/ShaderManager.h"
#include "Globals.h"

ShadowMapCube::ShadowMapCube(glm::vec3 lightPos, glm::vec3 lightColor, float zNear, float zFar)
    : lightPos(lightPos), farPlane(zFar), color(lightColor)
{
    lightType = LightType::POINT;

    glGenFramebuffers(1, &ID);
    glGenTextures(1, &shadowCubeTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeTexture);

    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubeTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ShadowMapCube FBO incomplete: " << status << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
    setView(lightPos, glm::vec3(0.0f));
}

ShadowMapCube::~ShadowMapCube()
{
    glDeleteTextures(1, &shadowCubeTexture);
    shadowCubeTexture = 0;
}

void ShadowMapCube::setView(glm::vec3 newPosition, glm::vec3 newDirection)
{
    lightPos = newPosition;
    for (int i = 0; i < 6; i++)
        shadowMatrices[i] = proj * glm::lookAt(lightPos, lightPos + cubeFaces[i].dir, cubeFaces[i].up);
}

void ShadowMapCube::BeginDepthPass(unsigned int shaderID)
{
    ShaderManager::Activate(shaderID);

    for (int i = 0; i < 6; i++)
        glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "shadowMatrices[" + std::to_string(i) + "]"), 1, GL_FALSE, glm::value_ptr(shadowMatrices[i]));

    glUniform3f(ShaderManager::getLoc(shaderID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1f(ShaderManager::getLoc(shaderID, "farPlane"), farPlane);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapCube::EndDepthPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapCube::ExportUniformsTo(unsigned int shaderID, GLuint textureSlot, int lightIndex)
{
    ShaderManager::Activate(shaderID);
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeTexture);
    glUniform1i(ShaderManager::getLoc(shaderID, "pointShadowCubeMap[" + std::to_string(lightIndex) + "]"), textureSlot);
    glUniform1f(ShaderManager::getLoc(shaderID, "pointFarPlane[" + std::to_string(lightIndex) + "]"), farPlane);
    glUniform3fv(ShaderManager::getLoc(shaderID, "pointLightPos[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(lightPos));
    glUniform3fv(ShaderManager::getLoc(shaderID, "pointLightColor[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(color));
}

void ShadowMapCube::DrawDepthDebug(unsigned int shaderID, int faceIndex)
{
    ShaderManager::Activate(shaderID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeTexture);
    glUniform1i(ShaderManager::getLoc(shaderID, "depthCubeMap"), 0);

    glm::vec3 forward = cubeFaces[faceIndex].dir;
    glm::vec3 up      = cubeFaces[faceIndex].up;
    glm::vec3 right   = glm::normalize(glm::cross(forward, up));

    glUniform3fv(ShaderManager::getLoc(shaderID, "faceForward"), 1, glm::value_ptr(forward));
    glUniform3fv(ShaderManager::getLoc(shaderID, "faceRight"), 1, glm::value_ptr(right));
    glUniform3fv(ShaderManager::getLoc(shaderID, "faceUp"), 1, glm::value_ptr(up));

    glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(GLuint), GL_UNSIGNED_INT, squareIndices);
}