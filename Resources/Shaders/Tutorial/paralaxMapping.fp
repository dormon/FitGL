layout(location=0)out vec4 fColor;

uniform vec4 light = vec4(5,5,5,1);
uniform vec3  diffuseColor = vec3(0.5,0.5,0.5);
uniform vec3  lightColor = vec3(1,1,1);
uniform float shininess = 180;

uniform uint useNormalMapping = 0;
uniform uint useParalaxMapping = 0;

in vec4 vColor;
in vec3 vPosition;
in vec3 vNormal;
in vec2 vCoord;
in vec3 vCamera;

in vec2 vStartCoord;

vec3 computeNormal(int stpX,int stpY,ivec2 pos,uint maxLevel,uint level,float persistency){
  float d = 4;
  float dx = noise(pos+ivec2(stpX,0),maxLevel,level,persistency)-
  noise(pos-ivec2(stpX,0),maxLevel,level,persistency);
  float dy = noise(pos+ivec2(0,stpY),maxLevel,level,persistency)-
  noise(pos-ivec2(0,stpY),maxLevel,level,persistency);
  return normalize(vec3(-dx,-dy,d));
}

#define DEF_COMPUTE_NORMAL(normalFce,heightMapFce)\
vec3 normalFce(vec2 coord){\
  float d = 4;\
  float stpx = 0.01;\
  float stpy = 0.01;\
  float dx = heightMapFce(coord+vec2(stpx,0))-heightMapFce(coord-vec2(stpx,0));\
  float dy = heightMapFce(coord+vec2(0,stpy))-heightMapFce(coord-vec2(0,stpy));\
  return normalize(vec3(-dx,-dy,d));\
}

BEGINGRADIENT(colors)
  vec4(0.1,0.1,0.1,1),
  vec4(.0,.4,0,1),
  vec4(.7,.7,.6,1),
ENDGRADIENT

float marbleHeight(vec2 coord){
  uint noiseScale = 1000;
  return sin(noise(ivec2(coord*noiseScale),8u,8u,2)*10)*.5+.5;
}

DEF_COMPUTE_NORMAL(marbleNormal,marbleHeight)


void main(){
  vec3 normal = normalize(vNormal);

  uint noiseScale = 1000;
  int stpX = 10;
  int stpY = 10;

  vec2 coord;

  if(useParalaxMapping == 1){
    int N=20;
    vec2 newCoord;
    vec3 view = normalize(vCamera.xyz-vPosition.xyz);
    float paralaxDepth = 0.06;
    vec2 startCoord = (vPosition.xy+view.xy*(paralaxDepth/view.z))*.5+.5;
    for(int i=0;i<N;++i){
      float t=float(i)/(N-1);
      newCoord = startCoord+(vCoord-startCoord)*t;
      if(marbleHeight(newCoord)>(1-t))break;
    }
    coord = newCoord;
  }else
    coord = vCoord;


  if(useNormalMapping == 1 || useParalaxMapping ==1)
    normal = marbleNormal(coord);

  float height = marbleHeight(coord);
  vec3 diffColor = vec3(colors(height));




  float ambientFactor  = 0.3;
  float diffuseFactor  = diffuse(vPosition,normal,light.xyz);
  float specularFactor = specular(vPosition,normal,light.xyz,vCamera,shininess);


  vec3 phong = ambientFactor*diffColor + diffuseFactor*diffColor + specularFactor*lightColor;

  fColor = vec4(phong,1);
}
