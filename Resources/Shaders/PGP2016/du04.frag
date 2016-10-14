#version 430

out vec4 fragColor;
void main(){
	fragColor = gl_FrontFacing?vec4(1,0,0,1):vec4(0,1,0,1);
}
