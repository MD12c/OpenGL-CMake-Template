#include "ShadowCaster.h"

#include <stdexcept>

unsigned int ShadowCaster::getShaderIDfromType(LightSystem::LightType type)
{
    if (type == LightSystem::LightType::DIRECTION)
        return ShaderManager::IDs.shadowMap2D;
    else if (type == LightSystem::LightType::SPOT)
        return ShaderManager::IDs.shadowMap2D;
    else if (type == LightSystem::LightType::POINT)
        return ShaderManager::IDs.shadowMapCube;
    else
        throw std::runtime_error("[ERROR] Invalid light type");
};