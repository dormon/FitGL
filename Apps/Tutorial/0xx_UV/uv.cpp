#include <BaseApp.h>
#include <Gui.h>

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;
	ProgramObject programEnv, program;

	auto mainWindow = app.getMainWindow();

	PerspectiveCamera cam;
	OrbitManipulator manipulator(&cam);
	manipulator.setupCallbacks(app);
	manipulator.setZoom(3);
	
	GLuint diffuseTexture;
	GLuint specularTexture;

	GLuint vao;
	GLuint vaoEmpty;
	GLuint vbo;

	int32_t sphereSizeX = 20;
	int32_t sphereSizeY = 20;

	app.addInitCallback([&]() {
		std::string prefix = app.getResourceDir() + "Shaders/Tutorial/";
		auto vs = compileShader(GL_VERTEX_SHADER,
			"#version 450\n",
			Loader::text(prefix + "uv.vp"));
		auto fs = compileShader(GL_FRAGMENT_SHADER,
			"#version 450\n",
			Loader::text(prefix + "lighting.vp"),
			Loader::text(prefix + "uv.fp"));
		program = createProgram(vs, fs);
		
		std::string texPrefix = app.getResourceDir() + "Textures/Tutorial/";
		diffuseTexture = Loader::texture(texPrefix + "earth.png");
		specularTexture = Loader::texture(texPrefix + "earth_s.png");

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
		program.use();
		glBindTextureUnit(0, diffuseTexture);
		glBindTextureUnit(1, specularTexture);

		program.setMatrix4fv("p", value_ptr(cam.getProjection()));
		program.setMatrix4fv("v", value_ptr(cam.getView()));

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sphereSizeX*sphereSizeY * 6);
		glBindVertexArray(0);
	});
	return app.run();
}