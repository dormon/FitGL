#version 430

layout(points)in;
layout(triangle_strip,max_vertices=4)out;

out vec2 tc;

void main(){
	for(int i=0;i<4;++i){
		gl_Position=vec4(-1+2*(i%2),-1+2*(i/2),-1,1);
		tc = vec2(i%2,i/2);
		EmitVertex();
	}
	EndPrimitive();
}
