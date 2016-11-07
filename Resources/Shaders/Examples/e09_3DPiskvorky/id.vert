#version 430
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTc;

uniform mat4 v;
uniform mat4 p;
uniform mat4 m;

void main(){
	gl_Position=p*v*m*vec4(aPosition,1);
}
