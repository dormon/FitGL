#pragma once

#include <GL/glew.h>

#include <SDLWindow/SDLWindow.h>

#include <cstdlib>
#include <iostream>
#include <string>


/** TODO
X -include SDLWindow
X -event virtual functions
-dynamic event callbacks
X -remove old compile and texture functions
-add imgui
-add fps meter
-multiple windows

*/

typedef std::shared_ptr<SDLWindow> SDLWindowPtr;
typedef std::shared_ptr<SDLEventProc> SDLEventProcPtr;

class BaseApp {
public:
	BaseApp();
	virtual ~BaseApp();

	int run();
	void quit();

protected:
	SDLWindowPtr mainWindow;
	SDLEventProcPtr mainLoop;


	// events
	virtual void init() {}
	virtual void draw() {}

	// events for main window
	virtual void onResize(int /*w*/, int /*h*/) {}
	virtual void onMouseMove(int /*dx*/, int /*dy*/, int /*x*/, int /*y*/) {}
	virtual void onMousePress(Uint8 /*button*/, int /*x*/, int /*y*/) {}
	virtual void onMouseRelease(Uint8 /*button*/, int /*x*/, int /*y*/) {}
	virtual void onMouseWheel(int /*delta*/) {}
	virtual void onKeyPress(SDL_Keycode /*key*/, Uint16 /*mod*/) {}
	virtual void onKeyRelease(SDL_Keycode /*key*/, Uint16 /*mod*/) {}

};
