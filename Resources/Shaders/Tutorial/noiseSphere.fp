layout(location=0)out vec4 fColor;

uniform vec4 light;

uniform vec3  lightColor = vec3(1,1,1);
uniform float shininess = 40;

uniform int usePhongLighting = 0;
uniform int computeLightInFS = 0;

uniform float time = 0;


in vec3 vNormal;
in vec3 vPosition;
in vec3 vCamera;
in vec3 vColor;

void main(){
  float noiseValue = noise(ivec4(vec4(vPosition,time*0.1)*1000),8,8,2);
  vec3 diffuseColor = mix(vec3(.5,0,0),vec3(1,1,.3),noiseValue*.5+.5);

  vec3 lambert = lambertLighting(
    vPosition,
    normalize(vNormal),
    light.xyz,
    diffuseColor);

  vec3 phong = phongLighting(
    vPosition,
    normalize(vNormal),
    light.xyz,
    vCamera,
    shininess,
    diffuseColor,
    lightColor);

  if(computeLightInFS==1){
    if(usePhongLighting==1)
      fColor = vec4(phong  ,1);
    else
      fColor = vec4(lambert,1);
  }else
    fColor = vec4(vColor,1);

}
