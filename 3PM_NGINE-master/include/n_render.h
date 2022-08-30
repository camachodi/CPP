#ifndef __N_RENDER_H__
#define __N_RENDER_H__ 1

#include "n_data_components.h"
#include "n_dlist.h"
#include <mutex>
#include <list>

namespace NGINE
{

	class Render
	{
	public:
	
		Render();
		Render(Render&&) = default;
		~Render();
	
		//Submit a locker/mutex to wait all the process until do the next task
		void SubmitLock(DList&& dList);

		//Execute render process of the Ngine App
		void ExecuteAll();

		//Mutex to lock the list
		std::mutex mutex_;

		//Draw list
		std::list<DList>dList_;
	
	private:
	
	};
	
}

#endif