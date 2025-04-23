/***************************************************************************************************
 * @file  default.vert
 * @brief Default vertex shader
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 position;

uniform mat4 mvp;
uniform mat4 model;

void main() {
    vec4 pos = vec4(aPos, 1.0f);

    gl_Position = mvp * pos;
    position = (model * pos).xyz;
}