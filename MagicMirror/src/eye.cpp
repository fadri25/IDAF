#include "eye.h"


Eye::Eye() {

}

Eye::Eye(const glm::vec4& rect) : rect(rect), valid(true) {

}

Eye::Eye(const Eye& e) : rect(e.rect), valid(true) {
}


Eye::~Eye() {

}


void Eye::set(const glm::vec4& rect) {
	this->rect.x = rect.x;
	this->rect.y = rect.y;
	this->rect.z = rect.z;
	this->rect.w = rect.w;
	valid = true;
}


void Eye::invalidate() {
	valid = false;
}


glm::vec2  Eye::getCenter() const {
	
	glm::vec2 v;
	v.x = rect.x + rect.z * 0.5f;
	v.y = rect.y + rect.w * 0.5f;

	return v;
}
