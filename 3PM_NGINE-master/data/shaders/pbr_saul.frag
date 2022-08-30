#version 460

layout (location = 0) out vec4 FragColor;
layout (location = 5) out vec3 gNormal;
layout (location = 6) out vec3 gAlbedo;
layout (location = 7) out vec3 gPosition;

in vec3 normal;
in vec3 FragPos;
in vec4 vertexColor;
in vec3 WorldPos;
in vec4 svertexColor;
in vec2 uv_;
in vec3 cam_pos;
in vec4 fragPosLight[4];
in mat3 TBN;

//shadows
uniform int CastingShadow[4];

const float PI = 3.14159265359;
const vec3 Fdielectric = vec3(0.04);
const float Epsilon = 0.00001;

struct Light{
	ivec4 Type;
	vec4 Position;
	vec4 Direction;
	vec4 strengthNShine;
	vec4 lightColorAmbient_;
	vec4 lightColorDiffuse_;
	vec4 lightColorSpecular_;
	vec4 lightCLQ;
	vec4 coneRadius;
};

layout(std140, binding = 3) uniform LightUniform
{   
	Light lights[32];
}; 

//Material
uniform layout ( binding = 0 ) sampler2D t_albedo;
uniform layout ( binding = 1 ) sampler2D t_normal;
uniform layout ( binding = 2 ) sampler2D t_metallic;
uniform layout ( binding = 3 ) sampler2D t_roughness;
uniform layout ( binding = 4 ) sampler2D t_ao;
uniform layout ( binding = 5 ) sampler2D t_height;
uniform layout ( binding = 6 ) sampler2D t_specular;
//uniform float shineAmount;

//shadowmapS
uniform layout ( binding = 7 ) sampler2D shadowMap;
uniform layout ( binding = 8 ) sampler2D shadowMap1;
uniform layout ( binding = 9 ) sampler2D shadowMap2;
uniform layout ( binding = 10 ) sampler2D shadowMap3;

vec3 albedoV_ = texture(t_albedo, uv_).rgb;
vec3 normalV_ = texture(t_normal, uv_).rgb;
float metallicV_ = texture(t_metallic, uv_).r;
float roughnessV_ = texture(t_roughness, uv_).r;
float aoV_ = texture(t_ao, uv_).r;
vec3 specularV_ = texture(t_specular, uv_).rgb;

vec4 ambient = vec4(0.0, 0.0, 0.0, 1.0);
vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

vec4 result = vec4(0.0, 0.0, 0.0, 1.0);

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

//Calculate Specular
vec3 calculateSpecular(int ID,vec3 normal_, vec3 lightDirection, vec3 viewDirection, float diffuse)
{
	float specular = 0.0f;
	vec4 baseReflectivity = vec4(0.04);
	if (diffuse != 0.0f)
	{
		float specularLight = lights[ID].strengthNShine.z;
		
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal_, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

	vec3 Lo = vec3(0.0);
	vec3 N = normalize(normal_);
	vec3 V = viewDirection;
	vec3 L = normalize(lightDirection);
	vec3 H = normalize(V + L);

	float NdotV = max(dot(N, V), 0.001); 
	float NdotL = max(dot(N, L), 0.001);
	float HdotV = max(dot(H, V), 0.001);
	float NdotH = max(dot(N, H), 0.001);

	float D = ndfGGX(NdotH, roughnessV_);			
	float G = gaSchlickGGX(NdotV, NdotL, roughnessV_);	
	vec3 F = fresnelSchlick(HdotV, baseReflectivity.xyz);	

	vec3 specularValue = D * G * F ;
	specularValue /= 4.0 * NdotV * NdotL ;	
	specular += specularValue.r;
	specular -= (1 - (roughnessV_ )) * 0.5;

	vec3 kD = vec3(1.0f) - F;
	kD *= metallicV_.r;

	Lo += (kD * albedoV_ / PI + (specular * specularV_.r)) * NdotL * NdotH;

	return Lo;
}

//Shadows
float ProcessShadow(vec3 lightDirection, vec4 fragPosL, int idx){

	// Shadow value
	float shadow = 0.0f;
	vec3 lightCoords = fragPosL.xyz / fragPosL.w;
	float bias = max(0.0025f * (1.0f - dot(normal, lightDirection)), 0.0005f);
	int sampleRadius = 2;

		switch(idx) 
	{
		case 0: 

		if(lightCoords.z <= 1.0f)
		{
			lightCoords = (lightCoords + 1.0f) / 2.0f;
			float currentDepth = lightCoords.z;
			vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
			for(int y = -sampleRadius; y <= sampleRadius; y++)
			{
				for(int x = -sampleRadius; x <= sampleRadius; x++)
				{
					float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
					if (currentDepth > closestDepth + bias)
						shadow += 1.0f;     
				}    
			}

			shadow /= pow((sampleRadius * 2 + 1), 2);
		}

		break;
		case 1: 
		if(lightCoords.z <= 1.0f)
		{
			lightCoords = (lightCoords + 1.0f) / 2.0f;
			float currentDepth = lightCoords.z;
			vec2 pixelSize = 1.0 / textureSize(shadowMap1, 0);
			for(int y = -sampleRadius; y <= sampleRadius; y++)
			{
				for(int x = -sampleRadius; x <= sampleRadius; x++)
				{
					float closestDepth = texture(shadowMap1, lightCoords.xy + vec2(x, y) * pixelSize).r;
					if (currentDepth > closestDepth + bias)
						shadow += 1.0f;     
				}    
			}
			shadow /= pow((sampleRadius * 2 + 1), 2);
		}

		break;
		case 2: 
		if(lightCoords.z <= 1.0f)
		{
			lightCoords = (lightCoords + 1.0f) / 2.0f;
			float currentDepth = lightCoords.z;
			vec2 pixelSize = 1.0 / textureSize(shadowMap2, 0);
			for(int y = -sampleRadius; y <= sampleRadius; y++)
			{
				for(int x = -sampleRadius; x <= sampleRadius; x++)
				{
					float closestDepth = texture(shadowMap2, lightCoords.xy + vec2(x, y) * pixelSize).r;
					if (currentDepth > closestDepth + bias)
						shadow += 1.0f;     
				}    
			}
			shadow /= pow((sampleRadius * 2 + 1), 2);
		}

		break;

		case 3: 
			
		if(lightCoords.z <= 1.0f)
		{
			lightCoords = (lightCoords + 1.0f) / 2.0f;
			float currentDepth = lightCoords.z;
			vec2 pixelSize = 1.0 / textureSize(shadowMap3, 0);
			for(int y = -sampleRadius; y <= sampleRadius; y++)
			{
				for(int x = -sampleRadius; x <= sampleRadius; x++)
				{
					float closestDepth = texture(shadowMap3, lightCoords.xy + vec2(x, y) * pixelSize).r;
					if (currentDepth > closestDepth + bias)
						shadow += 1.0f;     
				}    
			}
			shadow /= pow((sampleRadius * 2 + 1), 2);
		}

		break;
		
		default:
		break;
	}

	return (1.0f - shadow);
}

vec4 direcLight(int ID)
{
	
	vec3 cam_pos_mod = cam_pos * TBN;
	vec3 FragPos_mod = FragPos * TBN;

	float ambient = lights[ID].strengthNShine.x;
	vec3 viewDirection = normalize((cam_pos_mod - FragPos_mod));
	
	// DIFUSE
	vec3 normal_ = normalize((texture(t_normal, uv_).xyz * 2.0 - 1.0));
	vec3 lightDirection = normalize(TBN * lights[ID].Direction.xyz);
	float diffuse = max(dot(normal_, lightDirection), 0.0f);

	//SHADOWS
	float shadow = 1.0f;
	for (int j = 0; j < 4; ++j){
		if(CastingShadow[j] == ID){
			shadow = (ProcessShadow(lightDirection, fragPosLight[j] ,j));
		}
	}

	//AO
	float ao = texture(t_ao, uv_).r;

	//METALIC
	vec4 baseReflectivity = vec4(0.1);
	vec4 resalbedo = (texture(t_albedo, uv_) * ao); 
	resalbedo = mix(baseReflectivity,resalbedo,1 - vec4(vec3(metallicV_.r),1.0));

	//SPECULAR || ROUGHNESS
	vec3 Lo = calculateSpecular(ID,normal_,lightDirection,viewDirection,diffuse) * shadow;
	//

	vec4 diffuseRes = resalbedo * (diffuse * (shadow) + ambient);

	vec4 res = (diffuseRes + vec4(Lo,1.0)) * vec4(lights[ID].lightColorDiffuse_.xyz,1.0);
	return res;
}


vec4 pointLight(int ID)
{	
	vec3 lightPos = lights[ID].Position.xyz;
	vec3 lightAmbientColor = lights[ID].lightColorAmbient_.xyz;
	float lightAmbientStr = lights[ID].strengthNShine.x;
	vec3 lightDiffuseColor = lights[ID].lightColorDiffuse_.xyz;
	float lightDiffuseStr = lights[ID].strengthNShine.y;
	vec3 lightSpecularColor = lights[ID].lightColorSpecular_.xyz;
	float lightSpecularStr = lights[ID].strengthNShine.z;
	float shineAmount = lights[ID].strengthNShine.a;
	float constant = lights[ID].lightCLQ.x;
	float linear = lights[ID].lightCLQ.y;
	float quadratic = lights[ID].lightCLQ.z;

	//DIFFUSE LIGHT
	vec3 normal_ = normalize(normalV_ * 2.0 - 1.0);
	normal_ = normalize(TBN * normal_);
	vec3 light_ = normalize(lightPos - FragPos);
	float diff = max(dot(normal_, light_), 0.0);
	vec3 diffuseRes = lightDiffuseColor * lightDiffuseStr * diff;

	//SPECULAR LIGHT
	vec3 viewDir = normalize(cam_pos - FragPos);
	vec3 reflectionDir = reflect(-light_, normal_);
	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shineAmount);
	vec3 specularRes = lightSpecularColor * lightSpecularStr * spec;

	//ATTENUATION
	float distance = length(lightPos - FragPos);
	float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));
	float darkSide=clamp(dot(light_,normal_), 0.0, 1.0);

	diffuseRes *= attenuation * darkSide;
	specularRes *= attenuation * darkSide;

	return vec4(diffuseRes + specularRes, 1.0); //+ albedoV_
}

vec4 spotLight(int ID)
{

	vec3 lightPos = lights[ID].Position.xyz;
	vec3 lightDir = lights[ID].Direction.xyz;
	vec3 lightAmbientColor = lights[ID].lightColorAmbient_.xyz;
	float lightAmbientStr = lights[ID].strengthNShine.x;
	vec3 lightDiffuseColor = lights[ID].lightColorDiffuse_.xyz;
	float lightDiffuseStr = lights[ID].strengthNShine.y;
	vec3 lightSpecularColor = lights[ID].lightColorSpecular_.xyz;
	float lightSpecularStr = lights[ID].strengthNShine.z;
	float shineAmount = lights[ID].strengthNShine.a;
	float constant = lights[ID].lightCLQ.x;
	float linear = lights[ID].lightCLQ.y;
	float quadratic = lights[ID].lightCLQ.z;
	float innerCone = lights[ID].coneRadius.x;
	float outerCone = lights[ID].coneRadius.y;

	//DIFFUSE LIGHT
	vec3 normal_ = normalize(normalV_ * 2.0 - 1.0);
	normal_ = normalize(TBN * normal_);
	vec3 light_ = normalize(lightPos - FragPos);
	float diff = max(dot(normal_, light_), 0.0);
	vec3 diffuseRes = lightDiffuseColor * lightDiffuseStr * diff;

	//SPECULAR LIGHT
	vec3 viewDir = normalize(cam_pos - FragPos);
	vec3 reflectionDir = reflect(-light_, normal_);
	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shineAmount);
	vec3 specularRes = lightSpecularColor * lightSpecularStr * spec;

	//CONE
	float theta = dot(light_, normalize(-lightDir));
	float epsilon = (outerCone - innerCone);
	float intensity = clamp((theta - innerCone) / epsilon, 0.0f, 1.0f);

	//ATTENUATION
	float distance = length(lightPos - FragPos);
	float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));
	float darkSide=clamp(dot(light_,normal_), 0.0, 1.0);

	diffuseRes *= attenuation * intensity * darkSide;
	specularRes *= attenuation * intensity * darkSide;

	return vec4(diffuseRes + specularRes, 1.0); //+ albedoV_

}


void calculateLights(int ID)
{

	switch(lights[ID].Type.x)
	{
		case 2:
				result += direcLight(ID);
				//FragColor = vec4(0.0, 0.0, 0.0, 1.0);
			break;

    	case 3:
				result += spotLight(ID);
				//FragColor = vec4(0.0, 0.0, 0.0, 1.0);
			break;

    	case 4:
				result += pointLight(ID);
				//FragColor = vec4(0.0, 0.0, 0.0, 1.0);
			break;

		default:
				//FragColor = vec4(0.0, 0.0, 0.0, 1.0);
			break;
	}
	
}
float near = 0.1; 
float far  = 70.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}


void main(){

	for(int i = 0; i < 32; i++)
	{
		calculateLights(i);
	}

	//result.xyz = result.xyz / (result.xyz + vec3(1.0));
    //result.xyz = pow(result.xyz, vec3(1.0/2.2));  

	FragColor =  result * vertexColor;

	gPosition = FragPos;
    
  	gNormal = normalize(normal);
	
	float resAlbedo = LinearizeDepth(gl_FragCoord.z)/far;
  	gAlbedo = vec3(resAlbedo,resAlbedo,resAlbedo);
}