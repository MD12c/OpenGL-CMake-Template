#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include "../Shaders/Shader.h"

class Material
{
protected:
    static void checkAndLoad(std::shared_ptr<Texture> texPtr,
                             const std::string&       name,
                             ShaderID                 shaderID)
    {
        if (texPtr)
            texPtr->texUnit(shaderID, name);
        else
            Texture::UnbindAt(shaderID, name);
    }

public:
    MaterialID ID;
    ShaderID   shaderID;

    Material(int ID, ShaderID shaderID) : ID(ID), shaderID(shaderID) {}
    ~Material() = default;

    virtual void Apply() const { Shader::Activate(shaderID); };
};

#endif