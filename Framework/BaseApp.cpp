#include "BaseApp.h"

BaseApp::BaseApp() {
	mainLoop = std::make_shared<SDLEventProc>(true); 

	mainWindow = std::make_shared<SDLWindow>();
	mainWindow->createContext("context", 450, SDLWindow::CORE, SDLWindow::Flag::DEBUG);

	glewExperimental = GL_TRUE;
	glewInit();


	mainLoop->addWindow("window", mainWindow);
	mainLoop->setIdleCallback(std::make_shared<CallbackInterface>(
		[&]() {
		//std::cout << "MainLoop::idle\n";
		draw();
		mainWindow->swap();
	}));
	
	mainLoop->setEventHandler(
		std::make_shared<EventHandlerInterface>(
			[&](EventDataPointer ptr)->bool {
		//std::cout << "MainLoop::event\n";

		auto e =(*static_cast<SDLEventData const*> (ptr)).event;

		switch (e.type) {
		case SDL_QUIT:
			quit();
			break;
		case SDL_KEYDOWN:
			onKeyPress(e.key.keysym.sym, e.key.keysym.mod);
			break;
		case SDL_KEYUP:
			onKeyRelease(e.key.keysym.sym, e.key.keysym.mod);
			break;
		case SDL_MOUSEMOTION:
			onMouseMove(e.motion.xrel, e.motion.yrel, e.motion.x, e.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMousePress(e.button.button, e.button.x, e.button.y);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseRelease(e.button.button, e.button.x, e.button.y);
			break;
		case SDL_MOUSEWHEEL:
			onMouseWheel(e.wheel.y);
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				onResize(e.window.data1, e.window.data2);
				break;
			}
			break;
		}

		return false; }));
	
}

BaseApp::~BaseApp() {
}

int BaseApp::run() {
	init();
	(*mainLoop)();
	return 0;
}

void BaseApp::quit(){
	mainLoop->quit();
}
