#include "..\include\n_transform.h"
#include "n_data_components.h"
#include "n_gpumanager.h"
#include "glm/ext/matrix_transform.hpp"

NGINE::Transform::Transform()
{

}

NGINE::Transform::Transform(uint32_t id)
{

	id_ = id;

}

NGINE::Transform::~Transform()
{

}

const glm::mat4x4& NGINE::Transform::GetTransform()
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];

	if (dataTransform.update_)
	{

		glm::mat4 matPos = glm::identity<glm::mat4>();
		glm::vec3 rot = glm::radians(dataTransform.rotation_);

		matPos = glm::translate(matPos, dataTransform.position_);

		matPos = glm::rotate(matPos, rot.x, glm::vec3(1.0f, .0f, .0f));
		matPos = glm::rotate(matPos, rot.y, glm::vec3(.0f, 1.0f, .0f));
		matPos = glm::rotate(matPos, rot.z, glm::vec3(.0f, .0f, 1.0f));

		matPos = glm::scale(matPos, dataTransform.scale_);

		dataTransform.transform_ = matPos;
		dataTransform.update_ = false;

	}

	return dataTransform.transform_;

}

const glm::mat4x4& NGINE::Transform::GetWorldTransform()
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];

	if (dataTransform.hLayer_ == 0)
	{

		if (dataTransform.worldUpdate_)
		{

			dataTransform.world_ = GetTransform();
			dataTransform.worldUpdate_ = false;

		}

		return dataTransform.world_;

	}

	dataTransform.world_ = Transform(dataTransform.parent_).GetWorldTransform() * GetTransform();
	dataTransform.worldUpdate_ = false;

	return dataTransform.world_;

}

void NGINE::Transform::SetTransform(const glm::mat4x4& transform)
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];

	dataTransform.transform_ = transform;

	dataTransform.update_ = false;
	dataTransform.worldUpdate_ = true;

}

void NGINE::Transform::SetPosition(const glm::vec3& position)
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];

	dataTransform.position_ = position;

	dataTransform.update_ = true;
	dataTransform.worldUpdate_ = true;

}

void NGINE::Transform::SetRotation(const glm::vec3& rotation)
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];

	dataTransform.rotation_ = rotation;

	dataTransform.update_ = true;
	dataTransform.worldUpdate_ = true;

}

void NGINE::Transform::SetScale(const glm::vec3& scale)
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];
	dataTransform.scale_ = scale;

	dataTransform.update_ = true;
	dataTransform.worldUpdate_ = true;

}

const glm::vec3& NGINE::Transform::GetPosition()
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];
	return dataTransform.position_;

}

const glm::vec3& NGINE::Transform::GetRotation()
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];
	return dataTransform.rotation_;

}

const glm::vec3& NGINE::Transform::GetScale()
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];
	return dataTransform.scale_;

}

void NGINE::Transform::Orientation(const glm::vec3& forward)
{

	DataTransform& dataTransform = GPUManager::GPUData()->GetDataEcs()->transforms_[id_];
	dataTransform.transform_ = glm::lookAt(dataTransform.position_, dataTransform.position_ + forward, dataTransform.position_ + glm::vec3(0.0f, 1.0f, 0.0f));

	if (dataTransform.transform_[0][0] == 1.0f || dataTransform.transform_[0][0] == -1.0f)
	{

		dataTransform.rotation_.x = 0.0f;
		dataTransform.rotation_.y = 0.0f;
		dataTransform.rotation_.z = glm::degrees(atan2f(dataTransform.transform_[0][2], dataTransform.transform_[2][3]));

	}
	else
	{

		dataTransform.rotation_.x = glm::degrees(atan2f(-dataTransform.transform_[1][2], dataTransform.transform_[1][1]));
		dataTransform.rotation_.y = glm::degrees(asinf(dataTransform.transform_[1][0]));
		dataTransform.rotation_.z = glm::degrees(atan2f(-dataTransform.transform_[2][0], dataTransform.transform_[0][0]));

	}

	dataTransform.update_ = true;

}

void NGINE::Transform::SetParent(Transform sParent)
{

	ECS* ecsData = GPUManager::GPUData()->GetDataEcs();
	DataTransform& dataTransform = ecsData->transforms_[id_];
	DataTransform& dataTransformParent = ecsData->transforms_[sParent.id_];

	dataTransform.parent_ = sParent.id_;
	dataTransform.hLayer_ = dataTransformParent.hLayer_ + 1;
	bool already=false;

	for (int i = 0; i < dataTransformParent.childrenH_.size()&& (!already); i++)
	{

		already=dataTransformParent.childrenH_[i]== id_;

	}

	if (!already)
	{

		dataTransformParent.childrenH_.push_back(id_);

	}
}

uint32_t NGINE::Transform::GetID() const
{

	return id_;

}


