layout(location=0)out vec4 fColor;

layout(binding=0)uniform sampler2D frame;

in vec2 vCoord;

uniform uvec2 windowSize = uvec2(1,1);

uniform uint mode = 0;

float toIntensity(vec3 color){
  return dot(vec3(.2126,.7152,.0722),color);
}


BEGINGRADIENT(redGradient)
  vec4(0.1,0,0,1),
  vec4(1,0,0,1),
ENDGRADIENT

ivec2 texelCoord = ivec2(gl_FragCoord.x,windowSize.y-gl_FragCoord.y-1);

void main(){
  fColor = vec4(1,1,1,1);

  float intensities[9];

  for(int y=-1;y<=1;++y)
    for(int x=-1;x<=1;++x)
      intensities[(1+y)*3+(1+x)] = dot(vec3(.2126,.7152,.0722),texelFetch(frame,texelCoord+ivec2(x,y),0).xyz);

  vec3 color = texelFetch(frame,texelCoord,0).xyz;


  float xSobelMask[9] = {-1,0,1,-2,0,2,-1,0,1};
  float ySobelMask[9] = {-1,-2,-1,0,0,0,1,2,1};

  float xSobel = 0;
  float ySobel = 0;

  for(int i=0;i<9;++i){
    xSobel += intensities[i]*xSobelMask[i];
    ySobel += intensities[i]*ySobelMask[i];
  }

  vec3 sobel = vec3(length(vec2(xSobel,ySobel)));
  //vec3 sobel = vec3(1-length(vec2(xSobel,ySobel)));
  //vec3 sobel = vec3(1-length(vec2(xSobel,ySobel)))*color;
  sobel = (1-sobel) * vec3(sin(toIntensity(color)*10)*.5+.5);
  //sobel = (1-sobel) * vec3(sin(toIntensity(color)*100)*.5+.5);
  
  vec3 origColor;
  vec3 newColor;

  origColor = color;
  newColor = sobel;

  if(mode == 0){
    fColor = vec4(newColor,1);
  }else if(mode == 1){
    if(vCoord.x<.5)
      fColor = vec4(newColor,1);
    else
      fColor = vec4(color,1);
  }
  else if(mode == 2){
    fColor = vec4(color,1);
  }
}
