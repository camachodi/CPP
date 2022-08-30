#ifndef __N_DLIST_H__
#define __N_DLIST_H__ 1

#include "n_object.h"
#include "n_window.h"
#include "n_pprocess.h"
#include <list>
#include <memory>
#include <mutex>

namespace NGINE
{

	class DList
	{
		
		//Base Command
		struct Command
		{

			virtual ~Command() = default;
			virtual void ExecuteOnGPU() = 0;

		};

		//Clear Screen Frame Command, used to clean Screen frames
		struct ClearCommand : public Command
		{

			virtual void ExecuteOnGPU() override;
			float r, g, b, a;

		};

		//Draw Entities Command, used to draw everything that can be physically visualized in the field
		struct DrawCommand : public Command
		{

			virtual void ExecuteOnGPU() override;
			uint32_t objectToDraw_;

		};

		//Draw Post Process Command, used to apply any kind of post process to the entities
		struct DrawPProcessCommand : public Command
		{

			virtual void ExecuteOnGPU() override;
			uint32_t objectToDraw_;
			uint32_t PProcess_;

		};

		//Window Swap Command used to determine the end of a frame and start a new one
		struct WindowSwapCommand : public Command
		{

			virtual void ExecuteOnGPU() override;
			NGINE::Window* window_;

		};

		//New Frame Command used to submit a new frame to the command list
		struct NewFrameCommand : public Command
		{

			virtual void ExecuteOnGPU() override;

		};

	public:

		DList();
		DList(DList&& lastCmd);
		~DList();

		//Execute Command List on GPU
		void ExecuteOnGPU(std::list<std::unique_ptr<Command>> commandList);

		//Auxiliary (non unique) Execute Command List on GPU
		void ExecuteOnGPU();

		//Move a command to the List of commands
		void Submit();

		//Gives the status of swap
		bool DoSwap() const;
		
		//Clear Frame Function
		DList& DoClear(float r, float g, float b, float a);

		//Submit draw command to list of draws
		DList& DoDraw(const Object& ObjToDraw);

		//Submit Post Process Command Draw to list of draws
		DList& DoDrawPProcess(const Object& ObjToDraw, const PProcess& PProcessToApply);

		//Submit Window swap command to command list
		DList& DoWindowSwap(NGINE::Window* window);

		//Submit new start of new frame to command list
		DList& DoNewFrame();

		//List of commands
		std::list<std::unique_ptr<Command>> commandList_;

	private:

		//Boolean that determines if swap is enabled or not
		bool swap_ = false;
    std::mutex mutex_;

	};

}

#endif
