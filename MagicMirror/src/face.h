#pragma once

#include <vec2.hpp>
#include <vec4.hpp>
#include "eye.h"

struct Face {

	static float margin;

	bool valid = false;

	glm::vec4 rect;

	Eye left;
	Eye right;

	Face();
	Face(const glm::vec4& rect);
	~Face();

	void set(const glm::vec4& rect);

	bool operator==(const glm::vec4& r) const;
	
	void invalidate();
	void swapEyes();
	void checkEyes();
	float getRoll() const;

	glm::vec2 getPointCoordinates(int position) const;

};