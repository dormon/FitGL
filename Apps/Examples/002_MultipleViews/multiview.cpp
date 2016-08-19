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

	std::string prefix = "../../Resources/Shaders/Examples/002_MultipleViews/";
	auto mainWindow = app.getMainWindow();

	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix+"lambert.vert"));
		auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "lambert.frag"));
		program = createProgram(vs, fs);	

		glCreateBuffers(1, &vbo);
		glNamedBufferData(vbo, sizeof(bunnyVertices), bunnyVertices, GL_STATIC_DRAW);

		glCreateBuffers(1, &ebo);
		glNamedBufferData(ebo, sizeof(bunny), bunny, GL_STATIC_DRAW);

		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindVertexBuffer(0, vbo, offsetof(BunnyVertex, position), sizeof(BunnyVertex));
		glBindVertexBuffer(1, vbo, offsetof(BunnyVertex, normal), sizeof(BunnyVertex));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexArrayElementBuffer(vao, ebo);
	});

	app.addDrawCallback([&]() {
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glUseProgram(program);	

		int w = mainWindow->getWidth();
		int h = mainWindow->getHeight();

		mat4 p = perspective(radians(45.0f),float(w)/float(h), 0.1f, 1000.0f);
		glUniformMatrix4fv(glGetUniformLocation(program, "p"), 1, 0, value_ptr(p));
		int z = 4;
		mat4 v = lookAt(vec3(z,z,z), vec3(0, 0, 0), vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(program, "v"), 1, 0, value_ptr(v));

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		
		
		glViewport(w / 2, h / 2, w / 2, h / 2);
		glDrawElements(GL_TRIANGLES, sizeof(bunny) / sizeof(short), GL_UNSIGNED_SHORT, 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		v = lookAt(vec3(z, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(program, "v"), 1, 0, value_ptr(v));
		glViewport(0, 0, w / 2, h / 2);
		glDrawElements(GL_TRIANGLES, sizeof(bunny) / sizeof(short), GL_UNSIGNED_SHORT, 0);


		v = lookAt(vec3(0, z, 0), vec3(0, 0, 0), vec3(1, 0, 0));
		glUniformMatrix4fv(glGetUniformLocation(program, "v"), 1, 0, value_ptr(v));		
		glViewport(0, h / 2, h / 2, h / 2);
		glDrawElements(GL_TRIANGLES, sizeof(bunny) / sizeof(short), GL_UNSIGNED_SHORT, 0);


		v = lookAt(vec3(0, 0, z), vec3(0, 0, 0), vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(program, "v"), 1, 0, value_ptr(v));
		glViewport(w / 2, 0, w / 2, h / 2);
		glDrawElements(GL_TRIANGLES, sizeof(bunny) / sizeof(short), GL_UNSIGNED_SHORT, 0);
	});
	return app.run();
}
