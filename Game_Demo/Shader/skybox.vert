#version 330 core
layout(location = 0) in vec3 aPos;                         // λ�ñ���������λ��ֵΪ 0
layout(location = 1) in float ahuei;

out vec3 TexCoords;
out float huei;

uniform mat4 projection;
uniform mat4 view;


void main()
{
    TexCoords = vec3(aPos.x,aPos.y,aPos.z);
    vec4 pos = projection * view * vec4(aPos,1.0);
    huei = ahuei;
    gl_Position = pos.xyww;
}