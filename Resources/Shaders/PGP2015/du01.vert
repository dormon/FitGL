#version 430

/*
* Atributy
*/
layout(location=0)in vec2 position;

/*
* Vystup
*/
out vec4 color;

/* Konstanty */
const vec2 offset[]=vec2[](
	vec2(0,0),
	vec2(-0.4,-0.2),
	vec2(0.4,-0.2));

const vec4 colors[]=vec4[](
	vec4(1,1,0,1),
	vec4(0,1,1,1),
	vec4(0,1,0,1));

void main(){
	gl_Position = vec4(position+offset[gl_InstanceID],0,1);
	color = colors[gl_InstanceID];
}
