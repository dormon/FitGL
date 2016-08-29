layout(location=0)out vec4 fColor;

layout(binding=0)uniform sampler2D frame;

in vec2 vCoord;

uniform uvec2 windowSize = uvec2(1,1);

uniform uint mode = 0;

BEGINGRADIENT(redGradient)
  vec4(0.1,0,0,1),
  vec4(1,0,0,1),
ENDGRADIENT

float gaussFunction(float x,float sigma){
  return inversesqrt(2*radians(180)*sigma*sigma)*exp(-x*x/(2*sigma*sigma));
}

ivec2 texelCoord = ivec2(gl_FragCoord.x,windowSize.y-gl_FragCoord.y-1);

void main(){
  fColor = vec4(1,1,1,1);

  vec3 color = texture(frame,vCoord).xyz;
  float intensity = dot(vec3(.2126,.7152,.0722),color);

  float redness = 1-distance(color.rbg,vec3(1,0,0))/sqrt(3);

  vec3 origColor;
  vec3 newColor;

  const int gaussSize = 10;
  vec3 gauss = vec3(0);
  float accumulatedWeights = 0;
  for(int x=-gaussSize;x<=gaussSize;++x)
    for(int y=-gaussSize;y<=gaussSize;++y){
      float distanceFromCenter = length(vec2(x,y));
      float g = gaussFunction(distanceFromCenter,10);
      accumulatedWeights += g;
      gauss += texelFetch(frame,texelCoord+ivec2(x,y),0).xyz*g;
    }
  gauss /= accumulatedWeights;
  
  origColor = color;
  newColor = gauss;

  //newColor = vec3(intensity>0.4);
  //newColor = mix(vec3(intensity>0.4),redGradient(intensity).xyz,smoothstep(0.6,.8,float(pow(redness,1))));

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
