#version 460

uniform layout(binding = 0) sampler2D t_albedo;

layout(std140, binding = 2) uniform Material
{

	uniform vec4 color_;
	uniform vec4 metallic_;

};

struct Light
{

	ivec4 TypeNShadow;
	vec4 Position;
	vec4 Direction;
	vec4 lightColorAmbient_;
	vec4 lightColorDiffuse_;
	vec4 lightColorSpecular_;
	vec4 lightCLQS;
	vec4 coneRadius;
	ivec4 ids_;

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

vec3 diffuseG = vec3(0.0);
vec3 ambientG = vec3(0.0);
vec3 specularG = vec3(0.0);
float shadowG = 1.0f;
bool direcctionalDone = false;

vec3 albedoV_ = texture(t_albedo, uv_).rgb;

vec4 result = vec4(0.0, 0.0, 0.0, 1.0);

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
		//FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		break;
	}

}

void main() {

	//FragColor = vec4(uv_.yx, 0.0, 1.0);

	//for (int i = 0; i < 1; i++)
	//{
	//	calculateLights(i);
	//}

	vec3 albedo_ = texture(t_albedo, uv_).rgb;

	//FragColor = result * color_;
	//FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	FragColor = vec4(albedo_ * color_.xyz, 1.0);

}