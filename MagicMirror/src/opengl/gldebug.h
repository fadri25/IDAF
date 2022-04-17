#pragma once

#include "includegl.h"

#include <sstream>

// Preprocessor Makro
// Ruft Funktion @arg x auf. Wenn diese als "false" resultiert haltet das programm
#define ASSERT(x) if(!x) { __debugbreak(); }

// Preprocessor Makro
// Hanliches Makro um Open GL Code zu debuggen
#define GL_CALL(x) clearError(); x; ASSERT(checkError(#x, __FILE__, __LINE__))


// Entfernt alle Fehler aus der Fehlerliste von OpenGL
static void clearError() {
	while (glGetError() != GL_NO_ERROR) {}
}

// Gibt alle Fehlermeldungen mit Hex-Code, Datei & Zeile auf der der Fehler passiert ist und
// falls vorhanden Fehlerbeschreibung in der Konsole aus
static bool checkError(const char* func, const char* file, int line) {
	bool noError = true;
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {

		std::stringstream ss;
		ss << "0x" << std::hex << error;
		printf("GL_ERROR [%s]\n%s\n%s\nLine : %i\nERROR : %s\n", ss.str().c_str(), func, file, line, glewGetErrorString(error));
		noError = false;
	}

	return noError;
}
