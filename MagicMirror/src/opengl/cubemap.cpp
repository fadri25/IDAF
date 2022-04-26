#include "cubemap.h"
#include <../../stb/stb_image.h>

#include "includegl.h"

CubeMap::CubeMap() {

}

CubeMap::CubeMap(const std::string* files) {


	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);


	int channels;
	for (int i = 0; i < 6; i++) {
		unsigned char* data = stbi_load(files[i].c_str(), &w, &h, &channels, STBI_rgb_alpha);

		if (!data) {
			printf("failed to load cube map texture: %s\n", files[i].c_str());
			__debugbreak();
		}

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
		);


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		stbi_image_free(data);
	}



}

CubeMap::CubeMap(const unsigned char* data, int width, int height) {
	
	w = width;
	h = height;


	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for (int i = 0; i < 6; i++) {
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGBA8, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, data
		);


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}


void CubeMap::bind(int i) const {
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}
