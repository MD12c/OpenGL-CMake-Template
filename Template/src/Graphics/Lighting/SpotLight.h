#ifndef SPOT_LIGHT_CLASS_H
#define SPOT_LIGHT_CLASS_H

#include "glad/glad.h"
#include "Globals.h"
#include "../Shaders/Shader.h"
#include "glm/gtc/type_ptr.hpp"

class SpotLight
{
private:
    glm::vec3 pos, dir, color;

    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

public:
    GLuint layerIndex;

    inline static bool updateUBOdata = true;

    struct SpotLightsUBO
    {
        glm::vec4  innerCone[8];
        glm::vec4  outerCone[8];
        glm::vec4  pos[8];
        glm::vec4  direction[8];
        glm::vec4  color[8];
        glm::mat4  shadowMatrix[8];
        glm::ivec4 layerIndex[8];
        int        numSpotLights;
        int        _pad[3];
    };

    GLfloat innerCone;
    GLfloat outerCone;

    SpotLight(GLuint layerIndex, glm::vec3 lightPos, glm::vec3 lightDirection, glm::vec3 lightColor, float fovDeg, float innerCone, float outerCone, float zNear, float zFar)
        : layerIndex(layerIndex), innerCone(innerCone), outerCone(outerCone), pos(lightPos), dir(lightDirection), color(lightColor)
    {
        proj = glm::perspective(glm::radians(fovDeg), (float)SHADOW_MAP_WIDTH / (float)SHADOW_MAP_HEIGHT, zNear, zFar);
        view = glm::lookAt(lightPos, lightPos + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void BeginDepthPass(ShaderID shaderID) const
    {
        Shader::Activate(shaderID);

        glUniformMatrix4fv(Shader::getLoc(shaderID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(Shader::getLoc(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    }

    void updateView(glm::vec3 newPos, glm::vec3 newDirection)
    {
        pos  = newPos;
        dir  = newDirection;
        view = glm::lookAt(newPos, newPos + newDirection, glm::vec3(0.0f, 1.0f, 0.0f));
        updateUBOdata = true;
    }

    void      setPosition(glm::vec3 newPos) { updateView(newPos, dir); }
    void      setDirection(glm::vec3 newDirection) { updateView(pos, newDirection); }
    void      setColor(glm::vec3 newColor) { color = newColor; }
    glm::vec3 getPosition() const { return pos; }
    glm::vec3 getDirection() const { return dir; }
    glm::vec3 getColor() const { return color; }
    glm::mat4 getShadowMatrix() const { return proj * view; }
};

#endif