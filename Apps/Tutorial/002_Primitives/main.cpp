#include <BaseApp.h>
#include <Gui.h>

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
	GLuint program;
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
			//points
			-0.8,0.5,
			-0.8,0.7,
			-0.6,0.7,
			-0.6,0.5,
			//lines
			-0.5,0.5,
			-0.5,0.7,
			-0.3,0.7,
			-0.3,0.5,
			//line_strip
			-0.2,0.5,
			-0.2,0.7,
			-0.0,0.7,
			-0.0,0.5,
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
		glUseProgram(program);
		glBindVertexArray(vao);
		 
		label("GL_POINTS", w*0.1, 0.25*h);
		glDrawArrays(GL_POINTS, 0, 4);

		label("GL_LINES", w*0.25, 0.25*h);
		glDrawArrays(GL_LINES, 4, 4);

		label("GL_LINE_STRIP", w*0.40, 0.25*h);
		glDrawArrays(GL_LINE_STRIP, 8, 4);

	});
	return app.run();
}