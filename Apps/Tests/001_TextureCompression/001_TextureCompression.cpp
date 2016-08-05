#include "001_TextureCompression.h"
#include <iostream>

#define TEX_SIZE 1024*16
#define PIXELS (TEX_SIZE*TEX_SIZE)
#define WORKGROUP_SIZE 32
#define PIXEL_PER_THREAD 32
#define WORKGROUPS (PIXELS/(WORKGROUP_SIZE*PIXEL_PER_THREAD))


TextureCompression::TextureCompression() {
}

void TextureCompression::init() {
	std::string prefix = "../Resources/Shaders/Tests/001_TextureCompression/";

	std::string def = "#version 450\n#define WORKGROUP_SIZE " + std::to_string(WORKGROUP_SIZE) + "\n";
	def += "#define PIXEL_PER_THREAD " + std::to_string(PIXEL_PER_THREAD) + "\n";
	std::cout << def << std::endl;

	auto cs = compileShader(GL_COMPUTE_SHADER, def + loadFile(prefix + "write.comp"));
	programWrite = linkShader(1, cs);


	auto vs = compileShader(GL_VERTEX_SHADER, loadFile(prefix + "draw.vert"));
	auto gs = compileShader(GL_GEOMETRY_SHADER, loadFile(prefix + "draw.geo"));
	auto fs = compileShader(GL_FRAGMENT_SHADER, loadFile(prefix + "draw.frag"));
	program = linkShader(3, vs, gs, fs);

	cs = compileShader(GL_COMPUTE_SHADER, def+loadFile(prefix + "read.comp"));
	programRead = linkShader(1, cs);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	/*char *data = new char[PIXELS * 4];
	for (int i = 0; i < PIXELS * 4; i++) {
		//data[i] = 255;
		data[i] = rand()%255;
	}*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8UI);
	
	glGenQueries(2, queries);

	std::cout << "\nBlack image " << TEX_SIZE << "x" << TEX_SIZE << "\n";
	glUseProgram(programWrite);
	glUniform1i(glGetUniformLocation(programWrite, "noise"), false);
	compute();

	std::cout << "Noise image " << TEX_SIZE << "x" << TEX_SIZE << "\n";
	glUseProgram(programWrite); 
	glUniform1i(glGetUniformLocation(programWrite, "noise"), true);	
	compute();


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void TextureCompression::compute() {
	for (int i = 0; i < 20; i++) {
		glUseProgram(programWrite);
		glBeginQuery(GL_TIME_ELAPSED, queries[0]);
		glDispatchCompute(TEX_SIZE / PIXEL_PER_THREAD, TEX_SIZE / WORKGROUP_SIZE, 1);
		glEndQuery(GL_TIME_ELAPSED);
		
		glUseProgram(programRead);
		glBeginQuery(GL_TIME_ELAPSED, queries[1]);
		glDispatchCompute(TEX_SIZE / PIXEL_PER_THREAD, TEX_SIZE / WORKGROUP_SIZE, 1);
		glEndQuery(GL_TIME_ELAPSED);

		unsigned int time;
		glGetQueryObjectuiv(queries[0], GL_QUERY_RESULT, &time);

		unsigned int time2;
		glGetQueryObjectuiv(queries[1], GL_QUERY_RESULT, &time2);

		float bandwidth = PIXELS * 4 / (time*1e-9);
		std::cout << "bandwidth write " << bandwidth*1e-9 << "\n";
		bandwidth = PIXELS * 4 / (time2*1e-9);
		std::cout << "bandwidth read " << bandwidth*1e-9 << "\n";

	}
}

void TextureCompression::draw() {
	glViewport(0, 0, width, height);

	glClearColor(0.68, 0.88, 0.93, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, tex);

	glDrawArrays(GL_POINTS, 0, 1);
}



int main(int /*argc*/, char ** /*argv*/) {
	TextureCompression app;
	return app.run();
}