#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 Normal;
out vec2 texCoord;
out vec3 crntPos;
out vec3 Tangent;

in DATA
{
    vec3 Position;
    vec3 Normal;
    vec2 texCoord;
    mat4 camMatrix;
    vec3 Tangent;
}
data_in[];

void main()
{
    for (int i = 0; i < 3; i++)
    {
        gl_Position = data_in[i].camMatrix * vec4(data_in[i].Position, 1.0f);
        Normal      = data_in[i].Normal;
        texCoord    = data_in[i].texCoord;
        crntPos     = data_in[i].Position;
        Tangent     = data_in[i].Tangent;
        EmitVertex();
    }

    EndPrimitive();
}