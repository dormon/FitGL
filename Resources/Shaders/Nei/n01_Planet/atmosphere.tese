#version 440


#define PI 3.1415926535897932384626433832795
layout(quads,equal_spacing)in;

in patch int instanceID;
uniform vec3 center = vec3(0,0,0);
uniform float radius = 1.0;

uniform uint seed = 0;

uniform mat4 v;
uniform mat4 p;


out vec3 pos;
out vec3 normal;


void main(void){
    int idx=instanceID&3;
    int idy=instanceID>>2;

    float radx = (gl_TessCoord.x)*PI*2.0;
    float rady = (gl_TessCoord.y)*PI;
    float x =  cos(radx)*sin(rady);
    float y =  cos(rady);
    float z =  sin(radx)*sin(rady);

    pos = vec3(x,y,z);
	normal = vec3(x,y,z);

    pos = center+ radius*1.1*pos;


    gl_Position=p*v*vec4(pos,1);
}
