#include "ShadowMapCube.h"

#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Globals.h"

ShadowMapCube::ShadowMapCube(GLuint layerIndex, glm::vec3 lightPos, float zNear, float zFar)
    : ShadowCaster(LightType::POINT, layerIndex), farPlane(zFar)
{
    proj = glm::perspective(glm::radians(90.0f), 1.0f, zNear, farPlane);
    setView(lightPos, glm::vec3(0.0f));
}

void ShadowMapCube::setView(glm::vec3 newPosition, glm::vec3 newDirection)
{
    for (int i = 0; i < 6; i++)
        shadowMatrices[i] = proj * glm::lookAt(newPosition, newPosition + cubeFaces[i].dir, cubeFaces[i].up);
}

void ShadowMapCube::BeginDepthPass(ShaderID shaderID, glm::vec3 lightPos)
{
    ShaderManager::Activate(shaderID);

    for (int i = 0; i < 6; i++)
        glUniformMatrix4fv(ShaderManager::getLoc(shaderID, "shadowMatrices[" + std::to_string(i) + "]"), 1, GL_FALSE, glm::value_ptr(shadowMatrices[i]));

    glUniform3f(ShaderManager::getLoc(shaderID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1f(ShaderManager::getLoc(shaderID, "farPlane"), farPlane);
    glUniform1i(ShaderManager::getLoc(shaderID, "lightLayerOffset"), layerIndex * 6);
}

void ShadowMapCube::ExportUniformsTo(ShaderID shaderID, int lightIndex, glm::vec3 lightPos, glm::vec3 lightDirection, glm::vec3 lightColor)
{
    ShaderManager::Activate(shaderID);
    glUniform1i(ShaderManager::getLoc(shaderID, "pointLayerIndex[" + std::to_string(lightIndex) + "]"), layerIndex);
    glUniform1f(ShaderManager::getLoc(shaderID, "pointFarPlane[" + std::to_string(lightIndex) + "]"), farPlane);
    glUniform3fv(ShaderManager::getLoc(shaderID, "pointLightPos[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(lightPos));
    glUniform3fv(ShaderManager::getLoc(shaderID, "pointLightColor[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(lightColor));
}
