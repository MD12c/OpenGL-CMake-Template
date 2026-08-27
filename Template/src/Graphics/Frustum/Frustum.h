#ifndef FRUSTUM_CLASS_H
#define FRUSTUM_CLASS_H

#include <array>

#include "Plane.h"
#include "BoundingSphere.h"

class Frustum
{
private:
    union
    {
        std::array<Plane, 6> planes;
        struct
        {
            Plane leftFace;
            Plane rightFace;

            Plane bottomFace;
            Plane topFace;

            Plane nearFace;
            Plane farFace;
        };
    };

public:
    Frustum();

    void setFrustumPlanes(const glm::mat4& vp);
    bool sphereInFrustum(const BoundingSphere& sphere) const;
};

#endif