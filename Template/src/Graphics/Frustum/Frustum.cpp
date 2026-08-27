#include "Frustum.h"

#include "Globals.h"

Frustum::Frustum()
{
}

void Frustum::setFrustumPlanes(const glm::mat4& vp)
{
    leftFace.normal.x = vp[0][3] + vp[0][0];
    leftFace.normal.y = vp[1][3] + vp[1][0];
    leftFace.normal.z = vp[2][3] + vp[2][0];
    leftFace.distance = vp[3][3] + vp[3][0];

    rightFace.normal.x = vp[0][3] - vp[0][0];
    rightFace.normal.y = vp[1][3] - vp[1][0];
    rightFace.normal.z = vp[2][3] - vp[2][0];
    rightFace.distance = vp[3][3] - vp[3][0];

    bottomFace.normal.x = vp[0][3] + vp[0][1];
    bottomFace.normal.y = vp[1][3] + vp[1][1];
    bottomFace.normal.z = vp[2][3] + vp[2][1];
    bottomFace.distance = vp[3][3] + vp[3][1];

    topFace.normal.x = vp[0][3] - vp[0][1];
    topFace.normal.y = vp[1][3] - vp[1][1];
    topFace.normal.z = vp[2][3] - vp[2][1];
    topFace.distance = vp[3][3] - vp[3][1];

    nearFace.normal.x = vp[0][3] + vp[0][2];
    nearFace.normal.y = vp[1][3] + vp[1][2];
    nearFace.normal.z = vp[2][3] + vp[2][2];
    nearFace.distance = vp[3][3] + vp[3][2];

    farFace.normal.x = vp[0][3] - vp[0][2];
    farFace.normal.y = vp[1][3] - vp[1][2];
    farFace.normal.z = vp[2][3] - vp[2][2];
    farFace.distance = vp[3][3] - vp[3][2];

    for (auto& plane : planes)
    {
        float len = glm::length(plane.normal);
        plane.normal /= len;
        plane.distance /= len;
    }
}

bool Frustum::sphereInFrustum(const BoundingSphere& sphere) const
{
    for (const auto& plane : planes)
    {
        if (plane.signedDistance(sphere.center) < -sphere.radius)
            return false;
    }
    return true;
}