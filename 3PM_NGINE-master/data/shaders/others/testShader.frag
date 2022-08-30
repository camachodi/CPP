#version 460
uniform layout ( binding = 0 ) sampler2D albedo;
uniform layout ( binding = 1 ) sampler2D normal;
uniform layout ( binding = 2 ) sampler2D metal;
uniform layout ( binding = 3 ) sampler2D specular;

layout(std140, binding = 1) uniform Material
{
    uniform vec4 albedoColor;
    uniform vec4 metalic; //x:% , y:smoothness

    uniform ivec4 flags; //x: albedo texture, y: normal map texture, z: metalic texture, w: specular texture

};

in vec2 uv_;
in vec3 FragPos;
in vec3 Normal;
in vec3 viewPos;
out vec4 FragColor;

vec3 diffuseG = vec3(0.0);
vec3 ambientG = vec3(0.0);
vec3 specularG = vec3(0.0);

bool direcctionalDone = false;


struct Light{
    ivec4 Type;//0:directional, 1:spotlight, 2: pointlight
    vec4 Pos;//w = trash
    vec4 Dir;//w = trash
    vec4 ColorDiffuse;//xyz:color  ,w: opacity
    vec4 Ambient; //xyz:color  ,w: strength
    vec4 SpecularColor;//xyz:color  ,w: strength
    vec4 values;//x: shyniness, y: linear,  z:quadratic
    vec4 values2;//x: inner, y: outter
    vec4 space;
};

layout(std140, binding = 3) uniform Lightning
{
        Light lights[16];
};


void main(){

    

	
	
	FragColor = texture(albedo, uv_);


}