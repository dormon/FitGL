#version 430
layout(location=0) in vec3 aPosition;

struct Ball {
	vec2 pos;
	vec2 speed;
	mat4 m;
};

layout(std430, binding = 0) buffer ballBuffer{
    Ball balls[];
};

uniform mat4 v;
uniform mat4 p;

out flat int id;

void main(){
	id=gl_InstanceID;
	gl_Position=p*v*balls[gl_InstanceID].m*vec4(aPosition,1);
}
