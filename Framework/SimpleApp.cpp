#include "SimpleApp.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <stdexcept>

using namespace std;

SimpleApp::SimpleApp() {
}

SimpleApp::~SimpleApp() {
}

int SimpleApp::run() {
	bool initOK = false;
	try {
		initGL();
		init();
		camera.resize(width, height);
		initOK = true;
	}catch (const exception &e) {
		cerr <<e.what();
	}
	try {
		SDL_Event e;
		bool quit = false;
		while (!quit) {

			while (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					onKeyPress(e.key.keysym.sym, e.key.keysym.mod);
					break;
				case SDL_KEYUP:
					onKeyRelease(e.key.keysym.sym, e.key.keysym.mod);
					break;
				case SDL_MOUSEMOTION:
					camera.move(e.motion.xrel, e.motion.yrel, e.motion.x, e.motion.y);
					onMouseMove(e.motion.xrel, e.motion.yrel, e.motion.x, e.motion.y);
					break;
				case SDL_MOUSEBUTTONDOWN:
					camera.press(e.button.button);
					onMousePress(e.button.button, e.button.x, e.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					camera.release(e.button.button);
					onMouseRelease(e.button.button, e.button.x, e.button.y);
					break;
				case SDL_MOUSEWHEEL:
					camera.wheel(e.wheel.y);
					onMouseWheel(e.wheel.y);
					break;

				case SDL_WINDOWEVENT:
					switch (e.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						width = e.window.data1;
						height = e.window.data2;
						onResize(width, height);
						camera.resize(width, height);
						break;

					}
					break;
				}
			}


			if (initOK)draw();
			frames++;
			int now = SDL_GetTicks();
			int dif = now - fpsTimer;
			if (dif > 1000) {
				float fps = 1000.0*float(frames) / dif;
				string title = "FPS: " + std::to_string(fps);
				SDL_SetWindowTitle(window, title.c_str());
				fpsTimer = now;
				frames = 0;
			}
			SDL_GL_SwapWindow(window);
		}



	}
	catch (string s) {
		cerr << s;
		return -1;
	}
	catch (const exception &e) {
		cerr << "Fatal Exception " << e.what() << endl;
		return -1;
	}

	return 0;
}

void SimpleApp::quit() {
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}

void SimpleApp::texImageFileBMP(GLuint target, GLuint level, GLuint internalFormat, std::string file) {
	SDL_Surface* surface = SDL_LoadBMP(file.c_str());
	if (!surface) throw "Failed to load texture " + file + " error:" + SDL_GetError() + "\n";
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	if ((surface->format->Rmask == 0xff0000) &&
		(surface->format->Gmask == 0xff00) &&
		(surface->format->Bmask == 0xff) &&
		(surface->format->Amask == 0))
	{
		glTexImage2D(target, level, internalFormat, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
	}
	else if ((surface->format->Rmask == 0xff) &&
		(surface->format->Gmask == 0xff00) &&
		(surface->format->Bmask == 0xff0000) &&
		(surface->format->Amask == 0))
	{
		glTexImage2D(target, level, internalFormat, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	}
}

GLuint SimpleApp::textureFromBMP(std::string file) {
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	texImageFileBMP(GL_TEXTURE_2D, 0, GL_RGB, file);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return tex;
}

string SimpleApp::loadFile(const string file) {
	return loadFile(file.c_str());
}
string SimpleApp::loadFile(const char * const file)
{
	ifstream stream(file);
	if (stream.fail()) throw runtime_error(string("Can't open \'") + file + "\'");
	return string(istream_iterator<char>(stream >> noskipws), istream_iterator<char>());
}

string SimpleApp::getShaderInfo(GLuint shader) {
	int length = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length <= 1) return "";
	string log(length, ' ');
	glGetShaderInfoLog(shader, length, NULL, &log[0]);
	return log;
}

string SimpleApp::getProgramInfo(GLuint shader) {
	int length = 0;
	glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length <= 1) return "";
	string log(length, ' ');
	glGetProgramInfoLog(shader, length, NULL, &log[0]);
	return log;
}

GLuint SimpleApp::compileShader(const GLenum type, string src) {
	GLuint shader = glCreateShader(type);
	const char * srcPtr = src.c_str();
	glShaderSource(shader, 1, &srcPtr, NULL);
	glCompileShader(shader);
	/*
	string log = getShaderInfo(shader);
	if (!log.empty()) cout<< "Shader compile log:"<<endl << log;
	*/
	return shader;
}

GLuint SimpleApp::linkShader(int count, ...) {
	GLuint program = glCreateProgram();

	// Attach shaders
	va_list args;
	va_start(args, count);
	for (size_t i = 0; i < count; ++i)
	{
		glAttachShader(program, va_arg(args, GLuint));
	}
	va_end(args);

	// Link program and check for errors
	glLinkProgram(program);
	string log = getProgramInfo(program);
	if (!log.empty()) cout << "Program link log:" << endl << log;

	int linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) throw std::runtime_error("Shader linking failed");

	return program;
}



#ifdef __linux__
#define __stdcall
#endif

void __stdcall debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, void * userParam)
{
	cout << message << endl;
}


void SimpleApp::initGL() {
	/* SDL */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		string er = string("SDL_INIT_ERROR") + SDL_GetError();
		throw er;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	if (multisample > 1) {
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisample);
	}
	/* Window */
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
	window = SDL_CreateWindow("OpenGL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
		SDL_WINDOW_OPENGL
		| (resizable ? SDL_WINDOW_RESIZABLE : 0)
		| (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
	SDL_GetWindowSize(window, &width, &height);


	if (window == NULL) {
		string er = string("SDL_CreateWindow") + SDL_GetError();
		throw er;
	}
	atexit(SDL_Quit);

	/* Context */
	context = SDL_GL_CreateContext(window);
	if (SDL_GL_SetSwapInterval(-1) == -1) {
		SDL_GL_SetSwapInterval(1);
	}

	/* Glew */
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		string er = string((char*)glewGetErrorString(error));
		throw er;
	}

	/* Debug logger*/

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, NULL, GL_FALSE);
	glDebugMessageCallback((GLDEBUGPROC)debugMessageCallback, NULL);

}

void SimpleApp::OrbitCamera::resize(int w, int h){
	width = w; height = h;
}

void SimpleApp::OrbitCamera::update(){
	float radx = glm::radians((float)rotx);
	float rady = glm::radians((float)roty);
	float x = zoom * cos(rady) * cos(radx);
	float y = zoom * sin(rady);
	float z = zoom * cos(rady) * sin(radx);

	eye = glm::vec3(x, y, z);
	eye += center;
	up = glm::vec3(0, 1, 0);
	view = glm::lookAt(eye, center, up);
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, near, far);

}

void SimpleApp::OrbitCamera::wheel(int d) {
	float c = 1.1;
	zoom = d < 0 ? zoom*c : zoom / c;
}

void SimpleApp::OrbitCamera::press(Uint8 b) {
	if (button == b) {
		pressed = true;
	}
}

void SimpleApp::OrbitCamera::release(Uint8 b) {
	if (button == b) {
		pressed = false;
	}
}

void SimpleApp::OrbitCamera::move(int dx, int dy, int x, int y) {
	if (pressed) {
		rotx += dx;
		roty += dy;
		roty = max(min(roty, 89), -89);
	}
	posx = x;
	posy = y;
}
