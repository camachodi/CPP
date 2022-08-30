#version 460
uniform layout ( binding = 0 ) sampler2D newFrame;
uniform layout ( binding = 1 ) sampler2D depth;
uniform layout ( binding = 2 ) sampler2D oldFrame;
uniform layout ( binding = 3 ) sampler2D lastTexture;
layout(std140, binding = 3) uniform Lightning
{
        uniform vec4 LightDir;
        uniform vec4 LightingColor;
};
layout(std140, binding = 4) uniform PostProcessData
{
        uniform vec4 pp_color_;
};
in vec4 vertexColor;
in vec2 uv_;
in vec3 FragPos;
in vec3 Normal;

in vec3 viewPos;
out vec4 FragColor;

void main(){
  
    vec2 uv=uv_;
    uv.y=1.0-uv.y;

    vec4 d=texture(depth,uv);

    ivec2 size=textureSize(depth,0);
    vec2 location=size*uv;
	vec4 color;
    vec2 p1=(location+ivec2(1.0,0.0))/size;
	vec2 p2=(location-ivec2(1.0,0.0))/size;
	vec2 p3=(location+ivec2(0.0,1.0))/size;
	vec2 p4=(location-ivec2(0.0,1.0))/size;
    if(texture(depth,uv).x!=1.0&&(
    texture(depth,p1).x==1.0||
    texture(depth,p2).x==1.0||
    texture(depth,p3).x==1.0||
    texture(depth,p4).x==1.0)
    ){
    	color=pp_color_;
    }else{
    	color=texture(lastTexture,uv);
    }
    FragColor=color;
  }