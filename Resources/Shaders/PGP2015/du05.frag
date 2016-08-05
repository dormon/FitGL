#version 430

in vec3 position;
in vec3 normal;
in vec2 tc;

in vec4 shadowPos;

uniform vec3 La;
uniform vec3 Ld;
uniform vec3 lightPosition;

uniform vec3 Kd;
layout(binding=0)uniform sampler2D tex;
layout(binding=1)uniform sampler2DShadow texSM;

out vec4 fragColor;

void main(){
	vec3 color = texture(tex,tc).xyz+Kd;

	//float d = texture(texSM,tc).x;
	float shadow = textureProj(texSM,shadowPos).x;
	
	vec3 L = normalize(lightPosition-position);
    vec3 A = La*color;
    vec3 D = Ld*max(dot(L,normal),0)*color;
	
	fragColor = vec4(A+D*shadow,1);
	//fragColor = vec4(normal*0.5+0.5,1);
}
