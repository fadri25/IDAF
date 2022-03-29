#pragma once

#include <vec4.hpp>

struct Material {


	glm::vec4 specular;
	glm::vec4 diffuse;
	glm::vec4 ambient;

	Material();
	Material(const glm::vec4& specular, const glm::vec4& diffuse, const glm::vec4& ambient);
	~Material();


};