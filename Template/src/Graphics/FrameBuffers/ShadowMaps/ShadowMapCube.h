#ifndef SHADOW_MAP_CUBE_CLASS_H
#define SHADOW_MAP_CUBE_CLASS_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "ShadowCaster.h"

class ShadowMapCube : public ShadowCaster
{
private:
    GLuint layerIndex;
    float  farPlane;

    glm::mat4 proj     = glm::mat4(1.0f);
    glm::vec3 lightPos = glm::vec3(1.0f);
    glm::vec3 color    = glm::vec3(1.0f);

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

    ShadowMapCube(GLuint layerIndex, glm::vec3 lightPos, glm::vec3 lightColor, float zNear, float zFar);

    void setView(glm::vec3 newPosition, glm::vec3 newDirection) override;
    void BeginDepthPass(unsigned int shaderID, ShadowSystem& shadowSystem) override;
    void ExportUniformsTo(unsigned int shaderID, int lightIndex) override;
    void DrawDepthDebug(unsigned int shaderID, int faceIndex);
};

#endif