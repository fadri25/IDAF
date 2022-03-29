#pragma once
#include "includegl.h"


// Wrapper-Klasse für ein Open GL Fenster
class Window {

	static Window* instance;

	GLFWwindow* window = nullptr;
	int x, y, w, h;

public:
	Window();
	Window(const char* title, int x, int y, int w, int h);
	~Window();

	void pollEvents() const;
	void clear() const;
	void swapBuffers() const;
	bool shouldClose() const;

	void setKeyFunc(GLFWkeyfun func) const;
	
	inline GLFWwindow* getHandle() const { return window; }
	inline int getWidth() const { return w; }
	inline int getHeight() const { return h; }

	static Window* getInstance();
};