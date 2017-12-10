// Takes an input triangle and outputs 6 triangles (18 vertices). Iterates over 6 cubemap faces 
// specifying each face as the output face generating triangle by transforming fragment position from
// world space by its face's light-space transformation matrix.

#version 330 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // Specifies which cubemap face to emit a primitive to
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  