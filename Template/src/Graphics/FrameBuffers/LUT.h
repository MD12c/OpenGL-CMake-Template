#ifndef LUT_CLASS_H
#define LUT_CLASS_H

#include "../GLObjects/Texture.h"

class LUT
{
private:
    Texture tex;

public:
    LUT();
    void ExportUniformsTo(ShaderIDs shaderID);
    void Draw(ShaderIDs shaderID);
};

#endif