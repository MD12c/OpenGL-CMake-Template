#ifndef LIGHT_STRUCT_H
#define LIGHT_STRUCT_H

#include "Globals.h"
#include "ShadowCaster.h"

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

#endif