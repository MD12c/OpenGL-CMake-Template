#ifndef PLANE_STRUCT_H
#define PLANE_STRUCT_H

#include "glm/glm.hpp"

struct Plane
{
    glm::vec3 normal   = { 0.0f, 1.0f, 0.0f };
    float     distance = 0.0f;

    float signedDistance(const glm::vec3& point) const
    {
        return glm::dot(normal, point) + distance;
    }
};

#endif