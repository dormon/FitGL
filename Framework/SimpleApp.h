#pragma once

#include <SDL2/SDL.h>
#include "GL/glew.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SimpleApp {
public:
	SimpleApp();
	virtual ~SimpleApp();

	int run();
	void quit();

protected:
	// events
	virtual void init(){}
	virtual void draw(){}
	virtual void onResize(int w, int h) {}

	virtual void onMouseMove(int dx, int dy, int x, int y) {}
	virtual void onMousePress(Uint8 button, int x, int y) {}
	virtual void onMouseRelease(Uint8 button, int x, int y) {}
	virtual void onMouseWheel(int delta) {}
	virtual void onKeyPress(SDL_Keycode key, Uint16 mod) {}
	virtual void onKeyRelease(SDL_Keycode key, Uint16 mod) {}
	
	// utils
	static void texImageFileBMP(GLuint target, GLuint level, GLuint internalFormat, std::string file);
	static GLuint textureFromBMP(std::string file);

	static std::string loadFile(const std::string file);
	static std::string loadFile(const char * const file);
	static std::string getShaderInfo(GLuint shader);
	static std::string getProgramInfo(GLuint shader);

	static GLuint compileShader(const GLenum type, std::string src);
	static GLuint linkShader(int count, ...);

	// orbit manipulator
	struct OrbitCamera {
		float zoom = 10;
		int posx=0, posy=0;
		int rotx=0, roty=0;
		float far = 1000;
		float near = 0.1;
		bool pressed = false;
		int width, height;
		Uint8 button = SDL_BUTTON_LEFT;
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec3 center;
		glm::vec3 eye;
		glm::vec3 up;
		void resize(int w,int h);
		void update();
		void wheel(int d);
		void press(Uint8 b);
		void release(Uint8 b);
		void move(int dx, int dy, int x, int y);
	} camera;




	void initGL();
	int width = 1024;
	int height = 768;
	bool resizable =true;
	bool fullscreen =false;	
	int multisample = 1;
	int frames = 0;
	int fpsTimer=0;

	SDL_Window* window;
	SDL_GLContext context;
};
