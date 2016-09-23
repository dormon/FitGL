#include "BaseApp.h"
#include <Debugging.h>
BaseApp::BaseApp() {
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
	
	mainLoop = std::make_shared<SDLEventProc>(true);

	mainWindow = addWindow();

	mainContext = mainWindow->createContext("context", 450, SDLWindow::CORE, SDLWindow::Flag::DEBUG,true);
	
	glewExperimental = GL_TRUE;
	glewInit();

	mainLoop->setIdleCallback(std::bind(&BaseApp::handleIdle, this));

	mainLoop->setEventHandler([this](SDL_Event const&e)->bool {
		handleEvent(e);
		return true;
	});

	setupMainWindowEvents();
	enableDebug();
	//SDL_CaptureMouse(SDL_TRUE);
}

BaseApp::~BaseApp() {
}

int BaseApp::run() {
	try {
		for (auto &c : initCallbacks)c();	
		(*mainLoop)();
		for (auto &c : cleanupCallbacks)c();
	}catch (const std::exception &e) {		
		std::cerr << e.what() << "\n";
#ifdef WIN32
		system("pause");
#else
		std::cout << "Press ENTER to continue...\n";
		std::cin.get();
#endif
		
	}catch (...) {
		std::cout << "Unknown exception\n";
	}
	
	return 0;
}

void BaseApp::quit() {
	mainLoop->quit();
}

SDLWindowShared BaseApp::addWindow(SDLWindowShared const & window) {
	windows.push_back(window);
	mainLoop->addWindow(std::to_string(window->getId()), window);
	int w = window->getWidth();
	int h = window->getHeight();
	window->setSize(w, h);
	return window;
}

SDLWindowShared BaseApp::addWindow(int width, int height, bool resizable, bool fullscreen, int multisample) {
	return addWindow(std::make_shared<SDLWindow>(width, height, resizable, fullscreen, multisample));
}

void BaseApp::handleEvent(SDL_Event const & e) {
	for (auto &ec : eventCallbacks) {
		if ((ec.type == ANY_EVENT || ec.type == e.type) &&
			(ec.window == ANY_WINDOW || ec.window == e.window.windowID)) {
			ec.callback(e);
		}
	}
}

void BaseApp::handleIdle() {
	dt = timer.elapsedFromLast();
	for (auto&c : updateCallbacks)c(dt);

	for (auto &w : windows) {
		w->makeCurrent(mainContext);
		ImGui_ImplSdlGL3_NewFrame(w->getWindowHandle());
		for (auto &dc : drawCallbacks) {
			auto dcw = dc.window.lock();
			if (dcw == w) dc.callback();
		}
		ImGui::Render();
		w->swap();
	}
}

void BaseApp::addInitCallback(Callback const & callback) {
	initCallbacks.push_back(callback);
}

void BaseApp::addCleanupCallback(Callback const & callback){
	cleanupCallbacks.push_back(callback);
}

void BaseApp::addUpdateCallback(UpdateCallback const & callback){
	updateCallbacks.push_back(callback);
}

void BaseApp::enableDebug() {
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, NULL, GL_FALSE);
	glDebugMessageCallback((GLDEBUGPROC)(defaultDebugMessage), NULL);
}

void BaseApp::addEventCallback(std::function<void(SDL_Event) > callback, int type, int window) {
	EventCallbackFilter ec = { callback, type, window };
	eventCallbacks.push_back(ec);
}

void BaseApp::addDrawCallback(Callback const & callback, SDLWindowShared window) {
	if (window == nullptr)window = mainWindow;
	DrawCallbackFilter dc = { callback, SDLWindowWeak(window) };
	drawCallbacks.push_back(dc);
}

void BaseApp::addResizeCallback(std::function<void(int, int) >f, int window) {
	addEventCallback([f](SDL_Event const&e) {
		if (e.window.event == /*SDL_WINDOWEVENT_RESIZED*/SDL_WINDOWEVENT_SIZE_CHANGED) {
			f(e.window.data1, e.window.data2);
		}
	}, SDL_WINDOWEVENT, window);
}

void BaseApp::addMouseMoveCallback(std::function<void(int, int, int, int) >f, int window) {
	addEventCallback([f](SDL_Event const&e) {
		f(e.motion.xrel, e.motion.yrel, e.motion.x, e.motion.y);
	}, SDL_MOUSEMOTION, window);
}

void BaseApp::addMousePressCallback(std::function<void(Uint8, int, int) >f, int window) {
	addEventCallback([f](SDL_Event const&e) {
		f(e.button.button, e.button.x, e.button.y);
	}, SDL_MOUSEBUTTONDOWN, window);
}

void BaseApp::addMouseReleaseCallback(std::function<void(Uint8, int, int) >f, int window) {
	addEventCallback([f](SDL_Event const&e) {
		f(e.button.button, e.button.x, e.button.y);
	}, SDL_MOUSEBUTTONUP, window);
}

void BaseApp::addMouseWheelCallback(std::function<void(int) > f, int window) {
	addEventCallback([f](SDL_Event const&e) {
		f(e.wheel.y);
	}, SDL_MOUSEWHEEL, window);
}

void BaseApp::addKeyPressCallback(std::function<void(SDL_Keycode, Uint16) > f, int window) {
	addEventCallback([f](SDL_Event const&e) {
		f(e.key.keysym.sym, e.key.keysym.mod);
	}, SDL_KEYDOWN, window);
}

void BaseApp::addKeyReleaseCallback(std::function<void(SDL_Keycode, Uint16) > f, int window) {
	addEventCallback([f](SDL_Event const&e) {
		f(e.key.keysym.sym, e.key.keysym.mod);
	}, SDL_KEYUP, window);
}

void BaseApp::setupMainWindowEvents() {
	using namespace std::placeholders;
	// init
	addInitCallback(std::bind(&BaseApp::init, this));
	addUpdateCallback(std::bind(&BaseApp::update, this, _1));
	// virtual function callbacks
	addResizeCallback(std::bind(&BaseApp::onResize, this, _1, _2), mainWindow->getId());
	addMouseMoveCallback(std::bind(&BaseApp::onMouseMove, this, _1, _2, _3, _4), mainWindow->getId());
	addMousePressCallback(std::bind(&BaseApp::onMousePress, this, _1, _2, _3), mainWindow->getId());
	addMouseReleaseCallback(std::bind(&BaseApp::onMouseRelease, this, _1, _2, _3), mainWindow->getId());
	addMouseWheelCallback(std::bind(&BaseApp::onMouseWheel, this, _1), mainWindow->getId());
	addKeyPressCallback(std::bind(&BaseApp::onKeyPress, this, _1, _2), mainWindow->getId());
	addKeyReleaseCallback(std::bind(&BaseApp::onKeyRelease, this, _1, _2), mainWindow->getId());
	// close on ESC
	addKeyPressCallback([this](int key, int) {
		if (key == SDLK_ESCAPE)quit();
	});
	// close on window X
	addEventCallback([this](SDL_Event const&e) {
		quit(); }, SDL_QUIT);
	// draw
	addDrawCallback(std::bind(&BaseApp::draw, this), mainWindow);
	//gui
	addInitCallback([&](){
		ImGui_ImplSdlGL3_Init(mainWindow->getWindowHandle()); 
	});
	addCleanupCallback([]() {
		ImGui_ImplSdlGL3_Shutdown();
	});
}

