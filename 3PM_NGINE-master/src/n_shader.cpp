#include "..\include\n_shader.h"

NGINE::Shader::Shader()
{

}

NGINE::Shader::Shader(uint32_t id)
{

	id_ = id;

}

NGINE::Shader::~Shader()
{

}

uint32_t NGINE::Shader::GetID() const
{

	return id_;

}
