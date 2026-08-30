#version 460 core

out vec4 FragColor;
in vec2  texCoords;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float     gamma;

const float offset_x = 1.0f / 800.0f;
const float offset_y = 1.0f / 800.0f;

vec2 offsets[9] = vec2[](
    vec2(-offset_x, offset_y), vec2(0.0f, offset_y), vec2(offset_x, offset_y),
    vec2(-offset_x, 0.0f), vec2(0.0f, 0.0f), vec2(offset_x, 0.0f),
    vec2(-offset_x, -offset_y), vec2(0.0f, -offset_y), vec2(offset_x, -offset_y));

// float kernel[9] = float[](
//     0, 1, 0,
//     1, -4, 1,
//     0, 1, 0);

// float kernel[9] = float[]
// (
//    1, 2, 1,
//    2, 4, 2,
//    1, 2, 1
// );

// float kernel[9] = float[]
// (
//    -1, -1, -1,
//    -1,  4, -1,
//    -1, -1, -1
// );

float kernel[9] = float[](
    0, 0, 0,
    0, 1, 0,
    0, 0, 0);

// float kernelPass1[9] = float[](
//     -1, 0, 1,
//     -2, 0, 2,
//     -1, 0, 1);

// float kernelPass2[9] = float[](
//     -1, -2, -1,
//     0, 0, 2,
//     1, 2, 1);

void main()
{
    vec3 color = vec3(0.0f);

    int iterations = 1;
    for (int i = 0; i < iterations; i++)
    {
        // for (int i = 0; i < 9; i++)
        //     color += vec3(texture(tex0, texCoords.st + offsets[i])) * kernelPass1[i];
        // for (int i = 0; i < 9; i++)
        //     color += vec3(texture(tex0, texCoords.st + offsets[i])) * kernelPass2[i];
    }
    for (int i = 0; i < 9; i++)
        color += vec3(texture(tex0, texCoords.st + offsets[i])) * kernel[i];

    // color += vec3(texture(tex0, texCoords));

    vec3 bloom = texture(tex1, texCoords).rgb;
    color += bloom;

    // float exposure = 0.1f;
    float exposure   = 1.0f;
    vec3  toneMapped = vec3(1.0f) - exp(-color * exposure);

    FragColor = vec4(pow(toneMapped, vec3(1.0f / gamma)), 1.0f);
}