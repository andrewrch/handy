#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 gWVP;
uniform mat4 gWorld;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;
out vec4 colour;

void main()
{
    gl_Position = gWVP * vec4(Position, 1.0);
    TexCoord0   = TexCoord;
    Normal0     = (gWorld * vec4(Normal, 0.0)).xyz;
    WorldPos0   = (gWorld * vec4(Position, 1.0)).xyz;
    //colour      = vec4(clamp(Position, 0.0, 1.0), 1.0);

    uint mask = uint(0x1f);
    uint uglVertexID = uint(gl_VertexID);

    uint b_mask = mask;
    uint g_mask = b_mask << 5;
    uint r_mask = g_mask << 5;
    float r = float((uglVertexID & r_mask) >> 10) / mask;
    float g = float((uglVertexID & g_mask) >> 5) / mask;
    float b = float((uglVertexID & b_mask)) / mask;

    colour      = vec4(r, g, b, 1.0);
    //colour      = vec4(1.0, 1.0, 1.0, 1.0);
}
