#pragma once

#include <GL/glew.h>
#include <SDLWindow/SDLWindow.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <functional>
#include <vector>

// 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ProgramObject.h>
#include <Shader.h>

/** @TODO
X -include SDLWindow
X -event virtual functions
X -dynamic event callbacks
X -remove old compile and texture functions
-delete more stuff from SDLWindow nad SDLEventProc :)
-add imgui
-add fps meter
X -multiple windows
-add remove functions
-fix closing windows

 */

typedef std::shared_ptr<SDLWindow> SDLWindowShared;
typedef std::weak_ptr<SDLWindow> SDLWindowWeak;
typedef std::shared_ptr<SDLEventProc> SDLEventProcShared;
#define ANY_EVENT -1
#define ANY_WINDOW -1
typedef std::function<void() > Callback;

class BaseApp {
public:
	BaseApp();
	virtual ~BaseApp();

	int run();
	void quit();

	// windows
	SDLWindowShared addWindow(SDLWindowShared const&window);
	SDLWindowShared addWindow(int width = 1024, int height = 768, bool resizable = true, bool fullscreen = false, int multisample = 1);
	SDLWindowShared getWindow(int i) { return windows[i]; }
	SDLWindowShared getMainWindow() { return mainWindow; }

	void addInitCallback(Callback const&callback);
	void enableDebug();

	//events
	void addEventCallback(std::function<void(SDL_Event) > callback, int type = ANY_EVENT, int windowID = ANY_WINDOW);
	void addDrawCallback(Callback const&callback, SDLWindowShared window=nullptr);
	// specific event callbacks
	void addResizeCallback(std::function<void(int /*w*/, int /*h*/) >f, int window = ANY_WINDOW);
	void addMouseMoveCallback(std::function<void(int /*dx*/, int /*dy*/, int /*x*/, int /*y*/) >f, int window = ANY_WINDOW);
	void addMousePressCallback(std::function<void(Uint8 /*button*/, int /*x*/, int /*y*/) >f, int window = ANY_WINDOW);
	void addMouseReleaseCallback(std::function<void(Uint8 /*button*/, int /*x*/, int /*y*/) >f, int window = ANY_WINDOW);
	void addMouseWheelCallback(std::function<void(int /*delta*/) >f, int window = ANY_WINDOW);
	void addKeyPressCallback(std::function<void(SDL_Keycode /*key*/, Uint16 /*mod*/) > f, int window = ANY_WINDOW);
	void addKeyReleaseCallback(std::function<void(SDL_Keycode /*key*/, Uint16 /*mod*/) > f, int window = ANY_WINDOW);

	// events for main window
	void setupMainWindowEvents();
	virtual void init() {}
	virtual void draw() {}
	virtual void onResize(int /*w*/, int /*h*/) {}
	virtual void onMouseMove(int /*dx*/, int /*dy*/, int /*x*/, int /*y*/) {}
	virtual void onMousePress(Uint8 /*button*/, int /*x*/, int /*y*/) {}
	virtual void onMouseRelease(Uint8 /*button*/, int /*x*/, int /*y*/) {}
	virtual void onMouseWheel(int /*delta*/) {}
	virtual void onKeyPress(SDL_Keycode /*key*/, Uint16 /*mod*/) {}
	virtual void onKeyRelease(SDL_Keycode /*key*/, Uint16 /*mod*/) {}

protected:
	struct EventCallbackFilter {
		std::function<void(SDL_Event) > callback;
		int type;
		int window;
	};

	struct DrawCallbackFilter {
		Callback callback;
		SDLWindowWeak window;
	};

	SDLWindowShared mainWindow;
	std::vector<SDLWindowShared> windows;
	SDLEventProcShared mainLoop;
	SDL_GLContext mainContext;

	std::vector<EventCallbackFilter> eventCallbacks;
	std::vector<DrawCallbackFilter> drawCallbacks;
	std::vector<Callback> initCallbacks;


	void handleEvent(SDL_Event const &e);
	void handleIdle();
};
