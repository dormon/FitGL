layout(vertices=1)out;

flat in uint vId[];
out patch uint cId;
out patch vec3 cCamera;

uniform mat4 v;

void main(){
  cId = vId[0];
  cCamera = (inverse(v)*vec4(0,0,0,1)).xyz;

  float level = 32;
  gl_TessLevelInner[0] = level;
  gl_TessLevelInner[1] = level;
  gl_TessLevelOuter[0] = level;
  gl_TessLevelOuter[1] = level;
  gl_TessLevelOuter[2] = level;
  gl_TessLevelOuter[3] = level;
}
