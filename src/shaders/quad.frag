#version 430 core
out vec4 FragColor;
in vec2 tex_coords;

layout(binding = 0) uniform sampler2D comp_image;

uniform float game_time;

void main()
{   
    FragColor = texture(comp_image, tex_coords);
}