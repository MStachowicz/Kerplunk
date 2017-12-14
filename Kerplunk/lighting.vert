#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent; 

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
	vec4 FragPosLightSpace;	
	mat3 TBN; // Matrix to transform lighting variables to tangent space
} vs_out;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
	vs_out.TexCoord = aTexCoords;
	// Passing a world-space vertex postion
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	// Adjusting the normal for non uniform transformations
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
	// Transforming the world-space vertex position to light space
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

	// Creating the TBN matrix for transforming into tangent space
	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));

	// applying the Gram-Schmidt process to re-orthogonalize TBN vectors.
    // re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);

	vs_out.TBN = mat3(T, B, N);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 