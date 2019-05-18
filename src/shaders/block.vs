#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 blockPos;
layout (location = 4) in int aBlockTexID;
layout (location = 5) in int aFaceLighting;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
flat out int BlockTexID;
flat out int FaceLighting;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    BlockTexID = aBlockTexID;
    FaceLighting = aFaceLighting;
    Normal = aNormal;
    
    gl_Position = projection * view * vec4(aPos+blockPos, 1.0);
}