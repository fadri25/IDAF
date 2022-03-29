#include "vertex.h"



Vertex::Vertex() : pos(glm::vec3(0.0f)), normal(glm::vec3(0.0f)) {

}

Vertex::Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& uv) 
	: pos(pos), normal(normal), uv(uv){

}

Vertex::~Vertex() {

}
