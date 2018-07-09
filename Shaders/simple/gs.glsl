#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT 
{
    vec2 tex_coords;
} gs_in[]; 

out vec2 tex_coords;

void main() 
{
    tex_coords = gs_in[0].tex_coords;

    for(int i = 0; i < 3; i++)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}