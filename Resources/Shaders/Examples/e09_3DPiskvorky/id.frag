#version 430

uniform uint id;
layout(location=0) out uint oId;

void main(){
	oId = id;
}
