#pragma once

#include <mat4x4.hpp>

#include "buffers.h"
#include "texture.h"
#include "shader.h"

#include "renderable.h"


// Klasse welche ein 3D-Modell repräsentiert
// Shaders dürfen nicht gelöscht werden in dem Destructor, da sich mehrere
// modelle einen Shader teilen können
class Model : public Renderable {

	glm::mat4 model;
	Vertexbuffer vb;
	Indexbuffer ib;
	Texture* tex;
	Shader* shader = nullptr;

public:
	Model();
	Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, Shader* shader);
	Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, const std::string& texFile, Shader* shader);
	~Model();

	void bind(const glm::mat4& projectionView) const override;

	void translate(const glm::vec3 t);
	void rotate(const glm::vec3 axis, float angle);
	void scale(const glm::vec3 s);
	void resetTransformationMatrix();

	void setShader(Shader* s);

	void createTexture(int w, int h, const char* data = nullptr);

	int getCount() const override;


	inline Texture* getTexture() { return tex; }
};