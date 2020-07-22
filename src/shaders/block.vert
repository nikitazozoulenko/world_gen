#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 blockPos;
layout (location = 4) in int aBlockTexID;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
flat out int BlockTexID;

uniform mat4 projView;

void main()
{
    TexCoords = aTexCoords;
    BlockTexID = aBlockTexID;
    Normal = aNormal;
    
    gl_Position = projView * vec4(aPos+blockPos, 1.0);
}