#include "ShadowMap2D.h"

#include "Globals.h"
#include "ShadowSystem.h"
#include "../../Shaders/ShaderManager.h"

ShadowMap2D::ShadowMap2D(GLuint layerIndex, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top, float zNear, float zFar)
    : layerIndex(layerIndex), lightPos(lightPos), direction(direction), color(lightColor)
{
    lightType = LightSystem::LightType::DIRECTION;
    proj      = glm::ortho(left, right, bottom, top, zNear, zFar);
    view      = glm::lookAt(lightPos, lightPos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

ShadowMap2D::ShadowMap2D(GLuint layerIndex, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone, float zNear, float zFar)
    : layerIndex(layerIndex), lightPos(lightPos), direction(direction), color(lightColor), innerCone(innerCone), outerCone(outerCone)
{
    lightType = LightSystem::LightType::SPOT;
    proj      = glm::perspective(glm::radians(fovDeg), (float)SHADOW_MAP_WIDTH / (float)SHADOW_MAP_HEIGHT, zNear, zFar);
    view      = glm::lookAt(lightPos, lightPos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowMap2D::setView(glm::vec3 newPosition, glm::vec3 newDirection)
{
    direction = newDirection;
    lightPos  = newPosition;
    view      = glm::lookAt(lightPos, lightPos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowMap2D::BeginDepthPass(unsigned int shaderID, ShadowSystem& shadowSystem)
{
    ShaderManager::Activate(shaderID);

    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));

    shadowSystem.BindDepthTarget(lightType, layerIndex);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap2D::ExportUniformsTo(unsigned int shaderID, int lightIndex)
{
    ShaderManager::Activate(shaderID);

    std::string type;
    glm::mat4   projView = proj * view;

    if (lightType == LightSystem::LightType::DIRECTION)
        type = "dir";
    else if (lightType == LightSystem::LightType::SPOT)
    {
        type = "spot";
        glUniform3fv(ShaderManager::getLoc(shaderID, type + "LightPos[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(lightPos));
        glUniform1f(ShaderManager::getLoc(shaderID, type + "LightInnerCone[" + std::to_string(lightIndex) + "]"), innerCone);
        glUniform1f(ShaderManager::getLoc(shaderID, type + "LightOuterCone[" + std::to_string(lightIndex) + "]"), outerCone);
    }
    else
        throw std::runtime_error("[ERROR] Invalid light type ShadowMap2D::ExportUniformsTo()");

    glUniform1i(ShaderManager::getLoc(shaderID, type + "LayerIndex[" + std::to_string(lightIndex) + "]"), layerIndex);
    glUniform3fv(ShaderManager::getLoc(shaderID, type + "LightDirection[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(direction));
    glUniform3fv(ShaderManager::getLoc(shaderID, type + "LightColor[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(color));
    glUniformMatrix4fv(ShaderManager::getLoc(shaderID, type + "ShadowMatrix[" + std::to_string(lightIndex) + "]"), 1, GL_FALSE, glm::value_ptr(projView));
}

void ShadowMap2D::DrawDepthDebug(unsigned int shaderID, ShadowSystem& shadowSystem)
{
    // ShaderManager::Activate(shaderID);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    // glDrawElements(GL_TRIANGLES, sizeof(squareIndices) / sizeof(GLuint), GL_UNSIGNED_INT, squareIndices);
}