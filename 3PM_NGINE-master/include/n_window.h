#ifndef __N_WINDOW_H__
#define __N_WINDOW_H__ 1

#include <stdint.h>
#include "n_ui.h"

struct GLFWwindow;

namespace NGINE
{

	class ECS;

	class Window
	{

	public:

		Window();
		Window(const Window& w);
		~Window();

		//Initialize the parameters of Ngine App window
		bool init(int32_t width, int32_t height, char* windowName);

		//Determines if window should close or not
		bool closeWindow();

		//Events and swap buffers of GLFW
		void swap(ECS* ECSData);

		//Terminate GLFW
		void EndGLFW();

		//Draw Hierarchy of Parenting
		void DrawImguiWindow(ECS* ECSData, uint32_t id);

		//Return a reference of the window
		GLFWwindow* getWindow();

		//Return an instance of the window
		static NGINE::Window* getInstance();

		//Window width
		int32_t w_;

		//Window height
		int32_t h_;

	private:

		//GLFW window of the NGine
		GLFWwindow* window_;

		//Window Instance
		static Window* instance_;

	};

}
#endif