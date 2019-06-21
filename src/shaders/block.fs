#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
flat in int block;


void main()
{   
    // // diffuse 
    vec3 norm = normalize(Normal);
    //vec3 cubeLight = normalize(vec3(8,7,6)); VERY GOOD
    vec3 cubeLight = normalize(vec3(3,2,1));
    float diff = max(dot(norm, cubeLight), dot(norm, -cubeLight));
    
    vec3 color = vec3(0.7, 0.7, 0.7);
    FragColor = diff * vec4(color.x, color.y, color.z, 1.0f);
}