layout(location=0)out vec4 fColor;

layout(binding=0)uniform sampler2D diffuseTextureTop ;
layout(binding=1)uniform sampler2D diffuseTextureSide;
layout(binding=2)uniform sampler2D diffuseTextureDown;

uniform vec4  light = vec4(-10,10,-10,1);
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

  float pxFactor = clamp(dot(normal,vec3(+1, 0, 0)),0,1);
  float nxFactor = clamp(dot(normal,vec3(-1, 0, 0)),0,1);
  float pyFactor = clamp(dot(normal,vec3( 0,+1, 0)),0,1);
  float nyFactor = clamp(dot(normal,vec3( 0,-1, 0)),0,1);
  float pzFactor = clamp(dot(normal,vec3( 0, 0,+1)),0,1);
  float nzFactor = clamp(dot(normal,vec3( 0, 0,-1)),0,1);

  vec2 pxCoord = vPosition.zy*vec2(-1,+1)*.5+.5;
  vec2 nxCoord = vPosition.zy*vec2(+1,+1)*.5+.5;
  vec2 pyCoord = vPosition.xz*vec2(+1,-1)*.5+.5;
  vec2 nyCoord = vPosition.xz*vec2(+1,+1)*.5+.5;
  vec2 pzCoord = vPosition.xy*vec2(+1,+1)*.5+.5;
  vec2 nzCoord = vPosition.xy*vec2(-1,+1)*.5+.5;

  vec3 pxDiffuseColor = texture(diffuseTextureSide,pxCoord).xyz*pxFactor;
  vec3 nxDiffuseColor = texture(diffuseTextureSide,nxCoord).xyz*nxFactor;
  vec3 pyDiffuseColor = texture(diffuseTextureTop ,pyCoord).xyz*pyFactor;
  vec3 nyDiffuseColor = texture(diffuseTextureDown,nyCoord).xyz*nyFactor;
  vec3 pzDiffuseColor = texture(diffuseTextureSide,pzCoord).xyz*pzFactor;
  vec3 nzDiffuseColor = texture(diffuseTextureSide,nzCoord).xyz*nyFactor;

  vec3 diffColor = 
    pxDiffuseColor+
    nxDiffuseColor+
    pyDiffuseColor+
    nyDiffuseColor+
    pzDiffuseColor+
    nzDiffuseColor;


  vec3 phong = ambientFactor*diffColor + diffuseFactor*diffColor + specularFactor*lightColor;

  fColor = vec4(phong,1);
}
