#pragma once

#include <typeindex>

#include "texture.h"


class Framebuffer {

	uint32_t id;
	Texture* tex = nullptr;

public:
	Framebuffer();
	Framebuffer(int w, int h);
	~Framebuffer();


	void bind() const;
	
	void setTexture(Texture* t);

};