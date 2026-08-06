#ifndef SHADOW_MAP_CLASS_H
#define SHADOW_MAP_CLASS_H

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShadowCaster.h"

class ShadowMap2D : public ShadowCaster
{
private:
    GLuint shadowMapTexture;
    void   genTexture();

public:
    glm::vec3 lightPos;
    glm::vec3 direction;
    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    ShadowMap2D(glm::vec3 lightPos, glm::vec3 direction, float left, float right, float bottom, float top, float zNear, float zFar);
    ShadowMap2D(glm::vec3 lightPos, glm::vec3 direction, float fovDeg, float near, float far);
    void setView(glm::vec3 newPosition, glm::vec3 newDirection);

    void BeginDepthPass(const std::string& shaderName) override;
    void EndDepthPass() override;
    void ExportUniformsTo(const std::string& shaderName, GLuint textureSlot) override;
    void DrawDepthDebug(const std::string& shaderName) override;
};

#endif