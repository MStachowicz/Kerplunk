#version 330 core

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct PointLight {    
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;  
}; 

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;   
	
    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff; 
};

struct Material {
    sampler2D specularMap;
    sampler2D diffuseMap;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

#define NR_POINT_LIGHTS 4  

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotlight;
uniform Material material;

// Directional shadow mapping
uniform sampler2D shadowMap;
// Omnidirectional shadow mapping
uniform float omniFarPlane;
uniform	samplerCube omniShadowMap;
// normal map
uniform sampler2D normalMap;  
uniform bool isNormalMap;

in GS_OUT {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float OmniShadowCalculation(vec3 fragPos, vec3 lightPos);

// Settings
bool blinn = true; // Toggle between Phong and Blinn-Phong lighting
int shadowSamplingFactor = 2;

void main()
{
    // properties
	vec3 norm;
	
	if (isNormalMap)
	{
		// obtain normal from normal map in range [0,1]
		vec3 normal = texture(normalMap, fs_in.TexCoord).rgb;
		// transform normal vector to range [-1,1]
		norm = normalize(normal * 2.0 - 1.0); 
	}
	else
		norm = normalize(fs_in.Normal);

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    
	// phase 1: Directional lighting
     vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // phase 2: Point lights
   // for(int i = 0; i < NR_POINT_LIGHTS; i++)
       // result += CalcPointLight(pointLights[i], norm,  fs_in.FragPos, viewDir); 
		
	result += CalcPointLight(pointLights[0], norm,  fs_in.FragPos, viewDir);

    // phase 3: Spot light
    result += CalcSpotLight(spotlight, norm,  fs_in.FragPos, viewDir);          

	result = pow(result, vec3(1.0/2.2)); // gamma correction
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
	// diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	// specular shading
    float spec;
    	
	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}

	// combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuseMap, fs_in.TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseMap, fs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, fs_in.TexCoord));

	// calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);       

	// multiplying the diffuse and specular components by the inverse of the shadow factor (how much of the fragment is not in shadow)
	return  ambient + (1.0 - shadow) * (diffuse + specular);
	//return  ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
	// diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	// specular shading
    float spec;
    	
	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}

	// attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    
	// combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuseMap, fs_in.TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseMap, fs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, fs_in.TexCoord));
    
	ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

	// calculate shadow
    float shadow = OmniShadowCalculation(fs_in.FragPos, light.position);  

	// multiplying the diffuse and specular components by the inverse of the shadow factor (how much of the fragment is not in shadow)
	return  ambient + (1.0 - shadow) * (diffuse + specular);
    //return (ambient + diffuse + specular);
} 

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
	// diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	// specular shading
    float spec;
    	
	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}

	// attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	// spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
	// combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, fs_in.TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, fs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, fs_in.TexCoord));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

// Calculates the factor of how much of a fragment is in shadow 
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
	// transforming the light space position in clip space to NDC as fragPosLightSpace is not passed to the fragment shader via gl_Position
	// performing the manual perspective division
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; 
	// transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

	// Bias applied to surface shadows to prevent shadow acne
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 
    // check whether current frag pos is in shadow
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // Percentage closer filtering
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	// sample values around the proj coord values testing for shadow occlusion
    for(int x = -shadowSamplingFactor ; x <= shadowSamplingFactor; ++x)
    {
        for(int y = -shadowSamplingFactor; y <= shadowSamplingFactor; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
	// average the results by the total number of samples taken.
    shadow /= (9.0 * shadowSamplingFactor);
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);  

// Calculates the factor of how much of a fragment is in shadow for point lights using a cubemap depth map
float OmniShadowCalculation(vec3 fragPos, vec3 lightPos)
{
	// find direction vector from the light position to fragment
	vec3 fragToLight = fragPos - lightPos; 
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    
	// now test for shadows
	// Percentage-closer filtering
	float shadow = 0.0;
	float bias   = 0.15;
	int samples  = 20;
	float viewDistance = length(viewPos - fragPos);
	float diskRadius = (1.0 + (viewDistance / omniFarPlane)) / 25.0; // vary radius by distance of viewer for softer shadows when closer to shadow

	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(omniShadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= omniFarPlane;   // Undo mapping [0;1]
		if(currentDepth - bias > closestDepth)
			shadow += 1.0;
	}

	shadow /= float(samples);  

	// display closestDepth as debug (to visualize depth cubemap)
	// retrieve the closest depth value by sampling the cubemap using the direction
    //float closestDepth = texture(omniShadowMap, fragToLight).r;
	// transform the depth from [0,1] range back to original value
    //closestDepth *= omniFarPlane;
	//FragColor = vec4(vec3(closestDepth / omniFarPlane), 1.0);  

    return shadow;
}