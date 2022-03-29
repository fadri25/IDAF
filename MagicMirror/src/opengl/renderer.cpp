#include "renderer.h"

#include <iostream>

#include "includegl.h"


Camera* Renderer::cam;

const int Renderer::MAX_MODELS = 10;
Renderable** Renderer::renderables;
int Renderer::currentIndex = 0;
bool Renderer::inScene = false;


// Initialisierung
void Renderer::init(Camera* camera) {
	cam = camera;
	renderables = new Renderable *[MAX_MODELS];
}

// Arbeitsspeicher freigeben
void Renderer::terminate() {
	delete[] renderables;
}

// Fügt ein 3D-Modell in die Array der zu rendernden Modelle ein
void Renderer::submit(Renderable* r) {
	if (currentIndex == MAX_MODELS) {
		printf("Maximum model capacity reached! Model discarded\n");
		return;
	}

	renderables[currentIndex] = r;
	currentIndex++;
}

// Startet eine neue Szene
void Renderer::beginScene() {
	if (inScene) {
		printf("Renderer already began a Scene\n");
		return;
	}
	currentIndex = 0;
	cam->lock();
	inScene = true;
}

// Macht für jedes Modell einen draw call an den Grafikprozessor
void Renderer::render() {
	if (!inScene) {
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 pv = cam->getViewProjection();
	for (int i = 0; i < currentIndex; i++) {
		renderables[i]->bind(pv);
		glDrawElements(GL_TRIANGLES, renderables[i]->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

// Leert die Liste der Modelle und beendet die Szene
void Renderer::flush() {
	for (int i = 0; i < MAX_MODELS; i++) {
		renderables[i] = nullptr;
	}
	if (inScene) {
		endScene();
	}
}

// Beendet die Szene
void Renderer::endScene() {
	cam->unlock();
	inScene = false;
}