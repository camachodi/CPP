#version 460

uniform layout(binding = 0) sampler2D t_albedo;

uniform layout(binding = 1) sampler2D shadow1;
uniform layout(binding = 2) sampler2D shadow2;
uniform layout(binding = 3) sampler2D shadow3;
uniform layout(binding = 4) sampler2D shadow4;
uniform layout(binding = 5) sampler2D shadow5;

layout(std140, binding = 2) uniform Material
{

	uniform vec4 color_;
	uniform vec4 metallic_;

};

struct Light
{

	ivec4 TypeNShadow;//1
	vec4 Position;//2
	vec4 Direction;//3
	vec4 lightColorAmbient_;//4
	vec4 lightColorDiffuse_;//5
	vec4 lightColorSpecular_;//6
	vec4 lightCLQS;//7
	vec4 coneRadius;//8
	vec4 shadowColor;//9

	ivec4 ids_;//10

	mat4 projection_;
	mat4 matView_;

};

layout(std140, binding = 3) uniform Lightning
{

	Light[16] lights_;

};

in vec2 uv_;
in vec3 FragPos;
in vec3 Normal;
in vec3 viewPos;
out vec4 FragColor;

vec4 globalShadow_ = vec4(0.0, 0.0, 0.0, 1.0);

vec3 albedoV_ = texture(t_albedo, uv_).rgb;

vec4 result = vec4(0.0, 0.0, 0.0, 1.0);

vec4 GetShadowMaps(int id, vec2 cord)
{

	switch (id)
	{

	case 0: return texture(shadow1, cord);
	case 1: return texture(shadow2, cord);
	case 2: return texture(shadow3, cord);
	case 3: return texture(shadow4, cord);
	case 4: return texture(shadow5, cord);

	}

	return vec4(1.0);

}

vec4 debugin=vec4(1.0f,0.0f,1.0f,1.0f);

void CalculateShadows()
{
		//debugin	= vec4(lights_[1].TypeNShadow.y);

	for (int i = 0; i < 5 && globalShadow_.w > 0.0; i++)
	{

		if (lights_[i].TypeNShadow.y == 1)
		{

			if (lights_[i].TypeNShadow.x == 1)
			{

				vec4 pos = (lights_[i].projection_ * lights_[i].matView_ * vec4(FragPos, 1.0));
				vec3 lightCoords = pos.xyz / pos.w;

				if (lightCoords.z <= 1.0)
				{

					lightCoords = (lightCoords + 1.0) / 2.0;

					if (lightCoords.x >= 0.0f && lightCoords.x <= 1.0f && lightCoords.y >= 0.0f && lightCoords.y <= 1.0f)
					{

						float closestDepth = GetShadowMaps(i, lightCoords.xy).r + 0.0001;

						if (lightCoords.z > closestDepth)
						{

							globalShadow_.xyz += lights_[i].shadowColor.xyz / lights_[i].shadowColor.w;
							globalShadow_.w = 0.0;

						}

					}

				}

			}

		}

	}

}

vec4 directionalLight(int ID)
{

	vec3 lightDir = lights_[ID].Direction.xyz;
	vec3 lightAmbientColor = lights_[ID].lightColorAmbient_.xyz;
	float lightAmbientStr = lights_[ID].lightColorAmbient_.w;
	vec3 lightDiffuseColor = lights_[ID].lightColorDiffuse_.xyz;
	float lightDiffuseStr = lights_[ID].lightColorDiffuse_.w;
	vec3 lightSpecularColor = lights_[ID].lightColorSpecular_.xyz;
	float lightSpecularStr = lights_[ID].lightColorSpecular_.w;
	float shineAmount = lights_[ID].lightCLQS.w;

	//AMBIENT LIGHT
	vec3 ambienRes = lightAmbientColor * lightAmbientStr * albedoV_;

	//DIFFUSE LIGHT
	vec3 normal_ = normalize(Normal);
	vec3 light_ = normalize(-lightDir);
	float diff = max(dot(normal_, light_), 0.0);
	vec3 diffuseRes = lightDiffuseColor * lightDiffuseStr * diff * albedoV_;

	//SPECULAR LIGHT
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectionDir = reflect(-light_, normal_);
	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shineAmount);
	vec3 specularRes = lightSpecularColor * lightSpecularStr * spec; //* specularV_

	vec3 res = ambienRes + diffuseRes + specularRes;

	return vec4(res, 1.0);

}

vec4 spotLight(int ID)
{

	vec3 lightPos = lights_[ID].Position.xyz;
	vec3 lightDir = lights_[ID].Direction.xyz;
	vec3 lightAmbientColor = lights_[ID].lightColorAmbient_.xyz;
	float lightAmbientStr = lights_[ID].lightColorAmbient_.w;
	vec3 lightDiffuseColor = lights_[ID].lightColorDiffuse_.xyz;
	float lightDiffuseStr = lights_[ID].lightColorDiffuse_.w;
	vec3 lightSpecularColor = lights_[ID].lightColorSpecular_.xyz;
	float lightSpecularStr = lights_[ID].lightColorSpecular_.w;
	float shineAmount = lights_[ID].lightCLQS.w;
	float constant = lights_[ID].lightCLQS.x;
	float linear = lights_[ID].lightCLQS.y;
	float quadratic = lights_[ID].lightCLQS.z;
	float innerCone = lights_[ID].coneRadius.x;
	float outerCone = lights_[ID].coneRadius.y;

	//AMBIENT LIGHT
	vec3 ambienRes = lightAmbientColor * lightAmbientStr * albedoV_;

	//DIFFUSE LIGHT
	vec3 normal_ = normalize(Normal);  //normalV_ * 2.0f - 1.0f -> Normal
	vec3 light_ = normalize(lightPos - FragPos);
	float diff = max(dot(normal_, light_), 0.0);
	vec3 diffuseRes = lightDiffuseColor * lightDiffuseStr * diff * albedoV_;

	//SPECULAR LIGHT
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectionDir = reflect(-light_, normal_);
	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shineAmount);
	vec3 specularRes = lightSpecularColor * lightSpecularStr * spec; //* specularV_

	//CONE
	float theta = dot(light_, normalize(-lightDir));
	float epsilon = (outerCone - innerCone);
	float intensity = clamp((theta - innerCone) / epsilon, 0.0f, 1.0f);
	diffuseRes *= intensity;
	specularRes *= intensity;

	//ATTENUATION
	float distance = length(lightPos - FragPos);
	float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));

	ambienRes *= attenuation;
	diffuseRes *= attenuation;
	specularRes *= attenuation;

	vec3 res = ambienRes + diffuseRes + specularRes;

	return vec4(res, 1.0);

}

vec4 pointLight(int ID)
{

	vec3 lightPos = lights_[ID].Position.xyz;
	vec3 lightAmbientColor = lights_[ID].lightColorAmbient_.xyz;
	float lightAmbientStr = lights_[ID].lightColorAmbient_.w;
	vec3 lightDiffuseColor = lights_[ID].lightColorDiffuse_.xyz;
	float lightDiffuseStr = lights_[ID].lightColorDiffuse_.w;
	vec3 lightSpecularColor = lights_[ID].lightColorSpecular_.xyz;
	float lightSpecularStr = lights_[ID].lightColorSpecular_.w;
	float shineAmount = lights_[ID].lightCLQS.w;
	float constant = lights_[ID].lightCLQS.x;
	float linear = lights_[ID].lightCLQS.y;
	float quadratic = lights_[ID].lightCLQS.z;

	//AMBIENT LIGHT
	vec3 ambienRes = lightAmbientColor * lightAmbientStr * albedoV_;

	//DIFFUSE LIGHT
	vec3 normal_ = normalize(Normal);
	vec3 light_ = normalize(lightPos - FragPos);
	float diff = max(dot(normal_, light_), 0.0);
	vec3 diffuseRes = lightDiffuseColor * lightDiffuseStr * diff * albedoV_;

	//SPECULAR LIGHT
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectionDir = reflect(-light_, normal_);
	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shineAmount);
	vec3 specularRes = lightSpecularColor * lightSpecularStr * spec; //* specularV_

	//ATTENUATION
	float distance = length(lightPos - FragPos);
	float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));

	ambienRes *= attenuation;
	diffuseRes *= attenuation;
	specularRes *= attenuation;

	vec3 res = ambienRes + diffuseRes + specularRes;

	return vec4(res, 1.0);

}

void calculateLights(int ID)
{

	switch (lights_[ID].TypeNShadow.x)
	{

	case 0:
		FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		break;

	case 1:
		result += directionalLight(ID);
		//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		break;

	case 2:
		result += spotLight(ID);
		//FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		break;

	case 3:
		result += pointLight(ID);
		//FragColor = vec4(0.0, 0.0, 1.0, 1.0);
		break;

	default:
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		break;

	}

}

void main() {


	for (int i = 0; i < 1; i++)
	{
		calculateLights(i);
	}

	CalculateShadows();

	FragColor = vec4(result.xyz * color_.xyz * globalShadow_.w + globalShadow_.xyz, 1.0);

	//FragColor = texture(shadow2, uv_);
	//vec3 albedo_ = texture(t_albedo, uv_).rgb;
	//vec4 s=texture(shadow1, uv_);
	//FragColor = vec4(vec3(1.0-s.r/10.0),1.0);
	//FragColor = vec4(albedo_ * color_.xyz, 1.0);
	//FragColor = debugin;

}