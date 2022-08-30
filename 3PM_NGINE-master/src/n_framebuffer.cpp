#include "..\include\n_framebuffer.h"

NGINE::FrameBuffer::FrameBuffer()
{

}

NGINE::FrameBuffer::FrameBuffer(uint32_t id)
{

  id_ = id;

}

NGINE::FrameBuffer::~FrameBuffer()
{

}

uint32_t NGINE::FrameBuffer::GetID()
{

    return id_;

}
