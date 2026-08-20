#version 460 core
out vec4 FragColor;
  
in vec2 texCoords;

uniform sampler2D tex0;
uniform bool horizontal;

// How far from the center to take samples from the fragment you are currently on
const int radius = 12;
// Keep it between 1.0f and 2.0f (the higher this is the further the blur reaches)
float spreadBlur = 2.0f;
float weights[radius];

void main()
{             
    float x = 0.0f;
    float weights[radius];
    float weightSum = 0.0f;

    for (int i = 0; i < radius; i++)
    {
        if (spreadBlur <= 2.0f)
            x += 3.0f / radius;
        else
            x += 6.0f / radius;

        weights[i] = exp(-0.5f * pow(x / spreadBlur, 2.0f)) / (spreadBlur * sqrt(2 * 3.14159265f));
        weightSum += (i == 0) ? weights[i] : weights[i] * 2.0f; // center counted once, others twice (symmetric taps)
    }

    vec2 tex_offset = 1.0f / textureSize(tex0, 0);
    vec3 result = texture(tex0, texCoords).rgb * (weights[0] / weightSum);

    if (horizontal)
    {
        for (int i = 1; i < radius; i++)
        {
            result += texture(tex0, texCoords + vec2(tex_offset.x * i, 0.0)).rgb * (weights[i] / weightSum);
            result += texture(tex0, texCoords - vec2(tex_offset.x * i, 0.0)).rgb * (weights[i] / weightSum);
        }
    }
    else
    {
        for (int i = 1; i < radius; i++)
        {
            result += texture(tex0, texCoords + vec2(0.0, tex_offset.y * i)).rgb * (weights[i] / weightSum);
            result += texture(tex0, texCoords - vec2(0.0, tex_offset.y * i)).rgb * (weights[i] / weightSum);
        }
    }
    FragColor = vec4(result, 1.0f);
}