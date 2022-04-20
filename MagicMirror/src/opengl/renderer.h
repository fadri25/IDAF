#pragma once
#include "camera.h"
#include "renderable.h"
#include "../deltatime.h"

#include <vector>

class Renderer {

	static Camera* cam;
	static DeltaTime* delta;

	const static int MAX_MODELS;
	static std::vector<glm::mat4> matrices;
	static Renderable** renderables;
	static int currentIndex;
	static bool inScene;
	
	static bool discoMode;
	static float elapsedTime;
	const static float DISCO_DURATION;
	const static float DISCO_STEP;
	static float angle;


public:

	static glm::vec3 lightPos;
	static glm::vec3 lightColor;
	static glm::vec3 discoLightColor;
	static float ambientStrength;


	Renderer() = delete;

	static void init(Camera* camera, DeltaTime* d);
	static void terminate();

	static void submitMatrices(const std::vector<glm::mat4>& m);
	static void submit(Renderable* r);


	static void beginScene();
	static void clear();
	static void render();
	static void renderModelsWithMatrices(float alpha = 1.0f);
	static void flush();
	static void endScene();

	static void getReadyToParty();
	static void crashParty();
	static bool inDisco();

private:
	static void setShaderUniforms(Shader* shader);
	static void discoDiscoPartyParty();

};