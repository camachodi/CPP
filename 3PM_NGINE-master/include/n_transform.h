#ifndef __N_TRANSFORM_H__
#define __N_TRANSFORM_H__ 1

#include "glm/matrix.hpp"
#include <stdint.h>

namespace NGINE
{

	class Transform
	{

	public:

		Transform();
		Transform(uint32_t id);
		~Transform();

		//Returns the transform location of the component
		const glm::mat4x4& GetTransform();

		//Returns the world transform location of the component
		const glm::mat4x4& GetWorldTransform();

		//Set the transform location of the component
		void SetTransform(const glm::mat4x4& transform);

		//Set the position of the component
		void SetPosition(const glm::vec3& position);

		//Set the rotation of the component
		void SetRotation(const glm::vec3& rotation);

		//Set the scale of the component
		void SetScale(const glm::vec3& scale);

		//Returns the position of the component
		const glm::vec3& GetPosition();

		//Returns the rotation of the component
		const glm::vec3& GetRotation();

		//Returns the scale of the component
		const glm::vec3& GetScale();
		
		//Calculates the orientation of the component
		void Orientation(const glm::vec3& forward);
		
		//Set a parent to the component
		void SetParent(Transform sParent);

		//Returns transform id
		uint32_t GetID() const;
	
	protected:

		//Transform id
		uint32_t id_;

	};

}
#endif