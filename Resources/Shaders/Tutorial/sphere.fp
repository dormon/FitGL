layout(location=0)out vec4 fColor;

uniform vec4 light;

uniform vec3  diffuseColor = vec3(0.5,0.5,0.5);
uniform vec3  lightColor = vec3(1,1,1);
uniform float shininess = 180;

uniform int usePhongLighting = 0;
uniform int computeLightInFS = 0;



in vec3 vNormal;
in vec3 vPosition;
in vec3 vCamera;
in vec3 vColor;

void main(){

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

  if(computeLightInFS){
    if(usePhongLighting)
      fColor = vec4(phong  ,1);
    else
      fColor = vec4(lambert,1);
  }else
    fColor = vec4(vColor,1);

}
