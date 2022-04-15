#include "texture.h"

#include <../../stb/stb_image.h>

#include "includegl.h"

#include "gldebug.h"

#include <iostream>

Texture::Texture() {

}

Texture::Texture(const std::string& file) {

	if (file.length() == 0) {
		return;
	}

	int comp;
	unsigned char* image = stbi_load(file.c_str(), &w, &h, &comp, STBI_rgb_alpha);
	unsigned char* flipped = new unsigned char[w * 4 * h];
	flipImage(image, flipped, w, h);

	if (!image) {
		std::cout << "cannot load image: " << file << std::endl;
		__debugbreak();
	}

	GL_CALL(glGenTextures(1, &id));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, id));

	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GL_CALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));


	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, flipped));

	delete[] flipped;
	stbi_image_free(image);

}

Texture::Texture(const Texture& t) : id(t.id), w(t.w), h(t.h) {

}


Texture::~Texture() {
	glDeleteTextures(1, &id);
}

// W�hlt diese Textur als aktive Textur in der Open GL Statemachine aus
void Texture::bind(int index) const {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::setPixelData(int w, int h, int format, const unsigned char* data) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, format, GL_UNSIGNED_BYTE, data);
}


void Texture::set(uint32_t id, int w, int h) {
	this->id = id;
	this->w = w;
	this->h = h;
}


Texture* Texture::createTextureFromData(int w, int h, int format, const unsigned char* data) {
	
	unsigned int t = 0;

	GL_CALL(glGenTextures(1, &t));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, t));
	if (data) {
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, format, GL_UNSIGNED_BYTE, data));
	}
	GL_CALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GL_CALL(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	Texture* texture = new Texture();
	texture->set(t, w, h);


	return texture;
}

// Spiegelt eine Bilddatei @arg src und schreibt diese in @arg dst an der X-Achse
void Texture::flipImage(unsigned char* src, unsigned char* dst, int w, int h) {
	
	for (int y = 0; y < h; y++) {
		int srcI = y * w * 4;
		int dstI = w * 4 * h - (y + 1) * w * 4;
		memcpy_s(&dst[dstI], w * 4, &src[srcI], w * 4);
	}
}
