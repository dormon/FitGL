#version 430

in vec3 fPosition;
in vec3 fNormal;
in vec3 color;

/* Svetlo */
uniform vec3 lightPosition = vec3(100,500,100);
uniform vec3 La = vec3(0.6,0.6,0.6);
uniform vec3 Ld = vec3(0.4,0.4,0.4);

layout(location=0)out vec4 fragColor;


void main() {
    vec3 N = normalize(fNormal);
    vec3 L = normalize(lightPosition-fPosition);

    vec3 A = La*color;
    vec3 D = Ld*max(dot(L,N),0)*color;

    fragColor = vec4(A+D,1);
}
