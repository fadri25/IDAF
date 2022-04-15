#include "quad.h"

#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>

Quad::Quad() {


}

Quad::Quad(float w, float h, Shader* shader) : Quad(w, h, nullptr, shader) {

}


Quad::Quad(float w, float h, Texture* tex, Shader* shader) : w(w), h(h), tex(tex), shader(shader), model(1.0f) {

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.reserve(4);
	indices.reserve(6);

	vertices.emplace_back(Vertex({ -w, -h, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }));
	vertices.emplace_back(Vertex({ -w,  h, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }));
	vertices.emplace_back(Vertex({  w,  h, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }));
	vertices.emplace_back(Vertex({  w, -h, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }));

	indices.emplace_back(0);
	indices.emplace_back(2);
	indices.emplace_back(1);
	indices.emplace_back(0);
	indices.emplace_back(3);
	indices.emplace_back(2);

	vb.init(vertices);
	ib.init(indices);

}

Quad::~Quad() {

}



// Wählt Shader, Vertexarray, Vertexbuffer, Indexbuffer, 
// Transformationsmatrix und Textur als aktive Daten für Shaderprogramm @memeber shader aus
void Quad::bind(const glm::mat4& projectionView) const {
	glm::mat4 mvp = projectionView * model;
	shader->bind();
	shader->setUniformMat4("mvp", mvp);
	vb.bind();
	ib.bind();
	
	if (tex) 
		tex->bind();

}

// Wird nie aufgerufen. Implementier nur damit es konform ist mit @class Renderable
void Quad::bindWithMatrix(const glm::mat4& mvp, const glm::mat4& model) const {
}


// Translation um dem Vektor @arg t
void Quad::translate(const glm::vec3& t) {
	model = glm::translate(model, t);
}

// Rotation um die Achsen @arg axis um den Winkel @arg angle
void Quad::rotate(const glm::vec3& axis, float angle) {
	model = glm::rotate(model, angle, axis);
}

// Skalierung um den Vektor @arg s
void Quad::scale(const glm::vec3& s) {
	model = glm::scale(model, s);
}



int Quad::getCount() const { return ib.getCount(); }

void Quad::setTexture(Texture* tex) {
	this->tex = tex;
}
