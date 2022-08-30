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
    gl_Position = projection * view * MODEL * vec4(aPos, 1.0);
    FragPos = (projection * view * MODEL * vec4(aPos, 1.0)).xyz;
    Normal = mat3(transpose(inverse( MODEL))) * aNormal;//abs(aNormal);//
    //LightPos = lightDirection;
    //LightingColor=lightColor;
    vertexColor = m_color;
    viewPos = camPos;
}