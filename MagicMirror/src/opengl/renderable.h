#pragma once

#include <mat4x4.hpp>


// Rein virtuelle Klasse
// Dient dazu verschiedene Objekttypen an den Renderer senden zu können
class Renderable {



public:
	
	virtual void bind(const glm::mat4& projectionView) const = 0;
	virtual void bindWithMatrix(const glm::mat4& mvp, const glm::mat4& model) const = 0;

	virtual int getCount() const = 0;
};