#include "..\include\n_render.h"
#include "n_gpumanager.h"
#include "Minitrace/minitrace.h"

NGINE::Render::Render()
{

}

NGINE::Render::~Render()
{

}

void NGINE::Render::SubmitLock(DList&& dList)
{

	std::unique_lock<std::mutex>locker(mutex_);
	dList_.push_back(std::move(dList));
	//locker.unlock();

}

inline void drawRect(GLuint VAO, GLuint program,GLuint texture,GLsizei indices)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	//Bind Textures to material
	glBindTextures(0, 1, &texture);

	//VAO
	glBindVertexArray(VAO);

	//DRAW
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);

}

void NGINE::Render::ExecuteAll()
{

	GPUManager* dataGPU = GPUManager::GPUData();
	ECS* ecsData = dataGPU->GetDataEcs();
	DataCamera& dataCam = ecsData->cam_;

	std::list<DList> newDisplay;

	bool stopDl2 = false;

	while (dList_.size() > 0 && (!stopDl2))
	{

		std::unique_lock<std::mutex> ulock(mutex_);
		newDisplay.push_back(std::move(dList_.front()));
		dList_.pop_front();
		ulock.unlock();

		stopDl2 = newDisplay.back().DoSwap();

	}
	
	//SHADOWS
	glViewport(0, 0, shadowMapSizeX_, shadowMapSizeY_);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, dataGPU->PFDBuffer_, 0, sizeof(PerFrameData));

	MTR_BEGIN("N-Gine_Logic", "Shadows");

	static PerFrameData lightCam;

	for (int i = 0; i < 5; i++)
	{

		DataLight& light = ecsData->lights_[i];
		glBindFramebuffer(GL_FRAMEBUFFER, ecsData->shadowMaps_[i].frameBufferShadow_);
		lightCam.camPos_ = light.Position_;
		lightCam.matProj_ = light.matProj_;
		lightCam.matView_ = light.matView_;
		glNamedBufferSubData(dataGPU->PFDBuffer_, 0, sizeof(PerFrameData), &lightCam);

		newDisplay.front().ExecuteOnGPU();

	}

	MTR_END("N-Gine_Logic", "Shadows");

	GLuint lastTex=0;
	if (dataGPU->activatePP)
	{
		glViewport(0, 0, dataGPU->GetDataWindow()->w_, dataGPU->GetDataWindow()->h_);
   

		glBindTextures(2, 1, 0);
		for (int i = 1; i < ecsData->pprocesses_.size(); i++)
		{



			auto& ppt = ecsData->pprocesses_[i];

			glBindFramebuffer(GL_FRAMEBUFFER, ppt.FBO_);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
			glNamedBufferSubData(dataGPU->PFDBuffer_, 0, sizeof(PerFrameData), &ecsData->cam_.pfdCam_);
			ecsData->displayLists_[ecsData->pprocesses_[i].dList_].ExecuteOnGPU(std::move(ecsData->displayLists_[ecsData->pprocesses_[i].dList_].commandList_));


			//ecsData->displayLists_[ecsData->pprocesses_[i].dList_].commandList_.clear();

			glBindFramebuffer(GL_FRAMEBUFFER, ppt.FBOPProcess_);
      glBindTextures(0, 1, &ppt.FBTexture_);
      glBindTextures(1, 1, &ppt.FBTextureDepth_);
			
			glBindBufferRange(GL_UNIFORM_BUFFER, 4, dataGPU->PPBuffer_, 0, sizeof(PProcessParams));
			glNamedBufferSubData(dataGPU->PPBuffer_, 0, sizeof(PProcessParams), &ppt.params_);
			

			drawRect(
				ecsData->meshes_[dataGPU->Rect_.GetID()].vertexArray_,
				ecsData->shaders_[ppt.shader_].idProgram_,
				dataGPU->frame.fFramebufferTex_,
				ecsData->meshes_[dataGPU->Rect_.GetID()].sizeIndices_
			);

			
			lastTex = ppt.FBTexturePProcess_;
			glBindTextures(2, 1, &lastTex);
		}

	}

	dataGPU->ApplyPPScreenShader(dataGPU->ScreenShader_);

	MTR_BEGIN("N-Gine_Logic", "RenderWorld");

  glBindTextures(1, 1, &ecsData->shadowMaps_[0].textureShadow_);
  glBindTextures(2, 1, &ecsData->shadowMaps_[1].textureShadow_);
  glBindTextures(3, 1, &ecsData->shadowMaps_[2].textureShadow_);
  glBindTextures(4, 1, &ecsData->shadowMaps_[3].textureShadow_);
  glBindTextures(5, 1, &ecsData->shadowMaps_[4].textureShadow_);

  glBindFramebuffer(GL_FRAMEBUFFER, dataGPU->cam2FBO.fFBO_);
  glViewport(0, 0, dataGPU->GetDataWindow()->w_, dataGPU->GetDataWindow()->h_);

  glNamedBufferSubData(dataGPU->PFDBuffer_, 0, sizeof(PerFrameData), &ecsData->cam2_.pfdCam_);

  glBindBufferRange(GL_UNIFORM_BUFFER, 3, dataGPU->LightsBuffer_, 0, sizeof(ecsData->lights_));
  glNamedBufferSubData(dataGPU->LightsBuffer_, 0, sizeof(ecsData->lights_), &ecsData->lights_);

	newDisplay.front().ExecuteOnGPU();

	glBindFramebuffer(GL_FRAMEBUFFER, dataGPU->frame.fFBO_);
	glNamedBufferSubData(dataGPU->PFDBuffer_, 0, sizeof(PerFrameData), &ecsData->cam_.pfdCam_);
	bool stopDl = false;

	while (newDisplay.size() > 0 && (!stopDl))
	{

		DList dl = std::move(newDisplay.front());
		newDisplay.pop_front();


		stopDl = dl.DoSwap();
		dl.ExecuteOnGPU(std::move(dl.commandList_));

	}

	glBindFramebuffer(GL_FRAMEBUFFER, dataGPU->frame.sFBO_);

  glBindTextures(0, 1, &dataGPU->frame.fFramebufferTex_);
  glBindTextures(1, 1, &dataGPU->frame.fFramebufferTexDepth_);
  glBindTextures(2, 1, &lastTex);
	
	drawRect(
		ecsData->meshes_[dataGPU->Rect_.GetID()].vertexArray_,
		ecsData->shaders_[dataGPU->CurrentScreenShader_.GetID()].idProgram_,
		dataGPU->frame.fFramebufferTex_,
		ecsData->meshes_[dataGPU->Rect_.GetID()].sizeIndices_
	);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glBindTextures(0, 1, &dataGPU->frame.sFramebufferTex_);
  glBindTextures(1, 1, &dataGPU->frame.sFramebufferTex_);
  glBindTextures(2, 1, &dataGPU->frame.sFramebufferTex_);
  glBindTextures(3, 1, &dataGPU->cam2FBO.fFramebufferTex_);

  drawRect(
    ecsData->meshes_[dataGPU->Rect_.GetID()].vertexArray_,
    ecsData->shaders_[dataGPU->CurrentScreenShader_.GetID()].idProgram_,
    dataGPU->frame.fFramebufferTex_,
    ecsData->meshes_[dataGPU->Rect_.GetID()].sizeIndices_
  );

	MTR_END("N-Gine_Logic", "RenderWorld");

}
