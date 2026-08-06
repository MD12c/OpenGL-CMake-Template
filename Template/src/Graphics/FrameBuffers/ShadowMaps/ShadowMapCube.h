#ifndef SHADOW_MAP_CUBE_CLASS_H
#define SHADOW_MAP_CUBE_CLASS_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "ShadowCaster.h"

class ShadowMapCube : public ShadowCaster
{
private:
    GLuint shadowCubeTexture;
    float  farPlane;

public:
    glm::mat4 shadowMatrices[6];
    glm::vec3 lightPos;
    
    ShadowMapCube(glm::vec3 lightPos, float near, float far);

    void BeginDepthPass(const std::string& shaderName) override;
    void EndDepthPass() override;
    void ExportUniformsTo(const std::string& shaderName, GLuint textureSlot) override;
    void DrawDepthDebug(const std::string& shaderName) override;
};

#endif