#ifndef SHADOW_MAP_CUBE_CLASS_H
#define SHADOW_MAP_CUBE_CLASS_H

#include <optional>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "ShadowCaster.h"

class ShadowMapCube : public ShadowCaster
{
private:
    GLuint shadowCubeTexture;
    float  farPlane;

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

    std::optional<GLuint> debugTexture = {};

public:
    glm::mat4 shadowMatrices[6];
    glm::vec3 lightPos;
    glm::mat4 proj;

    ShadowMapCube(glm::vec3 lightPos, float near, float far);
    void setView(glm::vec3 newPosition);

    void BeginDepthPass(const std::string& shaderName) override;
    void EndDepthPass() override;
    void ExportUniformsTo(const std::string& shaderName, GLuint textureSlot) override;
    void DrawDepthDebug(const std::string& shaderName, int faceIndex);
};

#endif