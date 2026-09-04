#ifndef POINT_LIGHT_CLASS_H
#define POINT_LIGHT_CLASS_H

#include "../Shaders/Shader.h"
#include "Globals.h"

class PointLight
{
private:
    glm::vec3   pos, color;
    const float farPlane;

    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 shadowMatrices[6];

    struct CubeFace
    {
        glm::vec3 dir;
        glm::vec3 up;
    };

    static constexpr CubeFace cubeFaces[6] = {
        { { 1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
        { { -1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
        { { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
        { { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
        { { 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
        { { 0.0f, 0.0f, -1.0f }, { 0.0f, -1.0f, 0.0f } },
    };

public:
    GLuint layerIndex;

    inline static bool updateUBOdata = true;

    struct PointLightsUBO
    {
        glm::vec4  pos[8];
        glm::vec4  color[8];
        glm::vec4  farPlane[8];
        glm::ivec4 layerIndex[8];
        int        numPointLights;
        int        _pad[3];
    };

    PointLight(GLuint layerIndex, glm::vec3 lightPos, glm::vec3 lightColor, float zNear, float zFar)
        : layerIndex(layerIndex), farPlane(zFar), pos(lightPos), color(lightColor)
    {
        proj = glm::perspective(glm::radians(90.0f), 1.0f, zNear, farPlane);
        for (int i = 0; i < 6; i++)
            shadowMatrices[i] = proj * glm::lookAt(lightPos, lightPos + cubeFaces[i].dir, cubeFaces[i].up);
    }

    void BeginDepthPass(ShaderID shaderID) const
    {
        Shader::Activate(shaderID);

        for (int i = 0; i < 6; i++)
            glUniformMatrix4fv(Shader::getLoc(shaderID, "shadowMatrices[" + std::to_string(i) + "]"), 1, GL_FALSE, glm::value_ptr(shadowMatrices[i]));

        glUniform3f(Shader::getLoc(shaderID, "lightPos"), pos.x, pos.y, pos.z);
        glUniform1f(Shader::getLoc(shaderID, "farPlane"), farPlane);
        glUniform1i(Shader::getLoc(shaderID, "lightLayerOffset"), layerIndex * 6);
    }

    void setPosition(glm::vec3 newPos)
    {
        pos = newPos;
        for (int i = 0; i < 6; i++)
            shadowMatrices[i] = proj * glm::lookAt(newPos, newPos + cubeFaces[i].dir, cubeFaces[i].up);
            
        updateUBOdata = true;
    }
    void      setColor(glm::vec3 newColor) { color = newColor; }
    glm::vec3 getPosition() const { return pos; }
    glm::vec3 getColor() const { return color; }
    float     getFarPlane() const { return farPlane; }
};

#endif