#include <Manipulator.h>
#include <BaseApp.h>

#include <algorithm>

#define clamp(v,l,h) std::fmin(std::fmax(v,l),h)

using namespace glm;

void OrbitManipulator::update(float dt) {
	vec3 center = camera->getCenter();
	vec3 eye = camera->getEye();
	vec3 up = camera->getUp();

	vec3 dir = eye - center;
	vec3 a = cross(dir, up);
	vec3 f = normalize(cross(a, up));
	vec3 r = cross(f, up);
	
	vec3 move(0);
	// WSADEQ 
	if (keymap[SDLK_w]) {
		move += f*moveSpeed;
	}
	if (keymap[SDLK_s]) {
		move += -f*moveSpeed;
	}
	if (keymap[SDLK_d]) {
		move += r*moveSpeed;
	}
	if (keymap[SDLK_a]) {
		move += -r*moveSpeed;
	}
	if (keymap[SDLK_e]) {
		move += up*moveSpeed;
	}
	if (keymap[SDLK_q]) {
		move += -up*moveSpeed;
	}
	// middle mouse drag
	if (dragx!= 0 || dragy!= 0) {
		vec3 viewRight = normalize(cross(dir,up));
		vec3 viewUp = normalize(cross(viewRight, dir));
		move +=dragSpeed*( viewUp*float(dragy) + viewRight*float(dragx));
		dragx = 0; dragy = 0;
	}
	center += move*zoom;

	eye =vec3(-zoom, 0, 0);
	eye = rotate(eye, -rotSpeedY* radians(float(movey)), vec3(0, 0, 1));
	eye = rotate(eye, -rotSpeedX* radians(float(movex)), vec3(0, 1, 0));
	eye += center;
	camera->setEye(eye);
	camera->setCenter(center);
}

void OrbitManipulator::setupCallbacks(BaseApp & app) {
	app.addMousePressCallback([&](Uint8 c, int x, int y) {
		mouseButton[c] = true;
		//if (mouseButton[1] || mouseButton[3]) SDL_SetRelativeMouseMode(SDL_TRUE);
	});
	app.addMouseReleaseCallback([&](Uint8 c, int x, int y) {
		mouseButton[c] = false;
		//if (!(mouseButton[1] || mouseButton[3])) SDL_SetRelativeMouseMode(SDL_FALSE);
	});
	app.addMouseMoveCallback([&](int dx, int dy, int x, int y) {
		if (mouseButton[1] || mouseButton[3]) {
			movex += dx;
			movey += dy;
			movey = clamp(movey, -89, 89);
		}
		if (mouseButton[2]) {
			dragx += dx;
			dragy += dy;
		}
	});
	app.addMouseWheelCallback([&](int dy) {
		if (dy < 0) zoom *= 1.1;
		else zoom /= 1.1;
	});
	app.addKeyPressCallback([&](SDL_Keycode key, Uint16) {
		if (key < 256)keymap[key] = true;
	});
	app.addKeyReleaseCallback([&](SDL_Keycode key, Uint16) {
		if (key < 256)keymap[key] = false;
	});

	app.addUpdateCallback([&](float dt) {
		update(dt);
	});
}
