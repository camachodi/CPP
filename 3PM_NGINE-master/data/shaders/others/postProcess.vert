#version 460
layout(std140, binding = 0) uniform PerFrameData
{
    uniform mat4 projection;
    uniform mat4 view;
    uniform vec3 camPos;
};
layout(std140, binding = 1) uniform Material
{
        uniform vec4 m_color;
        //uniform vec4 light;
};
layout(std140, binding = 2) uniform Model
{
        uniform mat4 MODEL;
};


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 uv;

out vec4 vertexColor;
out vec2 uv_;
out vec3 FragPos;
out vec3 Normal;

out vec3 viewPos;

void main()
{

    uv_ = uv;

    gl_Position = vec4(aPos.xy,0.0, 1.0);
    FragPos = vec3((aPos.xy+vec2(1.0))/2.0,1.0); //vec3(view * MODEL * vec4(aPos, 1.0));
    //Normal = mat3(transpose(inverse(view * MODEL))) * aNormal;//abs(aNormal);//
    //LightPos = lightDirection;
    //LightingColor=lightColor;
    //vertexColor = m_color;
    //viewPos = camPos;
}