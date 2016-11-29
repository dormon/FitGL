#pragma once
#include "Shared.h"
namespace fgl {
  classShared(Camera);
  classShared(PerspectiveCamera);
};

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

class fgl::Camera {
public:
	Camera(glm::vec3 eye = glm::vec3(2, 0, 0), glm::vec3 center = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0));
	glm::mat4 getView() {
		if (dirty)compute();
		return view;
	}
	glm::mat4 getProjection() {
		if (dirty)compute();
		return projection;
	}

	glm::vec3 getEye() {
		if (dirty)compute();
		return eye;
	}
	void setEye(glm::vec3 const&e) {
		eye = e;
		dirty = true;
	}

	glm::vec3 getCenter() {
		if (dirty)compute();
		return center;
	}
	void setCenter(glm::vec3 const&c) {
		center = c;
		dirty = true;
	}
	glm::vec3 getUp() {
		if (dirty)compute();
		return up;
	}
	void setUp(glm::vec3 const&u) {
		up = u;
		dirty = true;
	}

	glm::mat4 getViewProjection() {
		if (dirty)compute();
		return vp;
	}

	glm::mat4 getInverseViewProjection() {
		if (dirty)compute();
		return ivp;
	}

	virtual void compute();
protected:
	glm::mat4 ivp;
	glm::mat4 vp;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 center;
	glm::vec3 eye;
	glm::vec3 up;
	bool dirty = true;
};

class fgl::PerspectiveCamera: public Camera, public std::enable_shared_from_this<fgl::PerspectiveCamera>{
public:
	PerspectiveCamera(float fovy=45, float aspect=(4.0/3.0), float near=0.1, float faar=1000);
	virtual void compute();

	void setAspect(float a) { aspect = a; dirty = true; }
	void setNear(float n) { near = n; dirty = true; }
	void setFar(float f) { faar = f; dirty = true; }
	void setFovy(float f) { fovy = f; dirty = true; }
protected:
	float fovy;
	float aspect;
	float near;
	float faar;
};