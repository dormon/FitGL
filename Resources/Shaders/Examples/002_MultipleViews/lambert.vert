#version 450
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;

uniform mat4 v;
uniform mat4 p;

out vec3 position;
out vec3 normal;

void main(){
	position = aPosition;
    normal = aNormal;
    gl_Position = p*v*vec4(aPosition,1);
}