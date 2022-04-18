#pragma once

#include <mat4x4.hpp>

#include "buffers.h"
#include "texture.h"
#include "shader.h"

#include "renderable.h"


// Repräsentiert ein 3D-Modell
// Shaders dürfen nicht gelöscht werden in dem Destructor, da sich mehrere
// modelle einen Shader teilen können
class Model : public Renderable {

public:
	enum Position {
		CENTER,
		TOP,
		BETWEEN_EYES,
		CHINN
	};

private:
	glm::mat4 model;
	Vertexbuffer vb;
	Indexbuffer ib;
	Texture* tex;
	Shader* shader = nullptr;
	glm::vec3 scaleVector;
	int position;


public:
	Model();
	Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, Shader* shader);
	Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, const std::string& texFile, Shader* shader, int position = CENTER);

	~Model();

	void bind(const glm::mat4& projectionView) const override;
	void bindWithMatrix(const glm::mat4& mvp, const glm::mat4& model) const override;


	void translate(const glm::vec3& t);
	void rotate(const glm::vec3& axis, float angle);
	void scale(const glm::vec3& s);
	void resetTransformationMatrix();

	void setShader(Shader* s);

	int getCount() const override;
	
	inline int getPosition() const { return position; }
	inline Texture* getTexture() { return tex; }

	Shader* getShader() const override;

};