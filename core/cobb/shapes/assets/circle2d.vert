#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 projection;

out vec2 uv;

void main() {
    uv = aPos + vec2(0.5); // map from [-0.5, 0.5] -> [0, 1]
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
}