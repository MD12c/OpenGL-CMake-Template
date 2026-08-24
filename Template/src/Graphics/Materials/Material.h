#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

#include "../Shaders/ShaderManager.h"

class Material
{
public:
    int ID;

    Material(int ID) : ID(ID) {}
    ~Material() = default;

    virtual void                     Apply() const       = 0;
    virtual ShaderIDs getShaderID() const = 0;
};

#endif