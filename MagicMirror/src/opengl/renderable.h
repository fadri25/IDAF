#pragma once

#include <mat4x4.hpp>

#include "shader.h"
#include "material.h"

// Rein virtuelle Klasse (Bedeutet es ist nicht möglich eine Instanz dieser Klasse direkt zu erstellen.
//							Es können nur Instanzen von Klassen, welche diese Klasse erweitern, erstellt werden)
// Dient dazu verschiedene Objekttypen an den Renderer senden zu können
class Renderable {

protected:
	glm::mat4 model;

public:
	
	virtual ~Renderable();

	virtual void bind(const glm::mat4& projectionView) const = 0;
	virtual void bindWithMatrix(const glm::mat4* mvp = nullptr, const glm::mat4* model = nullptr) const = 0;

	virtual int getCount() const = 0;

	virtual Shader* getShader() const = 0;
	virtual Material* getMaterial() const = 0;
	virtual glm::mat4 getTransform() const = 0;
};