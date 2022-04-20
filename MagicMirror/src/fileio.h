#pragma once
#include <string>
#include <vector>

void readFileAsString(const std::string& file, std::string& dst);

void readFileLineByLine(const std::string& file, std::vector<std::string>& list);

void loadApplicationSettings(const std::string& file, float* cycleDuration, float* fadeDuration);
