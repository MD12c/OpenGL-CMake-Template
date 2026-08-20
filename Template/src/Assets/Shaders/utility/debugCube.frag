#version 460 core

out vec4            FragColor;
in vec2             texCoord;
uniform samplerCube depthCubeMap;
uniform vec3        faceForward;
uniform vec3        faceRight;
uniform vec3        faceUp;

void main()
{
    vec2  uv    = texCoord * 2.0f - 1.0f;
    vec3  dir   = normalize(faceForward + uv.x * faceRight + uv.y * faceUp);
    float depth = texture(depthCubeMap, dir).r;

    if (depth != 1.0f)
        FragColor = vec4(1.0f);
    else
        FragColor = vec4(0.0f);
}