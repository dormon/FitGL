#version 430
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTc;

uniform mat4 v;
uniform mat4 p;
uniform mat4 smb;

out vec3 position;
out vec3 normal;
out vec2 tc;
out vec4 shadowPos;

void main(){
	position = aPosition;
	shadowPos = smb * (vec4(aPosition,1));
	normal = aNormal;
	tc = aTc;
	gl_Position=p*v*vec4(aPosition,1);
}
