#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();

	glm::mat4 GetViewMatrix() const;

	void Inputs(GLFWwindow* window);
	glm::vec3 GetPostion() { return mEye; }

	float getFOV() { return m_FOV; }
	float* getFOVByRefferance() { return &m_FOV; }
	void setFOV(float newVal) { m_FOV = newVal; }

	float mSpeed = 15.0f;

private:
	glm::vec3 mEye;
	glm::vec3 mViewDirection;
	glm::vec3 mUpVector;

	float m_FOV = 45.f;

	float sensitivity = 0.1f;
};
#endif