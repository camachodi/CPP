#ifndef __N_PPROCESS_H__
#define __N_PPROCESS_H__ 1

#include <stdint.h>

namespace NGINE
{

	class PProcess
	{
	public:

		PProcess();
		PProcess(uint32_t id);
		~PProcess();

		//Returns the post process id
		uint32_t GetID() const;

	private:

		//Post process id
		uint32_t id_;

	};

}

#endif
