#include "DU03.h"

#include <algorithm>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "bunny.h"

using namespace std;

DU03::DU03(){
	rmbPressed = false;
	lmbPressed = false;
	posx = 0, posy = 0;
	rotx = 170, roty = 25, zoom = 4;
	//rotx = 180, roty = 88, zoom = 1.22;
	
	animationEnabled = false;
	time = 0;
	lastFrameTics = 0;
	wireframe = false;
	debug = false;
	//fullscreen = true;	
}

void DU03::init() {
	/*
	* Vytvoreni shader programu
	*/

	string prefix = "../../Resources/Shaders/PGP2015/";

	/*
	* Kompilace shaderu
	*/
	vs = compileShader(GL_VERTEX_SHADER, loadFile(prefix + "du03.vert"));
	tcs = compileShader(GL_TESS_CONTROL_SHADER, loadFile(prefix + "du03.tesc"));
	tes = compileShader(GL_TESS_EVALUATION_SHADER, loadFile(prefix + "du03.tese"));
	fs = compileShader(GL_FRAGMENT_SHADER, loadFile(prefix + "du03.frag"));
	program = linkShader(4, vs, tcs, tes, fs);


	// zisakni indexu atributu
	positionAttrib = glGetAttribLocation(program, "position");
	normalAttrib = glGetAttribLocation(program, "normal");

	mUniform = glGetUniformLocation(program, "m");
	vUniform = glGetUniformLocation(program, "v");
	pUniform = glGetUniformLocation(program, "p");
	animationEnabledUniform = glGetUniformLocation(program, "animationEnabled");
	timeUniform = glGetUniformLocation(program, "time");
	

	/*
	* Kopirovani dat na GPU
	*/

	/*
	* Vytvoreni bufferu pro testovaci trojuhelnik
	*/
	glGenVertexArrays(1, &vaoTri);
	glBindVertexArray(vaoTri);
	float tri[] = { -0.5,0,-0.5, 0,1,0,0.5,0,-0.5, 0,1,0,-0.5,0,0.5, 0,1,0 };
	glGenBuffers(1, &vboTri);
	glBindBuffer(GL_ARRAY_BUFFER, vboTri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (GLvoid*)(0));
	glEnableVertexAttribArray(normalAttrib);
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));

	/*
	* Vytvoreni bufferu pro kralika
	*/
	glGenVertexArrays(1, &vaoBunny);
	glBindVertexArray(vaoBunny);
	glGenBuffers(1, &vboBunny);
	glBindBuffer(GL_ARRAY_BUFFER, vboBunny);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bunnyVertices), bunnyVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &eboBunny);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboBunny);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bunny), bunny, GL_STATIC_DRAW);

	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(struct BunnyVertex), (GLvoid*)(offsetof(struct BunnyVertex, position)));
	glEnableVertexAttribArray(normalAttrib);
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(struct BunnyVertex), (GLvoid*)(offsetof(struct BunnyVertex, normal)));

}


void DU03::draw() {
	/* Aktualizace casu animace - klavesa A*/
	Uint32 tics = SDL_GetTicks();
	Uint32 dt = tics - lastFrameTics;
	lastFrameTics = tics;
	if (animationEnabled) {
		time += dt;
	}

	/* Matice */
	updateMatrix();

	glViewport(0, 0, width, height);

	glClearColor(0.68, 0.88, 0.93, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	glUseProgram(program);

	glUniformMatrix4fv(mUniform, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(vUniform, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, glm::value_ptr(projection));

	glUniform1i(timeUniform, time);
	glUniform1i(animationEnabledUniform, animationEnabled);

	/* Nastaveni poctu kontrolnich bodu */
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	/* Prepinani na dratovy model - klavesa W*/
	if (wireframe) {
		glLineWidth(3.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	/* Prepinani mezi kreslenim kralika a jednoho trojuhelniku - klavesa D*/
	if (!debug) {
		glBindVertexArray(vaoBunny);
		glDrawElements(GL_PATCHES, sizeof(bunny) / sizeof(**bunny), GL_UNSIGNED_SHORT, 0);
	}else {
		glBindVertexArray(vaoTri);
		glDrawArrays(GL_PATCHES, 0, 3);
	}
}

void DU03::onMouseMove(int dx, int dy, int x, int y){
	if (rmbPressed||lmbPressed) {
		rotx += dx;
		roty += dy;
		roty = max(min(roty, 89), -89);
	}
	posx = x;
	posy = y;
}

void DU03::onMousePress(Uint8 button, int x, int y){
	switch (button) {
	case SDL_BUTTON_LEFT:
		lmbPressed = true;
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	case SDL_BUTTON_RIGHT:
		rmbPressed = true;
		break;
	}
}

void DU03::onMouseRelease(Uint8 button, int x, int y){
	switch (button) {
	case SDL_BUTTON_LEFT:
		lmbPressed = false;
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	case SDL_BUTTON_RIGHT:
		rmbPressed = false;
		break;
	}
}

void DU03::onMouseWheel(int delta){
	if (delta > 0) {
		zoom /= 1.1;
	}else {
		zoom *= 1.1;
	}
}

void DU03::onKeyPress(SDL_Keycode key, Uint16 mod){
	switch (key) {
	case SDLK_ESCAPE:
		quit();
	case SDLK_a:
		animationEnabled = !animationEnabled;
		if (animationEnabled) time = 0;
		break;
	case SDLK_d:
		debug = !debug;
		break;
	case SDLK_w:
		wireframe = !wireframe;
		break;
	}
}

void DU03::updateMatrix() {
	model = glm::mat4(1.0);

	float radx = glm::radians((float)rotx);
	float rady = glm::radians((float)roty);
	float x = zoom * cos(rady) * cos(radx);
	float y = zoom * sin(rady);
	float z = zoom * cos(rady) * sin(radx);

	glm::vec3 eye(x, y, z);
	glm::vec3 center(0, 0, 0);
	glm::vec3 up(0, 1, 0);
	view = glm::lookAt(eye, center, up);
	projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
}

int main(int /*argc*/, char ** /*argv*/){
	DU03 app;
	return app.run();
}