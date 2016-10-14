#version 430

/* Pocet vystupnich kontrolnich bodu 
 * Nas jediny kontrolni bod je pozice vrcholu nohy
 */
layout(vertices=1)out;

/* Vstupni kontrolni body(1) - pozice */
in vec3 tPosition[];

// vystupni promenna pro pozici
out patch vec3 ePosition;

void main(){
	/***** DU02 - DOPLNIT *****/
	/*
	 - Vhodne nastaveni teselacnich urovni 
	*/
	gl_TessLevelOuter[0]=1;
	gl_TessLevelOuter[1]=1;
	gl_TessLevelOuter[2]=1;
	gl_TessLevelOuter[3]=1;
	gl_TessLevelInner[0]=1;
	gl_TessLevelInner[1]=1;

	ePosition = tPosition[0];
}
