#include <iostream>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core.hpp>

#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>

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
#include "timer.h"
#include "face.h"
#include "eye.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

int frames = 0;

Model* model = nullptr;
Camera* camera;

std::vector<Model*> models;
int currentModel = 0;

static const int MAX_FACES = 10;

DeltaTime delta;

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
			glfwSetWindowShouldClose(window, 1);
		}

	}

}


std::string split(const std::string& s, const char delimeter) {
	int start = s.find_first_of('=');
	return s.substr(s.find_first_of('='));
}

// Lädt alle Models
void loadModels(Shader* shader) {
	std::string dir = "res/models/";
	const char* modelListFile = "models.txt";

	std::vector<std::string> modelList;
	readFileLineByLine(dir + modelListFile, modelList);
	

	models.reserve(modelList.size());

	for (int i = 0; i < modelList.size(); i++) {

		std::vector<std::string> info;
		readFileLineByLine(dir + modelList[i] + "/info.txt", info);
		int position = std::stoi(info[0]);

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::string modelDir = dir + modelList[i] + "/";
		std::string modelFile = modelDir + modelList[i] + ".obj";
		std::vector<std::string> textures;
		assetimporter::loadModel(modelDir, modelList[i] + ".obj", vertices, indices, textures);
		models.emplace_back(new Model(vertices, indices, /*modelDir + */ textures[0], shader, position));
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

// Winkel von gesicht zu Kamera berechnen
float getYaw(const glm::vec3& translation, float distToFace) {


	glm::vec3 camPos = camera->getPosition();
	glm::mat4 mvp = camera->getViewProjection() * glm::translate(glm::mat4(1.0f), translation);
	glm::vec4 pos = mvp * glm::translate(glm::mat4(1.0f), translation) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return (float)atan(pos.x / camPos.z);
}

void terminate() {
	delete camera;

	for (int i = models.size() -1; i > -1; i--) {
		delete models[i];
	}
}

int main() {

	Window window("GL_Window", 200, 200, 1920, 1080);
	camera = new Camera(60.f, 1920.0f / 1080.0f, 0.01f, 1000.0f);
	window.setKeyFunc(keyFunc);
	Shader shader("res/shaders/shader");
	Shader flatShader("res/shaders/flatshader");

	Renderer::init(camera);

	loadModels(&shader);
	model = models[currentModel];


	Face face_str[MAX_FACES];
	int currentFace = 0;




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
	float scale = 1.0f;// 0.05f;
	model->scale({scale, scale, scale});
	//model->translate({ 4.3f * 0.5f, 2.95f * 0.5f, 0.0f});

	float near = camera->getNear();
	float dist = camera->getZdistance();
	float d2 = dist - near;
	float distToFace = 10.0f + near;


	Quad quad(1.0f * camera->getAspectRatio(), 1.0f, &flatShader);
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
	
	std::vector<cv::Point> facePoints;
	//cv::Point facePos;
	
	//cv::namedWindow("w");

	while (true) {
		//Timer timer("main loop");

		cap >> frame;
		if (frame.empty()) {
			printf("empty frame");
			continue;
		}

		cv::flip(frame, frame, 1);
	

		if ((float)delta >= 1.0f) {
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
		double cvScale = 4.0;
		double fx = 1 / cvScale;

		// Resize the grayscale mmage 
		cv::resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
		cv::equalizeHist(smallImg, smallImg);



		// Detect faces in the scaled image
		faceCascade.detectMultiScale(smallImg, faces, 1.1, 5, 0, cv::Size(5, 5));
		
		if (facePoints.size() > 0) {
			facePoints.erase(facePoints.begin(), facePoints.end());
		}
		facePoints.reserve(faces.size());


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


			cv::rectangle(frame, cv::Point(scaledUp.x, scaledUp.y), cv::Point(scaledUp.width, scaledUp.height), blue, 3, 8, 0);

			cv::Point facePos;
			facePos.x = (scaledUp.x + (scaledUp.width - scaledUp.x) / 2);
			facePos.y = (scaledUp.y + (scaledUp.height - scaledUp.y) / 2);
			facePoints.emplace_back(facePos);


			if (currentFace < MAX_FACES) {
				face_str[currentFace].set({scaledUp.x, scaledUp.y, scaledUp.width - scaledUp.x, scaledUp.height - scaledUp.y });
				currentFace++;
			}
			Face* face_ptr = &face_str[currentFace - 1];

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

				cv::rectangle(frame, cv::Point(scaledUp.x, scaledUp.y), cv::Point(scaledUp.width, scaledUp.height), green, 3, 8, 0);
				
				glm::vec4 f_rect = { scaledUp.x, scaledUp.y, scaledUp.x + scaledUp.width, scaledUp.y + scaledUp.height };
				if (face_ptr->left.valid) {
					face_ptr->right.set(f_rect);
				}
				else {
					face_ptr->left.set(f_rect);
				}
			}

			if (face_ptr->left.valid && face_ptr->right.valid) {
				if (face_ptr->right.rect.x < face_ptr->left.rect.y) {
					face_ptr->swapEyes();
				}
			}
		}


		std::vector<glm::mat4> matrices;		
		matrices.reserve(currentFace);
		for (int i = 0; i < currentFace; i++) {
			Face* f = &face_str[i];
			
			float roll = f->getZangle();
			//printf("roll: %f\n", roll);
			glm::vec2 coords = f->getPointCoordinates(model->getPosition());
			glm::vec3 endPos = convertToGLCoords(coords.x, coords.y);
			float inverseScale = 1.0f / scale;
			glm::vec3 translation(endPos.x * inverseScale * (distToFace) * 0.65f, endPos.y * inverseScale * (distToFace) * 0.45f, 0.0f);
			glm::mat4 m(1.0f);
			float yaw = getYaw(translation, distToFace);
			printf("yaw: %f\n", yaw);
			//m = glm::rotate(m, roll, { 0.0f, 0.0f, 1.0f });
			m = glm::scale(m, { scale, scale, scale });
			m = glm::translate(m, translation);
			m = glm::rotate(m, yaw, { 0.0f, 1.0f, 0.0f });
			matrices.push_back(m);
		}

		
		Texture* tex = Texture::createTextureFromData(frameWidth, frameHeight, GL_BGR, frame.data);
		quad.setTexture(tex);


		Renderer::clear();
		
		Renderer::beginScene();
		Renderer::submit(&quad);
		Renderer::render();
		Renderer::endScene();
		

		Renderer::beginScene();
		Renderer::submitMatrices(matrices);
		Renderer::submit(model);
		Renderer::renderModelsWithMatrices();
		Renderer::endScene();
		Renderer::flush();
		frames++;
		delete tex;
		
		for (int i = 0; i < currentFace; i++)
			face_str[i].invalidate();
		currentFace = 0;
	

		window.swapBuffers();

		if (window.shouldClose()) {
			break;
		}

	}


	
	Renderer::terminate();
	terminate();

	return 0;
}