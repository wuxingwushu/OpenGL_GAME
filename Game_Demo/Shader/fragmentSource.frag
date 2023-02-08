#version 330 core            

in vec3 FragPos;//点
in vec3 Normal;//点向量
in vec3 Color;
in vec2 TexCoord;//贴图UV

//阴影的深度贴图
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

//阴影参数
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} fs_in;


//平行光
struct LightDirectional
{
    //vec3 pos;
    vec3 color;
    vec3 dirToLight;
};

//点光源
struct LightPoint
{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
    float constant;
    float linear;
    float quadratic;
};

//聚光灯
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
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
    // 检查当前片段是否在阴影中
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


//平行光
vec3 CalcLightDirectional(LightDirectional light, vec3 uNormal, vec3 dirToCamera,float bias)
{
    //diffuse max(dot(L,N),0)
    float diffIntensity = max(dot(light.dirToLight,uNormal),0.1);
    vec3 diffColor = diffIntensity * light.color * Color;// * texture(material.diffuse,TexCoord).rgb;

    //specular  pow(man(dot(R,Cam),0),shininess)
    vec3 R = normalize(reflect(-light.dirToLight,uNormal));
    float specIntensity = pow(max(dot(R,dirToCamera),0),64);
    vec3 specColot = specIntensity * light.color * Color;// * texture(material.specular,TexCoord).rgb;

    //计算阴影
    float shadow = 0.0f;
    if(dot(light.dirToLight,uNormal) > 0)shadow = ShadowCalculation(fs_in.FragPosLightSpace,bias);

    vec3 result = (1.0 - shadow) * (diffColor + specColot);
    return result;
}

//点光源
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

//聚光灯
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

        //消除阴影失真
        float bias = max(0 * (1.0 - dot(uNormal, dirToCamera)), 0.00005);
        finalResult += CalcLightDirectional(lightD,uNormal,dirToCamera,bias);//平行光
        //finalResult += CalcLightPoint(lightP,uNormal,dirToCamera);//点光源
        finalResult += CalcLightSpot(lightS,uNormal,dirToCamera);//聚光


        if(finalResult.x > Color.x){finalResult.x = Color.x;}
        if(finalResult.y > Color.y){finalResult.y = Color.y;}
        if(finalResult.z > Color.z){finalResult.z = Color.z;}

        FragColor = vec4(finalResult,1);
}