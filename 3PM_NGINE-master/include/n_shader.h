#ifndef __N_SHADER_H__
#define __N_SHADER_H__ 1

#include <stdint.h>

namespace NGINE
{

	class Shader
	{
	public:

		Shader();
		Shader(uint32_t id);
		~Shader();

		//Returns the shader id
		uint32_t GetID() const;

	private:

		//Shader id
		uint32_t id_;

	};

}

#endif
