layout(location=0)out vec4 fColor;

uniform mat4 v;
uniform mat4 p;


layout(binding=0)uniform samplerCube cubeTexture;

in vec2 vCoord;

void main(){
  
  vec4 viewSpace = inverse(p)*vec4(vCoord,1,1);
  viewSpace.xyz;//*=viewSpace.w;
  viewSpace.w=0;
  vec3 viewVector = (inverse(v)*viewSpace).xyz;


  fColor = vec4(viewVector,0);
  fColor = texture(cubeTexture,viewVector);
  //fColor = vec4(viewSpace.xyz,0);
}
