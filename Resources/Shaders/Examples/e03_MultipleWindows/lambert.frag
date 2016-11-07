#version 450
in vec3 position;
in vec3 normal;
uniform vec3 lightPosition=vec3(50,50,50);
uniform vec3 color = vec3(0.7,0.5,0.3);
out vec4 fragColor;           

void main(){
	vec3 L = normalize(lightPosition-position);
	vec3 N = normalize(normal);
	vec3 c = (0.3+0.7*max(dot(L,N),0))*color;
	fragColor = vec4(c,1);
}     
