#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
out vec2 UV;

uniform mat4 model_projection_mat;

void main(){

        gl_Position = model_projection_mat * vec4(vertexPosition_modelspace, 1.0);
        UV = vertexUV;
}

