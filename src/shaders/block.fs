#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
flat in int blockTexID;
in float face_lighting;
in vec3 Normal;

uniform sampler2DArray ourTexture;
uniform vec3 sun_dir;

void main()
{   
    //  // ambient
    float ambient = 0.3;
  	
    // // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-sun_dir);
    float diff = max(dot(norm, lightDir), 0.0);
    
    FragColor = min(ambient + diff, 1) * texture(ourTexture, vec3(TexCoords.x, TexCoords.y, blockTexID));
}