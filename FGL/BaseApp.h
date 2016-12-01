#pragma once
#include <Shared.h>
namespace fgl{
  classShared(BaseApp);
};

#include <geGLshared.h>

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
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <Gui.h>
#include <Loader.h>
#include <Timer.h>
#include <Camera.h>
#include <Manipulator.h>
#include <Shape.h>
//
#include <utils/utils.h>

#define ANY_EVENT -1
#define ANY_WINDOW -1
typedef std::function<void() > Callback;
typedef std::function<void(float) > UpdateCallback;

class fgl::BaseApp : public ge::gl::Context {
public:
  struct Options {
    int minimumVersion = 430;
    bool vsync = true;
    SDLWindow::Profile profile = SDLWindow::CORE;
    SDLWindow::Flag flags = SDLWindow::Flag::DEBUG;
  };

public:
	BaseApp();
	virtual ~BaseApp();

	int run();
	void quit();
	void waitForPress();
	
	// windows
	SDLWindowS addWindow(SDLWindowS const&window);
  SDLWindowS addWindow(int width = 1024, int height = 768, bool resizable = true, bool fullscreen = false, int multisample = 1);
  SDLWindowS getWindow(int i) { return windows[i]; }
  SDLWindowS getMainWindow() { return mainWindow; }

	// resources
	std::string getResourceDir() { return resourceDir; }
	void setResourceDir(std::string const&s) { resourceDir = s; }

	//
	float getTimeFromStart() { return timer.elapsedFromStart(); }
	float getDt() { return dt; }

	// callbacks
	void addEventCallback(std::function<void(SDL_Event) > callback, int type = ANY_EVENT, int windowID = ANY_WINDOW);
	void addDrawCallback(Callback const&callback, SDLWindowS window=nullptr);
	void addInitCallback(Callback const&callback);
	void addCleanupCallback(Callback const&callback);
	void addUpdateCallback(UpdateCallback const&callback);

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
	virtual void update(float dt) {}
	virtual void draw() {}
	virtual void onResize(int /*w*/, int /*h*/) {}
	virtual void onMouseMove(int /*dx*/, int /*dy*/, int /*x*/, int /*y*/) {}
	virtual void onMousePress(Uint8 /*button*/, int /*x*/, int /*y*/) {}
	virtual void onMouseRelease(Uint8 /*button*/, int /*x*/, int /*y*/) {}
	virtual void onMouseWheel(int /*delta*/) {}
	virtual void onKeyPress(SDL_Keycode /*key*/, Uint16 /*mod*/) {}
	virtual void onKeyRelease(SDL_Keycode /*key*/, Uint16 /*mod*/) {}
	
public:
	static Options options;
protected:
	struct EventCallbackFilter {
		std::function<void(SDL_Event) > callback;
		int type;
		int window;
	};

	struct DrawCallbackFilter {
		Callback callback;
    SDLWindowW window;
	};

  SDLWindowS mainWindow;
	std::vector<SDLWindowS> windows;
	SDLEventProcS mainLoop;
	SDL_GLContext mainContext = nullptr;

	std::vector<EventCallbackFilter> eventCallbacks;
	std::vector<DrawCallbackFilter> drawCallbacks;
	std::vector<Callback> initCallbacks;
	std::vector<Callback> cleanupCallbacks;
	std::vector<UpdateCallback> updateCallbacks;


	void handleEvent(SDL_Event const &e);
	void handleIdle();
	void handleIdleGL();
	void enableDebug();
	void initGL();


	Timer<float> timer;
	float dt;

	std::string resourceDir;
	bool initFailed = false;
};
