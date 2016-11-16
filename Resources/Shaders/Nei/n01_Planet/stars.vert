 #version 440

in vec4 pos;

uniform mat4 p;
uniform mat4 v;

void main(void){
    gl_PointSize = pos.w;
    gl_Position = p*v*vec4(pos.xyz,1);

    //gl_Position = vec4(pos.xy,0,1);
}
