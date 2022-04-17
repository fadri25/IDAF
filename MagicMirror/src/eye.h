#pragma once

#include <vec2.hpp>
#include <vec4.hpp>

// Repräsentiert ein Auge
struct Eye {

	bool valid = false;
	glm::vec4 rect;

	Eye();
	Eye(const glm::vec4& rect);
	Eye(const Eye& e);
	~Eye();

	void set(const glm::vec4& rect);
	inline bool isValid() const { return valid; }
	void invalidate();

	glm::vec2 getCenter() const;

};