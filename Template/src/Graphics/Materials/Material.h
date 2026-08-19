#ifndef MATERIAL_CLASS_H
#define MATERIAL_CLASS_H

class Material
{
public:
    int ID;
    Material(int ID);
    ~Material()                            = default;
    virtual void Apply(int shaderID) const = 0;
};

#endif