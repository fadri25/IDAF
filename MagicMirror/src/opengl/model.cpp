#include "model.h"

#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>



void printMat(const glm::mat4& m) {

	printf("Matrix:\n");
	for (int i = 0; i < 4; i++) {
		printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
	}
	printf("\n");
	system("cls");
}

Model::Model() {

}

Model::Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, Shader* shader) 
	: vb(verteces), ib(indeces), shader(shader) {

}


Model::Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, const std::string& texFile, Shader* shader)
	: vb(verteces), ib(indeces), tex(new Texture(texFile)), shader(shader) {

}

Model::~Model() {

}

void Model::bind(const glm::mat4& projectionView) const {
	shader->bind();
	glm::mat4 mvp = projectionView * model;
	glm::vec4 t = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	printMat(model);
	shader->setUniformMat4("mvp", mvp);
	
	vb.bind();
	ib.bind();
	tex->bind();

}

// Translation um dem Vektor @arg t
void Model::translate(const glm::vec3 t) {
	model = glm::translate(model, t);
}

// Rotation um die Achsen @arg axis um den Winkel @arg angle
void Model::rotate(const glm::vec3 axis, float angle) {
	model = glm::rotate(model, angle, axis);
}

// Skalierung um den Vektor @arg s
void Model::scale(const glm::vec3 s) {
	model = glm::scale(model, s);
}

void Model::resetTransformationMatrix() {
	model = glm::mat4(1.0f);
}


void Model::createTexture(int w, int h, const char* data) {
	tex = Texture::createTextureFromData(w, h, GL_RGBA, (const unsigned char*) data);
}


// Setzt Pointer zu einem Shader
void Model::setShader(Shader* s) {
	shader = s;
}


int Model::getCount() const { return ib.getCount(); }