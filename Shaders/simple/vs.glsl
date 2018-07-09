#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 v_normal;
layout(location = 2) in vec2 v_tex_coords;

uniform mat4 model_projection_mat;

out VS_OUT 
{
    vec2 tex_coords;
} vs_out;

void main()
{
    vec3 sdada = v_pos;

    gl_Position = model_projection_mat * vec4(sdada, 1.0);
    vs_out.tex_coords = v_tex_coords;
}

