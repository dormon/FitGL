#pragma once

#include <SDL2/SDL.h>
#include "GL/glew.h"

#include <cstdlib>
#include <iostream>
#include <string>

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

	void initGL();
	int width;
	int height;
	bool resizable;
	bool fullscreen;	
	int multisample;
	SDL_Window* window;
	SDL_GLContext context;
};
