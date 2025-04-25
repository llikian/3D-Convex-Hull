/***************************************************************************************************
 * @file  default.vert
 * @brief Default vertex shader
 **************************************************************************************************/

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aColor;

out vec3 position;
out vec3 color;

uniform mat4 mvp;
uniform mat4 model;

void main() {
    vec4 pos = vec4(aPos, 1.0f);

    gl_Position = mvp * pos;
    position = (model * pos).xyz;
    color = aColor;
}