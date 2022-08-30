#ifndef __N_DATA_COMPONENTS_H__
#define __N_DATA_COMPONENTS_H__ 1

#include "glm/matrix.hpp"
#include "../src/internals.h"
#include "glm.hpp"
#include <glad.h>
#include <stdint.h>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
namespace NGINE
{
	
	//Data Structure of Mesh Object Component
	struct ObjVert
	{

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec3 tangent_;
		glm::vec3 biTangent_;

		bool operator<(const ObjVert r) const
		{

			return memcmp((void*)this, (void*)&r, sizeof(ObjVert)) > 0;

		};

	};

	//Data Structure of Transform Component
	struct DataTransform
	{

		glm::mat4x4 transform_;
		glm::mat4x4 world_;

		glm::vec3 position_;
		glm::vec3 rotation_;
		glm::vec3 scale_;

		uint32_t id_;
		uint32_t parent_;
		uint32_t hLayer_;
		std::vector<uint32_t> childrenH_;

		bool update_;
		bool worldUpdate_;

	};

	//Data Structure of Textures
	struct DataTexture
	{

		GLuint id_;
		int32_t w_;
		int32_t h_;

	};

	//Data Structure of Shadow Maps
	struct DataShadowMap
	{

		GLuint frameBufferShadow_;
		GLuint textureShadow_;

	};

	//Data Structure of Shader Components
	struct DataShader
	{

		GLuint idVert_;
		GLuint idFrag_;
		GLuint idProgram_;

	};

	//Data Structure of Component Types
	struct DataObject
	{

		uint32_t components_[MAX];

	};

	//Data Structure of Post Process Color, can be used to other purposes
	struct PProcessParams
	{

		glm::vec4 rgba;

	};

	//Data Structure of Post Process
	struct DataPProcess
	{

		uint32_t shader_;
		uint32_t dList_;

		GLuint FBO_;
		GLuint FBOPProcess_;

		GLuint FBTexture_;
		GLuint FBTexturePProcess_;
		GLuint FBTextureDepth_;

		PProcessParams params_;

	};

	//Data Structure of Mesh Component
	struct DataMesh
	{

		GLuint vertexBuffer_;
		GLuint vertexArray_;
		GLuint vertexIndices_;

		size_t sizeIndices_;

	};

	//Data Structure of Material Params, can be used to load more than one texture per material
	struct ParamsMaterial
	{

		glm::vec4 color;
		glm::vec4 metalic;

		int32_t t_albedo = -1;
		/*
		int32_t t_normal = -1;
		int32_t t_metallic = -1;
		int32_t t_roughness = -1;
		int32_t t_ao = -1;
		int32_t t_height = -1;
		int32_t t_specular = -1;
		*/
	};

	//Data Structure of Mesh Component
	struct DataMaterial
	{

		uint32_t idShader_ = 0;
		uint32_t idTexture_ = 0;

		ParamsMaterial matParams_ = ParamsMaterial();

	};

	//Data Structure of PerFrameData
	struct PerFrameData
	{

		glm::mat4 matProj_;
		glm::mat4 matView_;
		glm::vec3 camPos_;
		float fillcamPos_;

	};

	//Data Structure of Light Component
	struct DataLight
	{

		LightType type_ = NONEL;//1
		int32_t castShadow_ = 1;
		glm::ivec2 fillF_ = { 1, 1 };

		glm::vec3 Position_ = { 0.0f, 0.0f, 0.0f };
		float fillPos = 0.0f;//2

		glm::vec3 Direction_ = { 0.0f, 0.0f, 0.0f };
		float fillDir = 0.0f;//3

		glm::vec3 colorAmbient_ = { 1.0f, 1.0f, 1.0f };
		float ambientStrn_ = 0.1f;//4

		glm::vec3 colorDiffuse_ = { 1.0f, 1.0f, 1.0f };
		float diffuseStrn_ = 0.7f;//5

		glm::vec3 colorSpecular_ = { 1.0f, 1.0f, 1.0f };
		float specularStrn_ = 0.1f;//6

		float constant_ = 1.0f;
		float linear_ = 0.09f;
		float quadratic_ = 0.032f;
		float shineAmount_ = 16.0f;//7

		float innerCone_ = 0.99f;
		float outerCone_ = 1.00f;
		glm::vec2 fillIO_ = { 0.0f, 0.0f };//8

		glm::vec4 shadowColor_ = { 0.0f, 0.0f, 0.0f, 1.0f };//9

		int32_t idFrameBuffer_;
		int32_t idObject_;
		int32_t idTransform_;
		int32_t fillID_;//10

		glm::mat4 matProj_;//14
		glm::mat4 matView_;//18

	};

	//Data Structure of Frame Buffers
	struct DataFrameBuffer
	{

		GLuint fFBO_;
		GLuint fFramebufferTex_;
		GLuint fFramebufferTexDepth_;

		int32_t w_;
		int32_t h_;

		GLuint sFBO_;
		GLuint sFramebufferTex_;
		int32_t PProcessShaderID_;
		GLuint finalRenderTexture_;

	};

	//Data Structure of Camera Component
	struct DataCamera
	{

		glm::vec3 camForward_;

		PerFrameData pfdCam_;

		float speed_;
		float speedMin_;
		float speedMax_;
		float sens_;

	};

}
#endif
