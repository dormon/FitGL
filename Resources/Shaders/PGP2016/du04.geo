#version 430
layout(triangles)in;

/***** DU04 - DOPLNIT *****/
/*
 - nastavit max pocet vertexu
*/
layout(triangle_strip, max_vertices = 8)out;

in flat mat4 m[];
uniform mat4 v;
uniform mat4 p;
uniform vec3 lightPosition;

void main(){
	mat4 vp = p*v;
	// vstupni trojuhelnik
	vec4 a=m[0]*gl_in[0].gl_Position;
	vec4 b=m[0]*gl_in[1].gl_Position;
	vec4 c=m[0]*gl_in[2].gl_Position;

	/***** DU04 - DOPLNIT *****/
	/*
	 - vygenerovat shadow volume
	 - pozor na orientaci a privraceni ke svetlu
	 - zapis trasformovane pozice do gl_Position
	*/

}
