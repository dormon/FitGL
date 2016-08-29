layout(location=0)out vec4 fColor;
layout(binding=0)uniform samplerCube cubeTexture;

uniform vec4  light = vec4(-1000,100,-1000,1);
uniform vec3  lightColor = vec3(1,1,1);
uniform float shininess = 180;

in vec3 ePosition;
in vec3 eNormal;
in vec3 eView;
in vec3 eCamera;

void main(){
  vec3 normal = normalize(eNormal);
  float specularFactor = specular(ePosition,normal,light.xyz,eCamera,shininess);
  fColor = vec4(1);
  vec3 reflected = reflect(eView,normal);
  vec3 refracted = refract(eView,normal,1.000293/1.330);
  vec3 reflectColor = texture(cubeTexture,reflected).rgb + specularFactor;
  vec3 refractColor = texture(cubeTexture,refracted).rgb;
  float probabilityOfReflection = .5+0*pow(1-clamp(dot(normal,-normalize(eView)),0,1),.5);
  fColor = vec4(reflectColor,max(specularFactor,0.8));
//  fColor = vec4(specularFactor,0,0,1);
}
