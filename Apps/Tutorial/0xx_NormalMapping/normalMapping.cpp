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

	GLuint vao;
	uint32_t useSpecularTexture = 0;
	uint32_t useNormalTexture = 0;
	uint32_t drawDiffuseTexture = 0;
	uint32_t drawSpecularTexture = 0;
	uint32_t drawNormalTexture = 0;

  std::shared_ptr<Texture> diffuseTexture;
  std::shared_ptr<Texture> specularTexture;
  std::shared_ptr<Texture> normalTexture;

	app.addInitCallback([&]() {
		std::string prefix = app.getResourceDir() + "Shaders/Tutorial/";
		auto vs = compileShader(GL_VERTEX_SHADER,
			"#version 450\n",
			Loader::text(prefix + "normalMapping.vp"));

		auto fs = compileShader(GL_FRAGMENT_SHADER,
			"#version 450\n",
			Loader::text(prefix + "lighting.vp"),
			Loader::text(prefix + "normalMapping.fp"));
		program = createProgram(vs, fs);


		std::string texPrefix = app.getResourceDir() + "Textures/Tutorial/";
		diffuseTexture = Loader::texture(texPrefix+"sponza_details_diff.png");
		specularTexture = Loader::texture(texPrefix + "sponza_details_spec.png");
		normalTexture = Loader::texture(texPrefix + "sponza_detail_ddn.png");

		glCreateVertexArrays(1, &vao);

		glClearColor(0, 0, 0, 1);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	});

	app.addResizeCallback([&](int w, int h) {
		glViewport(0, 0, w, h);
		cam->setAspect(float(w) / float(h));
	});

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program->use();

    diffuseTexture->bind(0);
    specularTexture->bind(1);
    normalTexture->bind(2);

    program->setMatrix4fv("p", value_ptr(cam->getProjection()));
    program->setMatrix4fv("v", value_ptr(cam->getView()));
		program->set1ui("useSpecularTexture", useSpecularTexture);
		program->set1ui("useNormalTexture", useNormalTexture);
		program->set1ui("drawDiffuseTexture", drawDiffuseTexture);
		program->set1ui("drawSpecularTexture", drawSpecularTexture);
		program->set1ui("drawNormalTexture", drawNormalTexture);


		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		textColor(1, 1, 0, 1);
		std::string msg;
		msg += "X - useSpecularTexture: " + std::to_string(useSpecularTexture) + "\n";
		msg += "C - useNormalTexture: " + std::to_string(useNormalTexture) + "\n";
		msg += "V - drawDiffuseTexture: " + std::to_string(drawDiffuseTexture) + "\n";
		msg += "B - drawSpecularTexture: " + std::to_string(drawSpecularTexture) + "\n";
		msg += "N - drawNormalTexture: " + std::to_string(drawNormalTexture) + "\n";
		label(msg, 0, 0, 300, 200);
	});

	app.addKeyPressCallback([&](SDL_Keycode c, Uint16) {
		if (c == 'x') useSpecularTexture ^= 1;
		if (c == 'c') useNormalTexture ^= 1;
		if (c == 'v') drawDiffuseTexture ^= 1;
		if (c == 'b') drawSpecularTexture ^= 1;
		if (c == 'n') drawNormalTexture ^= 1;
	});
	return app.run();
}