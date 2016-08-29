layout(location=0)out vec4 fColor;


layout(std430,binding=0)buffer CounterBuffer{uint counter;};

uniform uvec2 windowSize = uvec2(1024,768);
uniform float percent = 0.75;

in vec4 vColor;


void main(){
  uint nofFragments = windowSize.x*windowSize.y*2;
  uint fragmentNumber = atomicAdd(counter,1);

  float ratio = float(fragmentNumber)/float(nofFragments);
  if(ratio>percent)
    discard;
  
  fColor = vColor;

}
