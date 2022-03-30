#pragma once

#include <mat4x4.hpp>


struct Face {


	glm::mat4 model;

	Face();
	Face(const glm::mat4& rect);
	~Face();

};