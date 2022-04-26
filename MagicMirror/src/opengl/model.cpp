#include "model.h"

#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>

#include <math.h>


Model::Model() {

}

Model::Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, Shader* shader) 
	: vb(verteces), ib(indeces), shader(shader) {

}


Model::Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, const std::string& texFile, Shader* shader, 
			const Material& material, int position)
	: vb(verteces), ib(indeces), tex(new Texture(texFile)), shader(shader), material(material),  position(position) {

}

Model::~Model() {
	if (tex)
		delete tex;
}


// Wählt Shader, Vertexarray, Vertexbuffer, Indexbuffer, 
// Transformationsmatrix und Textur als aktive Daten für Shaderprogramm @memeber shader aus
void Model::bind(const glm::mat4& projectionView) const {
	shader->bind();
	glm::mat4 mvp = projectionView * model;
	//glm::vec4 t = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//printMat(model);
	shader->setUniformMat4("mvp", mvp);
	shader->setUniformMat4("model", model);
	
	vb.bind();
	ib.bind();
	tex->bind();

}


// Wählt Shader, Vertexarray, Vertexbuffer, Indexbuffer, 
// und Textur  mit Transformationsmatrix @arg mvp als aktive Daten für Shaderprogramm @memeber shader aus
void Model::bindWithMatrix(const glm::mat4* mvp, const glm::mat4* model) const {
	shader->bind();

	if (model) {
		shader->setUniformMat4("mvp", *mvp);
		shader->setUniformMat4("model", *model);
	}

	vb.bind();
	ib.bind();
	tex->bind();

	material.bind(shader);
}


// Translation um dem Vektor @arg t
void Model::translate(const glm::vec3& t) {
	//glm::vec3 trans = glm::vec3(t.x * (1.0f / scaleVector.x), t.y * (1.0f / scaleVector.y), t.z * (1.0f / scaleVector.z));
	model = glm::translate(model, t);
}

// Rotation um die Achsen @arg axis um den Winkel @arg angle
void Model::rotate(const glm::vec3& axis, float angle) {
	if (axis.y != 0.0f) {
		this->angle += angle;
		if (this->angle > (float) (M_PI * 2.0f)) {
			this->angle -= M_PI * 2.0f;
		}
		else if (this->angle < -((float)(M_PI * 2.0f))) {
			this->angle += M_PI * 2.0f;
		}
	}

	model = glm::rotate(model, angle, axis);
}

// Skalierung um den Vektor @arg s
void Model::scale(const glm::vec3& s) {
	scaleVector += s;
	model = glm::scale(model, s);
}

// Setzt die Transformationsmatrix auf eine Einheitsmatrix zurück
void Model::resetTransformationMatrix() {
	model = glm::mat4(1.0f);
}



// Setzt Pointer zu einem Shader
void Model::setShader(Shader* s) {
	shader = s;
}

void Model::setTexture(Texture* tex) {
	this->tex = tex;
}



int Model::getCount() const { 
	return ib.getCount(); 
}

Shader* Model::getShader() const {
	return shader;
}

Material* Model::getMaterial() const {
	return const_cast<Material*>(&material);
}

glm::mat4 Model::getTransform() const {
	return model;
}
