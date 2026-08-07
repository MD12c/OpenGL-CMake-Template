#include "ShadowMapCube.h"
#include "../../Shaders/ShaderManager.h"
#include "Globals.h"

ShadowMapCube::ShadowMapCube(glm::vec3 lightPos, float zNear, float zFar)
    : lightPos(lightPos), farPlane(zFar)
{
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
    setView(lightPos);
}

void ShadowMapCube::setView(glm::vec3 newPosition)
{
    lightPos = newPosition;
    for (int i = 0; i < 6; i++)
        shadowMatrices[i] = proj * glm::lookAt(lightPos, lightPos + cubeFaces[i].dir, cubeFaces[i].up);
}

void ShadowMapCube::BeginDepthPass(const std::string& shaderName)
{
    ShaderManager::Activate(shaderName);
    for (int i = 0; i < 6; i++)
        glUniformMatrix4fv(ShaderManager::GetUniformLoc(shaderName, "shadowMatrices[" + std::to_string(i) + "]"), 1, GL_FALSE, glm::value_ptr(shadowMatrices[i]));

    glUniform3f(ShaderManager::GetUniformLoc(shaderName, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1f(ShaderManager::GetUniformLoc(shaderName, "farPlane"), farPlane);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapCube::EndDepthPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapCube::ExportUniformsTo(const std::string& shaderName, GLuint textureSlot)
{
    ShaderManager::Activate(shaderName);
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeTexture);
    glUniform1i(ShaderManager::GetUniformLoc(shaderName, "shadowCubeMap"), textureSlot);
    glUniform1f(ShaderManager::GetUniformLoc(shaderName, "farPlane"), farPlane);
    glUniform3fv(ShaderManager::GetUniformLoc(shaderName, "lightPos"), 1, glm::value_ptr(lightPos));
}

void ShadowMapCube::DrawDepthDebug(const std::string& shaderName, int faceIndex)
{
    ShaderManager::Activate(shaderName);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeTexture);
    glUniform1i(ShaderManager::GetUniformLoc(shaderName, "depthCubeMap"), 0);

    glm::vec3 forward = cubeFaces[faceIndex].dir;
    glm::vec3 up      = cubeFaces[faceIndex].up;
    glm::vec3 right   = glm::normalize(glm::cross(forward, up));

    glUniform3fv(ShaderManager::GetUniformLoc(shaderName, "faceForward"), 1, glm::value_ptr(forward));
    glUniform3fv(ShaderManager::GetUniformLoc(shaderName, "faceRight"), 1, glm::value_ptr(right));
    glUniform3fv(ShaderManager::GetUniformLoc(shaderName, "faceUp"), 1, glm::value_ptr(up));

    glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(GLuint), GL_UNSIGNED_INT, squareIndices);
}