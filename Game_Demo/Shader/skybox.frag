#version 330 core
out vec4 FragColor;


in float huei;
in vec3 TexCoords;

uniform vec3 Color;
uniform vec3 Color1;

uniform samplerCube skybox;

void main()
{    
    //FragColor = texture(skybox, vec3(TexCoords.x,TexCoords.y,TexCoords.z));
    FragColor = vec4((huei - 0.2)*Color + (0.8 - huei)*Color1 ,0);
}