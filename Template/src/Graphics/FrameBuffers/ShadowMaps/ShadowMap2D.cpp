#include "ShadowMap2D.h"
#include "../../Shaders/ShaderManager.h"
#include "Globals.h"

ShadowMap2D::ShadowMap2D(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top, float zNear, float zFar)
    : lightPos(lightPos), direction(direction), color(lightColor)
{
    lightType = LightType::DIRECTION;
    genTexture();
    proj = glm::ortho(left, right, bottom, top, zNear, zFar);
    view = glm::lookAt(lightPos, lightPos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

ShadowMap2D::ShadowMap2D(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone, float zNear, float zFar)
    : lightPos(lightPos), direction(direction), color(lightColor), innerCone(innerCone), outerCone(outerCone)
{
    lightType = LightType::SPOT;
    genTexture();
    proj = glm::perspective(glm::radians(fovDeg), (float)SHADOW_MAP_WIDTH / (float)SHADOW_MAP_HEIGHT, zNear, zFar);
    view = glm::lookAt(lightPos, lightPos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

ShadowMap2D::~ShadowMap2D()
{
    glDeleteTextures(1, &shadowMapTexture);
    shadowMapTexture = 0;
}

void ShadowMap2D::setView(glm::vec3 newPosition, glm::vec3 newDirection)
{
    direction = newDirection;
    lightPos  = newPosition;
    view      = glm::lookAt(lightPos, lightPos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowMap2D::genTexture()
{
    glGenFramebuffers(1, &ID);
    glGenTextures(1, &shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap2D::BeginDepthPass(unsigned int shaderID)
{
    ShaderManager::Activate(shaderID);
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap2D::EndDepthPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap2D::ExportUniformsTo(unsigned int shaderID, GLuint textureSlot, int lightIndex)
{
    ShaderManager::Activate(shaderID);
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

    std::string type;
    glm::mat4   projView = proj * view;

    if (lightType == DIRECTION)
        type = "dir";
    else if (lightType == SPOT)
    {
        type = "spot";
        glUniform3fv(ShaderManager::getLoc(shaderID, type + "LightPos[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(lightPos));
        glUniform1f(ShaderManager::getLoc(shaderID, type + "LightInnerCone[" + std::to_string(lightIndex) + "]"), innerCone);
        glUniform1f(ShaderManager::getLoc(shaderID, type + "LightOuterCone[" + std::to_string(lightIndex) + "]"), outerCone);
    }
    else
        throw std::runtime_error("[ERROR] Invalid light type ShadowMap2D::ExportUniformsTo()");

    glUniform1i(ShaderManager::getLoc(shaderID, type + "ShadowMap[" + std::to_string(lightIndex) + "]"), textureSlot);
    glUniform3fv(ShaderManager::getLoc(shaderID, type + "LightDirection[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(direction));
    glUniform3fv(ShaderManager::getLoc(shaderID, type + "LightColor[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(color));
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, type + "ShadowMatrix[" + std::to_string(lightIndex) + "]"), 1, GL_FALSE, glm::value_ptr(projView));
}

void ShadowMap2D::DrawDepthDebug(unsigned int shaderID)
{
    ShaderManager::Activate(shaderID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(GLuint), GL_UNSIGNED_INT, squareIndices);
}