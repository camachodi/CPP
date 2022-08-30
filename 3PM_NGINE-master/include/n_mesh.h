#ifndef __N_MESH_H__
#define __N_MESH_H__ 1

#include "glad.h"
#include "glm/matrix.hpp"

namespace NGINE
{

	class Mesh
	{
	public:

		Mesh();
		Mesh(uint32_t id);
		~Mesh();

		//Returns mesh id
		uint32_t GetID() const;

	private:

		//Mesh id
		uint32_t id_;

	};

}

#endif
