#version 430
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTc;

uniform mat4 v;
uniform mat4 p;
uniform mat4 m;
uniform mat4 n;

out vec3 position;
out vec3 normal;
out vec2 tc;

void main(){
	position = aPosition;
	normal = mat3(n)*aNormal;
	tc = aTc;
	gl_Position=p*v*m*vec4(aPosition,1);
}
