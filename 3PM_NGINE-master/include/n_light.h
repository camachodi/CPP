#ifndef __N_LIGHT_H__
#define __N_LIGHT_H__ 1

#include "n_object.h"
#include "n_transform.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace NGINE
{
	class Object;

	class Light
	{

	public:

		Light();
		Light(uint32_t id);
		~Light();

		//Returns the id of the light generated
		uint32_t GetId();
		
		//Establish the direction of the light
		void SetDirection(const glm::vec3& dir);

		//Establish the position of the light
		void SetPosition(const glm::vec3& pos);

		void SetColorShadow(const glm::vec3& col);

		//Returns the light object
		Object GetObject();

		//Returns the light transform
		Transform GetTransform();

	private:

		//Light id
		uint32_t id_;

	};

}

#endif
