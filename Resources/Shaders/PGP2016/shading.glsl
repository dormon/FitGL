vec3 phong(vec3 pos, vec3 norm, vec3 color,
	vec3 lightPos, vec3 ambientLight, vec3 diffuseLight,
	vec3 camPos, vec3 specLight, float shininess){
	vec3 N = normalize(norm);
	vec3 L = normalize(lightPos-pos);
	vec3 A = color*ambientLight;
	vec3 D = color*diffuseLight*clamp(dot(N,L),0,1);
	vec3 R = reflect(-L,N);
	vec3 V = normalize(camPos-pos);
	vec3 S = shininess>0?specLight*pow(max(0,dot(R,V)),shininess):vec3(0);
	return A+D+S;
}

vec3 lambert(vec3 pos, vec3 norm, vec3 color,
	vec3 lightPos, vec3 ambientLight, vec3 diffuseLight){
	vec3 N = normalize(norm);
	vec3 L = normalize(lightPos-pos);
	vec3 A = color*ambientLight;
	vec3 D = color*diffuseLight*clamp(dot(N,L),0,1);
	return A+D;
}

vec3 normalView(vec3 normal){
	return normal*0.5+0.5;
}

vec3 depthView(float depth){
	return vec3(1.0-depth)*50;
}
