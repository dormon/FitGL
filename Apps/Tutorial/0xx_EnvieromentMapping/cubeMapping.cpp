#include <BaseApp.h>

#include <geGL/StaticCalls.h>
using namespace ge::gl;

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
  std::shared_ptr<Program> programEnv, program, programWater;

	auto mainWindow = app.getMainWindow();

	PerspectiveCamera cam;
	OrbitManipulator manipulator(&cam);
	manipulator.setupCallbacks(app);
	
  std::shared_ptr<Texture> diffuseTextureTop;
  std::shared_ptr<Texture> diffuseTextureSide;
  std::shared_ptr<Texture> diffuseTextureDown;
	GLuint cubeTexture;

	GLuint vao;
	GLuint vaoEmpty;
	GLuint vbo;

	int32_t sphereSizeX = 20;
	int32_t sphereSizeY = 20;
	uint32_t waterSizeX = 10;
	uint32_t waterSizeY = 10;

	app.addInitCallback([&]() {
		std::string prefix = app.getResourceDir() + "Shaders/Tutorial/";
		auto vs = compileShader(GL_VERTEX_SHADER,
			"#version 450\n",
			Loader::text(prefix + "cubeMapping.vp"));
		auto fs = compileShader(GL_FRAGMENT_SHADER,
			"#version 450\n",
			Loader::text(prefix + "lighting.vp"),
			Loader::text(prefix + "cubeMapping.fp"));
		program = createProgram(vs, fs);
		
		vs = compileShader(GL_VERTEX_SHADER,
			"#version 450\n",
			Loader::text(prefix + "environment.vp"));
		fs = compileShader(GL_FRAGMENT_SHADER,
			"#version 450\n",
			Loader::text(prefix + "environment.fp"));
		programEnv = createProgram(vs, fs);

		vs = compileShader(GL_VERTEX_SHADER,
			"#version 450\n",
			Loader::text(prefix + "water.vp"));
		auto tcs = compileShader(GL_TESS_CONTROL_SHADER,
			"#version 450\n",
			Loader::text(prefix + "water.cp"));
		auto tes = compileShader(GL_TESS_EVALUATION_SHADER,
			"#version 450\n",
			Loader::text(prefix + "noiseFunctions.vp"),
			Loader::text(prefix + "water.ep"));
		fs = compileShader(GL_FRAGMENT_SHADER,
			"#version 450\n",
			Loader::text(prefix + "lighting.vp"),
			Loader::text(prefix + "water.fp"));
		programWater = createProgram(vs, tcs, tes, fs);

		std::string texPrefix = app.getResourceDir() + "Textures/Tutorial/";
		diffuseTextureTop = Loader::texture(texPrefix + "grass.png");
		diffuseTextureSide = Loader::texture(texPrefix + "dirt.jpg");
		diffuseTextureDown = Loader::texture(texPrefix + "rock.jpg");
		cubeTexture = Loader::cubemap({ 
			texPrefix + "cube0.png", texPrefix + "cube1.png",
			texPrefix + "cube2.png", texPrefix + "cube3.png",
			texPrefix + "cube4.png", texPrefix + "cube5.png" });


		glCreateBuffers(1, &vbo);
		const uint32_t floatsPerVertex = 6;
		const uint32_t vertiesPerFace = 6;
		float*vertices = new float[sphereSizeX*sphereSizeY*vertiesPerFace*floatsPerVertex];
		for (int32_t y = 0; y<sphereSizeY; ++y) {
			for (int32_t x = 0; x<sphereSizeX; ++x) {
				for (uint32_t k = 0; k<vertiesPerFace; ++k) {
					const int32_t xOffset[] = { 0,1,0,0,1,1 };
					const int32_t yOffset[] = { 0,0,1,1,0,1 };
					float u = (float)(x + xOffset[k]) / sphereSizeX;
					float v = (float)(y + yOffset[k]) / sphereSizeY;
					float xAngle = -u*glm::two_pi<float>();
					float yAngle = v*glm::pi<float>();
					uint32_t faceId = y*sphereSizeX + x;
					uint32_t faceVertex = faceId*vertiesPerFace + k;
					vertices[faceVertex*floatsPerVertex + 0] = glm::cos(xAngle)*glm::sin(yAngle);
					vertices[faceVertex*floatsPerVertex + 1] = -glm::cos(yAngle);
					vertices[faceVertex*floatsPerVertex + 2] = glm::sin(xAngle)*glm::sin(yAngle);
					vertices[faceVertex*floatsPerVertex + 3] = 1;
					vertices[faceVertex*floatsPerVertex + 4] = u;
					vertices[faceVertex*floatsPerVertex + 5] = v;
				}
			}
		}
		glNamedBufferData(vbo, sizeof(float)*sphereSizeX*sphereSizeY*vertiesPerFace*floatsPerVertex, vertices, GL_STATIC_DRAW);
		delete[]vertices;

		glCreateVertexArrays(1, &vao);
		glEnableVertexArrayAttrib(vao, 0); 
		glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, 0, 0); 
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float)*floatsPerVertex);
		glEnableVertexArrayAttrib(vao, 1);
		glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, 0, 0);
		glVertexArrayVertexBuffer(vao, 1, vbo, sizeof(float) * 4, sizeof(float)*floatsPerVertex);
		glCreateVertexArrays(1, &vaoEmpty);
		glClearColor(0, 0, 0, 1);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	});

	app.addResizeCallback([&](int w, int h) {
		glViewport(0, 0, w, h);
		cam.setAspect(float(w) / float(h));
	});

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		programEnv->use();
		programEnv->setMatrix4fv("p", value_ptr(cam.getProjection()));
		programEnv->setMatrix4fv("v", value_ptr(cam.getView()));

		glBindTextureUnit(0, cubeTexture);
		glBindVertexArray(vaoEmpty);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		program->use();
    diffuseTextureTop->bind(0);
    diffuseTextureSide->bind(1);
    diffuseTextureDown->bind(2);
    glBindTextureUnit(3, cubeTexture);

    program->setMatrix4fv("p", value_ptr(cam.getProjection()));
    program->setMatrix4fv("v", value_ptr(cam.getView()));

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sphereSizeX*sphereSizeY * 6);
		glBindVertexArray(0);

		programWater->use();
		programWater->set1ui("waterSizeX", waterSizeX);
		programWater->set1ui("waterSizeY", waterSizeY);
		programWater->set1f("time", app.getTimeFromStart());
		programWater->setMatrix4fv("p", value_ptr(cam.getProjection()));
		programWater->setMatrix4fv("v", value_ptr(cam.getView()));

		glEnable(GL_BLEND);
		glBindTextureUnit(0, cubeTexture);
		glBindVertexArray(vaoEmpty);
		glPatchParameteri(GL_PATCH_VERTICES, 1);
		glDrawArrays(GL_PATCHES, 0, waterSizeX*waterSizeY);
		glBindVertexArray(0);
		glDisable(GL_BLEND);

	});
	return app.run();
}