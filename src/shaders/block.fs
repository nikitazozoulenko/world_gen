#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
flat in int BlockTexID;
flat in int FaceLighting;

uniform sampler2DArray ourTexture;
uniform vec3 sun_dir;

void main()
{   
    // // diffuse 
    vec3 norm = normalize(Normal);
    vec3 cubeLight = normalize(vec3(8,7,6));
    float diff = max(dot(norm, cubeLight), dot(norm, -cubeLight));
    
    FragColor = diff * pow(0.9, 15-FaceLighting) * texture(ourTexture, vec3(TexCoords.x, TexCoords.y, BlockTexID));
}