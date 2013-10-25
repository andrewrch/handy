#version 330

in vec4 vsVertexColourOut;
out vec4 fragColour;

void main()
{
    fragColour = vsVertexColourOut;
}
