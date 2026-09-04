#version 460 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BloomColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;  // fragment position
in vec3 Tangent;

uniform vec3 camPos;

uniform sampler2D   brdfLUT;
uniform sampler2D   albedo0;
uniform sampler2D   ao0;
uniform sampler2D   metalicRoughness0;
uniform sampler2D   normal0;
uniform sampler2D   displacement0;
uniform samplerCube irradiance0;
uniform samplerCube prefilteredmap0;

uniform vec3  albedoColor;  // if no useTexture
uniform float roughness;    // if no useRoughness
uniform float metalic;      // if no useMetalic

uniform bool useTexture;
uniform bool useAO;
uniform bool useRoughness;
uniform bool useMetalic;
uniform bool useNormal;
uniform bool useDisplacement;
uniform bool useIBL;

// #define MAX_DIR_LIGHTS 8
// #define MAX_SPOT_LIGHTS 8
// #define MAX_POINT_LIGHTS 8

uniform sampler2DArray dirShadowMaps;
uniform sampler2DArray spotShadowMaps;
uniform samplerCubeArray pointShadowMaps;

layout(std140) uniform DirLightsBlock
{
    vec4  dirLightDirection[8];
    vec4  dirLightColor[8];
    mat4  dirShadowMatrix[8];
    ivec4 dirLayerIndex[8];
    int   numDirLights;
};
// usage: dirLightDirection[i].xyz, dirLayerIndex[i].x

layout(std140) uniform SpotLightsBlock
{
    vec4  spotLightInnerCone[8];
    vec4  spotLightOuterCone[8];
    vec4  spotLightPos[8];
    vec4  spotLightDirection[8];
    vec4  spotLightColor[8];
    mat4  spotShadowMatrix[8];
    ivec4 spotLayerIndex[8];
    int   numSpotLights;
};
// usage: spotLightInnerCone[i].x, spotLightPos[i].xyz, spotLayerIndex[i].x

layout(std140) uniform PointLightsBlock
{
    vec4  pointLightPos[8];
    vec4  pointLightColor[8];
    vec4  pointFarPlane[8];
    ivec4 pointLayerIndex[8];
    int   numPointLights;
};
// usage: pointLightPos[i].xyz, pointFarPlane[i].x, pointLayerIndex[i].x

#define PI 3.14159265359

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

float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom       = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1  = GeometrySchlickGGX(NdotV, k);
    float ggx2  = GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.001 - cosTheta, 5.0);
}

vec3 direcLight(int i, vec3 lightDir, vec3 N)
{
    vec4  fragPosLight = dirShadowMatrix[i] * vec4(crntPos, 1.0);
    float shadow       = 0.0f;
    vec3  lightCoords  = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        lightCoords        = (lightCoords + 1.0f) / 2.0f;  // [-1, 1] range to [0, 1]
        float currentDepth = lightCoords.z;
        float bias         = max(0.0025f * (1.0f - dot(N, normalize(lightDir))), 0.0005f);

        int  sampleRadius = 2;
        vec2 pixelSize    = 1.0 / textureSize(dirShadowMaps, 0).xy;
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(dirShadowMaps, vec3(lightCoords.xy + vec2(x, y) * pixelSize, float(dirLayerIndex[i].x))).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    return (1.0f - shadow) * dirLightColor[i].xyz;
    // return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

vec3 spotLight(int i, vec3 surfaceToLightPos, vec3 N)
{
    vec4  fragPosLight = spotShadowMatrix[i] * vec4(crntPos, 1.0);
    float angle        = dot(normalize(spotLightDirection[i].xyz), normalize(-surfaceToLightPos));
    float inten        = clamp((angle - spotLightOuterCone[i].x) / (spotLightInnerCone[i].x - spotLightOuterCone[i].x), 0.0f, 1.0f);

    float shadow      = 0.0f;
    vec3  lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        lightCoords        = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        float bias         = max(0.0025f * (1.0f - dot(N, normalize(surfaceToLightPos))), 0.00005f);

        int  sampleRadius = 1;
        vec2 pixelSize    = 1.0 / textureSize(spotShadowMaps, 0).xy;
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(spotShadowMaps, vec3(lightCoords.xy + vec2(x, y) * pixelSize, float(spotLayerIndex[i].x))).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    return ((1.0f - shadow) * inten) * spotLightColor[i].xyz;
    // return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

vec3 pointLight(int i, vec3 surfaceToLightPos, vec3 N)
{
    float dist = length(surfaceToLightPos);
    if (dist < 0.0001) dist = 0.0001;
    float quadratic = 0.0075f;
    float linear    = 0.045f;
    float constant  = 1.0f;
    float inten     = 1.0f / (quadratic * dist * dist + linear * dist + constant);

    float shadow = 0.0f;
    float bias   = max(0.0025f * (1.0f - dot(N, normalize(surfaceToLightPos))), 0.05f);

    int   sampleRadius = 1;  // half the width of a square Smoothens out the shadows
    float offset       = 0.02f;
    for (int z = -sampleRadius; z <= sampleRadius; z++)
    {
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(pointShadowMaps, vec4(-surfaceToLightPos + vec3(x, y, z) * offset, float(pointLayerIndex[i].x))).r;
                closestDepth *= pointFarPlane[i].x;
                if (dist > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
    }
    // Average shadow
    shadow /= pow((sampleRadius * 2 + 1), 3);

    return ((1.0f - shadow) * inten) * pointLightColor[i].xyz;
    // return vec3(shadow);  // for debugging shadows (shows shadow regions in white)
}

vec3 Fr(vec3 crntAlbedoColor, float crntRoughness, float crntMetalic, vec3 F0, vec3 Wo, vec3 Wi, vec3 N, vec3 HalfWay)
{
    vec3  Flambert = crntAlbedoColor / PI;
    float cosTheta = max(dot(HalfWay, Wo), 0.0);
    vec3  Fresnel  = fresnelSchlick(cosTheta, F0);
    vec3  kd       = (vec3(1.0f) - Fresnel) * (1.0 - crntMetalic);

    float alpha             = max(crntRoughness * crntRoughness, 0.02);
    float k                 = (crntRoughness + 1.0) * (crntRoughness + 1.0) / 8.0;
    vec3  cookTorranceNum   = DistributionGGX(N, HalfWay, alpha) * GeometrySmith(N, Wo, Wi, k) * Fresnel;
    float cookTorranceDenum = 4 * max(dot(Wo, N), 0.02) * max(dot(Wi, N), 0.02);
    vec3  cookTorrance      = cookTorranceNum / cookTorranceDenum;

    return kd * Flambert + cookTorrance;
}

void main()
{
    const vec2 UVs = getUVs();

    float crntMetalic     = useMetalic ? texture(metalicRoughness0, UVs).b : metalic;
    float crntRoughness   = useRoughness ? texture(metalicRoughness0, UVs).g : roughness;
    vec4  crntAlbedoColor = useTexture ? texture(albedo0, UVs) : vec4(albedoColor, 1.0f);

    if (crntAlbedoColor.a < 0.1f) discard;

    if (length(camPos - crntPos) < 0.01f)
        discard;

    const vec3 Wo  = normalize(camPos - crntPos);  // view dir
    vec3       sum = vec3(0.0f);                   // PBR sum
    const vec3 N   = getNormal(UVs);               // normal

    vec3 F0 = vec3(0.04);
    F0      = mix(F0, crntAlbedoColor.xyz, crntMetalic);

    // IBL
    vec3 diffuse  = vec3(0.1f) * crntAlbedoColor.xyz;
    vec3 specular = vec3(0.0f);
    vec3 kd       = 1.0 - fresnelSchlick(max(dot(N, Wo), 0.0), F0);
    if (useIBL)
    {
        diffuse = texture(irradiance0, N).rgb * crntAlbedoColor.xyz;

        const float MAX_REFLECTION_LOD = 4.0;
        vec3        R                  = reflect(-Wo, N);
        vec3        prefilteredColor   = textureLod(prefilteredmap0, R, crntRoughness * MAX_REFLECTION_LOD).rgb;
        vec2        envBRDF            = texture(brdfLUT, vec2(max(dot(N, Wo), 0.0), crntRoughness)).rg;
        specular                       = prefilteredColor * (F0 * envBRDF.x + envBRDF.y);
    }

    const float ao      = useAO ? texture(ao0, UVs).r : 0.2;
    vec3        ambient = (kd * diffuse + specular) * ao;

    for (int i = 0; i < numPointLights; i++)
    {
        const vec3 toLight = pointLightPos[i].xyz - crntPos;  // light vec
        const vec3 Wi      = normalize(toLight);          // light dir
        const vec3 HalfWay = normalize(Wi + Wo);
        sum += Fr(crntAlbedoColor.xyz, crntRoughness, crntMetalic, F0, Wo, Wi, N, HalfWay) * pointLight(i, toLight, N) * max(dot(N, Wi), 0.0);
    }

    for (int i = 0; i < numSpotLights; i++)
    {
        const vec3 toLight = spotLightPos[i].xyz - crntPos;  // light vec
        const vec3 Wi      = normalize(toLight);             // light dir
        const vec3 HalfWay = normalize(Wi + Wo);
        sum += Fr(crntAlbedoColor.xyz, crntRoughness, crntMetalic, F0, Wo, Wi, N, HalfWay) * spotLight(i, toLight, N) * max(dot(N, Wi), 0.0);
    }

    for (int i = 0; i < numDirLights; i++)
    {
        const vec3 Wi      = normalize(-dirLightDirection[i].xyz);  // light dir
        const vec3 HalfWay = normalize(Wi + Wo);
        sum += Fr(crntAlbedoColor.xyz, crntRoughness, crntMetalic, F0, Wo, Wi, N, HalfWay) * direcLight(i, Wi, N) * max(dot(N, Wi), 0.0);
    }

    FragColor = vec4(ambient, 1.0f) + vec4(sum, 1.0f);

    // FragColor = vec4(spotLight(0, spotLightPos[0].xyz - crntPos, N), 1.0f);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 2.0f)
        BloomColor = vec4(FragColor.rgb, 1.0f);
    else
        BloomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}