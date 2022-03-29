#include "camera.h"

#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>

Camera::Camera() {

}

Camera::Camera(float fov, float aspect, float near, float far) : fov(fov), aspect(aspect) {
	projection = glm::perspective(glm::radians(fov), aspect, near, far);
	//projection = glm::ortho(-1.0f * aspect, 1.0f * aspect, -1.0f, 1.0f, 0.001f, 1000.0f);
	view = glm::mat4(1.0f);

	translate({ 0.0f, 0.0f, -5.0f });
}

Camera::~Camera() {

}

// Translation der Kamera um den Vektor @arg t
void Camera::translate(const glm::vec3& t) {
	
	if (locked) return;
	
	view = glm::translate(view, t);
}


// Rotation der Kamera um den die Achsen @arg axis um den Winkel @arg angle
void Camera::rotate(const glm::vec3 axis, float angle) {
	
	if (locked) return;
	
	view = glm::rotate(view, angle, axis);
}

// Blockiert Translationen der Kamera
void Camera::lock() {
	locked = true;
}

// Hebt Blockierung der Kamera auf
void Camera::unlock() {
	locked = false;
}

float Camera::getDistanceToProjectionPlane(float width) const {
	
	float half = width * 0.5f;
	return half / tan(fov * 0.5f);
}
