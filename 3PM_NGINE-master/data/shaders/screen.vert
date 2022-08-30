#version 460
layout(location = 0) in vec3 Position;
//layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;
//layout(location = 3) in vec3 Tan;
//layout(location = 4) in vec3 BiTan;

out vec2 uv;


void main(){
	gl_Position = vec4(Position.xy,0.0, 1.0);
	uv=UV;
}
