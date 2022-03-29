#pragma once

#include <string>

#include <mat4x4.hpp>

// Repräsentiert ein Shaderprogramm, welches sich im Arbeitsspeicher der Grafikkarte befindet
class Shader {

	static std::string VS_EXT;
	static std::string FS_EXT;

	uint32_t id;

	std::string vsrc = "";
	std::string fsrc = "";


public:
	Shader();
	Shader(const std::string& file);
	~Shader();

	void bind() const;

	int getUniformLocation(const char* name) const;

	void setUniformMat4(const char* name, const glm::mat4& m);


	static void compileShader(uint32_t& shader, const std::string src, int type);
	static void logShaderInfo(uint32_t shader, const std::string src, int type);

};