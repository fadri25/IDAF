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
	static float fadeInAlpha;

public:
	Renderer() = delete;

	static void init(Camera* camera);
	static void terminate();

	static void submitMatrices(const std::vector<glm::mat4>& m);
	static void submit(Renderable* r);

	static void setFadeInAlpha(float alpha);

	static void beginScene();
	static void clear();
	static void render();
	static void renderModelsWithMatrices(bool fade = false, float alpha = 1.0f);
	static void flush();
	static void endScene();


};