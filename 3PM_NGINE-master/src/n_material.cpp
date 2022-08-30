#include "..\include\n_material.h"
#include "n_gpumanager.h"

NGINE::Material::Material()
{
	
}

NGINE::Material::Material(uint32_t id)
{
	
	id_ = id;

}

NGINE::Material::Material(const Material& mat)
{
	
	id_ = mat.id_;

}

NGINE::Material::~Material()
{

}

//void NGINE::Material::SetData(const ParamsMaterial& matParams)
//{
//
//	DataMaterial& material = GPUManager::GPUData()->GetDataEcs()->materials_[id_];
//	material.matParams_ = matParams;
//
//}

void NGINE::Material::SetColor(float r, float g, float b, float a)
{

	NGINE::GPUManager::GPUData()->GetDataEcs()->materials_[this->id_].matParams_.color = glm::vec4(r, g, b, a);

}

void NGINE::Material::LoadShader(const Shader& shader)
{

	DataMaterial& matShader = GPUManager::GPUData()->GetDataEcs()->materials_[id_];
	matShader.idShader_ = shader.GetID();

}

void NGINE::Material::LoadTextureAlbedo(const Texture& texture)
{

	DataMaterial& albTexture = GPUManager::GPUData()->GetDataEcs()->materials_[id_];
	albTexture.idTexture_ = texture.GetID();
	//albTexture.matParams_.t_albedo = 1;

}

uint32_t NGINE::Material::GetID() const
{

	return id_;

}
