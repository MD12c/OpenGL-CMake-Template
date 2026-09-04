#ifndef SHADOW_SYSTEM_CLASS_H
#define SHADOW_SYSTEM_CLASS_H

#include "glad/glad.h"

#include "Globals.h"
#include "../Models/Model.h"
#include "../GLObjects/UBO.h"
#include "SpotLight.h"
#include "DirectionLight.h"
#include "PointLight.h"

class LightResources
{
private:
    float      zNear, zFar;
    MaterialID materialSphere;
    MaterialID material2D;
    MaterialID materialCube;

    UBO pointUBO;
    UBO dirUBO;
    UBO spotUBO;

    void genTexture(GLenum type, GLsizei maxDepth, GLuint* shadowMapTexture);

public:
    std::vector<PointLight>     pointLights;
    std::vector<DirectionLight> directionLights;
    std::vector<SpotLight>      spotLights;

    GLuint shadowMapDirArray;
    GLuint shadowMapSpotArray;
    GLuint shadowMapPointArray;

    GLuint frameBuf2D;
    GLuint frameBufCube;

    LightResources(float zNear, float zFar);
    ~LightResources();

    void BindShadowTextures(ShaderID shaderID) const;
    void DrawLightPlanes(ShaderID shaderID, glm::vec3 pos, glm::vec3 direction, glm::vec3 color) const;
    void DrawLightSpheres(ShaderID shaderID, glm::vec3 pos, glm::vec3 color) const;
    void ExportUniformsTo(ShaderID shaderID) const;
    void ShadowPass(Renderer* renderer, const std::vector<Model>& models, Transform transform) const;

    void addDirectionLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top);
    void addSpotLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone);
    void addPointLight(glm::vec3 lightPos, glm::vec3 lightColor);
};

#endif