#include <Camera.h>
#include <iostream>
PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float faar):
fovy(fovy),aspect(aspect),near(near), faar(faar){

}

void PerspectiveCamera::compute(){
	projection = glm::perspective(glm::radians(fovy), aspect, near, faar);
	Camera::compute();
}

Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up):
	eye(eye),center(center),up(up)
{
}

void Camera::compute(){
	view = glm::lookAt(eye, center, up);
	vp = view*projection;
	ivp = glm::inverse(vp);
}
