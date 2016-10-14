#version 430

#define PI 3.1415926535897932384626433832795

/* Tesselace - quads */
layout(quads,equal_spacing)in;

// pozice vrcholu nohy
in patch vec3 ePosition;

// matice
uniform mat4 tesView;
uniform mat4 tesProj;

// vyska nohy
uniform float legHeight;

// vystup - pozice a normala
out vec3 fPosition;
out vec3 fNormal;

// matice pro ukol 4
out flat mat4 m;


void main(){	

	/***** DU02 - DOPLNIT *****/
	/*
	 - v zavislosti na tesselacnich koordinatech vygenerovat nohu stolu
	 - potreba zapsat pozici (fPosition) a spravnou normalu (fNormal)
	*/

	fPosition = vec3(1);
	fNormal = vec3(1);
	gl_Position=tesProj*tesView*vec4(fPosition,1);

	m=mat4(1);
}
