#version 450

layout(location=0)out vec4 fColor;

in vec2 vCoord;
noperspective in vec2 vCoordNonperspective;

uniform uint mode = 0;

void main(){
  float freq = 0.1;
  vec2 coord;
  if(mode == 0)
    coord = vCoord;
  if(mode == 1)
    coord = vCoordNonperspective;
  bool xStripes = mod(coord.x,freq)/freq>0.5;
  bool yStripes = mod(coord.y,freq)/freq>0.5;
  fColor = vec4(xStripes != yStripes);
}
