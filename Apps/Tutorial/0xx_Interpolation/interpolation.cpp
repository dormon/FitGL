#include <BaseApp.h>

#include <geGL/StaticCalls.h>
using namespace ge::gl;
using namespace fgl;

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
  std::shared_ptr<Program> program;

	auto mainWindow = app.getMainWindow();

  PerspectiveCameraS cam = newPerspectiveCamera();
  OrbitManipulator manipulator(cam);
	manipulator.setupCallbacks(app);
	manipulator.setZoom(2);
	manipulator.setRotationX(90);
	cam->setCenter(glm::vec3(0.5,0.5,0));

	GLuint vbo;
	GLuint vao;
	uint32_t mode = 0;

	app.addInitCallback([&]() {
		std::string prefix = app.getResourceDir() + "Shaders/Tutorial/";
		auto vs = compileShader(GL_VERTEX_SHADER,
			Loader::text(prefix + "interpolation.vp"));
		auto fs = compileShader(GL_FRAGMENT_SHADER,
			Loader::text(prefix + "interpolation.fp"));
		program = createProgram(vs, fs);
	
		glCreateBuffers(1, &vbo);
		float vertices[] = { 0,0,0,1, 1,0,0,1, 0,1,0,1 };
		glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glCreateVertexArrays(1, &vao);
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float)*4);
		glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, 0, 0);
		glEnableVertexArrayAttrib(vao, 0);

		glClearColor(0.2,0.2,0.2, 1);
		glDisable(GL_CULL_FACE);
	});

	app.addResizeCallback([&](int w, int h) {
		glViewport(0, 0, w, h);
		cam->setAspect(float(w) / float(h));
	});

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->use();

    program->setMatrix4fv("mvp", glm::value_ptr(cam->getViewProjection()));
    program->set1ui("mode", mode);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		textColor(1, 1, 0, 1);
		label("X - change mode\nCurrent: "+std::string(mode?"nonperspective ":"perspective"));
	});

	app.addKeyPressCallback([&](SDL_Keycode c,Uint16) {
		if (c == 'x') mode ^= 1;
	});
	return app.run();
}