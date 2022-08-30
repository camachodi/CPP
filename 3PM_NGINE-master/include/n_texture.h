#ifndef __N_TEXTURE_H__
#define __N_TEXTURE_H__ 1

#include <stdint.h>

namespace NGINE
{

	class Texture
	{

	public:

		Texture();
		Texture(int32_t id);
		~Texture();

		//Returns texture id
		uint32_t GetID() const;

	private:

		//Texture id
		uint32_t id_;

	};

}

#endif
