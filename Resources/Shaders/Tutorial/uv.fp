layout(location=0)out vec4 fColor;

layout(binding=0)uniform sampler2D diffuseTexture ;
layout(binding=1)uniform sampler2D specularTexture;

uniform vec4  light = vec4(10,10,10,1);
uniform vec3  lightColor = vec3(1,1,1);
uniform float shininess = 40;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vCoord;
in vec3 vCamera;


void main(){
  vec3 normal = normalize(vNormal);

  float ambientFactor  = 0.3;
  float diffuseFactor  = diffuse(vPosition,normal,light.xyz);
  float specularFactor = specular(vPosition,normal,light.xyz,vCamera,shininess);

  specularFactor*=texture(specularTexture,vCoord).r;

  vec3 diffColor = texture(diffuseTexture,vCoord).xyz;

  vec3 phong = ambientFactor*diffColor + diffuseFactor*diffColor + specularFactor*lightColor;

  fColor = vec4(phong,1);
}
