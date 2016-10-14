#version 430
in flat int id;
layout(location=0) out int oId;

void main(){
	oId=id;
}
