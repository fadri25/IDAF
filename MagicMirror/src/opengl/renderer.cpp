#include "renderer.h"

#include <iostream>

#include "includegl.h"
#include <mat3x3.hpp>
#include <gtx/transform.hpp>
#include <gtc/matrix_transform.hpp>
#include "model.h"
#include "quad.h"

Camera* Renderer::cam;
DeltaTime* Renderer::delta = nullptr;


const int Renderer::MAX_MODELS = 10;
std::vector<glm::mat4> Renderer::matrices;
Renderable** Renderer::renderables;
int Renderer::currentIndex = 0;

bool Renderer::inScene = false;

glm::vec3 Renderer::lightPos = glm::vec3(0.0f, 0.0f, -0.5f);
glm::vec3 Renderer::lightColor = glm::vec3(1.0f);
glm::vec3 Renderer::lightColor2 = glm::vec3(1.0f);
float Renderer::ambientStrength = 0.4f;
bool Renderer::deamon = false;
float Renderer::elapsedTime = 0.0f;

CubeMap* Renderer::cubeMap = nullptr;
const float Renderer::DURATION = 0.05f;
const float Renderer::STEP = 360.0f * DURATION;
float Renderer::angle = 0.0f;

Framebuffer* Renderer::frameBuffer = nullptr;



// Initialisierung
void Renderer::init(Camera* camera, DeltaTime* d) {
	cam = camera;
	delta = d;
	renderables = new Renderable *[MAX_MODELS];
	frameBuffer = new Framebuffer();
}

// Arbeitsspeicher freigeben
void Renderer::terminate() {
	delete[] renderables;
	delete frameBuffer;
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
void Renderer::render(bool reflection) {
	if (!inScene) {
		return;
	}


	glm::mat4 mvp;
	for (int i = 0; i < currentIndex; i++) {

		renderables[i]->bindWithMatrix();

		Shader* shader = renderables[i]->getShader();
		
		if (reflection && cubeMap) {
			glm::mat4 rot = glm::mat4(1.0f);
			rot = glm::rotate(rot, ((Model*)renderables[i])->getAngle(), { 0.0f, 1.0f, 0.0f });

			glm::mat4 mdl = glm::mat4(glm::mat3(renderables[i]->getTransform()));
			
			shader->setUniformMat4("rot", rot);
			shader->setUniformMat4("model", mdl);
			cubeMap->bind(1);
		}
		else {
			shader->setUniformMat4("model", renderables[i]->getTransform());
		}
		
		mvp = cam->getViewProjection() * renderables[i]->getTransform();
		shader->setUniformMat4("mvp", mvp);
		shader->setFloat("alpha", 1.0f);
		setShaderUniforms(shader, reflection ? 1.0f : 0.0f);
		glDrawElements(GL_TRIANGLES, renderables[i]->getCount(), GL_UNSIGNED_INT, nullptr);
		
	}
	flush();
}

// Macht für jedes Modell einen draw call für jede Transformationsmatrix in @static matrices
void Renderer::renderModelsWithMatrices(float alpha) {
	if (!inScene) {
		return;
	}


	glm::mat4 pv = cam->getViewProjection();
	for (int i = 0; i < matrices.size(); i++) {
		glm::mat4 mvp = pv * matrices[i];

		for (int j = 0; j < currentIndex; j++) {
			renderables[j]->bindWithMatrix();
			Shader* shader = renderables[j]->getShader();
			shader->setUniformMat4("mvp", mvp);
			shader->setUniformMat4("model", matrices[i]);
			shader->setFloat3("cameraPos", cam->getPosition());
			shader->setFloat("alpha", alpha);
			setShaderUniforms(shader);

			glDrawElements(GL_TRIANGLES, renderables[j]->getCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	flush();
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



void Renderer::bar() {
	deamon = true;
	elapsedTime = 0.0f;
}

void Renderer::emply() {
	deamon = false;
}

bool Renderer::isDaemon() {
	return deamon;
}

void Renderer::setCubeMap(CubeMap* cm) {
	cubeMap = cm;
}



// Übergibt Werte an Shader @arg shader
void Renderer::setShaderUniforms(Shader* shader, float ambient) {
	if (deamon) {
		shader->setFloat3("lightColor", lightColor2);

	}
	else {
		shader->setFloat3("lightColor", lightColor);
	}

	shader->setFloat3("lightPos", lightPos);

	if (ambient == 0.0f) {
		shader->setFloat("ambientStrength", ambientStrength);
	}
	else {
		shader->setFloat("ambientStrength", ambient);
	}
}


void Renderer::foo() {
	elapsedTime += delta->getDelta();

	lightColor2.x = (sinf(angle) + 2) * 0.5f ;
	lightColor2.y = (sinf(angle + 30.0f) + 2) * 0.5f;
	lightColor2.z = (sinf(angle + 60.0f) + 2) * 0.5f;


	if (elapsedTime >= DURATION) {
		angle += STEP;
		elapsedTime = 0.0f;
	}

	if (angle >= 360.0f)
		angle -= 360.0f;

}
