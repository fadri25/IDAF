#pragma once
#include "camera.h"
#include "renderable.h"

#include <vector>

class Renderer {

	static Camera* cam;

	const static int MAX_MODELS;
	static std::vector<glm::mat4> matrices;
	static Renderable** renderables;
	static int currentIndex;
	static bool inScene;

public:
	Renderer() = delete;

	static void init(Camera* camera);
	static void terminate();

	static void submitMatrices(const std::vector<glm::mat4>& m);
	static void submit(Renderable* r);

	static void beginScene();
	static void clear();
	static void render();
	static void renderModelsWithMatrices();
	static void flush();
	static void endScene();


};