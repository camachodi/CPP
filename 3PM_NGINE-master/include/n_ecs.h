#ifndef __N_ECS_H__
#define __N_ECS_H__ 1

#include "n_data_components.h"
#include "n_dlist.h"
#include <stdint.h>
#include <vector>

namespace NGINE
{

	class ECS
	{

	public:

		ECS();
		~ECS();

		//Calculate transforms of every transform component
		void CalculateTransforms();

		//Calculate shadows only for Directional lights
		void CalculateShadows();

		//Calculate transforms for every light generated
		void CalculateTransformLights();

		//Data list that stores every object generated
		std::vector<DataObject> objects_;

		//Data of the camera
		DataCamera cam_;
		DataCamera cam2_;

		

		//Data list that stores every transform generated
		std::vector<DataTransform> transforms_;

		//Data list that stores every mesh generated
		std::vector<DataMesh> meshes_;

		//Data list that stores every texture generated
		std::vector<DataTexture> textures_;

		//Data list that stores every material generated
		std::vector<DataMaterial> materials_;
		
		//Data list that stores every lights with a limit
		DataLight lights_[MaxLights];

		//Number of lights active in the scene
		int32_t activeLights_;

		//Data list of shadow maps with a maximum of five
		DataShadowMap shadowMaps_[5];

		//Data list that stores every shader generated
		std::vector<DataShader> shaders_;

		//Data list that stores every frame buffer needed
		std::vector<DataFrameBuffer> frameBuffers_;

		//Data list that stores every post process generated
		std::vector<DataPProcess> pprocesses_;

		//Data list that stores every display list generated
		std::vector<DList> displayLists_;

		//Data list that stores the new id of sorted transform from the parent layer
		std::vector<uint32_t> sortedTransformID_;

	private:

		//Sort the transforms and ids of the objects attached to a parent, to determine the layer of parenting
		void ParentTransformSort(uint32_t pos, std::vector<DataTransform>& nTrans, uint32_t fatherID, uint32_t layer);

	};

}

#endif
