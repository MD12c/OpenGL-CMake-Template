#version 460 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;
in vec4 fragPosLight;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec3      camPos;

#define MAX_DIR_LIGHTS 2
#define MAX_SPOT_LIGHTS 4
#define MAX_POINT_LIGHTS 4

uniform int       numDirLights;
uniform vec3      dirLightDirection[MAX_DIR_LIGHTS];
uniform vec3      dirLightColor[MAX_DIR_LIGHTS];
uniform mat4      dirShadowMatrix[MAX_DIR_LIGHTS];
uniform sampler2D dirShadowMap[MAX_DIR_LIGHTS];

uniform int       numSpotLights;
uniform float     spotLightInnerCone[MAX_SPOT_LIGHTS];
uniform float     spotLightOuterCone[MAX_SPOT_LIGHTS];
uniform vec3      spotLightPos[MAX_SPOT_LIGHTS];
uniform vec3      spotLightDirection[MAX_SPOT_LIGHTS];
uniform vec3      spotLightColor[MAX_SPOT_LIGHTS];
uniform mat4      spotShadowMatrix[MAX_SPOT_LIGHTS];
uniform sampler2D spotShadowMap[MAX_SPOT_LIGHTS];

uniform int         numPointLights;
uniform vec3        pointLightPos[MAX_POINT_LIGHTS];
uniform vec3        pointLightColor[MAX_POINT_LIGHTS];
uniform float       pointFarPlane[MAX_POINT_LIGHTS];
uniform samplerCube pointShadowCubeMap[MAX_POINT_LIGHTS];

vec4 pointLight(int i)
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
    vec3  normal         = normalize(Normal);
    vec3  lightDirection = normalize(SurfaceToLightPos);
    float diffuse        = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3  viewDirection = normalize(camPos - crntPos);
        vec3  halfwayVec    = normalize(viewDirection + lightDirection);
        float specAmount    = pow(max(dot(normal, halfwayVec), 0.0f), 16);
        specular            = specAmount * specularLight;
    };

    float shadow       = 0.0f;
    vec3  fragToLight  = crntPos - pointLightPos[i];
    float currentDepth = length(fragToLight);
    float bias         = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);

    int   sampleRadius = 2;  // half the width of a square Smoothens out the shadows
    float offset       = 0.02f;
    for (int z = -sampleRadius; z <= sampleRadius; z++)
    {
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                //float closestDepth = texture(pointShadowCubeMap[i], fragToLight + vec3(x, y, z) * offset).r;
                float closestDepth = texture(pointShadowCubeMap[0], fragToLight + vec3(x, y, z) * offset).r;
                closestDepth *= pointFarPlane[i];
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
    }
    // Average shadow
    shadow /= pow((sampleRadius * 2 + 1), 3);

     return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * vec4(pointLightColor[i], 1.0f);
    // return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

vec4 direcLight(int i)
{
    vec4 fragPosLight = dirShadowMatrix[i] * vec4(crntPos, 1.0);

    vec3 lightDir = normalize(-dirLightDirection[i]);

    // diffuse lighting
    vec3  normal  = normalize(Normal);
    float diffuse = max(dot(normal, lightDir), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3  viewDirection = normalize(camPos - crntPos);
        vec3  halfwayVec    = normalize(viewDirection + lightDir);
        float specAmount    = pow(max(dot(normal, halfwayVec), 0.0f), 16);
        specular            = specAmount * specularLight;
    };

    float shadow      = 0.0f;
    vec3  lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        lightCoords        = (lightCoords + 1.0f) / 2.0f;  // [-1, 1] range to [0, 1]
        float currentDepth = lightCoords.z;
        float bias         = max(0.0025f * (1.0f - dot(normal, lightDir)), 0.0005f);

        int  sampleRadius = 2;
        vec2 pixelSize    = 1.0 / textureSize(dirShadowMap[i], 0);
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(dirShadowMap[i], lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow)) + texture(specular0, texCoord).r * specular * (1.0f - shadow)) * vec4(dirLightColor[i], 1.0f);
    // return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

vec4 spotLight(int i)
{
    vec4 fragPosLight = spotShadowMatrix[i] * vec4(crntPos, 1.0);

    // diffuse lighting
    vec3  lightDir          = normalize(-spotLightDirection[i]);
    vec3  normal            = normalize(Normal);
    vec3  surfaceToLightPos = normalize(spotLightPos[i] - crntPos);
    float diffuse           = max(dot(normal, surfaceToLightPos), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3  viewDirection = normalize(camPos - crntPos);
        vec3  halfwayVec    = normalize(viewDirection + surfaceToLightPos);
        float specAmount    = pow(max(dot(normal, halfwayVec), 0.0f), 16);
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

        int  sampleRadius = 2;
        vec2 pixelSize    = 1.0 / textureSize(spotShadowMap[i], 0);
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(spotShadowMap[i], lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * vec4(spotLightColor[i], 1.0f);
    // return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

void main()
{
    float ambient = 0.20f;
    vec4  result  = texture(diffuse0, texCoord) * ambient;

    for (int i = 0; i < numDirLights; i++) result += direcLight(i);
    for (int i = 0; i < numSpotLights; i++) result += spotLight(i);
    for (int i = 0; i < numPointLights; i++) result += pointLight(i);
    //result += pointLight(0);

    FragColor = result;
    // FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}