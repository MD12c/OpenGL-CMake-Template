#ifndef LIGHT_SYSTEM_CLASS_H
#define LIGHT_SYSTEM_CLASS_H

#include <vector>

#include "Globals.h"
#include "..\Models\Model.h"
#include "ShadowCaster.h"

class ShadowSystem;

class LightSystem
{
private:
    std::unique_ptr<ShadowSystem> shadowSystem = nullptr;

    float      zNear, zFar;
    Model      icoSphere;
    MaterialID materialSphere;
    MaterialID material2D;
    MaterialID materialCube;

public:
    struct Light
    {
    private:
        LightType type;
        glm::vec3 pos, dir, color;

    public:
        std::unique_ptr<ShadowCaster> caster = nullptr;

        Light(LightType type, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, std::unique_ptr<ShadowCaster> caster)
            : type(type), pos(lightPos), dir(direction), color(lightColor), caster(std::move(caster)) {}

        void updateCaster(glm::vec3 newPos, glm::vec3 newDirection)
        {
            pos = newPos;
            dir = newDirection;
            if (caster) caster->setView(pos, dir);
        }
        void      setPosition(glm::vec3 newPos) { updateCaster(newPos, dir); }
        void      setDirection(glm::vec3 newDirection) { updateCaster(pos, newDirection); }
        void      setColor(glm::vec3 newColor) { color = newColor; }
        LightType getType() const { return type; }
        glm::vec3 getPosition() const { return pos; }
        glm::vec3 getDirection() const { return dir; }
        glm::vec3 getColor() const { return color; }
    };

    LightSystem(float zNear, float zFar);

    void ExportUniforms(ShaderID shaderID, const std::vector<Light>& lights) const;
    void ShadowPass(const std::vector<Model>& models, const std::vector<Light>& lights, Transform transform = {{}, {}, {}}) const;
    void DrawLightSpheres(ShaderID shaderID, const std::vector<Light>& lights) const;

    void addDirectionLight(std::vector<Light>& lights, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float left, float right, float bottom, float top);
    void addSpotLight(std::vector<Light>& lights, glm::vec3 lightPos, glm::vec3 direction, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone);
    void addPointLight(std::vector<Light>& lights, glm::vec3 lightPos, glm::vec3 lightColor);
};

#endif