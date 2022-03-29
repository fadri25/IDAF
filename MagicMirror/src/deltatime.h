#pragma once

#include <ctime>

// Deltazeit zwischen Frames
class DeltaTime {

	std::clock_t last;
	std::clock_t now;
	double delta = 0.0;

public:
	DeltaTime();
	~DeltaTime();

	void update();


	float getDelta() const;

	operator float();

	void operator--(int x);
};