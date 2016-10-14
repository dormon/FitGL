//#extension GL_ARB_shader_draw_parameters : require
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;
layout(location=3) in int id;

struct Ball {
	vec2 pos;
	vec2 speed;
	mat4 m;
};

layout(std430, binding = 0) buffer ballBuffer{
    Ball balls[];
};

uniform mat4 v;
uniform mat4 p;

out vec3 position;
out vec3 normal;
out vec2 tc;
out flat int InstanceID;
out flat int DrawID;

mat4 getModelMatrix(){
	if(DrawID==0){
		return balls[gl_InstanceID].m;
	}else{
		return mat4(1);
	}
}

void main(){
	InstanceID=gl_InstanceID;
	DrawID=id;  //gl_DrawIDARB;
	position = aPosition;
	normal = mat3(getModelMatrix())*aNormal;
	tc = aTexCoord;
	gl_Position=p*v*getModelMatrix()*vec4(aPosition,1);
}
