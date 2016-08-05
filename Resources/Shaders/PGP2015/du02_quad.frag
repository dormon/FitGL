#version 430

in vec2 tc;


uniform vec3 lightPosition = vec3(50,500,50);
uniform vec3 La = vec3(0.6,0.6,0.6);
uniform vec3 Ld = vec3(0.4,0.4,0.4);
uniform vec4 clearColor = vec4(0.68,0.88,0.93,1);

uniform uint selectedCube=0;
uniform uint selectedFace=0;

layout(binding=0) uniform sampler2D texturePosition;
layout(binding=1) uniform sampler2D textureNormal;
layout(binding=2) uniform sampler2D textureColor;
layout(binding=3) uniform usampler2D textureId;



out vec4 fragColor;

void main(){

	// cteni z textur
	vec3 position = texture(texturePosition,tc).xyz;
	vec3 normal = texture(textureNormal,tc).xyz;
	vec3 color = texture(textureColor,tc).xyz;
	uint id = texture(textureId,tc).x;

	/* DEBUG 
	* pro zobrazeni jednotlivych textur
	* odkomentuje jeden z radku a return
	*/
	//fragColor = vec4(position,1);
	//fragColor = vec4(normal*0.5+0.5,1);
	//fragColor = vec4(color,1);
	//fragColor = vec4(float(id>>3)/16.0,float(id&7)/6.0,0,1);
	//return;


	// vypocet id kostky a steny
	uint faceId = id&7;
	uint cubeId = id>>3;
	if(cubeId==0){
		fragColor = clearColor;
		return;
	}
	cubeId--;
		
	// vypocet osvetleni
    vec3 L = normalize(lightPosition-position);
    vec3 A = La*color;
    vec3 D = Ld*max(dot(L,normal),0)*color;
	
	fragColor = vec4(A+D,1);

	// zvyrzaneni
	if( cubeId == selectedCube){
		if( faceId == selectedFace){
			fragColor += vec4(0,0.6,0,0);
		}else{
			fragColor += vec4(0.4,0.4,0.4,0);
		}
	}


}
