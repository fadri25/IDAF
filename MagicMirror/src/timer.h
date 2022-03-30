#pragma once

#include <chrono>

// Timer um Zeitverbrauch einer Funktion zu messen 
class Timer {

	std::chrono::steady_clock::time_point startTime;
	const char* code;

public:
	Timer(const char* code);
	~Timer();

};