in vec3 position;
in vec3 normal;
in vec2 tc;
in flat int InstanceID;
in flat int DrawID;

uniform vec3 La = vec3(0.6);
uniform vec3 Ld = vec3(0.6);
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform int pick;

out vec4 fragColor;

void main(){
	// barva 
	vec3 color = vec3(1,1,1);
	// lesklost
	float shininess = 0;

	switch(DrawID){ 
		case 0: {
			color = materialBall(InstanceID, tc);
			shininess = 16;
			if(InstanceID==pick) color +=vec3(0,1,0);
			break;
		}
		case 1:{
			color = materialLightWood(position);
			shininess=64;
			break;
		}
		case 3:{
			color = materialMarble(position);
			shininess = 64;
			break;
		}
		case 2:{
			color = materialTable(position);
			break;
		}
	}

	

	fragColor = vec4(phong(position,normal,color,lightPosition,La,Ld,cameraPosition,vec3(1),shininess),1);

	if(DrawID>65536)fragColor=vec4(1,0,0,1);

	//fragColor = vec4(normal*0.5+0.5,1);
}
