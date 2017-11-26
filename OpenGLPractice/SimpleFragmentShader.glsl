#version 330 core

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;

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

	color = (ambient + diffuse) * objectColor;
}