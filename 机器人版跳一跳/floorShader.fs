#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 normal;     //已经normalize的法向量
in vec3 pointPos;
uniform vec3 viewPos;

uniform int flag;
vec3 lightPos = vec3(0,5,0);      //光源的位置
vec3 lightColor = vec3(1);    //光的颜色

float ambientStrength = 0.6;  //环境光系数
float shiness = 32;      //高光系数specular 
float specularStrength = 0.1;   //镜面反射的系数

// 距离衰减系数
float constant = 1.0;         
float linear = 0.25;
float quadratic = 0.10;

void main()
{
    //环境光分量
    vec3 ambient = ambientStrength * lightColor;

    //漫反射
    vec3 lightDir = normalize(lightPos - pointPos);
    // 漫反射系数,防止系数小于0，乘于五是因为太暗了
    float diff = max(dot(normal, lightDir), 0.0)*5; 
    vec3 diffuse = diff * lightColor;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - pointPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
    vec3 specular = specularStrength * spec * lightColor;

    // 距离
    float distance = length(lightPos - pointPos);
    //衰减因子
    float weakness =  1.0 / (constant + linear*distance +quadratic);
    //最终结果
    vec3 result = weakness*(ambient + diffuse + specular);
    if(flag == 0){
	    // linearly interpolate between both textures (80% container, 20% awesomeface)
        FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2)*vec4(result,1);
    }else{
        FragColor = vec4(vec3(1.0, 0.0, 0.0)* result,1);   //mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    }
	//FragColor = vec4(vec3(1.0, 0.0, 0.0)* result,1);   //mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}