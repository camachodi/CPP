#ifndef __N_INTERNALS_H__
#define __N_INTERNALS_H__ 1

#include <stdint.h>

namespace NGINE
{

	enum LightType
	{

		NONEL = 0,
		//AMBIENT,
		DIRECTIONAL,
		SPOT,
		POINT,

	};

	enum ComponentType
	{
		
		TRANSFORM,
		MESH,
		MATERIAL,
		LIGHT,
		PPROCESS,
		MAX,

	};

	const uint32_t MaxLights = 16;
	const uint32_t ScreenW_ = 1280;
	const uint32_t ScreenH_ = 720;

	const uint32_t shadowMapSizeX_ = 4096;
	const uint32_t shadowMapSizeY_ = 4096;

}
#endif
