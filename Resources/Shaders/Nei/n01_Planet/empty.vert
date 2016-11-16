#version 440

out int vinstanceID;
void main(void){
    vinstanceID=gl_InstanceID;
    gl_Position = vec4(0,0,0,1);
}
