#include "fileio.h"

#include <fstream>



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


