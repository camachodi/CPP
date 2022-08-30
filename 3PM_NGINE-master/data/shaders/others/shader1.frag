#version 460
uniform layout ( binding = 0 ) sampler2D albedo;
layout(std140, binding = 3) uniform Lightning
{
        uniform vec4 LightDir;
        uniform vec4 LightingColor;
};
in vec4 vertexColor;
in vec2 uv_;
in vec3 FragPos;
in vec3 Normal;

in vec3 viewPos;
out vec4 FragColor;

void main(){
    float ambient=0.2;
    float specularLight=0.5f;
    

    vec3 normal=normalize(Normal);
    vec3 lightDirection=normalize(LightDir.xyz);
    float diffuse=max(dot(normal,lightDirection),0.0);

    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectionDir=reflect(-lightDirection,normal);
    float specAmount=pow(max(dot(viewDir,reflectionDir),0.0f),8);
    float specular=specularLight*specAmount;

    FragColor =texture(albedo,uv_)*vertexColor*LightingColor*(diffuse+ambient+specular);
    //FragColor =texture(albedo,uv_)*vertexColor*LightingColor*(diffuse);
    //FragColor =vec4(vec3(diffuse),1.0);
}