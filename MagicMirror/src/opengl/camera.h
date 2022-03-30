#pragma once

#include <vec3.hpp>
#include <mat4x4.hpp>

// Klasse die als Kamera fungiert und sich um mathematischen
// Operationen der Projektions- & SichtMatrix kümmert
class Camera {

	glm::mat4 projection;
	glm::mat4 view;
	bool locked = false;
	float aspect = 0.0f;
	float fov = 0.0f;
	float zDist = 0.0f;
	float near = 0.0f;

public:
	Camera();
	Camera(float fov, float aspect, float near, float far);
	~Camera();

	void translate(const glm::vec3& t);
	void rotate(const glm::vec3 r, float angle);

	void lock();
	void unlock();

	float getDistanceToProjectionPlane(float width) const;


	inline bool isLocked() const { return locked; }
	inline glm::mat4 getView() const { return view; }
	inline glm::mat4 getProjection() const { return projection; }
	inline glm::mat4 getViewProjection() const { return projection * view; }
	inline float getAspectRatio() const { return aspect; }
	inline float getZdistance() const { return zDist; }
	inline float getNear() const { return near; }
};