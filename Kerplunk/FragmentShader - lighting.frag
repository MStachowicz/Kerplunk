#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
  
void main()
{
	// DIFFUSE
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// AMBIENT
	float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

	// RESULT
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}