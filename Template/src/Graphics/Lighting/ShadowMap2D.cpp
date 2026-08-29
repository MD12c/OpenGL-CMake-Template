#include "ShadowMap2D.h"

#include "Globals.h"

#include "LightResources.h"

ShadowMap2D::ShadowMap2D(GLuint layerIndex, glm::vec3 lightPos, glm::vec3 direction, float left, float right, float bottom, float top, float zNear, float zFar)
    : ShadowCaster(LightType::DIRECTION, layerIndex)
{
    proj = glm::ortho(left, right, bottom, top, zNear, zFar);
    view = glm::lookAt(lightPos, lightPos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

ShadowMap2D::ShadowMap2D(GLuint layerIndex, glm::vec3 lightPos, glm::vec3 direction, float fovDeg, float innerCone, float outerCone, float zNear, float zFar)
    : ShadowCaster(LightType::SPOT, layerIndex), innerCone(innerCone), outerCone(outerCone)
{
    proj = glm::perspective(glm::radians(fovDeg), (float)SHADOW_MAP_WIDTH / (float)SHADOW_MAP_HEIGHT, zNear, zFar);
    view = glm::lookAt(lightPos, lightPos + direction, glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowMap2D::setView(glm::vec3 newPosition, glm::vec3 newDirection)
{
    view = glm::lookAt(newPosition, newPosition + newDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowMap2D::BeginDepthPass(ShaderID shaderID, glm::vec3 lightPos)
{
    Shader::Activate(shaderID);

    glUniformMatrix4fv(Shader::getLoc(shaderID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(Shader::getLoc(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void ShadowMap2D::ExportUniformsTo(ShaderID shaderID, int lightIndex, glm::vec3 lightPos, glm::vec3 lightDirection, glm::vec3 lightColor)
{
    Shader::Activate(shaderID);

    std::string type;
    glm::mat4   projView = proj * view;

    if (lightType == LightType::DIRECTION)
        type = "dir";
    else if (lightType == LightType::SPOT)
    {
        type = "spot";
        glUniform3fv(Shader::getLoc(shaderID, type + "LightPos[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(lightPos));
        glUniform1f(Shader::getLoc(shaderID, type + "LightInnerCone[" + std::to_string(lightIndex) + "]"), innerCone);
        glUniform1f(Shader::getLoc(shaderID, type + "LightOuterCone[" + std::to_string(lightIndex) + "]"), outerCone);
    }
    else
        throw std::runtime_error("[ERROR] Invalid light type ShadowMap2D::ExportUniformsTo()");

    glUniform1i(Shader::getLoc(shaderID, type + "LayerIndex[" + std::to_string(lightIndex) + "]"), layerIndex);
    glUniform3fv(Shader::getLoc(shaderID, type + "LightDirection[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(lightDirection));
    glUniform3fv(Shader::getLoc(shaderID, type + "LightColor[" + std::to_string(lightIndex) + "]"), 1, glm::value_ptr(lightColor));
    glUniformMatrix4fv(Shader::getLoc(shaderID, type + "ShadowMatrix[" + std::to_string(lightIndex) + "]"), 1, GL_FALSE, glm::value_ptr(projView));
}
