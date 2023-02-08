#version 330 core            

in vec3 FragPos;//��
in vec3 Normal;//������
in vec3 Color;
in vec2 TexCoord;//��ͼUV

//��Ӱ�������ͼ
uniform sampler2D depthMap;

/*
struct Material
{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
*/

//��Ӱ����
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} fs_in;


//ƽ�й�
struct LightDirectional
{
    //vec3 pos;
    vec3 color;
    vec3 dirToLight;
};

//���Դ
struct LightPoint
{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
    float constant;
    float linear;
    float quadratic;
};

//�۹��
struct LightSpot
{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
    float constant;
    float linear;
    float quadratic;
    float CosPhyInner;
    float CosPhyOutter;
};

//uniform Material material;
uniform LightDirectional lightD;
uniform LightPoint lightP;
uniform LightSpot lightS;


uniform vec3 cameraPos;


float ShadowCalculation(vec4 fragPosLightSpace,float bias)
{
    // ִ��͸�ӳ���
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // �任��[0,1]�ķ�Χ
    projCoords = projCoords * 0.5 + 0.5;
    // ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // ȡ�õ�ǰƬ���ڹ�Դ�ӽ��µ����
    float currentDepth = projCoords.z;
    // ��鵱ǰƬ���Ƿ�����Ӱ��
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias> pcfDepth  ? 1.0 : 0.0;        
        }
    }
    //shadow /= 12.0;
    if(shadow > 0)shadow = 0.75;
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}


//ƽ�й�
vec3 CalcLightDirectional(LightDirectional light, vec3 uNormal, vec3 dirToCamera,float bias)
{
    //diffuse max(dot(L,N),0)
    float diffIntensity = max(dot(light.dirToLight,uNormal),0.1);
    vec3 diffColor = diffIntensity * light.color * Color;// * texture(material.diffuse,TexCoord).rgb;

    //specular  pow(man(dot(R,Cam),0),shininess)
    vec3 R = normalize(reflect(-light.dirToLight,uNormal));
    float specIntensity = pow(max(dot(R,dirToCamera),0),64);
    vec3 specColot = specIntensity * light.color * Color;// * texture(material.specular,TexCoord).rgb;

    //������Ӱ
    float shadow = 0.0f;
    if(dot(light.dirToLight,uNormal) > 0)shadow = ShadowCalculation(fs_in.FragPosLightSpace,bias);

    vec3 result = (1.0 - shadow) * (diffColor + specColot);
    return result;
}

//���Դ
vec3 CalcLightPoint(LightPoint light, vec3 uNormal, vec3 dirToCamera)
{
    //attenuation
    float dist = length(light.pos - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    //diffuse max(dot(L,N),0)
    float diffIntensity = max(dot(normalize(light.pos - FragPos),uNormal),0) * attenuation;
    vec3 diffColor = diffIntensity * light.color;// * texture(material.diffuse,TexCoord).rgb;

    //specular  pow(man(dot(R,Cam),0),shininess)
    vec3 R = normalize(reflect(-normalize(light.pos - FragPos),uNormal));
    float specIntensity = pow(max(dot(R,dirToCamera),0),64) * attenuation;
    vec3 specColot = specIntensity * light.color;// * texture(material.specular,TexCoord).rgb;

    vec3 result = diffColor + specColot;
    return result;
}

//�۹��
vec3 CalcLightSpot(LightSpot light, vec3 uNormal, vec3 dirToCamera)
{
    //attenuation
    float dist = length(light.pos - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    float cosTheta = dot(normalize(FragPos - light.pos),-light.dirToLight);
    float spotRatio;
    if(cosTheta > light.CosPhyInner){
        //inside
        spotRatio = 1.0f;
    }
    else if(cosTheta > light.CosPhyOutter){
        //outside
        spotRatio = 1 - (cosTheta - light.CosPhyInner)/(light.CosPhyOutter - light.CosPhyInner);
    }
    else{
        spotRatio = 0.0f;
    }

    //diffuse max(dot(L,N),0)
    float diffIntensity = max(dot(normalize(light.pos - FragPos),uNormal),0) * attenuation;
    vec3 diffColor = diffIntensity * light.color;// * texture(material.diffuse,TexCoord).rgb;

    //specular  pow(man(dot(R,Cam),0),shininess)
    vec3 R = normalize(reflect(-normalize(light.pos - FragPos),uNormal));
    float specIntensity = pow(max(dot(R,dirToCamera),0),64) * attenuation;
    vec3 specColot = specIntensity * light.color;// * texture(material.specular,TexCoord).rgb;

    vec3 result = (diffColor + specColot) * spotRatio;
    return result;
}


out vec4 FragColor;
void main()
{
        vec3 finalResult = vec3(0,0,0);
        vec3 uNormal = normalize(Normal);
        vec3 dirToCamera = normalize(cameraPos - FragPos);

        //������Ӱʧ��
        float bias = max(0 * (1.0 - dot(uNormal, dirToCamera)), 0.00005);
        finalResult += CalcLightDirectional(lightD,uNormal,dirToCamera,bias);//ƽ�й�
        //finalResult += CalcLightPoint(lightP,uNormal,dirToCamera);//���Դ
        finalResult += CalcLightSpot(lightS,uNormal,dirToCamera);//�۹�


        if(finalResult.x > Color.x){finalResult.x = Color.x;}
        if(finalResult.y > Color.y){finalResult.y = Color.y;}
        if(finalResult.z > Color.z){finalResult.z = Color.z;}

        FragColor = vec4(finalResult,1);
}