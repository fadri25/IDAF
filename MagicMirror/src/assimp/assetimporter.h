#pragma once

#include <string>
#include <vector>

#include "../opengl/model.h"

namespace assetimporter {

	void loadModel(const std::string& file, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::string& texFile);



}