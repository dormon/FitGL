layout(location=0)out vec4 fColor;

uniform mat4 v;
uniform mat4 p;


layout(binding=0)uniform samplerCube cubeTexture;

struct Sphere{
  vec4 positionRadius;
  vec4 color;
};

layout(std430,binding=0)buffer Spheres{Sphere spheres[];};
uniform uint nofSpheres = 0;

in vec2 vCoord;

struct Ray{
  vec3 start;
  vec3 direction;
};

float sphereCollision(out Ray outRay,in Ray inRay,in Sphere sphere){
  //outRay = inRay;
  vec3 ps = inRay.start - sphere.positionRadius.xyz;
  float a = dot(inRay.direction,inRay.direction);
  float b = 2*dot(inRay.direction,ps);
  float c = dot(ps,ps)-sphere.positionRadius.w*sphere.positionRadius.w;
  float d = b*b-4*a*c;
  if(d<0)return -1;
  float t0 = (-b - sqrt(d))/2/a;
  float t1 = (-b + sqrt(d))/2/a;
  if(t0<=0 && t1<=0)return -1;
  float t;
  if(t0<0)t = t1;
  else if(t1<0)t = t0;
  else t = min(t0,t1);
  vec3 pos = inRay.start+inRay.direction*t;
  outRay.start = pos;
  outRay.direction = normalize(reflect(inRay.direction,normalize(pos-sphere.positionRadius.xyz)));
  return t;
}

float planeCollision(out Ray outRay,in Ray inRay,in vec4 plane){
  float t = (-plane.w -dot(inRay.start,plane.xyz))/dot(inRay.direction,plane.xyz);
  if(t<0)return -1;
  outRay.start = inRay.start+inRay.direction*t;
  outRay.direction = normalize(reflect(inRay.direction,normalize(plane.xyz)));
}

uint spheresCollision(out Ray outRay,in Ray inRay,uint skipSphere){
  //outRay = inRay;
  float minT = 10e10;
  uint sphereId = nofSpheres;
  for(uint k=0;k<nofSpheres;++k){
    if(k == skipSphere)continue;
    Ray ray;
    float t = sphereCollision(ray,inRay,spheres[k]);
    if(t<0)continue;
    if(t<minT){
      outRay = ray;
      minT = t;
      sphereId = k;
    }
  }
  return sphereId;
}

void main(){
  
  Ray ray;
  ray.start = (inverse(v)*vec4(0,0,0,1)).xyz;
  vec3 viewSpace = (inverse(p)*vec4(vCoord,1,1)).xyz;
  ray.direction = normalize((inverse(v)*vec4(viewSpace,0)).xyz);

  /*
  Sphere sphere = spheres[0];
  Ray newRay;
  float t;
  t = sphereCollision(newRay,ray,sphere);
  if(t>0){
    fColor = texture(cubeTexture,newRay.direction);
    return;
  }
  */
  Ray newRay;

  const uint rayDepth = 3;
  uint i=0;
  uint skipSphere = nofSpheres;
  for(i=0;i<rayDepth;++i){
    skipSphere=spheresCollision(newRay,ray,skipSphere);
    if(skipSphere == nofSpheres)break;
    ray = newRay;
  }
//  fColor = vec4(i/10.f);
  fColor = texture(cubeTexture,ray.direction);
  /*
  float t=spheresCollision(newRay,ray);
  if(t>0){
    fColor = texture(cubeTexture,newRay.direction);
    return;
  }

  fColor = texture(cubeTexture,ray.direction);
  */
  /*
  t = planeCollision(newRay,ray,vec4(0,1,0,0));
  if(t<0){
    fColor = texture(cubeTexture,ray.direction);
    return;
  }*/

  //fColor = vec4((mod(newRay.start.x,1)>0.5)!=(mod(newRay.start.z,1)>0.5));
//  fColor = texture(cubeTexture,newRay.direction);
}
