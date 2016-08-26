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
uniform vec3 color;
out vec4 fragColor;
void main(){
	fragColor=vec4(color,1);
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
			//line_loop
			0.1,0.5,
			0.1,0.7,
			0.3,0.7,
			0.3,0.5,
			//triangles
			-0.8,0.0,
			-0.7,0.2,
			-0.6,0.0,
			-0.5,0.2,
			-0.4,0.0,
			-0.3,0.2,
			//triangle_strip
			-0.2,0.0,
			-0.1,0.2,
			-0.0,0.0,
			0.1,0.2,
			0.2,0.0,
			0.3,0.2,
			//triangle_fan
			0,-0.4,
			-0.1,-0.2,
			0.1,-0.2,
			0.2,-0.4,
			0.1,-0.6,
			-0.1,-0.6,
			-0.2,-0.4,
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
		glUniform3f(glGetUniformLocation(program, "color"), 1, 1, 0);
		glPointSize(3);
		glLineWidth(3);
		 
		label("GL_POINTS", w*0.1, 0.25*h);
		glDrawArrays(GL_POINTS, 0, 4);

		label("GL_LINES", w*0.25, 0.25*h);
		glDrawArrays(GL_LINES, 4, 4);

		label("GL_LINE_STRIP", w*0.40, 0.25*h);
		glDrawArrays(GL_LINE_STRIP, 8, 4);

		label("GL_LINE_LOOP", w*0.55, 0.25*h);
		glDrawArrays(GL_LINE_LOOP, 12, 4);
				
		label("GL_TRIANGLES", w*0.15, 0.5*h);
		glDrawArrays(GL_TRIANGLES, 16, 6);

		label("GL_TRIANGLE_STRIP", w*0.45, 0.5*h);
		glDrawArrays(GL_TRIANGLE_STRIP, 22, 6);

		label("GL_TRIANGLE_FAN", w*0.45, 0.8*h);
		glDrawArrays(GL_TRIANGLE_FAN, 28, 7);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUniform3f(glGetUniformLocation(program, "color"), 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, 16, 6);
		glDrawArrays(GL_TRIANGLE_STRIP, 22, 6);
		glDrawArrays(GL_TRIANGLE_FAN, 28, 7);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUniform3f(glGetUniformLocation(program, "color"), 0, 1, 1);
		glPointSize(5);
		glDrawArrays(GL_POINTS, 0, 35);
	});
	return app.run();
}