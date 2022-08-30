#include "..\include\n_dlist.h"
#include "n_gpumanager.h"
#include <assert.h>

NGINE::DList::DList()
{

	swap_ = false;

}

NGINE::DList::DList(DList&& lastCmd)
{

	swap_ = lastCmd.swap_;
	lastCmd.swap_ = false;
	commandList_ = std::move(lastCmd.commandList_);

}

NGINE::DList::~DList()
{

}

void NGINE::DList::ClearCommand::ExecuteOnGPU()
{

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void NGINE::DList::DrawCommand::ExecuteOnGPU()
{

	GPUManager* gpuData = GPUManager::GPUData();
	ECS* ecs = gpuData->GetDataEcs();

	//OBJ Data
	DataObject obj = ecs->objects_[objectToDraw_];

	DataMesh mesh = ecs->meshes_[obj.components_[MESH]];
	DataMaterial material = ecs->materials_[obj.components_[MATERIAL]];

	DataShader shader = ecs->shaders_[material.idShader_];
	DataTransform transform = ecs->transforms_[obj.components_[TRANSFORM]];

	//Program
	glUseProgram(shader.idProgram_);

	//Bind Textures to material
	glBindTextures(0, 1, &ecs->textures_[material.idTexture_].id_);

	//Uniform Material
	glBindBufferRange(GL_UNIFORM_BUFFER, 2, gpuData->MatDataBuffer_, 0, sizeof(ParamsMaterial));
	glNamedBufferSubData(gpuData->MatDataBuffer_, 0, sizeof(ParamsMaterial), &material.matParams_);

	//Buffer Matrix Model
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, gpuData->TransformBuffer_, 0, sizeof(glm::mat4));
	glNamedBufferSubData(gpuData->TransformBuffer_, 0, sizeof(glm::mat4), &transform.world_);

	//VAO
	glBindVertexArray(mesh.vertexArray_);

	//DRAW
	glDrawElements(GL_TRIANGLES, mesh.sizeIndices_, GL_UNSIGNED_INT, 0);

}

void NGINE::DList::ExecuteOnGPU(std::list<std::unique_ptr<Command>> commandList)
{
	std::unique_lock<std::mutex>locker(mutex_);
	for (auto&& i : commandList)
	{

		i->ExecuteOnGPU();

	}
	locker.unlock();
}

void NGINE::DList::ExecuteOnGPU()
{
	for (auto& i : commandList_)
	{

		i->ExecuteOnGPU();

	}
}

void NGINE::DList::Submit()
{

	ExecuteOnGPU(std::move(commandList_));

}

bool NGINE::DList::DoSwap() const
{

	return swap_;

}

NGINE::DList& NGINE::DList::DoClear(float r, float g, float b, float a)
{

	ClearCommand ActCmd;
	ActCmd.r = r;
	ActCmd.g = g;
	ActCmd.b = b;
	ActCmd.a = a;

	ClearCommand ClrCmd;
	ClrCmd.r = 0.0f;
	ClrCmd.g = 0.0f;
	ClrCmd.b = 0.0f;
	ClrCmd.a = 1.0f;

	commandList_.push_back(std::make_unique<ClearCommand>(ActCmd));

	

	return *this;

}

NGINE::DList& NGINE::DList::DoDraw(const Object& ObjToDraw)
{
  GPUManager* gpuData = GPUManager::GPUData();

	DrawCommand dc;
	dc.objectToDraw_ = ObjToDraw.GetID();

	commandList_.push_back(std::make_unique<DrawCommand>(dc));


  if (gpuData->activatePP)
  {
		
    if (gpuData->activatePP && ObjToDraw.GetPProcess().GetID() != 0)
    {
      std::unique_lock<std::mutex>locker(mutex_);

      ECS* ecs = gpuData->GetDataEcs();
			DataPProcess& dpp= ecs->pprocesses_[ObjToDraw.GetPProcess().GetID()];
      ecs->displayLists_[dpp.dList_].commandList_.push_back(std::make_unique<DrawCommand>(dc));
			locker.unlock();
    }



  }

	return *this;

}

NGINE::DList& NGINE::DList::DoDrawPProcess(const Object& ObjToDraw, const PProcess& PProcessToApply)
{

	GPUManager* gpuData = GPUManager::GPUData();

	DrawCommand DrwCmd;
	DrwCmd.objectToDraw_ = ObjToDraw.GetID();
	commandList_.push_back(std::make_unique<DrawCommand>(DrwCmd));

	

	return *this;

}

NGINE::DList& NGINE::DList::DoWindowSwap(NGINE::Window* window)
{

	swap_ = true;
	WindowSwapCommand SwpCmd;

	SwpCmd.window_ = window;

	commandList_.push_back(std::make_unique<WindowSwapCommand>(SwpCmd));

	return *this;

}

NGINE::DList& NGINE::DList::DoNewFrame()
{

	commandList_.push_back(std::make_unique<NewFrameCommand>());

	return *this;

}

void NGINE::DList::DrawPProcessCommand::ExecuteOnGPU()
{

}

void NGINE::DList::WindowSwapCommand::ExecuteOnGPU()
{

}

void NGINE::DList::NewFrameCommand::ExecuteOnGPU()
{

}
