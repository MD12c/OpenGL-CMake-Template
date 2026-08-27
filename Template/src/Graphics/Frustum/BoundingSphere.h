#ifndef BOUNDING_SPHERE_STRUCT_H
#define BOUNDING_SPHERE_STRUCT_H

#include "glm/glm.hpp"

#include "../GLObjects/VBO.h"

struct BoundingSphere
{
    glm::vec3 center;
    float     radius;
};

inline BoundingSphere computeBoundingSphere(const std::vector<Vertex>& vertices)
{
    glm::vec3 minP(std::numeric_limits<float>::max());
    glm::vec3 maxP(std::numeric_limits<float>::lowest());

    for (const auto& v : vertices)
    {
        minP = glm::min(minP, v.position);
        maxP = glm::max(maxP, v.position);
    }

    glm::vec3 center = (minP + maxP) * 0.5f;

    float maxDistSq = 0.0f;
    for (const auto& v : vertices)
    {
        float distSq = glm::dot(v.position - center, v.position - center);
        maxDistSq    = std::max(maxDistSq, distSq);
    }

    return { center, std::sqrt(maxDistSq) };
}

#endif