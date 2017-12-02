#version 330 core
layout (triangles) in;
layout (triangle_strip) out; 
layout (max_vertices = 3) out;

in VS_OUT {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
} gs_in[];

out GS_OUT {
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
} gs_out;

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    //return position + vec4(direction, 0.0);
	return position; // use to switch off exploding effect
} 

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  

void main() {    
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
	gs_out.Normal = gs_in[0].Normal;
	gs_out.FragPos = gs_in[0].FragPos;
    gs_out.TexCoord = gs_in[0].TexCoord;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
	gs_out.Normal = gs_in[1].Normal;
	gs_out.FragPos = gs_in[1].FragPos;
    gs_out.TexCoord = gs_in[1].TexCoord;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
	gs_out.Normal = gs_in[2].Normal;
	gs_out.FragPos = gs_in[2].FragPos;
    gs_out.TexCoord = gs_in[2].TexCoord;
    EmitVertex();

    EndPrimitive();
}  