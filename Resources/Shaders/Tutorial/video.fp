layout(location=0)out vec4 fColor;

layout(binding=0)uniform sampler2D frame;

in vec2 vCoord;

uniform uvec2 windowSize = uvec2(1,1);

uniform uint mode = 0;

void main(){
  fColor = vec4(1,1,1,1);

  vec3 color = texture(frame,vCoord).xyz;

  vec3 origColor;
  vec3 newColor;

  origColor = color;
  newColor = color;

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
