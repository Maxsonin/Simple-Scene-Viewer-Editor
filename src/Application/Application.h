#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "../Camera/Camera.h"
#include "../Renderer/Renderer.h"

class ApplicationSettings;

class Application
{
private:
	GLFWwindow* m_MainWindow;

	Camera m_Camera;

	ApplicationSettings* m_settings;

	unsigned int m_windowWidth  = 1920;
	unsigned int m_windowHeight = 1080;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

public:
	Application(); ~Application();

	GLFWwindow* getMainApplicationWindow() { return m_MainWindow; }

	unsigned int getWindowWidth() { return m_windowWidth; }
	unsigned int getWindowHeight() { return m_windowHeight; }

	Camera getCamera() { return m_Camera; }

	void Run();

	void processInput();
};

#endif // !APPLICATION_H