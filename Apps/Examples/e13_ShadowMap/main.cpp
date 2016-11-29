#include <BaseApp.h>

#include <geGL/StaticCalls.h>
using namespace ge::gl;

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
  std::shared_ptr<Program> program;
  std::shared_ptr<Program> programShadowmap;

	auto mainWindow = app.getMainWindow();
	
	std::string prefix = app.getResourceDir() + "Shaders/Examples/e06_ModelLoader/";

	PerspectiveCamera cam;
	OrbitManipulator manipulator(&cam);
	manipulator.setupCallbacks(app);	
	NodeShared root;

	app.addInitCallback([&]() {
    auto vs = std::make_shared<Shader>(GL_VERTEX_SHADER, Loader::text(prefix + "phong.vert"));
    auto fs = std::make_shared<Shader>(GL_FRAGMENT_SHADER, Loader::text(prefix + "phong.frag"));
    program = std::make_shared<Program>(vs, fs);

		root = Loader::scene(app.getResourceDir() + "Models/sponza/sponza.fbx");
	});

	app.addResizeCallback([&](int w, int h) {
		glViewport(0, 0, w, h);
		cam.setAspect(float(w) / float(h));
	});
	
	app.addDrawCallback([&]() {
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//bunny

		program->use();
		program->setMatrix4fv("p", value_ptr(cam.getProjection()));
		program->setMatrix4fv("v", value_ptr(cam.getView()));

		drawNode(*program, root);

		fpsLabel();
		label("Orbit manipulator:\nWSAD - Move center\nEQ - Up/Down\nRMB/LMB drag - rotate\nMMB drag - move center\nWheel - zoom",0,20,200,200);
	});
	return app.run();
}
