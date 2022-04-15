#version 450 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 mvp;
uniform mat4 model;

out vec2 f_uv;

void main() {
	gl_Position = mvp * vec4(pos, 1.0);
	f_uv = uv;
}