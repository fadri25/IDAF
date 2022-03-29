#include "material.h"



Material::Material() {

}

Material::Material(const glm::vec4& specular, const glm::vec4& diffuse, const glm::vec4& ambient) 
	: specular(specular), diffuse(diffuse), ambient(ambient) {

}

Material::~Material() {

}