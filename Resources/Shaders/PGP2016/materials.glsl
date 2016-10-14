const vec3 colors[] = vec3[](
	vec3(1,1,0),
	vec3(0,0,1),
	vec3(1,0,0),
	vec3(1,0.2,1),
	vec3(1,0.5,0),
	vec3(0,1,0),
	vec3(0.5,0.25,0),
	vec3(0.1,0.1,0.1)
);

vec3 materialBall(int id, vec2 tc){
	
	if(id==15 || length(vec2(tc.x*2,tc.y)-0.5)<0.15) return vec3(1,1,1);
	if(id>=8){
		if(tc.y<0.3 || tc.y > 0.7){
			return vec3(1,1,1);
		}else{
			return colors[id%8];
		}
	}else{
		return colors[id%8];
	}
}



#define PI 3.14159265359
float hash( float n ){
    return fract(sin(n)*43758.5453);
}

float noise( vec3 x ){
    vec3 p = floor(x);
    vec3 f = fract(x);
    f       = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0 + 113.0*p.z;

    return mix(mix(mix( hash(n+0.0), hash(n+1.0),f.x),
                   mix( hash(n+57.0), hash(n+58.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
}
float noise( vec2 x ){
    vec2 p = floor(x);
    vec2 f = fract(x);
    f       = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0;

    return mix(mix( hash(n+0.0), hash(n+1.0),f.x),
               mix( hash(n+57.0), hash(n+58.0),f.x),f.y);
}

float noise( float x ){
    float p = floor(x);
    float f = fract(x);
    f       = f*f*(3.0-2.0*f);
    float n = p;

    return mix( hash(n+0.0), hash(n+1.0),f);
}

vec3 materialMarble(vec3 pos){
	pos*=0.1;
    float power = 1.5;
    float size = 5.0;
    float a = 1.0;
    float mul = 1.0;
    float n = 0.0;
    for(int i=0; i<6;i++){
        n+=a*noise(vec3(size*pos*mul));
        a*=0.5;
        mul*=2.0;
    }
    float c = sin(((pos.x+pos.y)*4.0+power*abs(n))*PI);
    return vec3(abs(c));
}


vec3 materialTable(vec3 pos){
	float f = noise(pos*20);
	return vec3(0.06,0.61,0.18)*(0.7+0.2*f);

	//return vec3(0.06,0.61,0.18);
}

float wood(vec3 pos){
    float power = 1.5;
    float size = 5.0;
    float a = 1.0;
    float mul = 1.0;
    float n = 0.0;
    for(int i=0; i<6;i++){
        n+=a*noise(vec3(size*pos*mul)*vec3(0.01,0.05,0.05));
        a*=0.5;
        mul*=2.0;
    }
	n=abs(n);
	float f = mod(n,0.2);
	f= f/0.2;
	f= abs(0.5-f)*2;
	return f;
}

vec3 materialWood(vec3 pos, vec3 c1, vec3 c2){
	float f = wood(pos);
	return mix(c1,c2,f);
}

vec3 materialLightWood(vec3 pos){
	return materialWood(pos,vec3(0.93,0.89,0.68),vec3(0.67,0.32,0.05));
}

vec3 materialDarkWood(vec3 pos){
	return materialWood(pos,vec3(0.47,0.30,0.30),vec3(0.25,0.25,0.25));
}