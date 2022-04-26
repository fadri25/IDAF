#pragma once

#include <ctime>
#include <chrono>

// Deltazeit zwischen Frames
class DeltaTime {

	std::chrono::steady_clock::time_point last;
	std::chrono::steady_clock::time_point now;

	double delta = 0.0;

public:
	DeltaTime();
	~DeltaTime();

	void update();


	float getDelta() const;

	operator float();

	void operator--(int x);
};