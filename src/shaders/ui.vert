
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoord;

uniform vec4 coords;

void main()
{   
    //Currently not using tex coords
    TexCoord = aTexCoords;

    //translate the corners
    float width = coords.z;
    float height = coords.w;
    vec2 vert = (aPos.xy+1.0)/2.0; //normalized [0,1]
    vert = vec2(vert.x*width, vert.y*height);
    vert = vert + coords.xy; //translate by x0, y0
    vert = vert*2.0 - 1.0;     //transform back to somewhere in [-1,1]

    gl_Position = vec4(vert.x, vert.y, aPos.z, 1.0);

    // gl_Position = vec4(aPos, 1.0);
}