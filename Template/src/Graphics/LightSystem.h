#ifndef LIGHT_SYSTEM_CLASS_H
#define LIGHT_SYSTEM_CLASS_H

#include <vector>

#include "Models/Model.h"

class ShadowCaster;
class ShadowSystem;

class LightSystem
{
private:
    std::unique_ptr<ShadowSystem> shadowSystem;
    float zNear, zFar;

public:
    std::vector<std::unique_ptr<ShadowCaster>> shadowCasters;

    enum LightType
    {
        DIRECTION,
        SPOT,
        POINT
    };

    LightSystem(float zNear, float zFar);

    void ExportUniforms(unsigned int shaderID);
    void ShadowPass(Model& model);

    void addLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top);
    void addLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone);
    void addLight(glm::vec3 lightPos, glm::vec3 lightColor);
};

#endif