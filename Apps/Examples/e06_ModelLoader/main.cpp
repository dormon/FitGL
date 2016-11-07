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

	NodeShared scene;

	std::string prefix = app.getResourceDir() + "Shaders/Examples/e06_ModelLoader/";
	std::string prefixModel = app.getResourceDir() + "Models/";
	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "phong.vert"));
		auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "phong.frag"));
		program = createProgram(vs, fs);

		scene = Loader::scene(prefixModel+ "texCube/texCube.obj");
	});

	app.addDrawCallback([&]() {
		int w = mainWindow->getWidth();
		int h = mainWindow->getHeight();
		glViewport(0, 0, w, h); 
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//bunny
		mat4 p = perspective(radians(45.f), float(w) / float(h), 0.1f, 1000.f);
		mat4 v = lookAt(zoom*vec3(-1,1,1), vec3(0, 0, 0), vec3(0, 1, 0));
		program.use();
		program.setMatrix4fv("p", value_ptr(p));
		program.setMatrix4fv("v", value_ptr(v));

		drawNode(program,scene);
	});
	return app.run();
}
