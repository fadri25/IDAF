#include "renderer.h"

#include <iostream>

#include "includegl.h"


Camera* Renderer::cam;
DeltaTime* Renderer::delta = nullptr;


const int Renderer::MAX_MODELS = 10;
std::vector<glm::mat4> Renderer::matrices;
Renderable** Renderer::renderables;
int Renderer::currentIndex = 0;

bool Renderer::inScene = false;

glm::vec3 Renderer::lightPos = glm::vec3(0.0f, 0.0f, -0.5f);
glm::vec3 Renderer::lightColor = glm::vec3(1.0f);
glm::vec3 Renderer::discoLightColor = glm::vec3(1.0f);
float Renderer::ambientStrength = 0.4f;
bool Renderer::discoMode = false;
float Renderer::elapsedTime = 0.0f;

const float Renderer::DISCO_DURATION = 0.05f;
const float Renderer::DISCO_STEP = 360.0f * DISCO_DURATION;
float Renderer::angle = 0.0f;



// Initialisierung
void Renderer::init(Camera* camera, DeltaTime* d) {
	cam = camera;
	delta = d;
	renderables = new Renderable *[MAX_MODELS];
}

// Arbeitsspeicher freigeben
void Renderer::terminate() {
	delete[] renderables;
}

// Fügt eine Liste  von Transformationsmatrizen die Liste des Renderers ein
void Renderer::submitMatrices(const std::vector<glm::mat4>& m) {
	matrices.reserve(m.size());
	matrices.insert(matrices.begin(), m.begin(), m.end());
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

// Leert das aktuelle Bild
void Renderer::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// Macht für jedes Modell einen draw call an den Grafikprozessor
void Renderer::render() {
	if (!inScene) {
		return;
	}


	if (discoMode)
		discoDiscoPartyParty();

	glm::mat4 pv = cam->getViewProjection();
	for (int i = 0; i < currentIndex; i++) {
		renderables[i]->bind(pv);
		Shader* shader = renderables[i]->getShader();
		shader->setFloat("alpga", 1.0f);
		setShaderUniforms(shader);
		glDrawElements(GL_TRIANGLES, renderables[i]->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}

// Macht für jedes Modell einen draw call für jede Transformationsmatrix in @static matrices
void Renderer::renderModelsWithMatrices(float alpha) {
	if (!inScene) {
		return;
	}

	if (discoMode)
		discoDiscoPartyParty();


	glm::mat4 pv = cam->getViewProjection();
	for (int i = 0; i < matrices.size(); i++) {
		glm::mat4 mvp = pv * matrices[i];

		for (int j = 0; j < currentIndex; j++) {
			renderables[j]->bindWithMatrix(mvp, matrices[i]);
			Shader* shader = renderables[j]->getShader();
			shader->setFloat3("cameraPos", cam->getPosition());
			shader->setFloat("alpha", alpha);
			setShaderUniforms(shader);

			glDrawElements(GL_TRIANGLES, renderables[j]->getCount(), GL_UNSIGNED_INT, nullptr);
		}
	}
}


// Leert die Liste der Modelle und beendet die Szene
void Renderer::flush() {
	for (int i = 0; i < MAX_MODELS; i++) {
		renderables[i] = nullptr;
	}
	if (matrices.size() > 0) {
		matrices.erase(matrices.begin(), matrices.end());
	}
	currentIndex = 0;
}

// Beendet die Szene
void Renderer::endScene() {
	cam->unlock();
	inScene = false;
}



void Renderer::getReadyToParty() {
	discoMode = true;
	elapsedTime = 0.0f;
}

void Renderer::crashParty() {
	discoMode = false;
}

bool Renderer::inDisco() {
	return discoMode;
}


// Übergibt Werte an Shader @arg shader
void Renderer::setShaderUniforms(Shader* shader) {
	if (discoMode) {
		shader->setFloat3("lightColor", discoLightColor);
	}
	else {
		shader->setFloat3("lightColor", lightColor);
	}
	shader->setFloat3("lightPos", lightPos);
	shader->setFloat("ambientStrength", ambientStrength);
}

// Disco Mode XD
void Renderer::discoDiscoPartyParty() {
	elapsedTime += delta->getDelta();

	discoLightColor.x = sinf(angle);
	discoLightColor.y = sinf(angle + 30.0f);
	discoLightColor.z = sinf(angle + 60.0f);

	printf("disco: %f %f %f\n", discoLightColor.x, discoLightColor.y, discoLightColor.z);

	if (elapsedTime >= DISCO_DURATION) {
		angle += DISCO_STEP;
		elapsedTime = 0.0f;
	}

	if (angle >= 360.0f)
		angle -= 360.0f;

}
