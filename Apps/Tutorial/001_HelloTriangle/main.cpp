#include <BaseApp.h>

#include <geGL/StaticCalls.h>
using namespace ge::gl;

std::string vsrc = R".(
#version 450
layout(location=0) in vec2 position;
void main() {
	gl_Position = vec4(position,0,1);
}
).";

std::string fsrc = R".(
#version 450
out vec4 fragColor;
void main(){
	fragColor=vec4(1,1,0,1);
}
).";

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;	
  std::shared_ptr<Program> program;
	GLuint vao;
	GLuint vbo;
	auto mainWindow = app.getMainWindow();
	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, vsrc);
		auto fs = compileShader(GL_FRAGMENT_SHADER, fsrc);
		program = createProgram(vs, fs);
		glCreateVertexArrays(1, &vao);
		glCreateBuffers(1, &vbo);
		float data[] = {
			0, 0.8,
			0.7, -0.4,
			-0.7, -0.4
		};
		glNamedBufferData(vbo, sizeof(data), data, GL_STATIC_DRAW);
		glEnableVertexArrayAttrib(vao, 0);
		glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, 0, 0);
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float)*2);
	});

	app.addDrawCallback([&]() {
		int w = mainWindow->getWidth();
		int h = mainWindow->getHeight();
		glViewport(0, 0, w, h);
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT);
    program->use();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	});
	return app.run();
}