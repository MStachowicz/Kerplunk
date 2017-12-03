#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
} vs_out;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
	vs_out.FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));

	// Adjusting the normal for non uniform transformations.
	vs_out.Normal = mat3(transpose(inverse(instanceMatrix))) * aNormal;

	vs_out.TexCoord = aTexCoords;
} 