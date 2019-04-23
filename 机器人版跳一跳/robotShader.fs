#version 330 core

out vec4 outColor;
in vec3 color;
in vec3 normal;     //�Ѿ�normalize�ķ�����
in vec3 pointPos;
uniform vec3 viewPos;
uniform int flag;
vec3 lightPos = vec3(0,5,0);      //��Դ��λ��
vec3 lightColor = vec3(1);    //�����ɫ

float ambientStrength = 0.6;  //������ϵ��
float shiness = 32;      //�߹�ϵ��specular 
float specularStrength = 0.1;   //���淴���ϵ��

// ����˥��ϵ��
float constant = 1.0;         
float linear = 0.25;
float quadratic = 0.10;

void main()
{
    if(flag==1){
        outColor = vec4(0,0,0,1);
    }else{
        //������
        vec3 ambient = ambientStrength * lightColor;

        //������
        vec3 lightDir = normalize(lightPos - pointPos);
        // ������ϵ��,��ֹϵ��С��0
        float diff = max(dot(normal, lightDir), 0.0); 
        vec3 diffuse = diff * lightColor;

        // ���淴��
        vec3 viewDir = normalize(viewPos - pointPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiness);
        vec3 specular = specularStrength * spec * lightColor;

        // ����
        float distance = length(lightPos - pointPos);
        //˥������
        float weakness =  1.0 / (constant + linear*distance +quadratic);
        //���ս��
        vec3 result = weakness*(ambient + diffuse + specular) * color;
	    outColor = vec4(result,1);
    }
}