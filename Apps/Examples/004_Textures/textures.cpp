#include <BaseApp.h>

#include <Loader.h>
#include <bunny.h>

using namespace glm;

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	mat4 p;
	int z = 4;
	auto mainWindow = app.getMainWindow();

	std::string prefix = "../../Resources/Shaders/Examples/004_Textures/";
	std::string texPrefix = "../../Resources/Textures/PGP2015/";

	GLuint texture[9];

	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "empty.vert"));
		auto gs = compileShader(GL_GEOMETRY_SHADER, Loader::text(prefix + "quad.geo"));
		auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "texture.frag"));
		program = createProgram(vs, gs, fs);

		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		texture[0] = Loader::texture(texPrefix + "up.bmp");
		texture[1] = Loader::texture(texPrefix + "down.bmp");
		texture[2] = Loader::texture(texPrefix + "front.bmp");
		texture[3] = Loader::texture(texPrefix + "back.bmp");
		texture[4] = Loader::texture(texPrefix + "left.bmp");
		texture[5] = Loader::texture(texPrefix + "right.bmp");
		texture[6] = Loader::texture(texPrefix + "du2.bmp");
		texture[7] = Loader::texture(texPrefix + "du05a.bmp");
		texture[8] = Loader::texture(texPrefix + "du05b.bmp");

	});

	app.addDrawCallback([&]() {
		int w = mainWindow->getWidth();
		int h = mainWindow->getHeight();
		glViewport(0, 0, w, h); 
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glUseProgram(program);

		for (int y = 0; y < 3; y++){
			for (int x = 0; x < 3; x++) {
				glUniform2f(glGetUniformLocation(program, "posMin"), -1 + x*(2.0 / 3), -1 + y*(2.0 / 3));
				glUniform2f(glGetUniformLocation(program, "posMax"), -1 + (x + 1)*(2.0 / 3), -1 + (y + 1)*(2.0 / 3));
				glBindTextureUnit(0, texture[y * 3 + x]);
				glDrawArrays(GL_POINTS, 0, 1);
			}
		}

	});
	return app.run();
}
