#version 430
//#extension GL_ARB_shader_draw_parameters : require
layout(location=0) in vec3 aPosition;
layout(location=3) in int id;

struct Ball {
	vec2 pos;
	vec2 speed;
	mat4 m;
};

layout(std430, binding = 0) buffer ballBuffer{
    Ball balls[];
};


out flat mat4 m;

mat4 getModelMatrix(){
	if(id/*gl_DrawIDARB*/==0){
		return balls[gl_InstanceID].m;
	}else{
		return mat4(1);
	}
}

void main(){
	m = getModelMatrix();
	gl_Position=vec4(aPosition,1);
}
