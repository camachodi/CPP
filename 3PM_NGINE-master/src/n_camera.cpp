#include "..\include\n_camera.h"
#include "n_gpumanager.h"
#include <glfw3.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

NGINE::Camera::Camera()
{

}

NGINE::Camera::~Camera()
{

}

void NGINE::Camera::CamMove(GLFWwindow* window, double w, double h, double sens)
{

	DataCamera& cam = GPUManager::GPUData()->GetDataEcs()->cam_;

	cam.speed_ = 1.0f;
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_C))
	{

		GPUManager::GPUData()->GetDataEcs()->cam2_=cam;

	}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
	{

		cam.pfdCam_.camPos_ += cam.speed_ * cam.camForward_;
	
	}

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
	{

		cam.pfdCam_.camPos_ += cam.speed_ * -glm::normalize(glm::cross(cam.camForward_, glm::vec3(0.0f, 1.0f, 0.0f)));

	}

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
	{

		cam.pfdCam_.camPos_ += cam.speed_ * -cam.camForward_;

	}

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
	{

		cam.pfdCam_.camPos_ += cam.speed_ * glm::normalize(glm::cross(cam.camForward_, glm::vec3(0.0f, 1.0f, 0.0f)));

	}

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE))
	{

		cam.pfdCam_.camPos_.y += cam.speed_;

	}

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{

		cam.pfdCam_.camPos_.y -= cam.speed_;

	}

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{

		cam.speed_ = cam.speedMax_;

	}
	else if (GLFW_RELEASE == glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{

		cam.speed_ = cam.speedMin_;

	}

	if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
	{

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		double mouseX;
		double mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sens * (float)(mouseY - (h / 2)) / h;
		float rotY = sens * (float)(mouseX - (w / 2)) / w;

		glm::vec3 newOrientation = glm::rotate(cam.camForward_, glm::radians(-rotX), glm::normalize(glm::cross(cam.camForward_, glm::vec3(0.0f, 1.0f, 0.0f))));

		if (abs(glm::angle(newOrientation, glm::vec3(0.0f, 1.0f, 0.0f)) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{

			cam.camForward_ = newOrientation;

		}

		cam.camForward_ = glm::rotate(cam.camForward_, glm::radians(-rotY), glm::vec3(0.0f, 1.0f, 0.0f));

		glfwSetCursorPos(window, w / 2, h / 2);

	}
	else if (GLFW_RELEASE == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
	{

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}

	cam.pfdCam_.matView_ = glm::lookAt(cam.pfdCam_.camPos_, cam.pfdCam_.camPos_ + cam.camForward_, glm::vec3(0.0f, 1.0f, 0.0f));

}
