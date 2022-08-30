#include "..\include\n_ecs.h"
#include "glm/ext/matrix_transform.hpp"

NGINE::ECS::ECS()
{

}

NGINE::ECS::~ECS()
{

}

void NGINE::ECS::CalculateTransforms()
{

	for (int i = 0; i < transforms_.size(); i++)
	{

		Transform(i).GetWorldTransform();

	}

}

void NGINE::ECS::CalculateShadows()
{

	for (int i = 0; i < 5; i++)
	{

		DataLight& lightData = lights_[i];

		if (lightData.type_ == LightType::DIRECTIONAL)
		{
			const glm::mat4& t= transforms_[lightData.idTransform_].world_;
			glm::vec3 fwd( t[0].z, t[1].z, t[2].z);
			fwd=glm::normalize(fwd);
			lightData.matView_ = glm::lookAt(lightData.Position_, lightData.Position_+fwd, glm::vec3(0.0f, 1.0f, 0.0f));
		
		}

	}

}

void NGINE::ECS::CalculateTransformLights()
{

	for (int i = 0; i < activeLights_; i++)
	{

		DataLight& light = lights_[i];
		DataTransform& ObjTrns = transforms_[light.idTransform_];

		glm::vec4 pos = Transform(light.idTransform_).GetWorldTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		light.Position_ = glm::vec3(pos.x, pos.y, pos.z);
		light.Direction_ = glm::normalize(glm::vec3(ObjTrns.world_[0].z, ObjTrns.world_[1].z, ObjTrns.world_[2].z));

	}

}

void NGINE::ECS::ParentTransformSort(uint32_t pos, std::vector<DataTransform>& nTrans, uint32_t fatherID, uint32_t layer)
{

	for (uint32_t i = pos; i < transforms_.size(); i++)
	{

		if (fatherID == transforms_[i].parent_)
		{

			DataTransform& trnsToModify = transforms_[i];

			trnsToModify.hLayer_ = layer;
			sortedTransformID_[trnsToModify.id_] = nTrans.size();

			nTrans.push_back(trnsToModify);

			if (trnsToModify.id_ > 0)
			{
			
				ParentTransformSort(i + 1, nTrans, trnsToModify.id_, layer + 1);

			}

		}

	}

}