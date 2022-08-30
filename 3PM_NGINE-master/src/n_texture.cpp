#include "..\include\n_texture.h"

NGINE::Texture::Texture()
{

}

NGINE::Texture::Texture(int32_t id)
{
	
	id_ = id;

}

NGINE::Texture::~Texture()
{

}

uint32_t NGINE::Texture::GetID() const
{

	return id_;

}
