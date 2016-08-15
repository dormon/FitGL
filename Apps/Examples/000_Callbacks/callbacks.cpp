#include "callbacks.h"


Callbacks::Callbacks() {
	std::cout << "ctor\n";
}

void Callbacks::init() {
	std::cout << "init\n";
}

void Callbacks::draw() {
	//std::cout << "draw\n";
	glClearColor(1,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Callbacks::onResize(int w, int h){
	std::cout << "onResize " << w << "," << h << "\n";
}

void Callbacks::onMouseMove(int dx, int dy, int x, int y){
	std::cout << "onMouseMove " << dx << "," <<dy <<" " << x<< "," << y << "\n";
}

void Callbacks::onMousePress(Uint8 button, int x, int y){
	std::cout << "onMousePress " << (int)button << " " << x << "," << y << "\n";
}

void Callbacks::onMouseRelease(Uint8 button, int x, int y){
	std::cout << "onMouseRelease " << (int)button << " " << x << "," << y << "\n";
}

void Callbacks::onMouseWheel(int delta){
	std::cout << "onMouseWheel " << delta << "\n";
}

void Callbacks::onKeyPress(SDL_Keycode key, Uint16 mod){
	std::cout << "onKeyPress " << key << " "<< mod << "\n";
}

void Callbacks::onKeyRelease(SDL_Keycode key, Uint16 mod){
	std::cout << "onKeyRelease " << key << " " << mod << "\n";
}

int main(int /*argc*/, char ** /*argv*/) {
	Callbacks app;
	return app.run();
}