#include <BaseApp.h>

std::string vsrc = R".(
#version 450
vec4 cc[3] = vec4[](vec4(1, 0, 0, 1), vec4(0, 1, 0, 1), vec4(0, 0, 1, 1));
vec2 pp[3] = vec2[](vec2(0, 0.8), vec2(-0.7, -0.4), vec2(0.7, -0.4));
out vec4 c;
void main() {
	c = cc[gl_VertexID];
	gl_Position = vec4(pp[gl_VertexID],1,1);
}).";

std::string fsrc = R".(
#version 450
in vec4 c;
out vec4 f;
void main(){
	f=c;
}
).";

int main(int /*argc*/, char ** /*argv*/) {
	BaseApp app;	
	GLuint program;
	GLuint vao;
	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, vsrc);
		auto fs = compileShader(GL_FRAGMENT_SHADER, fsrc);
		program = createProgram(vs, fs);
		glCreateVertexArrays(1, &vao);
	});

	app.addDrawCallback([&]() {
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	});
	return app.run();
}