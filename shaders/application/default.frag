/***************************************************************************************************
 * @file  default.frag
 * @brief Default fragment shader
 **************************************************************************************************/

#version 460 core

in vec3 position;
in vec3 color;

out vec4 fragColor;

uniform bool useUniformColor;
uniform vec3 uColor;
uniform float alpha;

void main() {
    if(useUniformColor) {
        fragColor = vec4(uColor, alpha);
    } else {
        fragColor = vec4(color, alpha);
    }
}
