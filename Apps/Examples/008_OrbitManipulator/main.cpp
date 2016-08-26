#include <BaseApp.h>

#include <bunny.h>


using namespace glm;

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
	ProgramObject program;

	auto mainWindow = app.getMainWindow();

	float zoom = 4;

	std::string prefix = "../../Resources/Shaders/Examples/002_MultipleViews/";
	GLuint vao, vbo, ebo;

	PerspectiveCamera cam;
	OrbitManipulator manipulator(&cam);
	manipulator.setupCallbacks(app);
	
	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "lambert.vert"));
		auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "lambert.frag"));
		program = createProgram(vs, fs);

		bunnyInit(vao, vbo, ebo);
	});

	app.addResizeCallback([&](int w, int h) {
		glViewport(0, 0, w, h);
		cam.setAspect(float(w) / float(h));
	});

	app.addUpdateCallback([&](float dt) {
		manipulator.update(dt);
	});

	app.addDrawCallback([&]() {
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//bunny

		program.use();
		program.setMatrix4fv("p", value_ptr(cam.getProjection()));
		program.setMatrix4fv("v", value_ptr(cam.getView()));
		program.set3f("color", 1, 0.5, 0.25);

		bunnyDraw();
	});
	return app.run();
}
