#version 430

/*
* Atributy
*/
layout(location=0)in vec3 position;

/*
* Vystupy
*/
out int vertexID;

void main(){
	gl_Position = vec4(position,1);
	vertexID = gl_VertexID;
}
