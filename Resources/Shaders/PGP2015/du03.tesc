#version 430

/**** STUDENT DOPLNI ****/
/* Pocet vystupnich kontrolnich bodu, pri kresleni trojuhelnika 3, pro kouli staci jeden*/
layout(vertices=3)out;

/* Normaly, pro kouli nebudou potreba*/
in vec3 tNormal[];
out vec3 eNormal[];

uniform int time;


/**** STUDENT DOPLNI ****/
/* Dve vystupni promenne pro stred a radius, promenne pro cely patch */
out patch vec3 center;
out patch float radius;

/**
 * Funkce pro vypocet stredu a polomeru kruznice vepsane
 */
void incircle(in vec3 A, in vec3 B, in vec3 C, out vec3 center, out float radius){
	float a = length(B-C);
	float b = length(C-A);
	float c = length(B-A);

	float perimeter = a + b + c;	
	float area2 = length(cross(B-A,C-A));

	center = (a*A + b*B + c*C) / (a + b + c);
	radius = area2 / perimeter;
}


void main(){
	/**** STUDENT DOPLNI ****/
	/* Vhodne nastaveni teselacnich urovni */
	gl_TessLevelOuter[0]=16;
	gl_TessLevelOuter[1]=16;
	gl_TessLevelOuter[2]=16;
	gl_TessLevelOuter[3]=16;
	gl_TessLevelInner[0]=16;
	gl_TessLevelInner[1]=16;

	/* Zapis kontrolnich bodu - pozice a normala, pro kouli nebude potreba*/
	gl_out[gl_InvocationID].gl_Position=gl_in[gl_InvocationID].gl_Position;
	eNormal[gl_InvocationID] = tNormal[gl_InvocationID];

	/**** STUDENT DOPLNI ****/
	/* Vypocet stredu a polomeru vepsane kruznice a zapis na vystupni promenne
	* Pozice kontrolnich bodu je v gl_in[].gl_Position
	*/
	incircle(
		gl_in[0].gl_Position.xyz,
		gl_in[1].gl_Position.xyz,
		gl_in[2].gl_Position.xyz,
		center,
		radius);
}
