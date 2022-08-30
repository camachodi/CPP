#ifndef __N_OBJECT_H__
#define __N_OBJECT_H__ 1

#include "n_transform.h"
#include "n_mesh.h"
#include "n_material.h"
#include "n_light.h"
#include "n_pprocess.h"
#include <stdint.h>
#include <string>

namespace NGINE
{

	class Light;
	
	class Object
	{

	public:

		Object();
		Object(uint32_t id);
		~Object();
		
		void Init(uint32_t id);
		uint32_t GetID() const;

		Mesh GetMesh() const;
		Light GetLight() const;
		Material GetMaterial() const;
		Transform GetTransform() const;
		PProcess GetPProcess() const;

		void SetMesh(const Mesh& mesh);
		void SetMaterial(const Material& material);
		void SetTransform(const Transform& transform);
		void SetPProcess(const PProcess& pprocess);

	private:

		uint32_t id_;

	};

}

#endif
