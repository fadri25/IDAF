#include "fileio.h"

#include <fstream>

#include "opengl/renderer.h"


#define LIGHT_POSITION "lightPos"
#define LIGHT_COLOR "lightColor"
#define AMBIENT_STRENGTH "ambientStrength"
#define FADE_DURATION "fadeDuration"
#define CYCLE_DURATION "cycleDuration"


// Liest Text aus einer Datei und speichert diesn in @arg dst
void readFileAsString(const std::string& file, std::string& dst) {

	std::fstream in(file);

	if (!in.is_open()) {
		dst = "";
		return;
	}

	for (std::string line; std::getline(in, line); ) {
		dst += line + "\n";
	}

}

// Liest Text aus einer Datei und speicher diese als List in @arg list
void readFileLineByLine(const std::string& file, std::vector<std::string>& list) {
	
	std::fstream in(file);

	if (!in.is_open()) {
		return;
	}

	for (std::string line; std::getline(in, line); ) {
		list.push_back(line);
	}

}

// Teilt einen Text in Stücke auf bei Zeichen @arg delimeter und schreib diese in @arg dst
void tokenize(const std::string& s, std::vector<std::string>& dst, const char delimeter = ' ') {

	int start = 0;
	int end = s.find(delimeter, start);
	
	if (end == std::string::npos)
		return;

	while (true) {
		if (end == std::string::npos) {
			dst.emplace_back(s.substr(start));
			break;
		}

		dst.emplace_back(s.substr(start, end));
		start = end + 1;
		end = s.find(delimeter, start);
	}
}

// Lädt Einstellungswerte aus Datei
void loadApplicationSettings(const std::string& file, float* cycleDuration, float* fadeDuration) {

	std::vector<std::string> list;
	readFileLineByLine(file, list);

	for (auto line : list) {
		std::vector<std::string> tokens;
		tokenize(line, tokens);

		if (tokens.size() == 0)
			continue;

		if (tokens[0].compare(LIGHT_POSITION) == 0) {
			std::vector<std::string> values;
			tokenize(tokens[1], values, ',');

			Renderer::lightPos.x = std::stof(values[0]);
			Renderer::lightPos.y = std::stof(values[1]);
			Renderer::lightPos.z = std::stof(values[2]);
		}
		else if (tokens[0].compare(LIGHT_COLOR) == 0) {
			std::vector<std::string> values;
			tokenize(tokens[1], values, ',');

			Renderer::lightColor.x = std::stof(values[0]);
			Renderer::lightColor.y = std::stof(values[1]);
			Renderer::lightColor.z = std::stof(values[2]);
		}
		else if (tokens[0].compare(AMBIENT_STRENGTH) == 0) {
			Renderer::ambientStrength = std::stof(tokens[1]);
		}
		else if (tokens[0].compare(FADE_DURATION) == 0) {
			*fadeDuration = std::stof(tokens[1]);
		}
		else if (tokens[0].compare(CYCLE_DURATION) == 0) {
			*cycleDuration = std::stof(tokens[1]);
		}
	}


}



