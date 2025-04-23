/***************************************************************************************************
 * @file  default.frag
 * @brief Default fragment shader
 **************************************************************************************************/

#version 460 core

in vec3 position;

out vec4 fragColor;

uniform vec3 color;

void main() {
    fragColor = vec4(color, 1.0f);
}
