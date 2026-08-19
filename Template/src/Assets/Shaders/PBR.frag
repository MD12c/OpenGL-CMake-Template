#version 460 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BloomColor;

in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;  // fragment position
in vec3 Tangent;

uniform sampler2D albedo0;
uniform sampler2D ao0;
uniform sampler2D normal0;
uniform sampler2D displacement0;

uniform float roughness;  // float for now
uniform float metalic;    // float for now

uniform bool useTexture;
uniform bool useNormal;
uniform bool useDisplacement;

uniform vec3 camPos;

uniform vec3 diffuseColor;

#define MAX_DIR_LIGHTS 2
#define MAX_SPOT_LIGHTS 4
#define MAX_POINT_LIGHTS 4

uniform int              numPointLights;
uniform vec3             pointLightPos[MAX_POINT_LIGHTS];
uniform vec3             pointLightColor[MAX_POINT_LIGHTS];
uniform float            pointFarPlane[MAX_POINT_LIGHTS];
uniform int              pointLayerIndex[MAX_POINT_LIGHTS];
uniform samplerCubeArray pointShadowMaps;

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
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 pointLight(int i, vec3 P, vec3 Wi, vec3 N)
{
    float dist = length(Wi);
    if (dist < 0.0001) dist = 0.0001;
    float quadratic = 0.0075f;
    float linear    = 0.045f;
    float constant  = 1.0f;
    float inten     = 1.0f / (quadratic * dist * dist + linear * dist + constant);

    // float shadow      = 0.0f;
    // vec3  fragToLight = -Wi;
    // float bias        = max(0.025f * (1.0f - dot(N, normalize(Wi))), 0.0005f);

    // int   sampleRadius = 1;  // half the width of a square Smoothens out the shadows
    // float offset       = 0.02f;
    // for (int z = -sampleRadius; z <= sampleRadius; z++)
    // {
    //     for (int y = -sampleRadius; y <= sampleRadius; y++)
    //     {
    //         for (int x = -sampleRadius; x <= sampleRadius; x++)
    //         {
    //             float closestDepth = texture(pointShadowMaps, vec4(fragToLight + vec3(x, y, z) * offset, float(pointLayerIndex[i]))).r;
    //             closestDepth *= pointFarPlane[i];
    //             if (dist > closestDepth + bias)
    //                 shadow += 1.0f;
    //         }
    //     }
    // }
    // // Average shadow
    // shadow /= pow((sampleRadius * 2 + 1), 3);

    //return ((1.0f - shadow) * inten) * pointLightColor[i];
    return (inten) * pointLightColor[i];
    // return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

vec3 Fr(vec3 P, vec3 Wi, vec3 Wo, vec3 N, vec2 UVs, vec3 HalfWay)
{
    vec3 F0        = vec3(0.04);
    F0             = mix(F0, texture(albedo0, UVs).rgb, metalic);
    vec3  Flambert = texture(albedo0, UVs).rgb / PI;
    float cosTheta = dot(Wi, N);
    vec3  Fresnel  = fresnelSchlick(cosTheta, F0);
    vec3  kd       = vec3(1.0f) - Fresnel;

    float alpha             = roughness * roughness;
    float k                 = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    vec3  cookTorranceNum   = DistributionGGX(N, HalfWay, alpha) * GeometrySmith(N, Wo, Wi, k) * Fresnel;
    float cookTorranceDenum = 4 * max(dot(Wo, N), 0.0) * max(dot(Wi, N), 0.0);
    cookTorranceDenum       = max(cookTorranceDenum, 0.000001);
    vec3 cookTorrance       = cookTorranceNum / cookTorranceDenum;

    return kd * Flambert + cookTorrance;
}

void main()
{
    const vec2 UVs = getUVs();

    const float ambient = 0.20f;
    vec4        result  = useTexture ? texture(albedo0, UVs) * ambient : vec4(diffuseColor, 1.0f) * ambient;

    const int   steps   = 100;
    const float dW      = 1.0f / steps;                 // step size
    vec3        sum     = vec3(0.0f);                   // PBR sum
    const vec3  P       = crntPos;                      // fragment pos
    const vec3  Wo      = normalize(camPos - crntPos);  // view dir
    const vec3  N       = getNormal(UVs);               // normal
    vec3        toLight = pointLightPos[0] - crntPos;   // light vec
    if (length(toLight) < 0.0001) toLight = vec3(0.0, 1.0, 0.0);
    const vec3 Wi      = normalize(toLight);  // light dir
    const vec3 HalfWay = normalize(Wi + Wo);
    sum += Fr(P, Wi, Wo, N, UVs, HalfWay) * pointLight(0, P, toLight, N) * max(dot(N, Wi), 0.0);

    FragColor = result + vec4(sum, 1.0f);
    
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0f)
        BloomColor = vec4(FragColor.rgb, 1.0f);
    else
        BloomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}