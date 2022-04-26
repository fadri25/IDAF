#pragma once
#include "camera.h"
#include "renderable.h"
#include "../deltatime.h"
#include "cubemap.h"
#include "framebuffer.h"

#include <vector>

class Renderer {

	static Camera* cam;
	static DeltaTime* delta;

	const static int MAX_MODELS;
	static std::vector<glm::mat4> matrices;
	static Renderable** renderables;
	static int currentIndex;
	static bool inScene;
	
	static CubeMap* cubeMap;
	static bool deamon;
	static float elapsedTime;
	const static float DURATION;
	const static float STEP;
	static float angle;

	static Framebuffer* frameBuffer;


public:

	static glm::vec3 lightPos;
	static glm::vec3 lightColor;
	static glm::vec3 lightColor2;
	static float ambientStrength;


	Renderer() = delete;

	static void init(Camera* camera, DeltaTime* d);
	static void terminate();

	static void submitMatrices(const std::vector<glm::mat4>& m);
	static void submit(Renderable* r);


	static void beginScene();
	static void clear();
	static void render(bool reflection = false);
	static void renderModelsWithMatrices(float alpha = 1.0f);
	static void flush();
	static void endScene();

	static void bar();
	static void foo();
	static void emply();
	static bool isDaemon();

	static void setCubeMap(CubeMap* cm);
private:
	static void setShaderUniforms(Shader* shader, float ambient = 0.0f);

};