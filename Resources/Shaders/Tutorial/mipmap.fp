layout(location=0)out vec4 fColor;

in vec2 vCoord;

layout(binding=0)uniform sampler2D diffuseTexture ;

void main(){
  fColor = texture(diffuseTexture,vCoord);
}
