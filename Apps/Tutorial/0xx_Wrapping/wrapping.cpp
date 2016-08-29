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

	GLuint vao;
	GLuint diffuseTexture;
	bool useMagLINEAR = true;
	bool useMipmap = true;
	bool useMinLINEAR = true;
	bool useBetweenLINEAR = true;

	app.addInitCallback([&]() {
		std::string prefix = "../../Resources/Shaders/Tutorial/";
		auto vs = compileShader(GL_VERTEX_SHADER,
			"#version 450\n",
			Loader::text(prefix + "mipmap.vp"));
		auto fs = compileShader(GL_FRAGMENT_SHADER,
			"#version 450\n",
			Loader::text(prefix + "lighting.vp"),
			Loader::text(prefix + "mipmap.fp"));
		program = createProgram(vs, fs);

		diffuseTexture = Loader::texture("../../Resources/Textures/Tutorial/chessboard.png");

		glGenVertexArrays(1, &vao);

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
		glBindTextureUnit(0, diffuseTexture);

		std::string mag;
		std::string min;
		if (useMagLINEAR) {
			glTextureParameteri(diffuseTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			mag = "GL_LINEAR";
		}
		else {
			glTextureParameteri(diffuseTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			mag = "GL_NEAREST";
		}

		if (useMipmap) {
			if (useMinLINEAR) {
				if (useBetweenLINEAR) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					min = "GL_LINEAR_MIPMAP_LINEAR";
				}
				else {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					min = "GL_LINEAR_MIPMAP_NEAREST";
				}
			}
			else {
				if (useBetweenLINEAR) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
					min = "GL_NEAREST_MIPMAP_LINEAR";
				}
				else {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					min = "GL_NEAREST_MIPMAP_NEAREST";
				}
			}
		}
		else {
			if (useMinLINEAR) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				min = "GL_LINEAR";
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				min = "GL_NEAREST";
			}
		}

		program.setMatrix4fv("v", glm::value_ptr(cam.getView()));
		program.setMatrix4fv("p", glm::value_ptr(cam.getProjection()));

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		textColor(1, 1, 0, 1);
		std::string msg;
		msg += "X - useMagLINEAR: " + std::to_string(useMagLINEAR) + "\n";
		msg += "C - useMinLINEAR: " + std::to_string(useMinLINEAR) + "\n";
		msg += "V - useMipmap: " + std::to_string(useMipmap) + "\n";
		msg += "B - useBetweenLINEAR: " + std::to_string(useBetweenLINEAR) + "\n";
		msg += "MAG_FILTER: " + mag + "\n";
		msg += "MIN_FILTER: " + min + "\n";
		label(msg, 0, 0, 300, 200);
	});

	app.addKeyPressCallback([&](SDL_Keycode c, Uint16) {
		if (c == 'x') useMagLINEAR ^= 1;
		if (c == 'c') useMinLINEAR ^= 1;
		if (c == 'v') useMipmap ^= 1;
		if (c == 'b') useBetweenLINEAR ^= 1;
	});
	return app.run();
}