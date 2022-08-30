#version 460

uniform layout ( binding = 0 ) sampler2D RenderTexture;
uniform layout ( binding = 1 ) sampler2D DepthTexture;
uniform layout ( binding = 2 ) sampler2D PPRenderTexture;
uniform layout ( binding = 3 ) sampler2D Cam2;

in vec2 uv;
out vec4 FragColor;

void main()
{

	vec2 uvFix = uv;
	uvFix.y = 1.0 - uvFix.y;
	vec4 ppcolor=texture(PPRenderTexture,uvFix);
	//FragColor=vec4(1.0,0.0,1.0,1.0);
	
	if(ppcolor.xyz==vec3(0.0)){
			FragColor=texture(RenderTexture, uvFix);
		}else{
			FragColor=ppcolor;
		}
		/*

	if(uvFix.x>0.5){
		if(ppcolor.xyz==vec3(0.0)){
			FragColor=texture(RenderTexture, uvFix);
		}else{
			FragColor=ppcolor;
		}
	
	}else{
		FragColor=texture(Cam2,uvFix);
	}
	*/
	

}