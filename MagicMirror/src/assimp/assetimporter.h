#pragma once

#include <string>
#include <vector>

#include "../opengl/model.h"

namespace assetimporter {

	void loadModel(const std::string& dir,
					const std::string& file, 
					std::vector<Vertex>& vertices,
					std::vector<uint32_t>& indices, 
					std::vector<std::string>& textures,
					Material& material);



}