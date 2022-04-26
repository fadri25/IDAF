#version 450 core

in vec3 f_pos;
in vec3 f_normal;
in vec2 f_uv;
in vec4 fragPos;
in float f_alpha;

uniform vec3 cameraPos;

layout (location = 0) uniform sampler2D tex0;
layout (location = 1) uniform samplerCube tex1;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

uniform float roughness;
uniform float specularStrength;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientStrength;
uniform float metallic;

out vec4 color;




void main(){


	// Umgebungsbeleuchtung 
    vec3 ambient = ambientStrength * lightColor;
	
	// Normalisieren des Normalvektors & des Lichtvektors
	vec3 norm = normalize(f_normal);
	vec3 lightDir = normalize(lightPos - vec3(fragPos.xyz));

	// Diffuse Lichtreflektion
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Gebündelte Lichtreflektion
	vec3 viewDir = normalize(cameraPos - vec3(fragPos.xyz));
	vec3 reflectDir = reflect(lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	// Farbreflektion
	vec3 eye = normalize(cameraPos - f_pos);
	vec3 reflection = reflect(eye, norm);


	// Pixel von Textur sampeln
	color = texture(tex0, f_uv);
	vec4 cubeColor = texture(tex1, reflection);
	color = vec4(mix(vec3(cubeColor.xyz), vec3(color.xyz), 0.0), 1.0);

	//UmgebungsLicht und Lichtreflektion auf den Pixelfarbton anwenden
	color = vec4((ambient + diffuse + specular) * vec3(color.xyz), 1.0);
}
