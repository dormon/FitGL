#version 440

layout(points)in;
layout(triangle_strip,max_vertices=4)out;

uniform mat4 v;
uniform mat4 p;

uniform vec3 beamStart;
uniform vec3 beamEnd;
uniform float beamWidth;

uniform vec3 camPos;

out vec2 ptc;
out vec2 tc;
out flat float fBeamLength;
out flat float fBeamWidth;

void main(void){
    mat4 vp = p*v;

    vec3 beamDir = beamEnd-beamStart;
    float beamLength = length(beamDir);
    vec3 camDir = camPos-beamStart;
    float width = 0.5*beamWidth;
    vec3 offset = width*normalize(cross(camDir,beamDir));

    vec3 s1 = beamStart+offset;
    vec3 s2 = beamStart-offset;

    vec3 e1 = beamEnd+offset;
    vec3 e2 = beamEnd-offset;

    gl_Position = vp*vec4(s2,1);
    tc = vec2(0,0);
    ptc = vec2(0,-width);
    fBeamLength = beamLength;
    fBeamWidth = width;
    EmitVertex();
    gl_Position = vp*vec4(s1,1);
    tc = vec2(0,1);
    ptc = vec2(0,+width);
    fBeamLength = beamLength;
    fBeamWidth = width;
    EmitVertex();

    gl_Position = vp*vec4(e2,1);
    tc = vec2(1,0);
    ptc = vec2(beamLength,-width);
    fBeamLength = beamLength;
    fBeamWidth = width;
    EmitVertex();
    gl_Position = vp*vec4(e1,1);
    tc = vec2(1,1);
    ptc = vec2(beamLength,width);
    fBeamLength = beamLength;
    fBeamWidth = width;
    EmitVertex();


    EndPrimitive();
}
