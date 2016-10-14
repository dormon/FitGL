#version 430

layout(location=0)in vec3 aPosition;

out vec3 tPosition;

void main(){
    tPosition=aPosition;
}
