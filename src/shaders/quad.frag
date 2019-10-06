#version 430 core
out vec4 FragColor;
in vec2 tex_coords;

layout(binding = 0) uniform sampler3D comp_image;

uniform float game_time;

void main()
{   
    FragColor = texture(comp_image, vec3(tex_coords, 0));
}