#version 430

// maly offset pro zabreneni sebekolize
#define e 0.001

// maximali pocet odrazenych paprsku 
#define maxBounce 100

/* Struktura pro kouli*/
struct Sphere{
	// stred
	vec3 center;
	// polomer
	float radius;
	// barva
	vec3 diffuse;
	// odrazivost (0-1)
	float reflectivity;
};

/* Struktura pro paprsek*/
struct Ray{
	// pocatek
	vec3 origin;
	// smer
	vec3 direction;
};

/* Struktura pro vysledek vypoctu kolize */
struct Collision{
	// Vzdalenost od pocatku paprsku
	float t;
	// pozice pruseciku
	vec3 intersection;
	// normala v bode pruseciku
	vec3 normal;
	// material zasazeneho objektu
	vec3 diffuse;
	float reflectivity;
};


in vec2 tc;

/* Svetlo */
uniform vec3 lightPosition = vec3(200,500,50);
uniform vec3 diffuseLight = vec3(0.8,0.8,0.8);
uniform vec3 ambientLight = vec3(0.2,0.2,0.2);

/* Kamera */
uniform vec3 cameraPosition;
/* Inverzni viewprojekcni matice */
uniform mat4 ivp;

/* textura skyboxu */
layout(binding=0)uniform samplerCube texSkybox;

/* Pocet kouli */
uniform int sphereCount;

/* Buffer s koulemi */
layout(std430, binding = 0) buffer sphereBuffer{
    Sphere spheres[];
};

/* Vystupni barva */
out vec4 fragColor;

/**
* Vypocet pruseciku paprsku s kouli 
* sp - struktura s daty koule
* r - paprsek
* col - vysledek, pri kolizi potreba zapsat vsechny polozky struktury
* return - true pokud kolize nastala
*/
bool collisionRaySphere(Sphere sp, Ray r, out Collision col){
	/**** STUDENT DOPLNI ****/
	/* Vypocet pruseciku paprsku s kouli 
	* napoveda: 
	* kouknete na rovnice koule a rovnice primky
	* pruseciku muze byt vice, berte ten nejblizsi	
	*/

	vec3 oc = r.origin-sp.center;
	float b = 2*dot(r.direction,oc);
	float c = dot(oc,oc)-sp.radius*sp.radius;
	float d = b*b-4*c;

	if(d<0)return false;
	float sd = sqrt(d);
	float t1 = (-b+sd)/2;
	float t2 = (-b-sd)/2;
	
	if(t1<=0&&t2<=0)return false;

	col.t=(t1>0 && t1<t2)?t1:t2;
	col.intersection = r.origin+col.t*r.direction;
	col.normal = normalize(col.intersection-sp.center);
	col.diffuse = sp.diffuse;
	col.reflectivity = sp.reflectivity;

	return true;
}

/* Vypocet kolize paprsku s podlozkou */
bool collisionRayGround(Ray r, float size, float height, out Collision col){
	float t = -(r.origin.y-height)/r.direction.y;
	vec3 i = r.origin+r.direction*t;
	if(t>0 && i.x>-size && i.z>-size  && i.x<size && i.z<size){
		col.t=t;
		col.intersection = i;
		col.diffuse = vec3(1,1,1)*0.8;
		col.reflectivity = 0.4;
		col.normal = r.origin.y>height?vec3(0,1,0):vec3(0,-1,0);
		return true;
	}else{
		return false;
	}
}

/**
* Vypocet nejblizsi kolize paprku se scenou 
* r - paprsek
* closest - vystupni promenna pro zapis nejblizsi kolize
* return - true pokud kolize nastala
*/
bool closestCollision(Ray r, out Collision closest){
	// priznak kolize
	bool collided = false;

	// promena pro kolizi
	Collision col;

	/* Kolize s podlozkou*/
	if(collisionRayGround(r,20,-10,col)){
		closest = col;
		collided=true;
	}

	/**** STUDENT DOPLNI ****/
	/* Vypocet nejblizsi kolize se vsemi koulemi
	*  Koule jsou ulezene v bufferu
	*/
	for(int i=0;i<sphereCount;i++){
		if(collisionRaySphere(spheres[i],r,col)){
			if(!collided|| col.t<closest.t){			
				collided = true;	
				closest = col;
			}
		}
	}

	return collided;
}

/* Vypocet barvy paprsku */
vec4 rayTrace(Ray r){
	// akumulator barvy
	vec3 rayColor=vec3(0);
	// koeficient odrazivosti
	float coef = 1;
	
	// vysledek kolize
	Collision col;

	// paprsek
	Ray ray = r;

	// pocitadlo odrazu
	int bounces=maxBounce;
	while(bounces>0 && coef>0.1){		
		if(closestCollision(ray,col)){
			// paprsek neco zasahl

			// smer ke svetlu
			vec3 lightDir = normalize(lightPosition-col.intersection);

			// normala
			vec3 normal = col.normal;

			// difuzni barva 
			vec3 color = coef*col.diffuse*(1-col.reflectivity);		
			// koeficient pro odrazivost
			coef*=col.reflectivity;

			// odrazeny paprsek
			ray = Ray(col.intersection,reflect(ray.direction,col.normal));
			ray.origin+=e*ray.direction;
			
			// paprsek ke svetlu
			Ray lightRay= Ray(col.intersection,lightDir);
			lightRay.origin+=e*lightRay.direction;

			// vypocet stinu
			bool shadow = closestCollision(lightRay,col);
			// pridani diffuzni barvy
			rayColor += ambientLight*color+(shadow?0:1)*max(0,dot(lightDir,normal))*color*diffuseLight;

			// snizeni poctu zbyvajicich odrazu
			bounces--;
		}else{
			// paprsek nic neprotne, vykreslime skybox, konec
			rayColor += texture(texSkybox,ray.direction).xyz*coef;
			bounces=0;
		}
	
	}

	return vec4(rayColor,1);
}


void main(){
	// vypocet smeru paprsku
	vec4 invEnd = ivp*vec4(tc*2-1,0,1);

	vec3 rayStart = cameraPosition;
	vec3 rayDir =  normalize(invEnd.xyz);

	fragColor = rayTrace(Ray(rayStart,rayDir));
}
