#include <iostream>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core.hpp>


#include "opengl/vertex.h"
#include "opengl/window.h"
#include "opengl/buffers.h"
#include "opengl/camera.h"
#include "opengl/shader.h"
#include "opengl/model.h"
#include "opengl/renderer.h"
#include "opengl/framebuffer.h"
#include "opengl/quad.h"

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utils/logger.hpp"


#include "assimp/assetimporter.h"

#include "deltatime.h"
#include "fileio.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

int frames = 0;

Model* model = nullptr;
Camera* camera;

std::vector<Model*> models;
int currentModel = 0;

DeltaTime delta;
bool running = true;

int frameWidth = 0;
int frameHeight = 0;
int midX = 0;
int midY = 0;


void keyFunc(GLFWwindow* window, int key, int scan, int action, int mods) {

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_A) {
			model->rotate({0.0f, 0.9f, 0.0f}, -3.141592f / 64.0f);
		}
		else if (key == GLFW_KEY_D) {
			model->rotate({ 0.0f, 1.0f, 0.0f }, 3.141592f / 64.0f);
		}
		
		if (key == GLFW_KEY_W) {
			model->rotate({ 1.0f, 0.0f, 0.0f }, 3.141592f / 64.0f);
		}
		else if (key == GLFW_KEY_S) {
			model->rotate({ 1.0f, 0.0f, 0.0f }, -3.141592f / 64.0f);
		}

		if (key == GLFW_KEY_DOWN) {
			model->translate({ 0.0f, -0.5f, 0.0f });
		}
		else if (key == GLFW_KEY_UP) {
			model->translate({ 0.0f, 0.5f, 0.0f });
		}

		if (key == GLFW_KEY_RIGHT) {
			model->translate({ 0.5f, 0.0f, 0.0f });
		}
		else if (key == GLFW_KEY_LEFT) {
			model->translate({ -0.5f, 0.0f, 0.0f });
		}

		if (key == GLFW_KEY_K) {
			model->translate({ 0.0f, 0.0f, -0.5f});
		}
		else if (key == GLFW_KEY_I) {
			model->translate({ 0.0f, 0.0f, 0.5f});
		}

		if (key == GLFW_KEY_N) {
			currentModel++;
			if (currentModel >= models.size()) currentModel = 0;
			model = models[currentModel];
		}
		else if (key == GLFW_KEY_P) {
			currentModel--;
			if (currentModel <= -1) currentModel = models.size() -1;
			model = models[currentModel];
		}

		if (key == GLFW_KEY_T) {
			model->scale({ 1.1f, 1.1f, 1.1f });
		}
		else if (key == GLFW_KEY_G) {
			model->scale({ 0.9f, 0.9f, 0.9f });
		}

		if (key == GLFW_KEY_F1) {
			model->resetTransformationMatrix();
		}

		if (key == GLFW_KEY_ESCAPE) {
			running = false;
		}

	}

}


// Lädt alle Models
void loadModels(Shader* shader) {
	std::string dir = "res/models/";
	const char* modelListFile = "models.txt";

	std::vector<std::string> modelList;
	readFileLineByLine(dir + modelListFile, modelList);

	models.reserve(modelList.size());

	for (int i = 0; i < modelList.size(); i++) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::string texFile; 
		assetimporter::loadModel(dir + modelList[i], vertices, indices, texFile);
		models.emplace_back(new Model(vertices, indices, texFile, shader));
	}

}


glm::vec3 convertToGLCoords(int srcX, int srcY) {

	glm::vec3 coords(0.0f);
	
	if (srcX > midX) {
	
		int temp = srcX - midX;
		coords.x = (float)temp / (float)midX;
	}
	else if (srcX < midX) {
		int temp = midX - srcX;
		coords.x = -((float)temp / (float)midX);
	}
	else {
		coords.x = 0.0f;
	}

	if (srcY > midY) {

		int temp = srcY - midY;
		coords.y = (float)temp / (float)midY;
	}
	else if (srcY < midY) {
		int temp = midY - srcY;
		coords.y = -((float)temp / (float)midY);
	}
	else {
		coords.y = 0.0f;
	}

	return coords;
}

glm::vec4 getScreenSpaceCoordinates(int x, int y) {
	glm::vec4 src(x, y, 0.0f, 1.0f);

	glm::mat4 modelMatrix(1.0f);

	glm::mat4 mvp = camera->getViewProjection() * modelMatrix;

	src = mvp * src;


	return src;
}


void terminate() {
	delete camera;

	for (int i = models.size() -1; i > -1; i--) {
		delete models[i];
	}
}

int main() {

	Window window("GL_Window", 200, 200, 1080, 720);
	camera = new Camera(60.f, 1080.0f / 720.0f, 0.01f, 1000.0f);
	window.setKeyFunc(keyFunc);
	Shader shader("res/shaders/shader");

	Renderer::init(camera);

	loadModels(&shader);
	model = models[currentModel];




	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);


	cv::VideoCapture cap(0);
	
	frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	midX = frameWidth / 2;
	midY = frameHeight / 2;



	cv::Mat frame;


	int x = (float) frameWidth * 0.75f;
	int y = (float) frameHeight * 0.75f;
	glm::vec3 pos = convertToGLCoords( x, y );
	//glm::vec4 translation = getScreenSpaceCoordinates(pos.x, pos.y);
	float scale = 0.05f;
	//model->translate({translation.x, translation.y, translation.z});
	//printf("translation: %f %f %f", translation.x, translation.y, translation.z);
	//model->scale({scale, scale, scale});
	//model->translate({ 2.5f, 2.5f, 0.0f});

	
	/*
	float d = camera->getDistanceToProjectionPlane((float)window.getWidth());
	glm::mat4 vp = camera->getViewProjection();
	glm::vec4 origin = vp * glm::mat4(1.0f) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::vec3 translation;
	translation.x = pos.x * (d + origin.z + 5.0f) / d;
	float k = translation.x / pos.x;
	translation.y = pos.y * k;
	*/
	//model->translate(translation);

	Quad quad(1.0f * camera->getAspectRatio(), 1.0f, &shader);
	quad.translate({ 0.0, 0.0, -10.0f});
	quad.scale({ 8.7f, 8.7f, 0.0f });
	

	while (running) {

		cap >> frame;
		Texture* tex = Texture::createTextureFromData(frameWidth, frameHeight, GL_BGR, frame.data);
		quad.setTexture(tex); 

		while ((float)delta >= 1.0f) {
			//printf("fps = %i\n", frames);
			//printf("delta = %f\n", (float)delta);
			//printf("frametime = %f\n", delta.getDelta());
			frames = 0;
			delta--;
		}
		delta.update();
		window.pollEvents();


		Renderer::beginScene();
		Renderer::submit(&quad);
		Renderer::submit(model);
		Renderer::render();
		Renderer::endScene();
		frames++;

	

		window.swapBuffers();

		delete tex;
		if (window.shouldClose()) {
			break;
		}

	}


	
	Renderer::terminate();
	terminate();

	return 0;
}