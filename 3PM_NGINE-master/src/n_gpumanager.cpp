#include "..\include\n_gpumanager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm.hpp"

#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>

NGINE::GPUManager::GPUManager()
{
}

NGINE::GPUManager::~GPUManager()
{
}

void NGINE::GPUManager::Init(NGINE::Window& w)
{

	window_ = &w;

	// CREATE RECT FRAME BUFFER

	ECSData_.cam_.camForward_ = glm::vec3(0.0f, 0.0f, 1.0f);
	ECSData_.cam_.pfdCam_.camPos_ = glm::vec3(0.0f, 0.0f, 0.0f);
	ECSData_.cam_.pfdCam_.matView_ = glm::lookAt(ECSData_.cam_.pfdCam_.camPos_, ECSData_.cam_.pfdCam_.camPos_ + ECSData_.cam_.camForward_, ECSData_.cam_.pfdCam_.camPos_ + glm::vec3(0.0f, 1.0f, 0.0f));
	ECSData_.cam_.pfdCam_.matProj_ = glm::perspective(45.0f, (float)(w.w_) / (float)(w.h_), 0.001f, 3001.0f);

	glCreateBuffers(1, &MatDataBuffer_);
	glNamedBufferStorage(MatDataBuffer_, sizeof(ParamsMaterial), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glCreateBuffers(1, &TransformBuffer_);
	glNamedBufferStorage(TransformBuffer_, sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glCreateBuffers(1, &PFDBuffer_);
	glNamedBufferStorage(PFDBuffer_, sizeof(PerFrameData), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glCreateBuffers(1, &LightsBuffer_);
	glNamedBufferStorage(LightsBuffer_, sizeof(ECSData_.lights_), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glCreateBuffers(1, &PPBuffer_);
	glNamedBufferStorage(PPBuffer_, sizeof(PProcessParams), nullptr, GL_DYNAMIC_STORAGE_BIT);

	//Rect frame buffers
	glCreateTextures(GL_TEXTURE_2D, 1, &frame.fFramebufferTex_);
	glTextureParameteri(frame.fFramebufferTex_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(frame.fFramebufferTex_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(frame.fFramebufferTex_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(frame.fFramebufferTex_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(frame.fFramebufferTex_, 1, GL_RGB8, w.w_, w.h_);


	glCreateTextures(GL_TEXTURE_2D, 1, &frame.fFramebufferTexDepth_);
	glTextureParameteri(frame.fFramebufferTexDepth_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(frame.fFramebufferTexDepth_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(frame.fFramebufferTexDepth_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(frame.fFramebufferTexDepth_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(frame.fFramebufferTexDepth_, 1, GL_DEPTH32F_STENCIL8, w.w_, w.h_);


	glCreateFramebuffers(1, &frame.fFBO_);
	glNamedFramebufferTexture(frame.fFBO_, GL_COLOR_ATTACHMENT0, frame.fFramebufferTex_, 0);
	glNamedFramebufferTexture(frame.fFBO_, GL_DEPTH_ATTACHMENT, frame.fFramebufferTexDepth_, 0);


	glCreateTextures(GL_TEXTURE_2D, 1, &frame.sFramebufferTex_);
	glTextureParameteri(frame.sFramebufferTex_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(frame.sFramebufferTex_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(frame.sFramebufferTex_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(frame.sFramebufferTex_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(frame.sFramebufferTex_, 1, GL_RGB8, w.w_, w.h_);

	glCreateFramebuffers(1, &frame.sFBO_);
	glNamedFramebufferTexture(frame.sFBO_, GL_COLOR_ATTACHMENT0, frame.sFramebufferTex_, 0);


	for (int i = 0; i < 5; i++)
	{

		glCreateFramebuffers(1, &ECSData_.shadowMaps_[i].frameBufferShadow_);
		glCreateTextures(GL_TEXTURE_2D, 1, &ECSData_.shadowMaps_[i].textureShadow_);
		glTextureParameteri(ECSData_.shadowMaps_[i].textureShadow_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(ECSData_.shadowMaps_[i].textureShadow_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(ECSData_.shadowMaps_[i].textureShadow_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTextureParameteri(ECSData_.shadowMaps_[i].textureShadow_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTextureStorage2D(ECSData_.shadowMaps_[i].textureShadow_, 1, GL_DEPTH32F_STENCIL8, shadowMapSizeX_, shadowMapSizeY_);
		glNamedFramebufferTexture(ECSData_.shadowMaps_[i].frameBufferShadow_, GL_DEPTH_ATTACHMENT, ECSData_.shadowMaps_[i].textureShadow_, 0);

	}

	Rect_=LoadMesh("../../data/geometries/canvas.obj");
	ScreenShader_=LoadShader("../../data/shaders/screen.frag", "../../data/shaders/screen.vert");
	CurrentScreenShader_= ScreenShader_;
	LoadTexture("../../data/textures/white.png");


	RectObject_ = CreateObject();
	RectObject_.SetMesh(Rect_);
	RectObject_.SetMaterial(CreateMaterial({ glm::vec4(1.0f,1.0f,1.0f,1.0f) }, CurrentScreenShader_));
	
	ECSData_.pprocesses_.push_back({});
	ECSData_.displayLists_.push_back(std::move(DList()));


  uint32_t id = ECSData_.textures_.size();
  DataTexture dt;
  dt.id_ = frame.sFramebufferTex_;
  dt.w_ = ScreenW_;
  dt.h_ = ScreenH_;
	ECSData_.textures_.push_back(dt);
	screenTexture=Texture(id);


	//cam2
	glCreateTextures(GL_TEXTURE_2D, 1, &cam2FBO.fFramebufferTex_);
	glTextureParameteri(cam2FBO.fFramebufferTex_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(cam2FBO.fFramebufferTex_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(cam2FBO.fFramebufferTex_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(cam2FBO.fFramebufferTex_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(cam2FBO.fFramebufferTex_, 1, GL_RGB8, w.w_, w.h_);


	glCreateTextures(GL_TEXTURE_2D, 1, &cam2FBO.fFramebufferTexDepth_);
	glTextureParameteri(cam2FBO.fFramebufferTexDepth_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(cam2FBO.fFramebufferTexDepth_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(cam2FBO.fFramebufferTexDepth_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(cam2FBO.fFramebufferTexDepth_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(cam2FBO.fFramebufferTexDepth_, 1, GL_DEPTH32F_STENCIL8, w.w_, w.h_);


	glCreateFramebuffers(1, &cam2FBO.fFBO_);
	glNamedFramebufferTexture(cam2FBO.fFBO_, GL_COLOR_ATTACHMENT0, cam2FBO.fFramebufferTex_, 0);
	glNamedFramebufferTexture(cam2FBO.fFBO_, GL_DEPTH_ATTACHMENT, cam2FBO.fFramebufferTexDepth_, 0);


	uint32_t id2 = ECSData_.textures_.size();
	DataTexture dt2;
	dt2.id_ = cam2FBO.fFramebufferTex_;
	dt2.w_ = ScreenW_;
	dt2.h_ = ScreenH_;
	ECSData_.textures_.push_back(dt2);
	cam2Texture = Texture(id2);

	ECSData_.cam2_=ECSData_.cam_;

}

NGINE::GPUManager* NGINE::GPUManager::GPUData()
{

	static GPUManager gpuData;
	return &gpuData;

}

NGINE::ECS* NGINE::GPUManager::GetDataEcs()
{

	return &ECSData_;

}

NGINE::Window* NGINE::GPUManager::GetDataWindow()
{

	return window_;

}

bool NGINE::GPUManager::compVert(glm::vec3 vertA, glm::vec2 uvA, glm::vec3 normA, glm::vec3 vertB, glm::vec2 uvB, glm::vec3 normB)
{

	if (vertA == vertB && uvA == uvB && normA == normB)
	{
		return true;
	}
	else
	{
		return false;

	}

}

NGINE::Mesh NGINE::GPUManager::LoadMesh(const char* path)
{

	std::vector<ObjVert> data_;
	std::vector<ObjVert> auxDrawdata_;

	std::vector<int> indices_;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path);

	std::map<ObjVert, int> uniqueVerts;

	for (auto& shape : shapes)
	{

		for (auto& index : shape.mesh.indices)
		{

			int vindex = index.vertex_index * 3;
			int nindex = index.normal_index * 3;

			ObjVert ov;

			ov.position = glm::vec3(
				attrib.vertices[vindex + 0],
				attrib.vertices[vindex + 1],
				attrib.vertices[vindex + 2]);

			ov.normal = glm::vec3(
				attrib.normals[nindex + 0],
				attrib.normals[nindex + 1],
				attrib.normals[nindex + 2]
			);

			ov.tangent_ = glm::vec3(0, 0, 0);
			ov.biTangent_ = glm::vec3(0, 0, 0);

			ov.uv = ((index.texcoord_index >= 0) ? glm::vec2(attrib.texcoords[2 * index.texcoord_index + 0], 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]) : glm::vec2());

			if (uniqueVerts.count(ov) == 0)
			{
				
				uniqueVerts[ov] = static_cast<uint32_t>(data_.size());
				data_.push_back(ov);

			}

			auxDrawdata_.push_back(ov);

			indices_.push_back(uniqueVerts[ov]);

		}

	}

	for (int i = 0; i < auxDrawdata_.size(); i += 3)
	{

		glm::vec3& v0 = auxDrawdata_[i + 0].position;
		glm::vec3& v1 = auxDrawdata_[i + 1].position;
		glm::vec3& v2 = auxDrawdata_[i + 2].position;

		glm::vec2& uv0 = auxDrawdata_[i + 0].uv;
		glm::vec2& uv1 = auxDrawdata_[i + 1].uv;
		glm::vec2& uv2 = auxDrawdata_[i + 2].uv;

		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		tangent = glm::normalize(tangent);
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
		bitangent = glm::normalize(bitangent);

		auxDrawdata_[i].tangent_ = tangent;
		auxDrawdata_[i + 1].tangent_ = tangent;
		auxDrawdata_[i + 2].tangent_ = tangent;

		auxDrawdata_[i].biTangent_ = bitangent;
		auxDrawdata_[i + 1].biTangent_ = bitangent;
		auxDrawdata_[i + 2].biTangent_ = bitangent;

	}

	for (int i = 0; i < indices_.size(); i++)
	{

		for (int j = 0; j < auxDrawdata_.size(); j++)
		{

			int k = indices_[i];

			if (compVert(data_[k].position, data_[indices_[k]].uv, data_[k].normal, auxDrawdata_[j].position, auxDrawdata_[j].uv, auxDrawdata_[j].normal))
			{

				data_[k].tangent_ += auxDrawdata_[j].tangent_;
				data_[k].biTangent_ += auxDrawdata_[j].biTangent_;

			}

		}

	}

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	GLuint meshData;
	glCreateBuffers(1, &meshData);
	glNamedBufferStorage(meshData, data_.size() * sizeof(ObjVert), nullptr, GL_DYNAMIC_STORAGE_BIT);

	GLuint handleElements;
	glCreateBuffers(1, &handleElements);
	glNamedBufferStorage(handleElements, indices_.size() * sizeof(unsigned int), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glVertexArrayElementBuffer(vao, handleElements);
	glVertexArrayVertexBuffer(vao, 0, meshData, 0, sizeof(ObjVert));

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);
	glEnableVertexArrayAttrib(vao, 3);
	glEnableVertexArrayAttrib(vao, 4);

	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3));
	glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec3));
	glVertexArrayAttribFormat(vao, 3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3));
	glVertexArrayAttribFormat(vao, 4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec3));


	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayAttribBinding(vao, 2, 0);
	glVertexArrayAttribBinding(vao, 3, 0);
	glVertexArrayAttribBinding(vao, 4, 0);

	glNamedBufferSubData(meshData, 0, data_.size() * sizeof(ObjVert), data_.data());
	glNamedBufferSubData(handleElements, 0, indices_.size() * sizeof(unsigned int), indices_.data());

	DataMesh dataMeshToLoad;
	dataMeshToLoad.vertexArray_ = vao;
	dataMeshToLoad.sizeIndices_ = indices_.size();

	uint32_t index = (uint32_t)ECSData_.meshes_.size();
	ECSData_.meshes_.push_back(dataMeshToLoad);

	return Mesh(index);

}

NGINE::Material NGINE::GPUManager::CreateMaterial(const ParamsMaterial& params, const Shader& shader)
{

	uint32_t index = ECSData_.materials_.size();

	DataMaterial materialData;
	materialData.matParams_ = params;
	materialData.idShader_ = shader.GetID();

	ECSData_.materials_.push_back(materialData);
	return Material(index);

}

std::string readFile(const char* fileName)
{
	FILE* file = fopen(fileName, "r");

	if (!file)
	{
		printf("I/O error. Cannot open shader file '%s'\n", fileName);
		return std::string();
	}

	fseek(file, 0L, SEEK_END);
	const auto bytesinfile = ftell(file);
	fseek(file, 0L, SEEK_SET);

	char* buffer = (char*)alloca(bytesinfile + 1);
	const size_t bytesread = fread(buffer, 1, bytesinfile, file);
	fclose(file);

	buffer[bytesread] = 0;

	static constexpr unsigned char BOM[] = { 0xEF, 0xBB, 0xBF };

	if (bytesread > 3)
	{
		if (!memcmp(buffer, BOM, 3))
			memset(buffer, ' ', 3);
	}

	std::string code(buffer);

	while (code.find("#include ") != code.npos)
	{
		const auto pos = code.find("#include ");
		const auto p1 = code.find('<', pos);
		const auto p2 = code.find('>', pos);
		if (p1 == code.npos || p2 == code.npos || p2 <= p1)
		{
			printf("Error while loading shader program: %s\n", code.c_str());
			return std::string();
		}
		const std::string name = code.substr(p1 + 1, p2 - p1 - 1);
		const std::string include = readFile(name.c_str());
		code.replace(pos, p2 - pos + 1, include.c_str());
	}

	return code;
}

NGINE::Shader NGINE::GPUManager::LoadShader(char const* frag, char const* vert)
{

	DataShader shaderData;

	shaderData.idVert_ = glCreateShader(GL_VERTEX_SHADER);
	shaderData.idFrag_ = glCreateShader(GL_FRAGMENT_SHADER);

	std::string svsource = readFile(vert).data();
	std::string sfsource = readFile(frag).data();

	char* vsource = new char[svsource.size() + 1];
	std::copy(svsource.begin(), svsource.end(), vsource);
	vsource[svsource.size()] = '\0';

	char* fsource = new char[sfsource.size() + 1];
	std::copy(sfsource.begin(), sfsource.end(), fsource);
	fsource[sfsource.size()] = '\0';

	glShaderSource(shaderData.idVert_, 1, &vsource, NULL);
	glShaderSource(shaderData.idFrag_, 1, &fsource, NULL);

	//VERTEX LOAD----------------------

	glCompileShader(shaderData.idVert_);
	GLint compiled = GL_FALSE;
	glGetShaderiv(shaderData.idVert_, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderData.idVert_, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderData.idVert_, maxLength, &maxLength, &errorLog[0]);

		printf("VERTEX:\n");

		for (char i : errorLog)
		{
		
			printf("%c", i);
		
		}

		printf("\n");
	}

	//FRAGMENT LOAD----------------------

	glCompileShader(shaderData.idFrag_);
	glGetShaderiv(shaderData.idFrag_, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderData.idFrag_, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderData.idFrag_, maxLength, &maxLength, &errorLog[0]);

		printf("FRAGMENT:\n");

		for (char i : errorLog)
		{

			printf("%c", i);
		
		}

		printf("\n");
	}

	shaderData.idProgram_ = glCreateProgram();
	

	glAttachShader(shaderData.idProgram_, shaderData.idVert_);
	glAttachShader(shaderData.idProgram_, shaderData.idFrag_);

	glLinkProgram(shaderData.idProgram_);

	delete[] vsource;
	delete[] fsource;

	uint32_t index = (uint32_t)ECSData_.shaders_.size();
	ECSData_.shaders_.push_back(shaderData);

	return Shader(index);

}

NGINE::Object NGINE::GPUManager::CreateObject()
{

	Object object;
	DataObject objectData;

	uint32_t objectID = ECSData_.objects_.size();
	DataTransform objectDataTransf;

	objectDataTransf.position_ = glm::vec3(0.0f, 0.0f, 0.0f);
	objectDataTransf.rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);
	objectDataTransf.scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
	objectDataTransf.transform_ = glm::identity<glm::mat4>();
	objectDataTransf.hLayer_ = 0;
	objectDataTransf.update_ = false;
	objectDataTransf.worldUpdate_ = true;
	objectDataTransf.parent_ = 0;
	objectDataTransf.id_ = objectID;

	for (int i = 0; i < MAX; i++)
	{

		objectData.components_[i] = 0;

	}

	objectData.components_[TRANSFORM] = objectID;

	ECSData_.objects_.push_back(objectData);
	ECSData_.transforms_.push_back(objectDataTransf);
	ECSData_.sortedTransformID_.push_back(objectID);

	object.Init(objectID);

	Transform objectTransf = Transform(objectID);

	objectTransf.SetPosition(glm::uvec3(0.0f, 0.0f, 0.0f));
	objectTransf.SetScale(glm::uvec3(1.0f, 1.0f, 1.0f));
	objectTransf.GetWorldTransform();

	object.SetTransform(Transform(objectID));

	return object;

}

NGINE::Texture NGINE::GPUManager::LoadTexture(const char* image)
{

	DataTexture textureData;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(0);
	unsigned char* bytes = stbi_load(image, &textureData.w_, &textureData.h_, &numColCh, 0);

	GLenum format;
	if (numColCh == 1)
		format = GL_RED;
	else if (numColCh == 3)
		format = GL_RGB;
	else if (numColCh == 4)
		format = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &textureData.id_);

	glTextureParameteri(textureData.id_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(textureData.id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(textureData.id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(textureData.id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureStorage2D(textureData.id_, 1, GL_RGBA8, textureData.w_, textureData.h_);
	glTextureSubImage2D(textureData.id_, 0, 0, 0, textureData.w_, textureData.h_, format, GL_UNSIGNED_BYTE, bytes);

	glGenerateTextureMipmap(textureData.id_);

	uint32_t index = ECSData_.textures_.size();
	ECSData_.textures_.push_back(textureData);

	stbi_image_free(bytes);

	return Texture(index);

}

NGINE::PProcess NGINE::GPUManager::CreatePProcess(const Shader& ppshader, const PProcessParams& data)
{

	DataPProcess ppData;
	ppData.shader_ = ppshader.GetID();
	ppData.dList_ = ECSData_.displayLists_.size();
	ECSData_.displayLists_.push_back(std::move(DList()));

	uint32_t ppIndex = ECSData_.pprocesses_.size();

	glCreateFramebuffers(1, &ppData.FBO_);

	glCreateTextures(GL_TEXTURE_2D, 1, &ppData.FBTexture_);

	glTextureParameteri(ppData.FBTexture_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(ppData.FBTexture_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(ppData.FBTexture_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(ppData.FBTexture_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureStorage2D(ppData.FBTexture_, 1, GL_RGB8, ScreenW_, ScreenH_);

	glNamedFramebufferTexture(ppData.FBO_, GL_COLOR_ATTACHMENT0, ppData.FBTexture_, 0);

	//-------------------


	glCreateTextures(GL_TEXTURE_2D, 1, &ppData.FBTextureDepth_);

	glTextureParameteri(ppData.FBTextureDepth_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(ppData.FBTextureDepth_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(ppData.FBTextureDepth_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(ppData.FBTextureDepth_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTextureStorage2D(ppData.FBTextureDepth_, 1, GL_DEPTH32F_STENCIL8, ScreenW_, ScreenH_);

	glNamedFramebufferTexture(ppData.FBO_, GL_DEPTH_ATTACHMENT, ppData.FBTextureDepth_, 0);

	//-------------------

	glCreateFramebuffers(1, &ppData.FBOPProcess_);

	glCreateTextures(GL_TEXTURE_2D, 1, &ppData.FBTexturePProcess_);

	glTextureParameteri(ppData.FBTexturePProcess_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(ppData.FBTexturePProcess_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(ppData.FBTexturePProcess_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(ppData.FBTexturePProcess_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(ppData.FBTexturePProcess_, 1, GL_RGB8, ScreenW_, ScreenH_);

	glNamedFramebufferTexture(ppData.FBOPProcess_, GL_COLOR_ATTACHMENT0, ppData.FBTexturePProcess_, 0);

	ppData.params_ = data;

	ECSData_.pprocesses_.push_back(ppData);
	return PProcess(ppIndex);

}

NGINE::Light NGINE::GPUManager::CreateLight(LightType type_)
{

	Object lightObj = CreateObject();
	uint32_t id= ECSData_.activeLights_;
	DataLight& light = ECSData_.lights_[id];
	ECSData_.activeLights_++;
	light.type_ = type_;
	light.idObject_=lightObj.GetID();
	light.idTransform_=lightObj.GetTransform().GetID();
	light.castShadow_=1;
	if (light.type_ == DIRECTIONAL)
	{

		light.matProj_ = glm::ortho(-90.0f, 90.0f, -90.0f, 90.0f, 0.1f, 2000.0f);

	}

	return Light(id);
}

void NGINE::GPUManager::ApplyPPScreenShader(const Shader& ppShader)
{

	CurrentScreenShader_=ppShader;

}
