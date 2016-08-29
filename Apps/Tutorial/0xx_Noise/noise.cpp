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

	GLuint vbo;
	GLuint vao;
	int32_t sphereSizeX = 20;
	int32_t sphereSizeY = 20;
	bool useGouraudNormal = true;
	bool usePhongLighting = true;
	bool computeLightInFS = true;

	app.addInitCallback([&]() {
		std::string prefix = "../../Resources/Shaders/Tutorial/";
		auto vs = compileShader(GL_VERTEX_SHADER,
			"#version 450\n",
			Loader::text(prefix + "lighting.vp"),
			Loader::text(prefix + "noiseFunctions.vp"),
			Loader::text(prefix + "noiseSphere.vp"));

		auto fs = compileShader(GL_FRAGMENT_SHADER,
			"#version 450\n",
			Loader::text(prefix + "lighting.vp"),
			Loader::text(prefix + "noiseFunctions.vp"),
			Loader::text(prefix + "noiseSphere.fp"));
		program = createProgram(vs, fs);

		glCreateBuffers(1, &vbo);
		float*vertices = new float[sphereSizeX*sphereSizeY * 6 * 4];
		for (int32_t y = 0; y<sphereSizeY; ++y) {
			for (int32_t x = 0; x< sphereSizeX; ++x) {
				for (int32_t k = 0; k<6; ++k) {
					const int32_t xOffset[] = { 0,1,0,0,1,1 };
					const int32_t yOffset[] = { 0,0,1,1,0,1 };
					float xAngle = (float)(x + xOffset[k]) / sphereSizeX*glm::two_pi<float>();
					float yAngle = (float)(y + yOffset[k]) / sphereSizeY*glm::pi<float>();
					vertices[((y* sphereSizeX + x) * 6 + k) * 4 + 0] = glm::cos(xAngle)*glm::sin(yAngle);
					vertices[((y* sphereSizeX + x) * 6 + k) * 4 + 1] = -glm::cos(yAngle);
					vertices[((y* sphereSizeX + x) * 6 + k) * 4 + 2] = glm::sin(xAngle)*glm::sin(yAngle);
					vertices[((y* sphereSizeX + x) * 6 + k) * 4 + 3] = 1;
				}
			}
		}
		glNamedBufferData(vbo, sizeof(float)* sphereSizeX* sphereSizeY * 6 * 4, vertices, GL_STATIC_DRAW);
		delete[]vertices;

		glCreateVertexArrays(1, &vao);
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, 4 * sizeof(float));
		glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, 0, 0);
		glEnableVertexArrayAttrib(vao, 0);

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

		program.use();
		program.setMatrix4fv("v", glm::value_ptr(cam.getView()));
		program.setMatrix4fv("p", glm::value_ptr(cam.getProjection()));


		program.set4f("light", 10, 10, 10, 1);

		program.set1ui("sphereSizeX", sphereSizeX);
		program.set1ui("sphereSizeY", sphereSizeY);

		program.set1i("useGouraudNormal", useGouraudNormal);
		program.set1i("usePhongLighting", usePhongLighting);
		program.set1i("computeLightInFS", computeLightInFS);

		program.set1f("time", app.getTimeFromStart());


		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sphereSizeX*sphereSizeY * 6);
		glBindVertexArray(0);

		textColor(1, 1, 0, 1);
		std::string msg;
		msg += "X - useGouraudNormal: " + std::to_string(useGouraudNormal) + "\n";
		msg += "C - usePhongLighting: " + std::to_string(usePhongLighting) + "\n";
		msg += "V - computeLightInFS: " + std::to_string(computeLightInFS) + "\n";
		msg += "B - reset\n";
		label(msg, 0, 0, 300, 200);
	});

	app.addKeyPressCallback([&](SDL_Keycode c, Uint16) {
		if (c == 'x') useGouraudNormal ^= 1;
		if (c == 'c') usePhongLighting ^= 1;
		if (c == 'v') computeLightInFS ^= 1;
		if (c == 'b') {
			useGouraudNormal = 0;
			usePhongLighting = 0;
			computeLightInFS = 0;
		}
	});
	return app.run();
}