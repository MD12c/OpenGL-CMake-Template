#ifndef SHADOW_MAP_CUBE_CLASS_H
#define SHADOW_MAP_CUBE_CLASS_H

#include "../Shaders/Shader.h"
#include "ShadowCaster.h"

class ShadowMapCube : public ShadowCaster
{
private:
    const float farPlane;

    glm::mat4 proj = glm::mat4(1.0f);

    struct CubeFace
    {
        glm::vec3 dir;
        glm::vec3 up;
    };

    static constexpr CubeFace cubeFaces[6] = {
        { { 1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
        { { -1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
        { { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
        { { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
        { { 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
        { { 0.0f, 0.0f, -1.0f }, { 0.0f, -1.0f, 0.0f } },
    };

public:
    glm::mat4 shadowMatrices[6];

    ShadowMapCube(GLuint layerIndex, glm::vec3 lightPos, float zNear, float zFar);

    void setView(glm::vec3 newPosition, glm::vec3 newDirection) override;
    void BeginDepthPass(ShaderID shaderID, glm::vec3 lightPos) override;
    void ExportUniformsTo(ShaderID shaderID, int lightIndex, glm::vec3 lightPos, glm::vec3 lightDirection, glm::vec3 lightColor) override;
};

#endif