#include "n_ngine.h"
#include <glfw3.h>

NGINE::Object cube1;
NGINE::Object cube2;
NGINE::Object cube3;
NGINE::Object cube4;
NGINE::Light light1;
NGINE::Light light2;
NGINE::Light light3;
NGINE::Light light4;
NGINE::Transform transform;
NGINE::Transform transform2;

void Start(NGINE::Window* window)
{

	NGINE::GPUManager* gpu = NGINE::GPUManager::GPUData();

	gpu->activatePP = true;
	gpu->activateMT = false;

	cube1 = gpu->CreateObject();
	cube2 = gpu->CreateObject();
	cube3 = gpu->CreateObject();
	cube4 = gpu->CreateObject();
	light1 = gpu->CreateLight(NGINE::DIRECTIONAL);
	light2 = gpu->CreateLight(NGINE::DIRECTIONAL);
	light3 = gpu->CreateLight(NGINE::DIRECTIONAL);
	light4 = gpu->CreateLight(NGINE::DIRECTIONAL);


	NGINE::Shader shader = gpu->LoadShader("../../data/shaders/testShader.frag", "../../data/shaders/testShader.vert");
	NGINE::Material material = gpu->CreateMaterial({ glm::vec4(1.0f,1.0f,1.0f,1.0f) }, shader);
	NGINE::Material material3 = gpu->CreateMaterial({ glm::vec4(1.0f,1.0f,1.0f,1.0f) }, shader);
	NGINE::Material material4 = gpu->CreateMaterial({ glm::vec4(1.0f,1.0f,1.0f,1.0f) }, shader);
	material4.LoadTextureAlbedo(gpu->cam2Texture);
	transform = cube1.GetTransform();
	NGINE::Mesh mesh = gpu->LoadMesh("../../data/geometries/CubeTesting.obj");

	NGINE::Texture texture = gpu->LoadTexture("../../data/textures/smile.jpg");
	material.LoadTextureAlbedo(texture);
	material.SetColor(0.0f, 1.0f, 0.0f, 1.0f);
	//transform.SetPosition(glm::vec3(0.0f, 0.0f, 2000.0f));

	cube1.SetMaterial(material);
	material3.LoadTextureAlbedo(gpu->screenTexture);
	cube3.SetMaterial(material3);
	cube3.GetTransform().SetPosition({ 10.0f,-20.0f,20.0f });
	cube4.GetTransform().SetPosition({ -10.0f,-20.0f,20.0f });
	cube1.SetMesh(mesh);
	cube3.SetMesh(mesh);
	cube4.SetMesh(mesh);
	cube4.SetMaterial(material4);

	cube1.GetTransform().SetScale(glm::vec3(100.0f, 0.5f, 100.0f));
	cube1.GetTransform().SetPosition(glm::vec3(0.0f, -25.0f, 0.0f));

	NGINE::Shader shader2 = gpu->LoadShader("../../data/shaders/testShader.frag", "../../data/shaders/testShader.vert");
	NGINE::Material material2 = gpu->CreateMaterial({ glm::vec4(1.0f,1.0f,1.0f,1.0f) }, shader2);
	transform2 = cube2.GetTransform();
	NGINE::Mesh mesh2 = gpu->LoadMesh("../../data/geometries/Wooden_Crate_01.obj");
	NGINE::Texture texture2 = gpu->LoadTexture("../../data/textures/rock_albedo.jpg");
	material2.LoadTextureAlbedo(texture2);
	material2.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	transform2.SetPosition(glm::vec3(2.0f, -10.0f, 10.0f));
	//cube2.GetTransform().SetParent(cube1.GetTransform());

	cube2.SetMaterial(material2);
	cube2.SetMesh(mesh2);

	//cube2.GetTransform().SetParent(cube1.GetTransform());


	light1.GetTransform().SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	light2.GetTransform().SetRotation(glm::vec3(-90.0f, 0.0f, 40.0f));
	light3.GetTransform().SetRotation(glm::vec3(-75.0f, 40.0f, 40.0f));
	light4.GetTransform().SetRotation(glm::vec3(-70.0f, 60.0f, 0.0f));

	light1.SetColorShadow(glm::vec3(0.5f, 0.0f, 0.0f));
	light2.SetColorShadow(glm::vec3(0.0f, 0.5f, 0.0f));
	light3.SetColorShadow(glm::vec3(0.0f, 0.0f, 0.5f));
	light4.SetColorShadow(glm::vec3(0.5f, 0.0f, 0.5f));

	NGINE::Shader shaderPP = gpu->LoadShader("../../data/shaders/ppShader.frag", "../../data/shaders/ppShader.vert");

	NGINE::PProcess pp1 = gpu->CreatePProcess(shaderPP, { glm::vec4(1.0f,0.0f,1.0f,1.0f) });
	NGINE::PProcess silouete = gpu->CreatePProcess(shaderPP, { glm::vec4(1.0f,0.0f,1.0f,1.0f) });
	NGINE::PProcess silouete2 = gpu->CreatePProcess(shaderPP, { glm::vec4(.0f,0.0f,1.0f,1.0f) });
	NGINE::PProcess silouete3 = gpu->CreatePProcess(shaderPP, { glm::vec4(1.0f,1.0f,1.0f,1.0f) });
	NGINE::PProcess silouete4 = gpu->CreatePProcess(shaderPP, { glm::vec4(1.0f,1.0f,0.0f,1.0f) });

	cube1.SetPProcess(silouete);
	cube2.SetPProcess(silouete2);
	cube3.SetPProcess(silouete3);
	cube4.SetPProcess(silouete4);

}

void Update(NGINE::Window* window, NGINE::DList* displayList, float deltaTime)
{

	float time_ = (float)glfwGetTime();

	displayList->DoClear(0.0f, 0.2f, 0.3f, 1.0f);

	//transform.SetPosition(glm::vec3(0.0f, cosf(time_) * 5.0f, /*cosf(time_) * 10.0f +*/ 10.0f + time_ + 0.5f));
	////transform.SetRotation(glm::vec3(0.0f, 0.0f, 30.0f * time_));
	//transform2.SetPosition(glm::vec3(cosf(time_) * 20.0f, cosf(time_) * 5.0f, cosf(time_) * 10.0f + 15.0f));

	displayList->DoDraw(cube2);
	displayList->DoDraw(cube1);
	displayList->DoDraw(cube3);
	displayList->DoDraw(cube4);
	//displayList->DoSwap();

}