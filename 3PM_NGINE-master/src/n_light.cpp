#include "..\include\n_light.h"
#include "n_gpumanager.h"

NGINE::Light::Light()
{

}

NGINE::Light::Light(uint32_t id)
{

  id_ = id;

}

NGINE::Light::~Light()
{

}

uint32_t NGINE::Light::GetId()
{

    return id_;;

}

void NGINE::Light::SetDirection(const glm::vec3& dir)
{

	DataLight& lightDir = GPUManager::GPUData()->GetDataEcs()->lights_[id_];
	Transform(lightDir.idTransform_).Orientation(dir);

}

void NGINE::Light::SetPosition(const glm::vec3& pos)
{

	DataLight& lightPos = GPUManager::GPUData()->GetDataEcs()->lights_[id_];
	Transform(lightPos.idTransform_).SetPosition(pos);

}

void NGINE::Light::SetColorShadow(const glm::vec3& col)
{

	DataLight& lightCol = GPUManager::GPUData()->GetDataEcs()->lights_[id_];
	lightCol.shadowColor_.x = col.x;
	lightCol.shadowColor_.y = col.y;
	lightCol.shadowColor_.z = col.z;

}

NGINE::Object NGINE::Light::GetObject()
{

	DataLight& light = GPUManager::GPUData()->GetDataEcs()->lights_[id_];
	return Object(light.idObject_);

}

NGINE::Transform NGINE::Light::GetTransform()
{

	DataLight& lightTrans = GPUManager::GPUData()->GetDataEcs()->lights_[id_];
	return Transform(lightTrans.idTransform_);

}
