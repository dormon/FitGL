#include <BaseApp.h>

#include <Loader.h>
#include <bunny.h>

#include <Gui.h>

using namespace glm;

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
	ProgramObject program;
	
	auto mainWindow = app.getMainWindow();

	float zoom = 4;
	
	std::string prefix = app.getResourceDir() + "Shaders/Examples/e02_MultipleViews/";
	GLuint vao, vbo, ebo;

	mat4 v, p;

	Timer<float> timer;


	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "lambert.vert"));
		auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "lambert.frag"));
		program = createProgram(vs, fs);

		bunnyInit(vao,vbo,ebo);
		v = lookAt(vec3(zoom, zoom, zoom), vec3(0, 0, 0), vec3(0, 1, 0));
	});

	app.addUpdateCallback([&](float dt) {
		//std::cout << app.getTimeFromStart() << "\n";
		if (timer.elapsedFromStart() > 1) {
			timer.reset();
			std::cout << "tick" <<"\n";
		}
		v = rotate(v, dt, vec3(0, 1, 0));
	});

	app.addDrawCallback([&]() {
		int w = mainWindow->getWidth();
		int h = mainWindow->getHeight();
		glViewport(0, 0, w, h); 
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//bunny
		p = perspective(radians(45.0f), float(w) / float(h), 0.1f, 1000.0f);

		program.use();
		program.setMatrix4fv("p", value_ptr(p));
		program.setMatrix4fv("v", value_ptr(v));
		program.set3f("color", 1,0.5,0.25);

		bunnyDraw();
	});
	return app.run();
}
