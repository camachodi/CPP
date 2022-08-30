#ifndef __N_GPUMANAGER_H__
#define __N_GPUMANAGER_H__ 1

#include "n_data_components.h"
#include "n_window.h"
#include "n_mesh.h"
#include "n_shader.h"
#include "n_material.h"
#include "n_object.h"
#include "n_camera.h"
#include "n_framebuffer.h"
#include "n_pprocess.h"
#include "n_ecs.h"
#include "glm/matrix.hpp"
#include "glm/vec3.hpp"
#include <string>
#include <map>

namespace NGINE
{

	class GPUManager
	{

	public:

		GPUManager();
		~GPUManager();

		//Window initializer
		void Init(NGINE::Window& w);

		//Returns the data stored in the GPU manager
		static GPUManager* GPUData();

		//Returns the data stored in the Entity Component System
		ECS* GetDataEcs();

		//Returns the data of the window app
		Window* GetDataWindow();

		//Auxiliary function to compare the data between to meshes
		bool compVert(glm::vec3 vertA, glm::vec2 uvA, glm::vec3 normA, glm::vec3 vertB, glm::vec2 uvB, glm::vec3 normB);
		
		//Load mesh into the engine
		Mesh LoadMesh(const char* path);

		//Create a new material to be used with the object surfaces
		Material CreateMaterial(const ParamsMaterial& params, const Shader& shader);

		//Load shader file into the engine
		Shader LoadShader(char const* frag, char const* vert);

		//Create an object component with all subcomponents
		Object CreateObject();

		//Load texture image into the engine
		Texture LoadTexture(const char* image);

		//Create a new post process
		PProcess CreatePProcess(const Shader& ppshader, const PProcessParams& data);

		//Create light component to illuminate the scene
		Light CreateLight(LightType type_);

		//Apply a desired post process on the screen
		void ApplyPPScreenShader(const Shader& ppShader);

	//SCREEN RECT FRAME BUFFER-------------------
		DataFrameBuffer frame;
		DataFrameBuffer cam2FBO;

	//BUFFERS-------------------------
		GLuint TransformBuffer_;
		GLuint PFDBuffer_;
		GLuint LightsBuffer_;
		GLuint MatDataBuffer_;
		GLuint PPBuffer_;

	//OBJECTS-------------------------
		Camera Camera_;


		Mesh Rect_;
		Object RectObject_;
		Texture screenTexture;
		Texture cam2Texture;
		std::vector<Object> ObjLua;

	//SHADERS-------------------------
		Shader ScreenShader_;
		Shader CurrentScreenShader_;

		//Boolean that determines if the post processes are active or not
		bool activatePP;

		//Multi thread
		bool activateMT;

	private:

		//ECS reference to get the data
		ECS ECSData_;

		//Window reference to get the data
		Window* window_;

	};

}

#endif
