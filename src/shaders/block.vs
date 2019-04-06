#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 model;
layout (location = 7) in int a_blockTexID;
layout (location = 8) in float a_face_lighting;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
flat out int blockTexID;
out float face_lighting;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    blockTexID = a_blockTexID;
    face_lighting = a_face_lighting;
    Normal = aNormal;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}