#version 450 core

in vec3 f_normal;
in vec2 f_uv;
in vec4 fragPos;


uniform sampler2D tex0;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform float roughness;
uniform float specularStrength;
uniform vec3 cameraPos;

out vec4 color;




void main(){

	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightPos = vec3(0.0, 0.0, -0.5);

	// Umgebungsbeleuchtung 
	float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;
	
	//Normalisieren des Normalvektors & des Lichtvektors
	vec3 norm = normalize(f_normal);
	vec3 lightDir = normalize(lightPos - vec3(fragPos.xyz));

	//Lichtreflektion
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	
	vec3 viewDir = normalize(cameraPos - vec3(fragPos.xyz));
	vec3 reflectDir = reflect(lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	// Pixel von Texture sampeln
	vec4 pixelColor = texture(tex0, f_uv);

	//UmgebungsLicht und Lichtreflektion auf den Pixelfarbton anwenden
	vec3 result = (ambient + diffuse + specular) * vec3(pixelColor.xyz);

	color = vec4(result, 1.0);

}