#version 330 core
out vec3 color;

void main(){
	vec3 lightColor = vec3(1, 1, 1);
	vec3 objectColor = vec3(1, 0.47, 0.25);

	float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor;

	color = ambient * objectColor;
}