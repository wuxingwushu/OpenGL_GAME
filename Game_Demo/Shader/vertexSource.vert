#version 330 core                                          
layout(location = 0) in vec3 aPos;                         // 位置变量的属性位置值为 0
layout(location = 1) in vec3 aNormal;//点向量
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexCoord;

//阴影参数
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} vs_out;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;
out vec2 TexCoord;

//3D转2D
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;   
uniform mat4 lightSpaceMatrix;

uniform vec3 cameraFos;

void main()
{
        /* 没有优化效果
        float vectorDot = cameraFos.x*aNormal.x+cameraFos.y*aNormal.y+cameraFos.z*aNormal.z;
        float vectorMold1 = sqrt(pow(cameraFos.x,2)+pow(cameraFos.y,2)+pow(cameraFos.z,2));
        float vectorMold2 = sqrt(pow(aNormal.x,2)+pow(aNormal.y,2)+pow(aNormal.z,2));
        float cosAngle  = acos((vectorDot)/(vectorMold1*vectorMold2));
        float Angle = 180 / 3.14 * cosAngle;
        if(Angle < 90)return;
        */
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        Normal = mat3(transpose(inverse(model))) * aNormal;
        FragPos = (model * vec4(aPos.xyz,1.0)).xyz;
        TexCoord = aTexCoord;
        Color = aColor;

        vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
        vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
        vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}