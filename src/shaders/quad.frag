#version 330 core
out vec4 FragColor;

in vec2 tex_coords;

uniform sampler2D loaded_texture;

void main()
{   
    FragColor = texture(loaded_texture, tex_coords);
}