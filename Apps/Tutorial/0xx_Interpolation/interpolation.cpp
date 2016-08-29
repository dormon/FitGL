#include <BaseApp.h>
#include <Gui.h>

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
	ProgramObject program;

	auto mainWindow = app.getMainWindow();

	PerspectiveCamera cam;
	OrbitManipulator manipulator(&cam);
	manipulator.setupCallbacks(app);
	manipulator.setZoom(2);
	manipulator.setRotationX(90);
	cam.setCenter(glm::vec3(0.5,0.5,0));

	GLuint vbo;
	GLuint vao;
	uint32_t mode = 0;
	GLint mvpUniform;
	GLint modeUniform;

	app.addInitCallback([&]() {
		std::string prefix = "../../Resources/Shaders/Tutorial/";
		auto vs = compileShader(GL_VERTEX_SHADER,
			Loader::text(prefix + "interpolation.vp"));
		auto fs = compileShader(GL_FRAGMENT_SHADER,
			Loader::text(prefix + "interpolation.fp"));
		program = createProgram(vs, fs);
		
		mvpUniform = glGetUniformLocation(program, "mvp");
		modeUniform = glGetUniformLocation(program, "mode");

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
		cam.setAspect(float(w) / float(h));
	});

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);

		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(cam.getViewProjection()));
		glUniform1ui(modeUniform, mode);

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