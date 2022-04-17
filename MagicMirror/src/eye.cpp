#include "eye.h"


Eye::Eye() : rect(0.0f), valid(false) {

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
	rect.x = 0;
	rect.y = 0;
	rect.z = 0;
	rect.w = 0;
	valid = false;
}

// Gibt Koordinaten des Zentrums des Auges an den Aufrufer zurück
glm::vec2  Eye::getCenter() const {
	
	if (!valid)
		return glm::vec2(0.0f);

	glm::vec2 v;
	v.x = rect.x + rect.z * 0.5f;
	v.y = rect.y + rect.w * 0.5f;

	return v;
}
