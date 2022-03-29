#pragma once

#include <typeindex>

#include "texture.h"


class Framebuffer {

	uint32_t id;
	Texture tex;


public:
	Framebuffer();
	Framebuffer(int w, int h);
	~Framebuffer();


	void bind() const;
	void bindAsTarget() const;
	
	inline Texture* getTexture() { return &tex; }

};