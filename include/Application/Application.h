#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "Camera/Camera.h"
#include "Renderer/Renderer.h"

class ApplicationSettings;

class Application
{
private:
	GLFWwindow* m_MainWindow;

	Camera m_Camera;

	ApplicationSettings* m_settings;

	unsigned int m_windowWidth  = 1920;
	unsigned int m_windowHeight = 1080;

	float m_Near = 0.01f;
	float m_Far = 1000.0f;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void ShowWorldSettings();

public:
	Application(); ~Application();

	GLFWwindow* getMainApplicationWindow() { return m_MainWindow; }

	unsigned int getWindowWidth() { return m_windowWidth; }
	unsigned int getWindowHeight() { return m_windowHeight; }

	Camera getCamera() { return m_Camera; }

	glm::mat4 GetProjectionMatrix()
	{
		float aspectRatio = float(m_windowWidth) / m_windowHeight;
		return glm::perspective(glm::radians(m_Camera.getFOV()), aspectRatio, m_Near, m_Far);
	}

	void Run();

	void processInput();
};

#endif