#version 430

layout(points)in;
layout(triangle_strip,max_vertices=4)out;

uniform vec2 posMin = vec2(-1,-1);
uniform vec2 posMax = vec2(1,1);
uniform vec2 tcMin = vec2(0,0);
uniform vec2 tcMax = vec2(1,1);

out vec2 tc;

void main(){
	gl_Position = vec4(posMin.x,posMin.y,0,1);
	tc = vec2(tcMin.x,tcMin.y);
	EmitVertex();

	gl_Position = vec4(posMax.x,posMin.y,0,1);
	tc = vec2(tcMax.x,tcMin.y);
	EmitVertex();

	gl_Position = vec4(posMin.x,posMax.y,0,1);
	tc = vec2(tcMin.x,tcMax.y);
	EmitVertex();

	gl_Position = vec4(posMax.x,posMax.y,0,1);
	tc = vec2(tcMax.x,tcMax.y);
	EmitVertex();

	EndPrimitive();
}
