#define PX_SCHED_IMPLEMENTATION 1
#define PX_MEM_IMPLEMENTATION 1

#include "n_window.h"
#include "n_gpumanager.h"
#include "n_render.h"
#include "n_dlist.h"
#include "internals.h"
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include "Minitrace/minitrace.h"
#include "Minitrace/minitrace.c"
#include "px_library/include/px_sched.h"

#include "imgui/include/imgui.h"
#include "imgui/include/imgui_impl_glfw.h"
#include "imgui/include/imgui_impl_opengl3.h"

NGINE::Window* NGINE::Window::instance_ = nullptr;

void Start(NGINE::Window* window_);
void Update(NGINE::Window* window_, NGINE::DList* displayList, float deltaTime);

NGINE::Window::Window()
{

}

NGINE::Window::~Window()
{

}

bool NGINE::Window::init(int32_t width, int32_t height, char* windowName)
{

	if (glfwInit() != GLFW_TRUE)
	{
	
		return false;
	
	}

	instance_ = this;
	
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwSwapInterval(1);

	NGINE::Window::window_ = glfwCreateWindow(width, height, windowName, NULL, NULL);

	w_ = width;
	h_ = height;

	if (NGINE::Window::window_ == nullptr)
	{

		glfwTerminate();
		return false;

	}

	glfwMakeContextCurrent(NGINE::Window::window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{

		printf("GLAD Error. Initialize failure");
		return false;

	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	ImGui_ImplGlfw_InitForOpenGL(window_, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	return true;

}

bool NGINE::Window::closeWindow()
{

	return !glfwWindowShouldClose(NGINE::Window::window_);

}

void auxLayering(NGINE::ECS* ECSData, int id) {
	char idToName[32];
	sprintf(idToName, "%d", ECSData->transforms_[id].id_);

	if (ImGui::TreeNode(idToName))
	{
		if (ECSData->transforms_[id].childrenH_.size() > 0) {
			for (int i = 0; i < ECSData->transforms_[id].childrenH_.size(); i++) {
				auxLayering(ECSData, ECSData->transforms_[ECSData->transforms_[id].childrenH_[i]].id_);
				//
			}
		}
		ImGui::TreePop();

	}
	
}

void NGINE::Window::swap(ECS* ECSData)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("HLayering");

	for (int i = 0; i < ECSData->transforms_.size(); i++)
	{

		if (ECSData->transforms_[i].hLayer_ == 0)
		{
			/*
			char idToName[32];
			sprintf(idToName, "%d", ECSData->transforms_[i].id_);

			if (ImGui::TreeNode(idToName))
			{
				if (ECSData->transforms_[i].childrenH_.size()>0) {
					
				}
				ImGui::TreePop();

			}
			*/
			auxLayering(ECSData,i);
		}

	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(NGINE::Window::window_);
	glfwPollEvents();

}

void NGINE::Window::EndGLFW()
{

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

}

void NGINE::Window::DrawImguiWindow(ECS* ECSData, uint32_t id)
{

	char idToName[32];
	sprintf(idToName, "%d", id);

	if (ImGui::TreeNode(idToName))
	{
		
		for (int i = 0; i < ECSData->transforms_[id].childrenH_.size(); i++)
		{
			
			DrawImguiWindow(ECSData, ECSData->transforms_[id].childrenH_[i]);

		}

		ImGui::TreePop();

	}

}

GLFWwindow* NGINE::Window::getWindow()
{

	return window_;

}

NGINE::Window* NGINE::Window::getInstance()
{

	return NGINE::Window::instance_;

}

int main(void)
{

	NGINE::Window window_;

	window_.init(NGINE::ScreenW_, NGINE::ScreenH_, "NGine");

	NGINE::GPUManager* GPUData = NGINE::GPUManager::GPUData();

	GPUData->Init(window_);

	NGINE::ECS* ECSData = GPUData->GetDataEcs();
	NGINE::Render renderData;
	NGINE::DList DListData;

	Start(&window_);

	px_sched::Scheduler schd;
	schd.init();
	px_sched::Sync schedSync;

	NGINE::Window* windowRef = &window_;

	mtr_init("n-gine_trace.json");
	mtr_register_sigint_handler();
	MTR_META_THREAD_NAME("MULTI-THREADING");
	 
	auto job = [&window_, &DListData, &renderData, &ECSData]
	{
	
		Update(&window_, &DListData, 0.0f);

		ECSData->CalculateTransforms();
		ECSData->CalculateTransformLights();
		ECSData->CalculateShadows();

		DListData.DoWindowSwap(&window_);
		renderData.SubmitLock(std::move(DListData));
	
	};

	MTR_BEGIN("N-Gine_Logic", "logic","Shadows","RenderCMDs","Swap");
  if (GPUData->activateMT)
	{

    schd.run(job, &schedSync);

  }
  else {
    Update(&window_, &DListData, 0.0f);

    ECSData->CalculateTransforms();
    ECSData->CalculateTransformLights();
    ECSData->CalculateShadows();

    DListData.DoWindowSwap(&window_);
    renderData.SubmitLock(std::move(DListData));
  }



	while (window_.closeWindow()) {

		schd.waitFor(schedSync);

		MTR_END("N-Gine_Logic", "logic");
		MTR_BEGIN("N-Gine_Logic", "logic");
		if (GPUData->activateMT) {
			schd.run(job, &schedSync);

		}
		else {
      Update(&window_, &DListData, 0.0f);

      ECSData->CalculateTransforms();
      ECSData->CalculateTransformLights();
      ECSData->CalculateShadows();

      DListData.DoWindowSwap(&window_);
      renderData.SubmitLock(std::move(DListData));
		}

		GPUData->Camera_.CamMove(window_.getWindow(), NGINE::ScreenW_, NGINE::ScreenH_, 25.0f);

		MTR_BEGIN("N-Gine_Rendering", "rendering");
		
		renderData.ExecuteAll();
		MTR_BEGIN("N-Gine_Rendering", "Swap");

		//-----------------------------------------HERE 2

		windowRef->swap(ECSData);

		MTR_END("N-Gine_Rendering", "Swap");
		MTR_END("N-Gine_Rendering", "rendering");

	}

	//--------------------------------------------HERE 3
	
	mtr_flush();
	mtr_shutdown();
	
	window_.EndGLFW();

	return 0;

}