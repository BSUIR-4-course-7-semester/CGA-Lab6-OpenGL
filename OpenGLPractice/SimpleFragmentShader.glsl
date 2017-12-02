#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;
uniform vec3 viewPos;

out vec3 color;

void main(){
	vec3 lightColor = vec3(1, 1, 1);
	vec3 objectColor = vec3(1.0f, 0.5f, 0.31f);

    vec3 ambient = lightColor * materialAmbient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightColor * (diff * materialDiffuse);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
	vec3 specular = lightColor * (spec * materialSpecular);

	color = ambient + diffuse + specular;
}