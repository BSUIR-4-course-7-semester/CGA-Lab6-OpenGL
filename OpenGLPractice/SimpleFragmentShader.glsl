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
uniform Material material;
uniform vec3 viewPos;

out vec3 color;

void main(){
	vec3 lightColor = vec3(1, 1, 1);
	vec3 objectColor = vec3(1, 0.47, 0.25);

	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	color = (ambient + diffuse + specular) * objectColor;
}