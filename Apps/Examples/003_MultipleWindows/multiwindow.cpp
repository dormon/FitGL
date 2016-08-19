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
	auto window0 = app.getMainWindow();
	window0->setSize(200, 200);
	window0->move(100, 100);
	auto window1 = app.addWindow(200, 200);
	window1->move(100, 300);
	auto window2 = app.addWindow(200, 200);
	window2->move(300, 100);
	auto window3 = app.addWindow(200, 200);
	window3->move(300, 300);

	std::string prefix = "../../Resources/Shaders/Examples/003_MultipleWindows/";

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

		int w = window0->getWidth();
		int h = window0->getHeight();
		glViewport(0, 0, w, h);
		p = perspective(radians(45.0f),float(w)/float(h), 0.1f, 1000.0f);
		glUniformMatrix4fv(glGetUniformLocation(program, "p"), 1, 0, value_ptr(p));
		int z = 4;
		mat4 v = lookAt(vec3(z,z,z), vec3(0, 0, 0), vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(program, "v"), 1, 0, value_ptr(v));

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
		
		glDrawElements(GL_TRIANGLES, sizeof(bunny) / sizeof(short), GL_UNSIGNED_SHORT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	});

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int w = window1->getWidth();
		int h = window1->getHeight();
		glViewport(0, 0, w, h);
		mat4 v = lookAt(vec3(z, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(program, "v"), 1, 0, value_ptr(v));
		glDrawElements(GL_TRIANGLES, sizeof(bunny) / sizeof(short), GL_UNSIGNED_SHORT, 0);
	}, window1);

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int w = window2->getWidth();
		int h = window2->getHeight();
		glViewport(0, 0, w, h);
		mat4 v = lookAt(vec3(0, z, 0), vec3(0, 0, 0), vec3(1, 0, 0));
		glUniformMatrix4fv(glGetUniformLocation(program, "v"), 1, 0, value_ptr(v));
		glDrawElements(GL_TRIANGLES, sizeof(bunny) / sizeof(short), GL_UNSIGNED_SHORT, 0);
	}, window2);

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int w = window3->getWidth();
		int h = window3->getHeight();
		glViewport(0, 0, w, h);
		mat4 v = lookAt(vec3(0, 0, z), vec3(0, 0, 0), vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(program, "v"), 1, 0, value_ptr(v));
		glDrawElements(GL_TRIANGLES, sizeof(bunny) / sizeof(short), GL_UNSIGNED_SHORT, 0);
	}, window3);
	return app.run();
}
