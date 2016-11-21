#include <BaseApp.h>

#include <geGL/StaticCalls.h>
using namespace ge::gl;

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
  std::shared_ptr<Program> program;

	auto mainWindow = app.getMainWindow();

	PerspectiveCamera cam;
	OrbitManipulator manipulator(&cam);
	manipulator.setupCallbacks(app);
	manipulator.setZoom(2);
	manipulator.setRotationX(90);

	GLuint vao;
	uint32_t useNormalMapping = 0;
	uint32_t useParalaxMapping = 0;

	app.addInitCallback([&]() {
		std::string prefix = app.getResourceDir() + "Shaders/Tutorial/";
		auto vs = compileShader(GL_VERTEX_SHADER,
			"#version 450\n",
			Loader::text(prefix + "paralaxMapping.vp"));

		auto fs = compileShader(GL_FRAGMENT_SHADER,
			"#version 450\n",
			Loader::text(prefix + "lighting.vp"),
			Loader::text(prefix + "noiseFunctions.vp"),
			Loader::text(prefix + "gradients.vp"), 
			Loader::text(prefix + "paralaxMapping.fp"));
		program = createProgram(vs, fs);

		glCreateVertexArrays(1, &vao);

		glClearColor(0, 0, 0, 1);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	});

	app.addResizeCallback([&](int w, int h) {
		glViewport(0, 0, w, h);
		cam.setAspect(float(w) / float(h));
	});

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program->use();
		program->setMatrix4fv("v", glm::value_ptr(cam.getView()));
		program->setMatrix4fv("p", glm::value_ptr(cam.getProjection()));

		program->set1ui("useNormalMapping", useNormalMapping);
		program->set1ui("useParalaxMapping", useParalaxMapping);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


		textColor(1, 1, 0, 1);
		std::string msg;
		msg += "X - useNormalMapping: " + std::to_string(useNormalMapping) + "\n";
		msg += "C - useParalaxMapping: " + std::to_string(useParalaxMapping) + "\n";
		label(msg, 0, 0, 300, 200);
	});

	app.addKeyPressCallback([&](SDL_Keycode c, Uint16) {
		if (c == 'x') useNormalMapping ^= 1;
		if (c == 'c') useParalaxMapping ^= 1;
	});
	return app.run();
}