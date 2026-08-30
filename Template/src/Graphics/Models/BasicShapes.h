#ifndef BASIC_SHAPES_H
#define BASIC_SHAPES_H

#include "Model.h"

class BasicShapes
{
public:
    Model icoSphere;
    Mesh  plane;

    BasicShapes();
    ~BasicShapes() = default;
};

#endif