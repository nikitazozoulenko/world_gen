#version 430 core
out vec4 FragColor;

in vec2 tex_coords;

layout(binding = 0) uniform sampler2D ray_image;

void main()
{   
    FragColor = texture(ray_image, tex_coords);
}