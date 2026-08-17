#version 460 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;  // fragment position
in vec3 Tangent;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
uniform sampler2D displacement0;

uniform bool useTexture;
uniform bool useNormal;
uniform bool useDisplacement;

uniform vec3 camPos;

uniform float shininess;
uniform vec3  diffuseColor;

#define MAX_DIR_LIGHTS 2
#define MAX_SPOT_LIGHTS 4
#define MAX_POINT_LIGHTS 4

uniform int            numDirLights;
uniform vec3           dirLightDirection[MAX_DIR_LIGHTS];
uniform vec3           dirLightColor[MAX_DIR_LIGHTS];
uniform mat4           dirShadowMatrix[MAX_DIR_LIGHTS];
uniform int            dirLayerIndex[MAX_DIR_LIGHTS];
uniform sampler2DArray dirShadowMaps;

uniform int            numSpotLights;
uniform float          spotLightInnerCone[MAX_SPOT_LIGHTS];
uniform float          spotLightOuterCone[MAX_SPOT_LIGHTS];
uniform vec3           spotLightPos[MAX_SPOT_LIGHTS];
uniform vec3           spotLightDirection[MAX_SPOT_LIGHTS];
uniform vec3           spotLightColor[MAX_SPOT_LIGHTS];
uniform mat4           spotShadowMatrix[MAX_SPOT_LIGHTS];
uniform int            spotLayerIndex[MAX_SPOT_LIGHTS];
uniform sampler2DArray spotShadowMaps;

uniform int              numPointLights;
uniform vec3             pointLightPos[MAX_POINT_LIGHTS];
uniform vec3             pointLightColor[MAX_POINT_LIGHTS];
uniform float            pointFarPlane[MAX_POINT_LIGHTS];
uniform int              pointLayerIndex[MAX_POINT_LIGHTS];
uniform samplerCubeArray pointShadowMaps;

mat3 getTBN()
{
    vec3 N = normalize(Normal);
    vec3 T = normalize(Tangent - N * dot(Tangent, N));
    vec3 B = cross(T, N);
    return mat3(T, B, N);
}

vec3 getNormal(vec2 UVs)
{
    if (!useNormal)
        return normalize(Normal);

    mat3 TBN    = getTBN();
    vec3 mapped = texture(normal0, UVs).rgb * 2.0f - 1.0f;
    return normalize(TBN * mapped);
}

vec2 getUVs()
{
    if (!useDisplacement)
        return texCoord;

    mat3 TBN            = transpose(getTBN());
    vec3 viewDirTangent = normalize(TBN * (camPos - crntPos));

    float       heightScale       = 0.07f;
    const float minLayers         = 8.0f;
    const float maxLayers         = 64.0f;
    float       numLayers         = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDirTangent)));
    float       layerDepth        = 1.0f / numLayers;
    float       currentLayerDepth = 0.0f;

    vec2 S        = viewDirTangent.xy / viewDirTangent.z * heightScale;
    vec2 deltaUVs = S / numLayers;

    vec2  UVs                  = texCoord;
    float currentDepthMapValue = texture(displacement0, UVs).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        UVs += deltaUVs;
        currentDepthMapValue = texture(displacement0, UVs).r;
        currentLayerDepth += layerDepth;
    }

    vec2  prevTexCoords = UVs - deltaUVs;
    float afterDepth    = currentDepthMapValue - currentLayerDepth;
    float beforeDepth   = texture(displacement0, prevTexCoords).r - currentLayerDepth + layerDepth;
    float weight        = afterDepth / (afterDepth - beforeDepth);
    UVs                 = prevTexCoords * weight + UVs * (1.0f - weight);

    if (UVs.x > 1.0 || UVs.y > 1.0 || UVs.x < 0.0 || UVs.y < 0.0)
        discard;

    return UVs;
}

vec4 pointLight(int i, vec2 UVs)
{
    vec3 SurfaceToLightPos = pointLightPos[i] - crntPos;

    // intensity of light with respect to distance
    float dist = length(SurfaceToLightPos);
    if (dist < 0.0001) dist = 0.0001;
    float quadratic = 0.0075f;
    float linear    = 0.045f;
    float constant  = 1.0f;
    float inten     = 1.0f / (quadratic * dist * dist + linear * dist + constant);

    // diffuse lighting
    vec3  normal         = getNormal(UVs);
    vec3  lightDirection = normalize(SurfaceToLightPos);
    float diffuse        = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3  viewDirection = normalize(camPos - crntPos);
        vec3  halfwayVec    = normalize(viewDirection + lightDirection);
        float specAmount    = pow(max(dot(normal, halfwayVec), 0.0f), shininess);
        specular            = specAmount * specularLight;
    };

    float shadow       = 0.0f;
    vec3  fragToLight  = crntPos - pointLightPos[i];
    float currentDepth = length(fragToLight);
    float bias         = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);

    int   sampleRadius = 1;  // half the width of a square Smoothens out the shadows
    float offset       = 0.02f;
    for (int z = -sampleRadius; z <= sampleRadius; z++)
    {
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(pointShadowMaps, vec4(fragToLight + vec3(x, y, z) * offset, float(pointLayerIndex[i]))).r;
                closestDepth *= pointFarPlane[i];
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
    }
    // Average shadow
    shadow /= pow((sampleRadius * 2 + 1), 3);

    if (useTexture)
        return (texture(diffuse0, UVs) * (diffuse * (1.0f - shadow) * inten) + texture(specular0, UVs).r * specular * (1.0f - shadow) * inten) * vec4(pointLightColor[i], 1.0f);
    else
        return (vec4(diffuseColor, 1.0f) * (diffuse * (1.0f - shadow)) + vec4(diffuseColor, 1.0f) * specular * (1.0f - shadow)) * vec4(pointLightColor[i], 1.0f);
    // return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

vec4 direcLight(int i, vec2 UVs)
{
    vec4 fragPosLight = dirShadowMatrix[i] * vec4(crntPos, 1.0);

    vec3 lightDir = normalize(-dirLightDirection[i]);

    // diffuse lighting
    vec3  normal  = getNormal(UVs);
    float diffuse = max(dot(normal, lightDir), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3  viewDirection = normalize(camPos - crntPos);
        vec3  halfwayVec    = normalize(viewDirection + lightDir);
        float specAmount    = pow(max(dot(normal, halfwayVec), 0.0f), shininess);
        specular            = specAmount * specularLight;
    };

    float shadow      = 0.0f;
    vec3  lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        lightCoords        = (lightCoords + 1.0f) / 2.0f;  // [-1, 1] range to [0, 1]
        float currentDepth = lightCoords.z;
        float bias         = max(0.0025f * (1.0f - dot(normal, lightDir)), 0.0005f);

        int  sampleRadius = 1;
        vec2 pixelSize    = 1.0 / textureSize(dirShadowMaps, 0).xy;
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(dirShadowMaps, vec3(lightCoords.xy + vec2(x, y) * pixelSize, float(dirLayerIndex[i]))).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    if (useTexture)
        return (texture(diffuse0, UVs) * (diffuse * (1.0f - shadow)) + texture(specular0, UVs).r * specular * (1.0f - shadow)) * vec4(dirLightColor[i], 1.0f);
    else
        return (vec4(diffuseColor, 1.0f) * (diffuse * (1.0f - shadow)) + vec4(diffuseColor, 1.0f) * specular * (1.0f - shadow)) * vec4(dirLightColor[i], 1.0f);
    // return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

vec4 spotLight(int i, vec2 UVs)
{
    vec4 fragPosLight = spotShadowMatrix[i] * vec4(crntPos, 1.0);

    // diffuse lighting
    vec3  lightDir          = normalize(-spotLightDirection[i]);
    vec3  normal            = getNormal(UVs);
    vec3  surfaceToLightPos = normalize(spotLightPos[i] - crntPos);
    float diffuse           = max(dot(normal, surfaceToLightPos), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3  viewDirection = normalize(camPos - crntPos);
        vec3  halfwayVec    = normalize(viewDirection + surfaceToLightPos);
        float specAmount    = pow(max(dot(normal, halfwayVec), 0.0f), shininess);
        specular            = specAmount * specularLight;
    };

    // calculates the intensity of the crntPos based on its angle to the center of the light cone
    float angle = dot(-lightDir, -surfaceToLightPos);
    float inten = clamp((angle - spotLightOuterCone[i]) / (spotLightInnerCone[i] - spotLightOuterCone[i]), 0.0f, 1.0f);

    float shadow      = 0.0f;
    vec3  lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        lightCoords        = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        float bias         = max(0.0025f * (1.0f - dot(normal, surfaceToLightPos)), 0.00005f);

        int  sampleRadius = 1;
        vec2 pixelSize    = 1.0 / textureSize(spotShadowMaps, 0).xy;
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(spotShadowMaps, vec3(lightCoords.xy + vec2(x, y) * pixelSize, float(spotLayerIndex[i]))).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    if (useTexture)
        return (texture(diffuse0, UVs) * (diffuse * (1.0f - shadow) * inten) + texture(specular0, UVs).r * specular * (1.0f - shadow) * inten) * vec4(spotLightColor[i], 1.0f);
    else
        return (vec4(diffuseColor, 1.0f) * (diffuse * (1.0f - shadow)) + vec4(diffuseColor, 1.0f) * specular * (1.0f - shadow)) * vec4(spotLightColor[i], 1.0f);
    // return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

void main()
{
    vec2 UVs = getUVs();

    float ambient = 0.20f;
    vec4  result  = useTexture ? texture(diffuse0, UVs) * ambient : vec4(diffuseColor, 1.0f) * ambient;

    for (int i = 0; i < numDirLights; i++) result += direcLight(i, UVs);
    for (int i = 0; i < numSpotLights; i++) result += spotLight(i, UVs);
    for (int i = 0; i < numPointLights; i++) result += pointLight(i, UVs);

    FragColor = result;
    // FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0f)
        BloomColor = vec4(FragColor.rgb, 1.0f);
    else
        BloomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}