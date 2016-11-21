#include "callbacks.h"


Callbacks::Callbacks() {
	std::cout << "ctor\n";
}

void Callbacks::init() {
	std::cout << "init\n";
	
	window2 = addWindow(200,200);
	window2->setTitle("Window #2");
	window2->move(1000, 50);
	addMouseMoveCallback([](int dx, int dy, int y, int x) {
		std::cout << "Second window mous muf "<< x<<","<<y <<"\n";
	}, window2->getId());
		
	addDrawCallback([&]() {
		glViewport(0, 0, 100, 100);
		glClearColor(0, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}, window2);


	addMousePressCallback([](char b, int y, int x) {
		std::cout << "Second window mous pres1 " <<int(b)<<" "<< x << "," << y << "\n";
	}, window2->getId());

	addMousePressCallback([](char b, int y, int x) {
		std::cout << "Second window mous pres2 " << int(b) << " " << x << "," << y << "\n";
	}, window2->getId());

	addMousePressCallback([](char b, int y, int x) {
		std::cout << "Second window mous pres3 " << int(b) << " " << x << "," << y << "\n";
	}, window2->getId());

	
	addMouseMoveCallback([](int dx, int dy, int y, int x) {
		std::cout << "MainWindow mous muf2 " << x << "," << y << "\n";
	}, mainWindow->getId());

}

void Callbacks::draw() {
	glClearColor(1,1,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Callbacks::onResize(int w, int h){
	std::cout << "MainWindow onResize " << w << "," << h << "\n";
}

void Callbacks::onMouseMove(int dx, int dy, int x, int y){
	std::cout << "MainWindow onMouseMove " << dx << "," <<dy <<" " << x<< "," << y << "\n";
}

void Callbacks::onMousePress(Uint8 button, int x, int y){
	std::cout << "MainWindow onMousePress " << (int)button << " " << x << "," << y << "\n";
}

void Callbacks::onMouseRelease(Uint8 button, int x, int y){
	std::cout << "MainWindow onMouseRelease " << (int)button << " " << x << "," << y << "\n";
}

void Callbacks::onMouseWheel(int delta){
	std::cout << "MainWindow onMouseWheel " << delta << "\n";
}

void Callbacks::onKeyPress(SDL_Keycode key, Uint16 mod){
	std::cout << "MainWindow onKeyPress " << key << " "<< mod << "\n";
}

void Callbacks::onKeyRelease(SDL_Keycode key, Uint16 mod){
	std::cout << "MainWindow onKeyRelease " << key << " " << mod << "\n";
}

int main(int /*argc*/, char ** /*argv*/) {
	Callbacks app;
	return app.run();
}