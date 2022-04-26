#version 450 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 rot;
uniform float alpha;

out vec3 f_pos;
out vec3 f_normal;
out vec2 f_uv;
out vec4 fragPos;
out float f_alpha;

void main() {

	// Position auf dem Bildschirm berechnen
	gl_Position = mvp * vec4(pos, 1.0);
	// Pixelposition in World-space berechnen (fur Beleuchtung)
	fragPos = model * vec4(pos, 1.0);
	
	// Werte an Fragmentshader weitergeben
	f_normal = (rot * vec4(normal, 1.0)).xyz;
	f_uv = uv;
	f_pos = (rot * vec4(pos, 1.0)).xyz;
	f_alpha = alpha;
}