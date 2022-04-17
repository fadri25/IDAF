#include "face.h"
#include <vec3.hpp>

#include "opengl/model.h"

#include <math.h>


float Face::margin = 15.0f;


Face::Face() : valid(false) {

}

Face::Face(const glm::vec4& rect) : rect(rect), left({ 0.0f }), right({ 0.0f }), valid(true) {

}

Face::~Face() {

}


void Face::set(const glm::vec4& rect) {
	this->rect.x = rect.x;
	this->rect.y = rect.y;
	this->rect.z = rect.z;
	this->rect.w = rect.w;
	valid = true;
}


bool Face::operator==(const glm::vec4& r) const {

	float dx = (r.x + r.z * 0.5f) - (rect.x + rect.z * 0.5f);
	float dy = (r.y + r.w * 0.5f) - (rect.y + rect.w * 0.5f);

	float dist = sqrt(dx * dx + dy * dy);

	return dist <= margin;
}


void Face::invalidate() {
	left.invalidate();
	right.invalidate();
	
	rect.x = 0;
	rect.y = 0;
	rect.z = 0;
	rect.w = 0;

	valid = false;
}

// Vertauscht die Augenkoordinaten
void Face::swapEyes() {
	Eye temp = left;
	left = right;
	right = temp;
}

// Falls ein Auge nicht erkannt wurde, findet diese Funktion heraus ob es das rechte oder linke Auge ist
void Face::checkEyes() {
	if (right.valid) {
		if (right.getCenter().x < rect.x + rect.z * 0.5f) {
			left = right;
			right.invalidate();
		}
	}
	else if (left.valid) {
		if (left.getCenter().x > rect.x + rect.z * 0.5f) {
			right = left;
			left.invalidate();
		}
	}
}


// Gibt Winkel des Gesichts um die Z-Achse an den Aufrufer zurück
float Face::getRoll() const {
	
	if (!left.valid || !right.valid)
		return 0.0f;

	glm::vec2 cLeft = left.getCenter();
	glm::vec2 cRight = right.getCenter();

	float dx = cRight.x - cLeft.x;
	float dy = cLeft.y - cRight.y;

	if (dx == 0.0f)
		return 3.141592f;


	return (float) atan(dy / dx);
}


// Gibt gewünschte Position im Gesicht an den Aufrufer zurück
glm::vec2 Face::getPointCoordinates(int position) const {

	glm::vec2 coords;
	switch (position) {

		case Model::Position::CENTER: 
			coords.x = rect.x + rect.z * 0.5f;
			coords.y = rect.y + rect.w * 0.5f;
			break;

		case Model::Position::TOP: 
			coords.x = rect.x + rect.z * 0.5f;
			coords.y = rect.y;
			break;

		case Model::Position::BETWEEN_EYES:

			glm::vec2 lc(0.0f);
			glm::vec2 rc(0.0f);

			if (!left.valid && right.valid) {
				rc = right.getCenter();
				int mid = rect.x + rect.z * 0.5f;
				lc.x = mid + (mid - rc.x) * 0.5f;
				lc.y = rc.y;
			}
			else if (!right.valid && left.valid) {
				lc = left.getCenter();
				int mid = rect.x + rect.z * 0.5f;
				rc.x = mid + (mid - lc.x) * 0.5f;
				rc.y = lc.y;
			}
			else if (!right.valid && !left.valid) {
				coords.x = rect.x + rect.z * 0.5f;
				coords.y = rect.y + rect.w * 0.4f;
				return coords;
			}
			else {
				lc = left.getCenter();
				rc = right.getCenter();
			}
			
			float dx = rc.x - lc.x;
			float dy = rc.y - lc.y;

			coords.x = lc.x + dx * 0.5f;
			coords.y = lc.y + dy * 0.5f;

			break;
	}

	return coords;
}
