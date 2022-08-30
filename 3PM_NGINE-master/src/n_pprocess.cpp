#include "..\include\n_pprocess.h"

NGINE::PProcess::PProcess()
{

  id_ = 0;

}

NGINE::PProcess::PProcess(uint32_t id)
{

  id_ = id;

}

NGINE::PProcess::~PProcess()
{

}

uint32_t NGINE::PProcess::GetID() const
{
    return id_;
}
