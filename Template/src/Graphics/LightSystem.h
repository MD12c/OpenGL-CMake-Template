#ifndef LIGHT_SYSTEM_CLASS_H
#define LIGHT_SYSTEM_CLASS_H

#include <vector>

#include "Globals.h"
#include "Models/Model.h"
#include "FrameBuffers/ShadowMaps/ShadowCaster.h"

class ShadowSystem;

class LightSystem
{
private:
    std::unique_ptr<ShadowSystem> shadowSystem;

    struct Light
    {
        std::unique_ptr<ShadowCaster> caster = nullptr;
        LightType                     type;
        glm::vec3                     pos, dir, color;

        Light(LightType type, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, std::unique_ptr<ShadowCaster> caster)
            : type(type), pos(lightPos), dir(direction), color(lightColor), caster(std::move(caster)) {}

        void updateCaster(glm::vec3 newPos, glm::vec3 newDirection)
        {
            pos = newPos;
            dir = newDirection;
            if (caster) caster->setView(pos, dir);
        }
        void setPosition(glm::vec3 newPos) { updateCaster(newPos, dir); }
        void setDirection(glm::vec3 newDirection) { updateCaster(pos, newDirection); }
        void setColor(glm::vec3 newColor) { color = newColor; }
    };
    std::vector<Light> lights;

    float zNear, zFar;

public:
    LightSystem(float zNear, float zFar);

    void ExportUniforms(unsigned int shaderID);
    void ShadowPass(Model& model);

    unsigned int getShaderIDfromType(LightType type);

    void addLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top);
    void addLight(glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone);
    void addLight(glm::vec3 lightPos, glm::vec3 lightColor);

    inline glm::vec3 getLightPos(int index) { return lights.at(index).pos; }
    inline glm::vec3 getLightDir(int index) { return lights.at(index).dir; }
    inline glm::vec3 getLightColor(int index) { return lights.at(index).color; }

    inline void setLightPos(int index, glm::vec3 pos) { lights.at(index).setPosition(pos); }
    inline void setLightDir(int index, glm::vec3 dir) { lights.at(index).setDirection(dir); }
    inline void setLightColor(int index, glm::vec3 color) { lights.at(index).setColor(color); }
};

#endif