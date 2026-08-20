#version 460 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D depthMap;

void main()
{
    float depth = texture(depthMap, texCoord).r;
    if (depth != 1.0f)
        FragColor = vec4(1.0f);
    else
        FragColor = vec4(0.0f);
}