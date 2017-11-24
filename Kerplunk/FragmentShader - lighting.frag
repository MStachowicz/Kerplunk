#version 330 core
struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;  // positional light
	vec3 direction; // directional light

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
	// Attenuation
	float constant;
    float linear;
    float quadratic;
};

struct Spotlight {
    vec3 position;
    vec3 direction;
    float cutoff; 
	float outerCutoff;

	// Attenuation
	float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform Spotlight spotlight;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    // AMBIENT
	//vec3 ambient  = light.ambient * material.ambient;
	vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, TexCoord));

    // DIFFUSE
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);    // positional light
	//vec3 lightDir = normalize(-light.direction);			// directional light
	float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse  = light.diffuse * (diff * material.diffuse);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, TexCoord));

    //SPECULAR 
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * (spec * material.specular);   
	vec3 specular = light.specular * spec * vec3(texture(material.specularMap, TexCoord));

    // Attenuation
	float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // RESULT POSITIONAL
    vec3 result = ambient + diffuse + specular;



    // SPOTLIGHT
	//float strength = 0.5;
    //float theta = dot(lightDir, normalize(-spotlight.direction));

		// AMBIENT
		ambient = spotlight.ambient * vec3(texture(material.diffuseMap, TexCoord));
        // DIFFUSE
		norm = normalize(Normal);
		lightDir = normalize(spotlight.position - FragPos);
        diff = max(dot(norm, lightDir), 0.0);
        diffuse = spotlight.diffuse * diff * vec3(texture(material.diffuseMap, TexCoord));
        //SPECULAR 
		viewDir = normalize(viewPos - FragPos);
        reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular = spotlight.specular * spec * vec3(texture(material.specularMap, TexCoord));
        
		// Spotlight soft edges
		float theta     = dot(lightDir, normalize(-spotlight.direction));
		float epsilon   = spotlight.cutoff - spotlight.outerCutoff;
		float intensity = clamp((theta - spotlight.outerCutoff) / epsilon, 0.0, 1.0);   
		diffuse  *= intensity;
		specular *= intensity;

		// Attenuation
		float SpotDistance = length(spotlight.position - FragPos);
		float SpotAttenuation = 1.0 / (spotlight.constant + spotlight.linear * SpotDistance + spotlight.quadratic * (SpotDistance * SpotDistance));
		ambient  *= SpotAttenuation; 
		diffuse  *= SpotAttenuation;
        specular *= SpotAttenuation; 

		// RESULT SPOTLIGHT
		result += ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
}
