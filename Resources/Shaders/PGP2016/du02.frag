
in vec3 fPosition;
in vec3 fNormal;

/* Svetlo */
uniform vec3 lightPosition;
uniform vec3 La = vec3(0.6,0.6,0.6);
uniform vec3 Ld = vec3(0.4,0.4,0.4);
uniform vec3 cameraPosition;

layout(location=0)out vec4 fragColor;


void main() {
	vec3 color = materialMarble(fPosition);
	float shininess = 64;
	fragColor = vec4(phong(fPosition,fNormal,color,lightPosition,La,Ld,cameraPosition,vec3(1),shininess),1);
}
