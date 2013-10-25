#version 330

in vec2 vsTexCoordOut;
in vec3 vsNormalOut;
in vec3 vsWorldPosOut;
uniform sampler2D gSampler;

out vec4 fragColour;

void main()
{                                    
    //In.Normal = normalize(FSin.Normal);
    //In.WorldPos = FSin.WorldPos;                                          
    fragColour = texture(gSampler, vsTexCoordOut.xy);
}
