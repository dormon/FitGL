#pragma once
#include "Shared.h"
namespace fgl{
  class BaseApp;
  classShared(Manipulator);
  classShared(OrbitManipulator);
}
#include <Camera.h>
#include <functional>

class fgl::Manipulator {
public:
	Manipulator(CameraW c) :camera(c) {}
	virtual ~Manipulator() {}

	void setCamera(CameraW c) { camera=c; }
  CameraW getCamera() { return camera;}

	virtual void update(float dt) {}
	virtual void setupCallbacks(fgl::BaseApp &app) {}
	
protected:
	CameraW camera;
};


class fgl::OrbitManipulator: public fgl::Manipulator{
public:
	OrbitManipulator(CameraW c=CameraS()) :Manipulator(c) {}
	virtual ~OrbitManipulator() {}

	virtual void update(float dt);
	virtual void setupCallbacks(fgl::BaseApp &app);

	void setZoom(float z) { zoom = z; }
	void setRotationX(float x) { movex = x; }
	void setRotationY(float y) { movey = y; }
protected:
	bool mouseButton[6] = {};
	float movex = 0;
	float movey = 0;
	int dragx = 0;
	int dragy = 0;
	float zoom = 30;
	float rotSpeedX = 1;
	float rotSpeedY = 1;
	float moveSpeed = 0.001f;
	float dragSpeed = 0.001f;
	bool keymap[256] = {};
};