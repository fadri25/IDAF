#pragma once


#include <vec2.hpp>
#include <vec3.hpp>

// Repräsentierung der Daten von Scheitelpunkten
struct Vertex {

	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;

	Vertex();
	Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& uv);
	~Vertex();




};