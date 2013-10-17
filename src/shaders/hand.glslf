#version 330

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;
in vec4 colour;
out vec4 fragColour;

uniform sampler2D gSampler;

void main()
{
    fragColour = texture2D(gSampler, TexCoord0.xy);
}
