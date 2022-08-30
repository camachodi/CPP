#include "..\include\n_object.h"
#include "n_gpumanager.h"

NGINE::Object::Object()
{
	
}

NGINE::Object::Object(uint32_t id)
{

	id_ = id;

}

NGINE::Object::~Object()
{

}

void NGINE::Object::Init(uint32_t id)
{

	id_ = id;

}

NGINE::Transform NGINE::Object::GetTransform() const
{

	DataObject& transform = GPUManager::GPUData()->GetDataEcs()->objects_[id_];
	return Transform(transform.components_[TRANSFORM]);

}

NGINE::PProcess NGINE::Object::GetPProcess() const
{
	DataObject& pprocess = GPUManager::GPUData()->GetDataEcs()->objects_[id_];
	return NGINE::PProcess(pprocess.components_[PPROCESS]);
}

NGINE::Mesh NGINE::Object::GetMesh() const
{

	DataObject& mesh = GPUManager::GPUData()->GetDataEcs()->objects_[id_];
	return Mesh(mesh.components_[MESH]);

}

NGINE::Material NGINE::Object::GetMaterial() const
{

	DataObject& material = GPUManager::GPUData()->GetDataEcs()->objects_[id_];
	return Material(material.components_[MATERIAL]);

}

NGINE::Light NGINE::Object::GetLight() const
{

	DataObject& light = GPUManager::GPUData()->GetDataEcs()->objects_[id_];
	return NGINE::Light(light.components_[LIGHT]);

}

void NGINE::Object::SetTransform(const Transform& transform)
{

	DataObject& sTransform = GPUManager::GPUData()->GetDataEcs()->objects_[id_];
	sTransform.components_[TRANSFORM] = transform.GetID();

}

void NGINE::Object::SetPProcess(const PProcess& pprocess)
{

	DataObject& sPP = GPUManager::GPUData()->GetDataEcs()->objects_[id_];
	sPP.components_[PPROCESS] = pprocess.GetID();

}

void NGINE::Object::SetMesh(const Mesh& mesh)
{

	DataObject& sMesh = GPUManager::GPUData()->GetDataEcs()->objects_[id_];
	sMesh.components_[MESH] = mesh.GetID();

}

void NGINE::Object::SetMaterial(const Material& material)
{
	
	DataObject& sMaterial = GPUManager::GPUData()->GetDataEcs()->objects_[id_];
	sMaterial.components_[MATERIAL] = material.GetID();

}

uint32_t NGINE::Object::GetID() const
{

	return id_;

}
