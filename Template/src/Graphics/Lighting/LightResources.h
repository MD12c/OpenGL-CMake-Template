#ifndef SHADOW_SYSTEM_CLASS_H
#define SHADOW_SYSTEM_CLASS_H

#include "glad/glad.h"

#include "Globals.h"
#include "..\Shaders\ShaderManager.h"
#include "../Models/Model.h"
#include "Light.h"

class LightResources
{
private:
    int nextDirLayer   = 0;
    int nextSpotLayer  = 0;
    int nextPointLayer = 0;

    float      zNear, zFar;
    Model      icoSphere;
    MaterialID materialSphere;
    MaterialID material2D;
    MaterialID materialCube;

    void  genTexture(GLenum type, GLsizei maxDepth, GLuint* shadowMapTexture);
    GLint RegisterCaster(LightType type);

public:
    std::vector<Light> lights;

    GLuint shadowMapDirArray;
    GLuint shadowMapSpotArray;
    GLuint shadowMapPointArray;

    GLuint frameBuf2D;
    GLuint frameBufCube;

    LightResources(float zNear, float zFar);
    ~LightResources();

    void BindShadowTextures(ShaderID shaderID) const;
    void DrawLightSpheres(ShaderID shaderID) const;
    void ExportUniformsTo(ShaderID shaderID) const;
    void ShadowPass(Renderer* renderer, const std::vector<Model>& models, Transform transform) const;

    void addDirectionLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top);
    void addSpotLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone);
    void addPointLight(glm::vec3 lightPos, glm::vec3 lightColor);
};

#endif