#version 430

#define PI 3.1415926535897932384626433832795


/**** STUDENT DOPLNI ****/
/* Prepnuti na teselaci quadu*/
layout(quads,equal_spacing)in;
//layout(triangles,equal_spacing)in;

// normaly, pro kouli neni potreba
in vec3 eNormal[];

/**** STUDENT DOPLNI ****/
/* Dve vstupni promenne pro stred a radius kruznice vepsane*/
patch in vec3 center;
patch in float radius;

/* Matice */
uniform mat4 v;
uniform mat4 p;
uniform mat4 m;

/* Prepinac zapnuti animace - klavesa A */
uniform bool animationEnabled;
/* Cas animace v ms*/
uniform int time;

/* Vystupni promenne */
out vec3 fPosition;
out vec3 fNormal;
out vec3 color;


void main(){
	
	/**** STUDENT DOPLNI ****/
	/* Veskery nasledujici kod nahradte za vlastni */
	/* Je potreba zapsat na vsechny vystupni promenne (gl_Position,fPosition,fNormal,color)
	   Vyuzijte gl_TessCoord k vypoctu pozice a normaly vrcholu koule
	   Klavesou D prepnete na zobrazeni pouze jednoho trojuhelniku
	   Klavesou W prepnete na zobrazeni dratoveho modelu
	*/



	/**** BONUS ZA NULA BODU ****/
	/* Pokud vas ukol bavi a mate cas */
	/* Vytvrte jednoduchou animaci dle vlastniho uvazeni */



	/* Pozice a normala ziskana z kontrolnich bodu, pro kouli nebude potreba */
    vec3 pA = gl_in[0].gl_Position.xyz;
    vec3 pB = gl_in[1].gl_Position.xyz;
    vec3 pC = gl_in[2].gl_Position.xyz;

	vec3 nA = eNormal[0];
    vec3 nB = eNormal[1];
    vec3 nC = eNormal[2];

#if 0
	/* Interpolace trojuhelnika - barycentricke souradnice */
	fPosition = pA*gl_TessCoord.x+pB*gl_TessCoord.y+pC*gl_TessCoord.z;
	fNormal = nA*gl_TessCoord.x+nB*gl_TessCoord.y+nC*gl_TessCoord.z;
	color = vec3(1,0,0);
    gl_Position=p*v*m*vec4(fPosition,1);
#endif

#if 0
	/* Interpolace quadu - kartezke souradnice */
	vec3 a = pB-pA;
	vec3 b = pC-pA;
	fPosition = pA+a*gl_TessCoord.x+b*gl_TessCoord.y;
	fNormal = -cross(a,b);
	color = vec3(1,0,0);
	gl_Position=p*v*m*vec4(fPosition,1);
#endif	

#if 1
	// animace kralik
	float coef;
	float radiusCoef =max(1/4.0,0.02/radius);

	int timemod = time%8000;
	if(timemod<4000)
		coef=min(1,timemod/1000.0*radiusCoef);
	else
		coef=1-min(1,(timemod-4000)/1000.0*radiusCoef);

	if(!animationEnabled)coef=0;
	float coef2 = abs(coef-0.5)*2;
	coef2*=coef2*coef2;

	vec3 offset = vec3(0,0,5)*coef;
	if(animationEnabled)offset-=vec3(0,0,2.5);

	vec3 center2 = center+offset;
		center2.xy*=coef2;
	
	float radx = gl_TessCoord.x*PI*2;
	float rady = gl_TessCoord.y*PI;
	float x =  cos(radx)*sin(rady);
	float y =  cos(rady);
 	float z =  sin(radx)*sin(rady);

	fPosition = center2+ radius*vec3(x,y,z);
	fNormal = vec3(x,y,z);

	color = coef2*vec3(1,1,0)+(1-coef2)*vec3(1,0,0);
    gl_Position=p*v*m*vec4(fPosition,1);
#endif

#if 0
	// animace - tvorba koule
	
	float cc = clamp((time-4000.0)/5000.0,0.001,1.0);
	float l = 0.5-cc/2;
	float h = 0.5+cc/2;
	float d = h-l;

	float xscale = d<0.5?sin(d*PI):1;
	float yscale = cos(h*PI);

	float radx = (l+d*gl_TessCoord.x)*PI*2;
	float rady = (l+d*gl_TessCoord.y)*PI;

	float x =  cos(radx)*sin(rady);
	float y =  cos(rady)/yscale;
 	float z =  sin(radx)*sin(rady)/xscale;

	fPosition = center+ radius*vec3(y,x,z);
	fNormal = vec3(x,y,z);

	color = vec3(1,0,0);
	gl_Position=p*v*m*vec4(fPosition,1);
#endif 

}
