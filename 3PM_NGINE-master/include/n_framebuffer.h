#ifndef __N_FRAMEBUFFER_H__
#define __N_FRAMEBUFFER_H__ 1

#include <stdint.h>

namespace NGINE
{

	class FrameBuffer
	{

	public:

		FrameBuffer();
		FrameBuffer(uint32_t id);
		~FrameBuffer();

		//Returns the frame buffer id
		uint32_t GetID();

	private:

		//Frame Buffer Id
		uint32_t id_;

	};

}

#endif
