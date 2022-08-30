#version 460

layout(std140, binding = 0) uniform PerFrameData
{
  uniform mat4 PROJ;
  uniform mat4 VIEW;
  uniform vec3 camera_Position;
  uniform int isWireframe;
}; 

layout(std140, binding = 1) uniform Material   
{   
  uniform vec4 m_color;   
}; 

layout(std140, binding = 2) uniform Model   
{   
  uniform mat4 MODEL;   
}; 

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 normal;
out vec3 FragPos;
out vec4 vertexColor;
out vec3 WorldPos;
out vec4 svertexColor;
out vec2 uv_;
out vec3 cam_pos;
out vec4 fragPosLight[4];
out mat3 TBN;

uniform mat4 lightProjection[4];
uniform int CastingShadow[4];

void main(){
    FragPos = vec3(MODEL * vec4(aPos, 1.0));
    gl_Position = PROJ * VIEW * MODEL * vec4(aPos, 1.0);
    uv_ = uv;
    vertexColor = m_color;
    cam_pos = camera_Position;
  
    for(int i=0; i<4; ++i){
        fragPosLight[i] = lightProjection[i] * vec4(FragPos.xyz, 1.0f);
    }

    mat3 normalMatrix = transpose(inverse(mat3(MODEL)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    TBN = mat3(T, B, N);    
    
    normal = mat3(transpose(inverse(MODEL))) * aNormal;
    cam_pos = cam_pos;
    FragPos  = FragPos;
}