layout(location=0)out vec4 fColor;

uniform vec4 light = vec4(0,0,10,1);
uniform vec3  diffuseColor = vec3(0.5,0.5,0.5);
uniform vec3  lightColor = vec3(1,1,1);
uniform float shininess = 40;

uniform uint useNormalTexture = 0;
uniform uint useSpecularTexture = 0;

uniform uint drawDiffuseTexture = 0;
uniform uint drawSpecularTexture = 0;
uniform uint drawNormalTexture = 0;


in vec4 vColor;
in vec3 vPosition;
in vec3 vNormal;
in vec2 vCoord;
in vec3 vCamera;

layout(binding=0)uniform sampler2D diffuseTexture ;
layout(binding=1)uniform sampler2D specularTexture;
layout(binding=2)uniform sampler2D normalTexture;

void main(){
  vec3 normal = normalize(vNormal);

  if(useNormalTexture == 1)
    normal = normalize(texture(normalTexture,vCoord).xyz*2-1);

  float ambientFactor  = 0.3;
  float diffuseFactor  = diffuse(vPosition,normal,light.xyz);
  float specularFactor = specular(vPosition,normal,light.xyz,vCamera,shininess);

  if(useSpecularTexture == 1)
    specularFactor*=texture(specularTexture,vCoord).r;

  vec3 diffColor = texture(diffuseTexture,vCoord).xyz;

  vec3 phong = ambientFactor*diffColor + diffuseFactor*diffColor + specularFactor*lightColor;
  //vec3 phong = phongLighting(vPosition,normalize(vNormal),light.xyz,vCamera.xyz,shininess,diffColor,lightColor);

  if(drawDiffuseTexture == 1){
    fColor = texture(diffuseTexture,vCoord);
    return;
  }

  if(drawSpecularTexture == 1){
    fColor = texture(specularTexture,vCoord);
    return;
  }

  if(drawNormalTexture == 1){
    fColor = texture(normalTexture,vCoord);
    return;
  }
   
  fColor = vec4(phong,1);
}
