#include "window.h"

#include <iostream>




Window* Window::instance = nullptr;

Window::Window() : Window("Default", 200, 200, 1080, 720) {

}

Window::Window(const char* title, int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {

	if (instance) {
		printf("ILLEGAL TO CREATE A SECOND OPENGL WINDOW FOR THIS APPLICATION!!!!!\n");
		__debugbreak();
		return;
	}

	if (glfwInit() == GLFW_FALSE) {
		printf("failed to load glfw!\n");
		__debugbreak();
		return;
	}

	glfwWindowHint(GLFW_DECORATED, NULL);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	w = glfwGetVideoMode(monitor)->width;
	h = glfwGetVideoMode(monitor)->height;

	window = glfwCreateWindow(w, h, title, nullptr, nullptr);
	glfwMakeContextCurrent(window);
	

	if (glewInit() != GLEW_OK) {
		printf("failed to load glew!\n");
		__debugbreak();
		return;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);



	instance = this;

}

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

