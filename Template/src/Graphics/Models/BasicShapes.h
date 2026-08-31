#ifndef BASIC_SHAPES_H
#define BASIC_SHAPES_H

#include "Model.h"

class BasicShapes
{
public:
    Model icoSphere;
    Model plane;

    BasicShapes();
    ~BasicShapes() = default;
};

#endif