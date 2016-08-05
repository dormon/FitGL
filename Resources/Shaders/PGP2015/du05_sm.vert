#version 430
layout(location=0) in vec3 aPosition;

uniform mat4 vp;

void main(){
	gl_Position=vp*vec4(aPosition,1);
}
