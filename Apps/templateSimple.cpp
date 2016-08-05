#include "APP_NAME.h"

#include <algorithm>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "bunny.h"

using namespace std;

APP_NAME::APP_NAME(){
	rmbPressed = false;
	lmbPressed = false;
	posx = 0, posy = 0;
	rotx = 170, roty = 25, zoom = 4;
	
	animationEnabled = false;
	time = 0;
	lastFrameTics = 0;
	wireframe = false;
	debug = false;
	fullscreen = false;
}

void APP_NAME::init() {
	string prefix = "../../Resources/Shaders/PGP2015/";

	auto vs = compileShader(GL_VERTEX_SHADER, loadFile(prefix + "APP_NAME.vert"));
	auto fs = compileShader(GL_FRAGMENT_SHADER, loadFile(prefix + "APP_NAME.frag"));
	program = linkShader(2, vs, fs);

	mUniform = glGetUniformLocation(program, "m");
	vUniform = glGetUniformLocation(program, "v");
	pUniform = glGetUniformLocation(program, "p");
	animationEnabledUniform = glGetUniformLocation(program, "animationEnabled");
	timeUniform = glGetUniformLocation(program, "time");  	
}


void APP_NAME::draw() {
	/* Aktualizace casu animace - klavesa A*/
	Uint32 tics = SDL_GetTicks();
	Uint32 dt = tics - lastFrameTics;
	lastFrameTics = tics;
	if (animationEnabled) {
		time += dt;
	}

	/* Matice */
	updateMatrix();

	glViewport(0, 0, width, height);

	glClearColor(0.68, 0.88, 0.93, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	glUseProgram(program);

	glUniformMatrix4fv(mUniform, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(vUniform, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, glm::value_ptr(projection));

	glUniform1i(timeUniform, time);
	glUniform1i(animationEnabledUniform, animationEnabled);

	/* do stuff */
}

void APP_NAME::onMouseMove(int dx, int dy, int x, int y){
	if (rmbPressed||lmbPressed) {
		rotx += dx;
		roty += dy;
		roty = max(min(roty, 89), -89);
	}
	posx = x;
	posy = y;
}

void APP_NAME::onMousePress(Uint8 button, int x, int y){
	switch (button) {
	case SDL_BUTTON_LEFT:
		lmbPressed = true;
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	case SDL_BUTTON_RIGHT:
		rmbPressed = true;
		break;
	}
}

void APP_NAME::onMouseRelease(Uint8 button, int x, int y){
	switch (button) {
	case SDL_BUTTON_LEFT:
		lmbPressed = false;
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	case SDL_BUTTON_RIGHT:
		rmbPressed = false;
		break;
	}
}

void APP_NAME::onMouseWheel(int delta){
	if (delta > 0) {
		zoom /= 1.1;
	}else {
		zoom *= 1.1;
	}
}

void APP_NAME::onKeyPress(SDL_Keycode key, Uint16 mod){
	switch (key) {
	case SDLK_ESCAPE:
		quit();
	case SDLK_a:
		animationEnabled = !animationEnabled;
		if (animationEnabled) time = 0;
		break;
	case SDLK_d:
		debug = !debug;
		break;
	case SDLK_w:
		wireframe = !wireframe;
		break;
	}
}

void APP_NAME::updateMatrix() {
	model = glm::mat4(1.0);

	float radx = glm::radians((float)rotx);
	float rady = glm::radians((float)roty);
	float x = zoom * cos(rady) * cos(radx);
	float y = zoom * sin(rady);
	float z = zoom * cos(rady) * sin(radx);

	glm::vec3 eye(x, y, z);
	glm::vec3 center(0, 0, 0);
	glm::vec3 up(0, 1, 0);
	view = glm::lookAt(eye, center, up);
	projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
}

int main(int /*argc*/, char ** /*argv*/){
	APP_NAME app;
	return app.run();
}