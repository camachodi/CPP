#version 460

uniform layout ( binding = 0 ) sampler2D RenderTexture;
uniform layout ( binding = 1 ) sampler2D DepthTexture;
uniform layout ( binding = 2 ) sampler2D PPRenderTexture;

layout(std140, binding = 4) uniform PPParams
{
        uniform vec4 ppColor;
};

in vec2 uv;
out vec4 FragColor;

void main()
{

	vec2 uvFix = uv;
	uvFix.y = 1.0 - uvFix.y;
  /*
	if(texture(DepthTexture,uvFix).x<1.0){
		FragColor=ppColor;
	}else{
		FragColor=texture(PPRenderTexture,uvFix);
	}
  */
	ivec2 size=textureSize(DepthTexture,0);
  vec2 location=size*uvFix;
	vec4 color;
  vec2 right=(location+ivec2(1.0,0.0))/size;
	vec2 left=(location-ivec2(1.0,0.0))/size;
	vec2 up=(location+ivec2(0.0,1.0))/size;
	vec2 down=(location-ivec2(0.0,1.0))/size;
    if(
    (
      texture(DepthTexture,uvFix).x!=1.0&&(
      texture(DepthTexture,right).x==1.0||
      texture(DepthTexture,left).x==1.0||
      texture(DepthTexture,up).x==1.0||
      texture(DepthTexture,down).x==1.0)
    )||
    (
      texture(DepthTexture,uvFix).x==1.0&&(
      texture(DepthTexture,right).x!=1.0||
      texture(DepthTexture,left).x!=1.0||
      texture(DepthTexture,up).x!=1.0||
      texture(DepthTexture,down).x!=1.0)
    )
    
    ){
    	color=ppColor;
    }else{
    	color=texture(PPRenderTexture,uvFix);
    }
    FragColor=color;



}