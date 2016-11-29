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
	
	TextureS cubeTexture;

	GLuint vaoEmpty;
	GLuint sphereBuffer;
	GLuint sphereCount = 30;

	app.addInitCallback([&]() {
		std::string prefix = app.getResourceDir() + "Shaders/Tutorial/";
		auto vs = compileShader(GL_VERTEX_SHADER,
			"#version 450\n",
			Loader::text(prefix + "fsRayTracing.vp"));
		auto fs = compileShader(GL_FRAGMENT_SHADER,
			"#version 450\n",
			Loader::text(prefix + "fsRayTracing.fp"));
		program = createProgram(vs, fs);
		

		std::string texPrefix = app.getResourceDir() + "Textures/Tutorial/";
		cubeTexture = Loader::cubemap({ 
			texPrefix + "cube0.png", texPrefix + "cube1.png",
			texPrefix + "cube2.png", texPrefix + "cube3.png",
			texPrefix + "cube4.png", texPrefix + "cube5.png" });


		glCreateBuffers(1, &sphereBuffer);
		struct Sphere {
			glm::vec4 positionRadius;
			glm::vec4 color;
		};
		sphereCount = 30;
		Sphere*spheres = new Sphere[sphereCount];

		for (uint32_t i = 0; i<sphereCount; ++i) {
			float t = (float)i / sphereCount;
			float angle = glm::radians<float>(3 * 360.f*t);
			spheres[i].positionRadius = glm::vec4(glm::cos(angle), t * 4, glm::sin(angle), glm::sin(t * 10)*.2f + .3f);
			spheres[i].color = glm::vec4(0.f, 1.f, 0.f, 1.f);
		}

		glNamedBufferData(sphereBuffer, sizeof(Sphere)*sphereCount, spheres, GL_STATIC_DRAW);
		delete[]spheres;

		glCreateVertexArrays(1, &vaoEmpty);
		glClearColor(0, 0, 0, 1);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	});

	app.addResizeCallback([&](int w, int h) {
		glViewport(0, 0, w, h);
		cam->setAspect(float(w) / float(h));
	});

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program->use();
		program->setMatrix4fv("p", value_ptr(cam->getProjection()));
		program->setMatrix4fv("v", value_ptr(cam->getView()));

    cubeTexture->bind(0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sphereBuffer);
		program->set1ui("nofSpheres", sphereCount);
		glBindVertexArray(vaoEmpty);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	});
	return app.run();
}