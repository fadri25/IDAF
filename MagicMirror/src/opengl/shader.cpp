#include "shader.h"

#include "includegl.h"

#include "../fileio.h"

std::string Shader::VS_EXT = ".vert";
std::string Shader::FS_EXT = ".frag";


Shader::Shader() {

}

Shader::Shader(const std::string& file) {
	readFileAsString(file + VS_EXT, vsrc);
	readFileAsString(file + FS_EXT, fsrc);

	uint32_t vs;
	uint32_t fs;
	compileShader(vs, vsrc, GL_VERTEX_SHADER);
	compileShader(fs, fsrc, GL_FRAGMENT_SHADER);


	// Erstellung un Verlinkung eines kompletten Shaderprogrammes
	id = glCreateProgram();
	glAttachShader(id, vs);
	glAttachShader(id, fs);

	glLinkProgram(id);

	int isLinked;
	glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		int maxLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		int length = 0;
		char* buffer = new char[maxLength];
		glGetProgramInfoLog(id, maxLength, &length, buffer);
		printf("failed to link program\n");
		printf("info log:\n%s\n", buffer);
		delete[] buffer;
		__debugbreak();
	}

	glDetachShader(id, vs);
	glDetachShader(id, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);


}

Shader::~Shader() {
	glDeleteProgram(id);
}

// Wählt dieses Shaderprogramm als aktiven Shader in der Open Gl Statemachine aus
void Shader::bind() const {
	glUseProgram(id);
}

// Findet die "Position" der gesuchten Uniformvariabel in dem Shaderprogramm
int Shader::getUniformLocation(const char* name) const {
	bind();
	int location = glGetUniformLocation(id, name);
	return location;
}

// Ladet einen einzelnen "float" wert in die gewünschte Variabel mit dem Namen in @arg name des Shaderprogramms
void Shader::setFloat(const char* name, float v) {
	int location = getUniformLocation(name);
	glUniform1f(location, v);
}


// Ladet die Daten einer 4X4 Matrix in die gewünschte Variabel mit dem Namen von @arg name des Shaderprogramms
void Shader::setUniformMat4(const char* name, const glm::mat4& m) {
	int location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
}

// Kompiliert ein Shaderprogramm
void Shader::compileShader(uint32_t& shader, const std::string src, int type) {
	shader = glCreateShader(type);
	const char* ptr = src.c_str();
	glShaderSource(shader, 1, &ptr, NULL);

	glCompileShader(shader);

	int isCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		logShaderInfo(shader, src, type);
	}
}

// Gibt Fehlermeldungen bei dem Scheitern der Kompilierung eines Shaderprogrammes aus
void Shader::logShaderInfo(uint32_t shader, const std::string src, int type) {
	int maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	int length = 0;
	char* buffer = new char[maxLength];
	glGetShaderInfoLog(shader, maxLength, &length, buffer);
	switch (type) {
		case GL_VERTEX_SHADER: printf("failed to compile vertex shader\n"); break;
	}
	printf("source code:\n%s\n", src);
	printf("info log:\n%s\n", buffer);
	delete[] buffer;
	__debugbreak();
}