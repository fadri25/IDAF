#include "material.h"



Material::Material() {

}

Material::Material(const Material& m) 
	: specular(m.specular), diffuse(m.diffuse), ambient(m.ambient), roughness(m.roughness), specularStrength(m.specularStrength),
	metallic(m.metallic) {

}


Material::Material(const glm::vec4& specular, 
	const glm::vec4& diffuse, 
	const glm::vec4& ambient, 
	float roughness, 
	float specularStength,
	float metallic)
	: specular(specular), 
	diffuse(diffuse), 
	ambient(ambient), 
	roughness(roughness), 
	specularStrength(specularStrength), 
	metallic(metallic) {

}

Material::~Material() {

}

// Sendet Werte an das Shaderprogramm @arg shader
void Material::bind(Shader* shader) const {

	shader->setFloat4("ambientColor", ambient);
	shader->setFloat4("diffuseColor", diffuse);
	shader->setFloat4("specularColor", specular);
	shader->setFloat("roughness", roughness);
	shader->setFloat("specularStrength", specularStrength);
	shader->setFloat("metallic", metallic);

}
