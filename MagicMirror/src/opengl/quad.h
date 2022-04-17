#pragma once

#include "renderable.h"
#include "texture.h"
#include "buffers.h"
#include "shader.h"


// Repräsentiert eine vierecke Ebene mit Textur
// Verwendet zusammen mit Framebuffer
class Quad : public Renderable {


	float w = 0.0f;
	float h = 0.0f;
	Vertexbuffer vb;
	Indexbuffer ib;
	Shader* shader = nullptr;
	Texture* tex = nullptr;

	glm::mat4 model;

public:
	Quad();
	Quad(float w, float h, Shader* shader);
	Quad(float w, float h, Texture* tex, Shader* shader);
	~Quad();



	void bind(const glm::mat4& projectionView) const override;
	void bindWithMatrix(const glm::mat4& mvp, const glm::mat4& model) const override;

	void translate(const glm::vec3& t);
	void rotate(const glm::vec3& axis, float angle);
	void scale(const glm::vec3& s);

	int getCount() const override;

	void setTexture(Texture* tex);

	Shader* getShader() const override;


};