#include "ShadowCaster.h"

#include <stdexcept>

unsigned int ShadowCaster::getShaderIDfromType(LightType type)
{
    if (type == DIRECTION)
        return ShaderManager::IDs.shadowMap2D;
    else if (type == SPOT)
        return ShaderManager::IDs.shadowMap2D;
    else if (type == POINT)
        return ShaderManager::IDs.shadowMapCube;
    else
        throw std::runtime_error("[ERROR] Invalid light type");
};