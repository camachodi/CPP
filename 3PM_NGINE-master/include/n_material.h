#ifndef __N_MATERIAL_H__
#define __N_MATERIAL_H__ 1

#include "n_data_components.h"
#include "n_shader.h"
#include "n_texture.h"
#include <glad.h>
#include <glm/vec4.hpp>

namespace NGINE
{

	class Material
	{
	public:

		Material();
		Material(uint32_t id);
		Material(const Material& mat);
		~Material();

		//Set the default color of the material
		//void SetData(const ParamsMaterial& matParams);

		//Set the default material color
		void SetColor(float r, float g, float b, float a);

		//Load shader into the material
		void LoadShader(const Shader& shader);

		//Load the albedo texture into the material
		void LoadTextureAlbedo(const Texture& texture);

		//Returns the material id
		uint32_t GetID() const;

	private:

		//Material id
		uint32_t id_;

	};

}

#endif
