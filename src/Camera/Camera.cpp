#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

Camera::Camera()
{
	mEye = glm::vec3(0.0f, 3.0f, 0.0f);
	glm::vec3 target = { 5.0f, 1.0f, 5.0f };
	mViewDirection = glm::normalize(target - mEye); 
	mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(mEye, mEye + mViewDirection, mUpVector);
}

void Camera::Inputs(GLFWwindow* window)
{
	static float deltaTime = 0.0f;
	static float lastFrame = 0.0f;

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float speed = mSpeed * deltaTime;

	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mEye += speed * mViewDirection;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mEye += speed * -glm::normalize(glm::cross(mViewDirection, mUpVector));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mEye += speed * -mViewDirection;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mEye += speed * glm::normalize(glm::cross(mViewDirection, mUpVector));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mEye += speed * mUpVector;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		mEye += speed * -mUpVector;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		mSpeed = 25.0f ;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		mSpeed = 15.0f;
	}


	// Handles mouse inputs
	static bool firstClick = true;
	static double origMouseX, origMouseY;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		if (firstClick)
		{
			origMouseX = mouseX; origMouseY = mouseY;
			firstClick = false;
		}

		// Calculate the offset since the last frame
		double offsetX = mouseX - origMouseX;
		double offsetY = origMouseY - mouseY; // Reversed since y-coordinates range from bottom to top

		// Update for the next frame
		origMouseX = mouseX;
		origMouseY = mouseY;

		// Apply sensitivity scaling
		float rotX = static_cast<float>(offsetX) * sensitivity;
		float rotY = static_cast<float>(offsetY) * sensitivity;

		// Create a quaternion representing the new orientation
		glm::vec3 right = glm::normalize(glm::cross(mViewDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::quat pitch = glm::angleAxis(glm::radians(rotY), right);
		glm::quat yaw = glm::angleAxis(glm::radians(-rotX), glm::vec3(0.0f, 1.0f, 0.0f));

		// Update the view direction
		mViewDirection = glm::rotate(yaw * pitch, mViewDirection);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		firstClick = true;
	}
}