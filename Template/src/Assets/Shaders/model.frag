#version 460 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 crntPos;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4      lightColor;
uniform vec3      lightPos;
uniform vec3      camPos;

void main()
{
    float ambient = 0.20f;

    vec3  normal         = normalize(Normal);
    vec3  lightDirection = normalize(lightPos - crntPos);
    float diffuse        = max(dot(normal, lightDirection), 0.0f);

    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight       = 0.50f;
        vec3  viewDirection       = normalize(camPos - crntPos);       // frag -> camera
        vec3  reflectionDirection = reflect(-lightDirection, normal);  // frag -> reflected light direction

        vec3 halfwayVec = normalize(viewDirection + lightDirection);

        float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
        specular         = specAmount * specularLight;
    }

    // FragColor = texture(diffuse0, texCoord) * lightColor * (diffuse + ambient + specular);
    FragColor = (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}