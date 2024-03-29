#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in ivec4 boneIDs1;
layout (location = 4) in ivec4 boneIDs2;
layout (location = 5) in ivec4 boneIDs3;
layout (location = 6) in ivec4 boneIDs4;
layout (location = 7) in vec4 weights1;
layout (location = 8) in vec4 weights2;
layout (location = 9) in vec4 weights3;
layout (location = 10) in vec4 weights4;

out vec2 vsTexCoordOut;
out vec3 vsNormalOut;
out vec3 vsWorldPosOut;
out vec4 vsVertexColourOut;

const int MAX_BONES = 100;

uniform mat4 gWVP;
uniform mat4 gWorld;
uniform mat4 gBones[MAX_BONES];

void main()
{       
    //mat4 boneTransform = mat4(1.0f); 
    mat4 boneTransform = mat4(0.0); // += gBones[boneIDs1[i]] * weights1[i];
    for (int i = 0; i < 4; i++)
      boneTransform += gBones[boneIDs1[i]] * weights1[i];
    for (int i = 0; i < 4; i++)
      boneTransform += gBones[boneIDs2[i]] * weights2[i];
    for (int i = 0; i < 4; i++)
      boneTransform += gBones[boneIDs3[i]] * weights3[i];
    for (int i = 0; i < 4; i++)
      boneTransform += gBones[boneIDs4[i]] * weights4[i];
    
    vec4 PosL      = boneTransform * vec4(position, 1.0);
    gl_Position    = gWVP * PosL;
    vec4 NormalL   = boneTransform * vec4(normal, 0.0);
    vsTexCoordOut  = texCoord;
    vsNormalOut    = (gWorld * NormalL).xyz;
    vsWorldPosOut  = (gWorld * PosL).xyz;                                

    // Logic to colour vertices different colours
    uint mask = uint(0x1f);
    uint uglVertexID = uint(gl_VertexID);
    uint b_mask = mask;
    uint g_mask = b_mask << 5;
    uint r_mask = g_mask << 5;
    float r = float((uglVertexID & r_mask) >> 10) / mask;
    float g = float((uglVertexID & g_mask) >> 5) / mask;
    float b = float((uglVertexID & b_mask)) / mask;
    vsVertexColourOut = vec4(r, g, b, 1.0);
}
