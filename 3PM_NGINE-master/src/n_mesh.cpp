#include "..\include\n_mesh.h"

NGINE::Mesh::Mesh()
{

}

NGINE::Mesh::Mesh(uint32_t id)
{

	id_ = id;

}

NGINE::Mesh::~Mesh()
{

}

uint32_t NGINE::Mesh::GetID() const
{

	return id_;

}
