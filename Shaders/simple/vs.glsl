#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 v_normal;
layout(location = 2) in vec2 v_tex_coords;

uniform mat4 model_projection_mat;

out vec2 out_tex_coords;

void main()
{
    gl_Position = model_projection_mat * vec4(v_pos, 1.0);
    out_tex_coords = v_tex_coords;
}

