#version 440

layout(vertices=1)out;

uniform vec3 camPos=vec3(0,0,0);
uniform vec3 center = vec3(0,0,0);

in int vinstanceID[];
out patch int instanceID;

void main(void){
    instanceID=vinstanceID[0];
    float l = length(camPos-center);
    float z = 1-(max(min(l,26),4)-4)/22.0;

    float level = 4+60*z*z*z;

    gl_TessLevelOuter[0]=level;
    gl_TessLevelOuter[1]=level;
    gl_TessLevelOuter[2]=level;
    gl_TessLevelOuter[3]=level;
    gl_TessLevelInner[0]=level;
    gl_TessLevelInner[1]=level;
}
