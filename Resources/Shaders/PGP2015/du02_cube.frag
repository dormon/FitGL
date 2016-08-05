#version 430

/*
* Vstupy
*/
in vec3 normal; // normala
in vec2 tc; // texturovaci koordinaty
in vec3 position; // pozice
flat in int id; // id

/* uniformy */
layout(binding=0)uniform sampler2D tex; // textura s travou


/*
* vystupy
*/
/**** STUDENT DOPLNI ****/
/*
* Shader zapisuje do ctyr textur - ctyri vystupy
* pozice,normala,barva,id
* pozor na typ vystupu
*/
layout(location = 0) out vec3 oPosition;
layout(location = 1) out vec3 oNormal;
layout(location = 2) out vec3 oColor;
layout(location = 3) out int oId;
//...


void main(){
	// barva
	vec3 color = texture(tex,tc).xyz;

	/**** STUDENT DOPLNI ****/
	/*
	* Zapis do vystupnich vsech promenny
	*/
	oPosition = position;
	oNormal=normal;
	oColor = color;
	oId = id;
}
