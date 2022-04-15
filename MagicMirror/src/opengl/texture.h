#pragma once

#include <typeindex>
#include <string>

// Repräsentiert eine Textur, welche sich in dem Arbeitsspeicher der Grafikkarte befindet
class Texture {

	uint32_t id;
	int w, h;

public:
	Texture();
	Texture(const std::string& file);
	Texture(const Texture& t);
	~Texture();

	void bind(int index = 0) const;
	void setPixelData(int w, int h, int format, const unsigned char* data);

	void set(uint32_t id, int w, int h);

	inline uint32_t getID() const { return id; }

	static Texture* createTextureFromData(int w, int h, int format, const unsigned char* data = nullptr);

private:
	static void flipImage(unsigned char* src, unsigned char* dst, int w, int h);

};