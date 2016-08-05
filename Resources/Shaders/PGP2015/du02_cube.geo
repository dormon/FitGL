#version 430
/*
* Tento shader vygeneruje kostky z bodu na spravne pozici
*/


/* 
* Nastaveni
*/
layout(points)in;
layout(triangle_strip,max_vertices=24)out;

/*
* Vstupy
*/
in int vertexID[];

/*
* Uniformy
*/
uniform mat4 m; //modelova matice
uniform mat4 v; //view matice
uniform mat4 p; //projekcni matice

/*
* vystupy
*/
out vec3 normal;
out vec3 position;
out vec2 tc;
flat out int id;

/* Konstanty */
const vec3 positions[]=vec3[](
	vec3(0,0,0),
	vec3(1,0,0),
	vec3(1,1,0),
	vec3(0,1,0),
	vec3(0,0,1),
	vec3(1,0,1),
	vec3(1,1,1),
	vec3(0,1,1)
);
const vec3 normals[]=vec3[](
	vec3(0,0,-1),
	vec3(0,0,1),
	vec3(1,0,0),
	vec3(-1,0,0),
	vec3(0,1,0),
	vec3(0,-1,0)
);
const vec2 tcs[]=vec2[](
	vec2(0,0),
	vec2(1.0/3.0,0),
	vec2(2.0/3.0,0),
	vec2(1.0,0),
	vec2(0,1),
	vec2(1.0/3.0,1),
	vec2(2.0/3.0,1),
	vec2(1,1)
);

void emitVert(int i, int n, int t){
	position = positions[i]+gl_in[0].gl_Position.xyz-vec3(0.5,0.5,0.5);
	gl_Position=p*v*m*vec4(position,1);
	normal = normals[n];
	id = ((vertexID[0]+1)<<3)+n;
	tc= tcs[t];
	EmitVertex();
}

void emitCube(){
	emitVert(0,0,5);
	emitVert(1,0,6);
	emitVert(3,0,1);
	emitVert(2,0,2);
	EndPrimitive();

	emitVert(5,1,5);
	emitVert(4,1,6);
	emitVert(6,1,1);
	emitVert(7,1,2);
	EndPrimitive();

	emitVert(1,2,5);
	emitVert(5,2,6);
	emitVert(2,2,1);
	emitVert(6,2,2);
	EndPrimitive();

	emitVert(4,3,5);
	emitVert(0,3,6);
	emitVert(7,3,1);
	emitVert(3,3,2);
	EndPrimitive();

	emitVert(3,4,2);
	emitVert(2,4,3);
	emitVert(7,4,6);
	emitVert(6,4,7);
	EndPrimitive();

	emitVert(4,5,0);
	emitVert(5,5,1);
	emitVert(0,5,4);
	emitVert(1,5,5);
	EndPrimitive();
}

void main(){
	emitCube();	
}
