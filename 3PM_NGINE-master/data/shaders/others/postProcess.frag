#version 460
uniform layout ( binding = 0 ) sampler2D newFrame;
uniform layout ( binding = 1 ) sampler2D depth;
uniform layout ( binding = 2 ) sampler2D oldFrame;
uniform layout ( binding = 3 ) sampler2D final;
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

    vec2 uv =uv_;
    uv.y=1.0-uv.y;
    vec4 color=texture(newFrame,uv_);
    vec4 color2=texture(oldFrame,uv_);
    /*if(uv_.x>0.5){
   		FragColor =texture(newFrame,uv_);
   	}else{
   		FragColor =texture(oldFrame,uv_);
   	}*/

    if(color2.x>0.0||color2.y>0.0||color2.z>0.0){
    	FragColor=color2;
    }else{
    	FragColor=color;
    }

 
}