#include "timer.h"

#include <iostream>

Timer::Timer(const char* code) : code(code) {
	startTime = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
	auto endTime = std::chrono::high_resolution_clock::now();
	printf("Code : %s took %f milliseconds to execute\n", code, std::chrono::duration<double, std::milli>(endTime - startTime).count());
}