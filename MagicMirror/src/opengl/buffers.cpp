#include "buffers.h"

#include "gldebug.h"

Vertexbuffer::Vertexbuffer() {

}

Vertexbuffer::Vertexbuffer(std::vector<Vertex> data, int mode) : data(data) {
	//this->data.reserve(data.size());
	//this->data.insert(this->data.begin(), data.begin(), data.end());

	GL_CALL(glGenVertexArrays(1, &va));
	GL_CALL(glBindVertexArray(va));

	GL_CALL(glGenBuffers(1, &id));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), &this->data[0], GL_STATIC_DRAW));

	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));


	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal)));


	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv)));

	GL_CALL(glBindVertexArray(0));

}

Vertexbuffer::~Vertexbuffer() {
	GL_CALL(glDeleteVertexArrays(1, &va));
	GL_CALL(glDeleteBuffers(1, &id));
}

// Initialisierungsfunktion
void Vertexbuffer::init(std::vector<Vertex> data, int mode) {
	this->data.reserve(data.size());
	this->data.insert(this->data.begin(), data.begin(), data.end());

	GL_CALL(glGenVertexArrays(1, &va));
	GL_CALL(glBindVertexArray(va));

	GL_CALL(glGenBuffers(1, &id));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), &this->data[0], GL_STATIC_DRAW));

	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos)));


	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal)));


	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv)));

	GL_CALL(glBindVertexArray(0));
}





// Wählt diesen Buffer als aktiven Vertexbuffer in der Open Gl Statemachine aus
void Vertexbuffer::bind() const {
	GL_CALL(glBindVertexArray(va));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void Vertexbuffer::bindOnlyBuffer() const {
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, id));
}






Indexbuffer::Indexbuffer() {

}

Indexbuffer::Indexbuffer(std::vector<uint32_t> data, int mode) : data(data) {


	GL_CALL(glGenBuffers(1, &id));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint32_t), &data[0], GL_STATIC_DRAW));
}

Indexbuffer::~Indexbuffer() {

	GL_CALL(glDeleteBuffers(1, &id));
}

// Initialisierungsfunktion
void Indexbuffer::init(std::vector<uint32_t> data, int mode) {
	this->data.reserve(data.size());
	this->data.insert(this->data.begin(), data.begin(), data.end());


	GL_CALL(glGenBuffers(1, &id));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint32_t), &data[0], GL_STATIC_DRAW));

}

// Wählt diesen Buffer als aktiven Indexbuffer in der Open Gl Statemachine aus
void Indexbuffer::bind() const {
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

