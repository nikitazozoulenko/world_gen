#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
flat in int BlockTexID;

uniform sampler2DArray ourTexture;
uniform vec3 sun_dir;

void main()
{   
    // // diffuse 
    vec3 norm = normalize(Normal);
    //vec3 cubeLight = normalize(vec3(8,7,6)); VERY GOOD
    vec3 cubeLight = normalize(vec3(3,2,1));
    float diff = max(dot(norm, cubeLight), dot(norm, -cubeLight));
    
    FragColor = diff * texture(ourTexture, vec3(TexCoords.x, TexCoords.y, BlockTexID));
}