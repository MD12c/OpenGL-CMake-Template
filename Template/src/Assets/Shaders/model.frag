#version 460 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;
in vec4 fragPosLight;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D shadowMap;
uniform vec4      lightColor;
uniform vec3      lightPos;
uniform vec3      lightDirection;
uniform vec3      camPos;

vec4 pointLight()
{
    vec3 SurfaceToLightPos = lightPos - crntPos;

    // intensity of light with respect to distance
    float dist      = length(SurfaceToLightPos);
    float quadratic = 0.0075f;
    float linear    = 0.045f;
    float constant  = 1.0f;
    float inten     = 1.0f / (quadratic * dist * dist + linear * dist + constant);

    // ambient lighting
    float ambient = 0.20f;

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

    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 direcLight()
{
    vec3 lightDir = normalize(-lightDirection);

    // ambient lighting
    float ambient = 0.20f;

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

    float shadow = 0.0f;
    // Sets lightCoords to cull space
    vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        // Get from [-1, 1] range to [0, 1] range just like the shadow map
        lightCoords        = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        // Prevents shadow acne
        float bias = max(0.025f * (1.0f - dot(normal, lightDir)), 0.0005f);

        // Smoothens out the shadows
        int  sampleRadius = 2;
        vec2 pixelSize    = 1.0 / textureSize(shadowMap, 0);
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        // Get average shadow
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) + ambient) + texture(specular0, texCoord).r * specular * (1.0f - shadow)) * lightColor;
    //return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

vec4 spotLight()
{
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    // ambient lighting
    float ambient = 0.20f;

    // diffuse lighting
    vec3  lightDir          = normalize(-lightDirection);
    vec3  normal            = normalize(Normal);
    vec3  surfaceToLightPos = normalize(lightPos - crntPos);
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
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    float shadow = 0.0f;
    // Sets lightCoords to cull space
    vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
    if (lightCoords.z <= 1.0f)
    {
        // Get from [-1, 1] range to [0, 1] range just like the shadow map
        lightCoords        = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        // Prevents shadow acne
        float bias = max(0.0025f * (1.0f - dot(normal, surfaceToLightPos)), 0.00005f);

        // Smoothens out the shadows
        int  sampleRadius = 2;
        vec2 pixelSize    = 1.0 / textureSize(shadowMap, 0);
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        // Get average shadow
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * lightColor;
    //return vec4(vec3(shadow), 1.0f);  // for debugging shadows (shows shadow regions in white)
}

void main()
{
    FragColor = spotLight();
}