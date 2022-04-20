#version 450 core

in vec3 f_normal;
in vec2 f_uv;
in vec4 fragPos;
in float f_alpha;


uniform sampler2D tex0;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform float roughness;
uniform float specularStrength;
uniform vec3 cameraPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientStrength;

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

	// Pixel von Texture sampeln
	vec4 pixelColor = texture(tex0, f_uv);

	//UmgebungsLicht und Lichtreflektion auf den Pixelfarbton anwenden
	vec3 result = (ambient + diffuse + specular) * vec3(pixelColor.xyz);

	color = vec4(result, f_alpha);

}
