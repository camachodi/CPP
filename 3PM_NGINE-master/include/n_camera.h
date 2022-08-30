#ifndef __N_CAMERA_H__
#define __N_CAMERA_H__ 1

#include <stdint.h>
#include "glm/vec3.hpp"
#include "n_window.h"

namespace NGINE
{

	class Camera
	{

	public:

		Camera();
		~Camera();

		//Camera Move input
		void CamMove(GLFWwindow* window, double w, double h, double sens);

		
	private:

	};

}

#endif
