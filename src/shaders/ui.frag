  
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 color;
uniform float smoothing_r;

void main()
{
    // FragColor = texture(ourTexture, TexCoord);
    FragColor = vec4(color,1);
}
