#pragma once
#include "texture.h"

#include <typeindex>


class CubeMap : public Texture {


	

public:
	CubeMap();
	CubeMap(const std::string* file);
	CubeMap(const unsigned char* data, int width, int height);

	void bind(int i = 0) const override;


};