#version 450 core

in vec2 f_uv;


//uniform vec4 tint;
uniform sampler2D tex0;

out vec4 color;

void main() {
	color = texture(tex0, f_uv);
}