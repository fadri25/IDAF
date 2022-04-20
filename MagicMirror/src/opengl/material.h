#pragma once

#include <vec4.hpp>
#include "shader.h"


// Repräsentiert ein Material, welches die Beschaffenheit und die Eigenschaften einer Oberfläche beschreibt
struct Material {


	glm::vec4 specular;
	glm::vec4 diffuse;
	glm::vec4 ambient;

	float specularStrength = 1.0f;
	float roughness = 0.0f;

	Material();
	Material(const Material& m);
	Material(const glm::vec4& specular, const glm::vec4& diffuse, const glm::vec4& ambient, float roughness, float specularStength);
	~Material();

	void bind(Shader* shader) const;
};