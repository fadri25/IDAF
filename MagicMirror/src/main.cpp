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
			model->translate({ 0.0f, -0.1f, 0.0f});
		}
		else if (key == GLFW_KEY_UP) {
			model->translate({ 0.0f, 0.1f, 0.0f });
		}

		if (key == GLFW_KEY_RIGHT) {
			model->translate({ 0.1f, 0.0f, 0.0f });
		}
		else if (key == GLFW_KEY_LEFT) {
			model->translate({ -0.1f, 0.0f, 0.0f });
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
		coords.y = -((float)temp / (float)midY);
	}
	else if (srcY < midY) {
		int temp = midY - srcY;
		coords.y = ((float)temp / (float)midY);
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
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1080);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

	
	frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	midX = frameWidth / 2;
	midY = frameHeight / 2;



	cv::Mat frame;


	int x = (float) frameWidth * 0.75f;
	int y = (float) frameHeight * 0.75f;
	glm::vec3 pos = convertToGLCoords( x, y );
	float scale = 0.05f;
	model->scale({scale, scale, scale});
	//model->translate({ 4.3f * 0.5f, 2.95f * 0.5f, 0.0f});

	float near = camera->getNear();
	float dist = camera->getZdistance();
	float d2 = dist - near;
	float distToFace = 10.0f + near;

	// IMPORTANT!!!! viewport right: 4.3f, left: -4.3f, top: 2.9f, bottom: -2.9f

	//glm::vec3 translation(pos.x * 4.3f * d2 / dist, pos.y * 2.9f * d2 / dist, 0.0f);
	//model->translate(translation);

	Quad quad(1.0f * camera->getAspectRatio(), 1.0f, &shader);
	quad.translate({ 0.0f, 0.0f, -10.0f });
	quad.scale({ 8.7f, 8.7f, 0.0f }); // 8.7f

	cv::CascadeClassifier faceCascade;
	faceCascade.load("haarcascades/haarcascade_frontalface_default.xml");

	if (faceCascade.empty()) {
		printf("failed to load frontal face haarcascade\n");
		__debugbreak();
		return 0;
	}

	cv::CascadeClassifier eyeCascade;
	eyeCascade.load("haarcascades/haarcascade_eye.xml");

	if (eyeCascade.empty()) {
		printf("failed to load frontal eye haarcascade\n");
		__debugbreak();
		return 0;
	}
	

	//cv::namedWindow("w");

	while (running) {

		cap >> frame;
		if (frame.empty()) {
			printf("empty frame");
			continue;
		}

		cv::flip(frame, frame, 1);
	

		while ((float)delta >= 1.0f) {
			//printf("fps = %i\n", frames);
			//printf("delta = %f\n", (float)delta);
			//printf("frametime = %f\n", delta.getDelta());
			frames = 0;
			delta--;
		}
		delta.update();
		window.pollEvents();


		// opencv
		std::vector<cv::Rect> faces;
		std::vector<cv::Rect> eyes;

		// Create empyt image for grayscale and down sized version of original image
		cv::Mat gray, smallImg;

		// Convert image to grayscale
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		// Scale value
		double cvScale = 2.0;
		double fx = 1 / cvScale;

		// Resize the grayscale mmage 
		cv::resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
		cv::equalizeHist(smallImg, smallImg);



		// Detect faces in the scaled image
		faceCascade.detectMultiScale(smallImg, faces, 1.1, 5, 0, cv::Size(10, 10));
		cv::Point facePos;

		cv::Mat smallImgROI;
		cv::Scalar blue = cv::Scalar(255, 0, 0);
		cv::Scalar green = cv::Scalar(0, 255, 0);
		cv::Scalar red = cv::Scalar(0, 0, 255);
		// Iterate through rectangles around faces in the image
		for (size_t i = 0; i < faces.size(); i++) {
			cv::Rect r = faces[i];

			cv::Rect scaledUp;
			scaledUp.x = cvRound(r.x * cvScale);
			scaledUp.y = cvRound(r.y * cvScale);
			scaledUp.width = cvRound((r.x + r.width - 1) * cvScale);
			scaledUp.height = cvRound((r.y + r.height - 1) * cvScale);

			// Draw a rectangle around the face
			/*cv::rectangle(frame, cv::Point(cvRound(r.x * scale), cvRound(r.y * scale)),
					cv::Point(cvRound((r.x + r.width - 1) * scale),
					cvRound((r.y + r.height - 1) * scale)), color, 3, 8, 0);*/

			cv::rectangle(frame, cv::Point(scaledUp.x, scaledUp.y), cv::Point(scaledUp.width, scaledUp.height), blue, 3, 8, 0);

			//facePos.x = (scaledUp.x + (int) ((float) scaledUp.width * 0.5f));
			//facePos.y = (scaledUp.y + (int) ((float) scaledUp.height * 0.5f));
			facePos.x = (scaledUp.x + (scaledUp.width - scaledUp.x) / 2);
			facePos.y = (scaledUp.y + (scaledUp.height - scaledUp.y) / 2);

			// Copy section where the face is in the original image
			smallImgROI = smallImg(r);

			eyeCascade.detectMultiScale(smallImgROI, eyes, 1.1, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(1, 1), cv::Size(30, 30));

			// Draw circles around eyes
			for (size_t j = 0; j < eyes.size(); j++) {

				cv::Rect nr = eyes[j];
				scaledUp.x = cvRound((r.x + nr.x) * cvScale);
				scaledUp.y = cvRound((r.y + nr.y) * cvScale);
				scaledUp.width = cvRound((r.x + nr.x + nr.width - 1) * cvScale);
				scaledUp.height = cvRound((r.y + nr.y + nr.height - 1) * cvScale);
				/*
				center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
				center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
				radius = cvRound((nr.width + nr.height) * 0.25 * scale);
				circle(frame, center, radius, color, 3, 8, 0);
				*/

				cv::rectangle(frame, cv::Point(scaledUp.x, scaledUp.y), cv::Point(scaledUp.width, scaledUp.height), green, 3, 8, 0);
			
			}
		}


	
		if (facePos.x != 0.0f && facePos.y != 0.0f) {
			model->resetTransformationMatrix();
			model->scale({ scale, scale, scale });
			glm::vec3 endPos = convertToGLCoords(facePos.x, facePos.y);

			float inverseScale = 1.0f / scale;

			printf("facePos: %i %i\n", facePos.x, facePos.y);
			printf("midXY: %i %i", midX, midY);
			glm::vec3 translation(endPos.x * inverseScale * (distToFace) * 0.45f, endPos.y * inverseScale * (distToFace) * 0.45f, 0.0f);
			model->translate(translation);
		}
		else {
			model->resetTransformationMatrix();
			model->scale({ scale, scale, scale });
		}


		//cv::line(frame, cv::Point(midX, 0), cv::Point(midX, frameHeight), 1, 8, 0);
		//cv::line(frame, cv::Point(0, midY), cv::Point(frameWidth, midY), 1, 8, 0);
		//cv::imshow("w", frame);

		Texture* tex = Texture::createTextureFromData(frameWidth, frameHeight, GL_BGR, frame.data);
		quad.setTexture(tex);

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