#pragma once

#include <mat4x4.hpp>

#include "buffers.h"
#include "texture.h"
#include "shader.h"

#include "renderable.h"
#include "material.h"


// Repräsentiert ein 3D-Modell
class Model : public Renderable {

public:

	// Enumeratoren für Positionen
	enum Position {
		CENTER,
		TOP,
		BETWEEN_EYES,
		CHINN,
		NONE
	};

private:
	glm::mat4 model;
	float angle = 0.0f;
	Vertexbuffer vb;
	Indexbuffer ib;
	Texture* tex;
	Shader* shader = nullptr;
	glm::vec3 scaleVector;
	Material material;
	int position;


public:
	Model();
	Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, Shader* shader);
	Model(std::vector<Vertex> verteces, std::vector<uint32_t> indeces, const std::string& texFile, Shader* shader, 
		const Material& material ,int position = CENTER);

	~Model();

	void bind(const glm::mat4& projectionView) const override;
	void bindWithMatrix(const glm::mat4* mvp = nullptr, const glm::mat4* model = nullptr) const override;


	void translate(const glm::vec3& t);
	void rotate(const glm::vec3& axis, float angle);
	void scale(const glm::vec3& s);
	void resetTransformationMatrix();

	void setShader(Shader* s);
	void setTexture(Texture* tex);

	inline int getPosition() const { return position; }
	inline Texture* getTexture() { return tex; }
	inline float getAngle() const { return angle; }

	int getCount() const override;
	Shader* getShader() const override;
	Material* getMaterial() const override;
	glm::mat4 getTransform() const override;


};