
#version 330 core
out vec4 FragColor;

in vec3 Normal;

void main()
{   
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 cubeLight = normalize(vec3(8,7,6));
    float diff = max(dot(norm, cubeLight), dot(norm, -cubeLight));

    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    
    FragColor = diff * color;
}