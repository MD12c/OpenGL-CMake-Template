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

    glm::mat4 proj  = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(1.0f);

    // Spot Light only
    GLfloat innerCone;
    GLfloat outerCone;

public:
    glm::vec3 lightPos;
    glm::vec3 direction;

    ShadowMap2D(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top, float zNear, float zFar);
    ShadowMap2D(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone, float zNear, float zFar);
    ~ShadowMap2D();

    void setView(glm::vec3 newPosition, glm::vec3 newDirection) override;
    void BeginDepthPass(unsigned int shaderID) override;
    void EndDepthPass() override;
    void ExportUniformsTo(unsigned int shaderID, GLuint textureSlot, int lightIndex) override;
    void DrawDepthDebug(unsigned int shaderID);
};

#endif