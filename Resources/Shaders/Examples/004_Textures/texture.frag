#version 430
layout(binding =0) uniform sampler2D tex;
in vec2 tc;

out vec4 fragColor;
void main(){
	vec3 c = texture(tex,tc).xyz;
	fragColor = vec4(c,1);
}
