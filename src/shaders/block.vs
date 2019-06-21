#version 330 core
layout (location = 0) in vec3 aVertPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aBlockPos;
layout (location = 4) in int aBlock;

out vec3 Normal;
out vec2 TexCoords;
flat out int block;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    Normal = aNormal;
    block = aBlock;
    
    gl_Position = projection * view  * vec4(aVertPos+aBlockPos, 1.0);
}