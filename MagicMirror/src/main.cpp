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
#include "opengl/cubemap.h"

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

#include <math.h>


#define LOG(x) printf(#x + "\n")

bool debug = false;

bool keyOnePressed = false;
bool keyTwoPressed = false;
bool keyThreePressed = false;


int frames = 0;

Model* model = nullptr;
Quad* quad = nullptr;
Model* model2 = nullptr;
CubeMap* cubeMap = nullptr;
int sequence = 0;

Camera* camera;

Shader* shader = nullptr;
Shader* flatShader = nullptr;
Shader* reflectionShader = nullptr;

std::vector<Model*> models;
int currentModel = 0;
int nextModel = currentModel + 1;
bool inCycle = true;
bool inFade = false;
static float cycleDuration = 10.0f;
static float fadeDuration = 5.0f;
float elapsedTime = 0.0f;


static const int MAX_MODELS = 10;

DeltaTime delta;

int frameWidth = 0;
int frameHeight = 0;
int midX = 0;
int midY = 0;

void startFade(int increment);
void notify();

// Erkennt Input und führt die gewünschte Funktionen für die Taster aus, und Funktionen für debuggin Zwecke auf der Tastatur
void keyFunc(GLFWwindow* window, int key, int scan, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (debug) {
			if (key == GLFW_KEY_A) {
				model->rotate({ 0.0f, 0.9f, 0.0f }, -M_PI / 64.0f);
			}
			else if (key == GLFW_KEY_D) {
				model->rotate({ 0.0f, 1.0f, 0.0f }, M_PI / 64.0f);
			}

			if (key == GLFW_KEY_W) {
				model->rotate({ 1.0f, 0.0f, 0.0f }, M_PI / 64.0f);
			}
			else if (key == GLFW_KEY_S) {
				model->rotate({ 1.0f, 0.0f, 0.0f }, -M_PI / 64.0f);
			}

			if (key == GLFW_KEY_DOWN) {
				model->translate({ 0.0f, -0.1f, 0.0f });
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
				model->translate({ 0.0f, 0.0f, -0.5f });
			}
			else if (key == GLFW_KEY_I) {
				model->translate({ 0.0f, 0.0f, 0.5f });
			}

			if (key == GLFW_KEY_T) {
				model->scale({ 1.1f, 1.1f, 1.1f });
			}
			else if (key == GLFW_KEY_G) {
				model->scale({ 0.9f, 0.9f, 0.9f });
			}

			if (key == GLFW_KEY_F2) {
				model->resetTransformationMatrix();
			}
		}

		

		if (!inFade) {
			if (key == GLFW_KEY_N) {
				inCycle = false;
				startFade(1);
				elapsedTime = 0.0f;
				keyOnePressed = true;
				notify();
			}
			else if (key == GLFW_KEY_P) {
				inCycle = false;
				startFade(-1);
				elapsedTime = 0.0f;
				keyTwoPressed = true;
				notify();
			}
			else if (key == GLFW_KEY_C) {
				keyThreePressed = true;
				notify();

				if (inCycle) {
					return;
				}

				inCycle = true;
				elapsedTime = 0.0f;
			}
		}
		else {
			if (key == GLFW_KEY_N) {
				keyOnePressed = true;
				notify();
			}
			else if (key == GLFW_KEY_P) {
				keyTwoPressed = true;
				notify();
			}
			else if (key == GLFW_KEY_C) {
				keyThreePressed = true;
				notify();
			}
		}


		if (key == GLFW_KEY_F1) {
			debug = !debug;
		}

		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, 1);
		}

	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_N) {
			keyOnePressed = false;
		}
		else if (key == GLFW_KEY_P) {
			keyTwoPressed = false;
		}
		else if (key == GLFW_KEY_C) {
			keyThreePressed = false;
		}
	}

}

void notify() {
	if (keyOnePressed && keyTwoPressed && keyThreePressed) {
		if (!Renderer::isDaemon()) {
			Renderer::bar();
			quad->setShader(shader);
			sequence = 1;
		}
		else {
			Renderer::emply();
			quad->setShader(flatShader);
			sequence = 2;
		}
	}
}



// Lädt alle Modelle
void loadModels(Shader* shader) {
	std::string dir = "res/models/";

	std::vector<std::string> files;
	readFileLineByLine(dir + "models.txt", files);
	

	models.reserve(files.size());

	for (int i = 0; i < files.size(); i++) {

		std::vector<std::string> info;
		readFileLineByLine(dir + files[i] + "/info.txt", info);
		
		int position = info.size() > 0 ? std::stof(info[0]) : Model::Position::NONE;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::string> textures;
		Material material;

		std::string modelDir = dir + files[i] + "/";
		assetimporter::loadModel(modelDir, files[i] + ".obj", vertices, indices, textures, material);
		models.emplace_back(new Model(vertices, indices, textures[0], shader, material, position));
	}

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<std::string> textures;
	Material material;

	assetimporter::loadModel("res/models/discoball/", "discoball.obj", vertices, indices, textures, material);
	material.metallic = 1.0f;
	material.specularStrength = 1.0f;
	model2 = new Model(vertices, indices, textures[0], reflectionShader, material, Model::Position::NONE);
	model2->translate({0.0f, 6.0f, 2.0f});

}


// Wechsel zu dem nächsten 3D-Modell
void swapModels() {
	currentModel = nextModel;
	model = models[currentModel];
}

// Beginnt Übergangsverlauf
void startFade(int increment) {
	inFade = true;
	nextModel = currentModel + increment;
	if (nextModel == models.size()) {
		nextModel = 0;
	}
	else if (nextModel == -1) {
		nextModel = models.size() - 1;
	}
}

// Beendet Übergangsverlauf
void endFade() {
	swapModels();
	inFade = false;
}



// Konvertiert relative Fensterkoordinaten zu OpenGL Koordinaten
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

// Räumt Arbeitsspeicher auf
void terminate() {
	delete camera;

	for (int i = models.size() -1; i > -1; i--) {
		delete models[i];
	}

	delete quad;
	delete cubeMap;
	delete model2;

	delete shader;
	delete reflectionShader;
	delete flatShader;

}

// Einstiegspunkt des Programmes
int main() {

	Window window("GL_Window", 200, 200, 1920, 1080);
	camera = new Camera(60.0f, 1920.0f / 1080.0f, 0.01f, 1000.0f);
	window.setKeyFunc(keyFunc);
	
	shader = new Shader("res/shaders/shader");
	flatShader = new Shader("res/shaders/flatshader");
	reflectionShader = new Shader("res/shaders/cubemapshader");

	{
		std::string dir = "res/cubemap/";
		const std::string* files = new std::string[6] {
			dir + "right.png",
			dir + "left.png",
			dir + "top.png",
			dir + "bottom.png",
			dir + "front.png",
			dir + "back.png"

		};

		cubeMap = new CubeMap(files);
		delete[] files;
	}

	Renderer::init(camera, &delta);

	loadApplicationSettings("res/settings.txt", &cycleDuration, &fadeDuration);
	loadModels(shader);
	model = models[currentModel];


	Face faces[MAX_MODELS];
	int currentFace = 0;


	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	// Verbindung zu Kamera aufbauen
	cv::VideoCapture cap(0 /*"http://169.254.104.153:8080/?action=stream"*/);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 1080);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

	
	frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	midX = frameWidth / 2;
	midY = frameHeight / 2;


	cv::Mat frame;


	float scale = 1.0f;

	float near = camera->getNear();
	float dist = camera->getZdistance();
	float distToFace = 10.0f + near;


	// Fläche für die Darstellung des Kamerabildes erstellen und positionieren
	quad = new Quad(1.0f * camera->getAspectRatio(), 1.0f, flatShader);
	quad->translate({ 0.0f, 0.0f, -10.0f });
	quad->scale({ 11.55f, 11.55f, 0.0f }); // 8.7f
	Texture* tex = Texture::createTextureFromData(frameWidth, frameHeight, GL_RGBA, nullptr);
	quad->setTexture(tex);

	// Neurale Netzwerke für die Gesichts- & Augenerkennung laden
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


	// Hauptschlaufe
	while (true) {

		// Nächstes Bild von der Kamera lesen
		cap >> frame;
		if (frame.empty()) {
			printf("empty frame");
			__debugbreak();
			continue;
		}

		// Bild an der Y-Achse Spiegeln
		cv::flip(frame, frame, 1);
	

		// Logik für Transparenzübergang und Auto-wechseltimer
		float fadeInAlpha = 0.0f;
		if (inFade) {
			elapsedTime += delta.getDelta();
			if (elapsedTime >= fadeDuration) {
				endFade();
				elapsedTime = 0.0f;
			}
			else {
				fadeInAlpha = elapsedTime / fadeDuration;
			}
		}
		else if (inCycle) {
			elapsedTime += delta.getDelta();
			if (elapsedTime >= cycleDuration) {
				startFade(1);
				elapsedTime = 0.0f;
			}
		}

		
		if ((float)delta >= 1.0f) {
			//printf("fps = %i\n", frames);
			//printf("delta = %f\n", (float)delta);
			//printf("frametime = %f\n", delta.getDelta());
			frames = 0;
			delta--;
		}
		delta.update();

		// Eventhandling
		window.pollEvents();


		// opencv
		std::vector<cv::Rect> faceRects;
		std::vector<cv::Rect> eyes;

		// Leere Bilder für Graustufe und Skalierte version des Kamerabildes
		cv::Mat gray, smallImg;

		// Zu Graustufe konvertieren
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		// Scale value
		double cvScale = 4.0;
		double fx = 1 / cvScale;

		// Bild skalieren
		cv::resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
		cv::equalizeHist(smallImg, smallImg);



		// Gesichter in Bild erkennen
		faceCascade.detectMultiScale(smallImg, faceRects, 1.1, 5, 0, cv::Size(5, 5));
		
		if (facePoints.size() > 0) {
			facePoints.erase(facePoints.begin(), facePoints.end());
		}
		facePoints.reserve(faceRects.size());


		cv::Mat smallImgROI;
		cv::Scalar blue = cv::Scalar(255, 0, 0);
		cv::Scalar green = cv::Scalar(0, 255, 0);
		cv::Scalar red = cv::Scalar(0, 0, 255);

		// Koordinaten verarbeiten
		for (size_t i = 0; i < faceRects.size(); i++) {
			cv::Rect r = faceRects[i];

			cv::Rect scaledUp;
			scaledUp.x = cvRound(r.x * cvScale);
			scaledUp.y = cvRound(r.y * cvScale);
			scaledUp.width = cvRound((r.x + r.width - 1) * cvScale);
			scaledUp.height = cvRound((r.y + r.height - 1) * cvScale);

			// Rechteck um das Gesicht zeichnen für Debugging
			if (debug) {
				cv::rectangle(frame, cv::Point(scaledUp.x, scaledUp.y), cv::Point(scaledUp.width, scaledUp.height), blue, 3, 8, 0);
			}

			cv::Point facePos;
			facePos.x = (scaledUp.x + (scaledUp.width - scaledUp.x) / 2);
			facePos.y = (scaledUp.y + (scaledUp.height - scaledUp.y) / 2);
			facePoints.emplace_back(facePos);


			if (currentFace < MAX_MODELS) {
				faces[currentFace].set({scaledUp.x, scaledUp.y, scaledUp.width - scaledUp.x, scaledUp.height - scaledUp.y });
				currentFace++;
			}
			Face* face_ptr = &faces[currentFace - 1];

			// Sektion des Gesichts in separates Bild kopieren um die Augenerkennung zu optimieren
			smallImgROI = smallImg(r);


			// Augen erkennen
			eyeCascade.detectMultiScale(smallImgROI, eyes, 1.1, 3, 0, cv::Size(5, 5), cv::Size(30, 30));

			// Augenkoordinaten verarbeiten
			for (size_t j = 0; j < eyes.size(); j++) {

				cv::Rect nr = eyes[j];
				scaledUp.x = cvRound((r.x + nr.x) * cvScale);
				scaledUp.y = cvRound((r.y + nr.y) * cvScale);
				scaledUp.width = cvRound((r.x + nr.x + nr.width - 1) * cvScale);
				scaledUp.height = cvRound((r.y + nr.y + nr.height - 1) * cvScale);

				// Rechteck um Auge zeichnen
				if (debug) {
					cv::rectangle(frame, cv::Point(scaledUp.x, scaledUp.y), cv::Point(scaledUp.width, scaledUp.height), green, 3, 8, 0);
				}

				glm::vec4 f_rect = { scaledUp.x, scaledUp.y, scaledUp.width - scaledUp.x, scaledUp.height - scaledUp.y };
				if (face_ptr->left.valid) {
					face_ptr->right.set(f_rect);
				}
				else {
					face_ptr->left.set(f_rect);
				}
			}

			// Kontrollieren ob linkes und rechtes Auge in einem Gesicht nicht vertauscht sind
			if (face_ptr->left.valid && face_ptr->right.valid) {
				if (face_ptr->right.rect.x < face_ptr->left.rect.y) {
					face_ptr->swapEyes();
				}
			}
			else if (face_ptr->left.valid || face_ptr->right.valid) {
				face_ptr->checkEyes();
			}
		}


		// Liste von Transformationsmatrizen vorbereiten
		std::vector<glm::mat4> matrices;		
		matrices.reserve(currentFace);
		std::vector<glm::mat4> secondaryMatrices;
		if (inFade) {
			secondaryMatrices.reserve(currentFace);
		}

		// Translationsvektor berechnen
		for (int i = 0; i < currentFace; i++) {
			Face* f = &faces[i];
			
			glm::vec2 coords = f->getPointCoordinates(model->getPosition());
			
			glm::vec3 endPos = convertToGLCoords(coords.x, coords.y);
			float inverseScale = 1.0f / scale;
			glm::vec3 translation(endPos.x * inverseScale * (distToFace), endPos.y * inverseScale * (distToFace) * 0.7f, 0.0f);
			float yaw = getYaw(translation, distToFace) * 0.75f;
			
			glm::mat4 m(1.0f);
			m = glm::scale(m, { scale, scale, scale });
			m = glm::translate(m, translation);
			m = glm::rotate(m, yaw, { 0.0f, 1.0f, 0.0f });
			matrices.push_back(m);
			
			// In Tranparenzübergangsphase das selbe für das nächste 3D-Modell
			if (inFade) {
				glm::vec2 coords2 = f->getPointCoordinates(models[nextModel]->getPosition());
				glm::vec3 endPos2 = convertToGLCoords(coords2.x, coords2.y);
				glm::vec3 translation2(endPos2.x* inverseScale* (distToFace), endPos2.y* inverseScale* (distToFace) * 0.7f, 0.0f);
				glm::mat4 m2(1.0f);
				m2 = glm::scale(m2, { scale, scale, scale });
				m2 = glm::translate(m2, translation2);
				m2 = glm::rotate(m2, yaw, { 0.0f, 1.0f, 0.0f });
				secondaryMatrices.push_back(m2);
			}
		}


		// Textur für Spiegelbild updaten
		tex->write(frame.data, frameWidth, frameHeight, GL_BGR);

		// Daten an Renderer übergeben und Drawcall ausführen
		Renderer::clear();
		Renderer::beginScene();
		
		if (Renderer::isDaemon()) {
			Renderer::foo();
		}

		Renderer::submit(quad);
		Renderer::render();

	
		switch (sequence) {
			case 1: {
				glm::vec4 pos = camera->getViewProjection() * model2->getTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				if (pos.y > 5.5f) {
					model2->translate({ 0.0f, -0.5f * delta.getDelta(), 0.0f});
				}
				else {
					sequence = 0;
				}
				break;
			}
			case 2: {
				glm::vec4 pos = camera->getViewProjection() * model2->getTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				if (pos.y < 10.0f) {
					model2->translate({ 0.0f, 0.5f * delta.getDelta(), 0.0f });
				}
				else {
					sequence = 0;
				}
				break;
			}
		}

		if (Renderer::isDaemon() || sequence != 0) {
			Renderer::setCubeMap(cubeMap);
			model2->rotate({0.0f, 1.0f, 0.0f}, M_PI * 0.1f * delta.getDelta());
			Renderer::submit(model2);
			Renderer::render(true);
			Renderer::setCubeMap(nullptr);
		}

		
		Renderer::submitMatrices(matrices);
		Renderer::submit(model);
		
		if (inFade) {
			Renderer::renderModelsWithMatrices(1.0f - fadeInAlpha);

			Renderer::submitMatrices(secondaryMatrices);
			Model* m = models[nextModel];
			Renderer::submit(dynamic_cast<Renderable*>(models[nextModel]));
			Renderer::renderModelsWithMatrices(fadeInAlpha);
		}
		else {
			Renderer::renderModelsWithMatrices();
		}
		
		Renderer::endScene();
		frames++;
		

		for (int i = 0; i < currentFace; i++)
			faces[i].invalidate();
		currentFace = 0;
	

		window.swapBuffers();


		if (window.shouldClose()) {
			break;
		}

	}


	// Arbeitsspeicher freigeben
	Renderer::terminate();
	terminate();

	return 0;
}