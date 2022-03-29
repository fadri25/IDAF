#include "window.h"

#include <iostream>




Window* Window::instance = nullptr;

// Constructor Funktionen
Window::Window() : Window("Default", 200, 200, 1080, 720) {

}

Window::Window(const char* title, int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {

	// Verhindern, dass mehr als ein Open Gl Fenster erstellt wird
	if (instance) {
		printf("ILLEGAL TO CREATE A SECOND OPENGL WINDOW FOR THIS APPLICATION!!!!!\n");
		__debugbreak();
		return;
	}

	// Initialisierung der Gl Frameworks
	if (glfwInit() == GLFW_FALSE) {
		printf("failed to load glfw!\n");
		__debugbreak();
		return;
	}

	// Erstellen eines Open Gl Fenster
	window = glfwCreateWindow(1080, 720, "window", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Initialisierung des Gl Extension Wranglers
	if (glewInit() != GLEW_OK) {
		printf("failed to load glew!\n");
		__debugbreak();
		return;
	}

	// Stellt Farbe für @func glClear() auf gewünschten Wert ein @param(rot, grün, blau, alpha)
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	// Aktivierung verschiedener Grafiktreiberfunktionen
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Verhindert dass Rückseiten eines Modelles dargestellt werden
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);



	instance = this;

}

// Destructor Funktion
Window::~Window() {
	if (window)
		glfwDestroyWindow(window);

	// 
	glfwTerminate();
}


// Verarbeitet Interaktionen mit dem Fenster
void Window::pollEvents() const {
	glfwPollEvents();
}


// Färbt Fenster mit der einer Farbe uniform ein
void Window::clear() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Wechselt Frontbuffer mit Backbuffer aus um das neue Bild auf den Monitor zu projezieren
void Window::swapBuffers() const {
	glfwSwapBuffers(window);
}
 
// Benachrichtigt ob das Fenster sich schliessen sollte
bool Window::shouldClose() const {
	return glfwWindowShouldClose(window) == 1;
}

void Window::setKeyFunc(GLFWkeyfun func) const {
	glfwSetKeyCallback(window, func);
}


// Stellt einen Pointer zu dem Fenster bereit;
Window* Window::getInstance() {
	return instance;
}

