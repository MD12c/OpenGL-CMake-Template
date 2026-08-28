#ifndef SHADOW_MAP_CLASS_H
#define SHADOW_MAP_CLASS_H

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../Shaders/ShaderManager.h"
#include "ShadowCaster.h"

class ShadowMap2D : public ShadowCaster
{
private:
    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    // Spot Light only
    GLfloat innerCone;
    GLfloat outerCone;

public:
    ShadowMap2D(GLuint layerIndex, glm::vec3 lightPos, glm::vec3 direction, float left, float right, float bottom, float top, float zNear, float zFar);
    ShadowMap2D(GLuint layerIndex, glm::vec3 lightPos, glm::vec3 direction, float fovDeg, float innerCone, float outerCone, float zNear, float zFar);

    void setView(glm::vec3 newPosition, glm::vec3 newDirection) override;
    void BeginDepthPass(ShaderID shaderID, glm::vec3 lightPos) override;
    void ExportUniformsTo(ShaderID shaderID, int lightIndex, glm::vec3 lightPos, glm::vec3 lightDirection, glm::vec3 lightColor) override;
};

#endif