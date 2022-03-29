#pragma once
#include "camera.h"
#include "renderable.h"

class Renderer {

	static Camera* cam;

	const static int MAX_MODELS;
	static Renderable** renderables;
	static int currentIndex;
	static bool inScene;

public:
	Renderer() = delete;

	static void init(Camera* camera);
	static void terminate();


	static void submit(Renderable* r);
	static void beginScene();
	static void render();
	static void flush();
	static void endScene();


};