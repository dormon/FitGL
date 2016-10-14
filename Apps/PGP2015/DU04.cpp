#include "DU04.h"

#include <algorithm>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using namespace std;

DU04::DU04() {
	camera.rotx = 20, camera.roty = 45;
}

void DU04::init() {
	/*
	* Vytvoreni shader programu
	*/

	string shaderPrefix = RESOURCE_DIR"Shaders/PGP2015/";
	string texturePrefix = RESOURCE_DIR"Textures/PGP2015/";

	/*
	* Kompilace shaderu
	*/
	vs = compileShader(GL_VERTEX_SHADER, loadFile(shaderPrefix + "du04.vert"));
	gs = compileShader(GL_GEOMETRY_SHADER, loadFile(shaderPrefix + "du04.geo"));
	fs = compileShader(GL_FRAGMENT_SHADER, loadFile(shaderPrefix + "du04.frag"));
	program = linkShader(3, vs, gs, fs);


	// uniformy
	ivpUniform = glGetUniformLocation(program, "ivp");
	animationEnabledUniform = glGetUniformLocation(program, "animationEnabled");
	timeUniform = glGetUniformLocation(program, "time");
	cameraPositionUniform = glGetUniformLocation(program, "cameraPosition");
	sphereCountUniform = glGetUniformLocation(program, "sphereCount");


	/*
	*	Kopirovani dat na GPU
	*/
	
	/* Alokace bufferu pro koule */
	glGenBuffers(1, &sphereBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sphereBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Sphere)*sphereCount, NULL, GL_DYNAMIC_DRAW);
	
	glGenTextures(1, &texSkybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texSkybox);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	texImageFileBMP(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, texturePrefix + "up.bmp");
	texImageFileBMP(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, texturePrefix + "down.bmp");
	texImageFileBMP(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, texturePrefix + "left.bmp");
	texImageFileBMP(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, texturePrefix + "right.bmp");
	texImageFileBMP(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, texturePrefix + "front.bmp");
	texImageFileBMP(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, texturePrefix + "back.bmp");

	/* vygenerovani jmena pro VAO a jeho aktivace */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

/* Funkce pro prepocet HSV->RGB */
void color_Hue(float*Color, float Angle) {
#define ASTEP 60.0
	float ActAngle;//akualni uhel pro slozku barvy
	for (int c = 2; c >= 0; --c) {//projdeme barvy BGR poradi
		ActAngle = fmodf(Angle + (2 - c)*ASTEP * 2, 360) / ASTEP;
		if (ActAngle >= 5)Color[c] = 6 - ActAngle;
		else if (ActAngle >= 3)Color[c] = 1;
		else if (ActAngle >= 2)Color[c] = ActAngle - 2;
		else Color[c] = 0;

	}
#undef ASTEP
}
const glm::vec3 toGray(.299, .587, .114);
glm::vec3 color_HSV(float Angle, float Saturaion, float Value) {
	glm::vec3 color;
	color_Hue(&color[0], Angle);
	color *= Value;
	float Gray = glm::dot(color, toGray);
	color = glm::mix(glm::vec3(Gray), color, Saturaion);
	return color;
}

void DU04::updateScene(Uint32 time){
	/* Ziskani pointeru */
	Sphere* ptr = (Sphere*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	ptr[0].center = glm::vec3(0, 0, 0);
	ptr[0].radius = 5;
	ptr[0].diffuse = glm::vec3(0, 0, 0);
	ptr[0].reflectivity = 1;
	
	for (int i = 1; i < sphereCount; i++) {
		float x = 7 * sin(0.5*time/1000.0+ i/(sphereCount-1.0)*M_PI*2);
		float y = 7 * cos(0.5*time/1000.0+ i / (sphereCount - 1.0)*M_PI*2);

		ptr[i].center = glm::vec3(x, 0, y);
		ptr[i].radius = 1;

		ptr[i].diffuse = color_HSV(i / 17.*360., 1, 1);
		ptr[i].reflectivity = 0.8;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}




void DU04::draw() {
	Uint32 tics = SDL_GetTicks();
	Uint32 dt = tics - lastFrameTics;
	lastFrameTics = tics;
	if (animationEnabled) {
		time += dt;
	}

	/* Aktualizace pozic kouli */
	updateScene(time);

	/* Matice */
	updateMatrix();

	glViewport(0, 0, width, height);
		
	glUseProgram(program);

	glUniformMatrix4fv(ivpUniform, 1, GL_FALSE, glm::value_ptr(ivp));

	glUniform1i(timeUniform, time);
	glUniform1i(animationEnabledUniform, animationEnabled);
	glUniform1i(sphereCountUniform, sphereCount);
	glUniform3fv(cameraPositionUniform, 1,glm::value_ptr(camera.eye));

	glDrawArrays(GL_POINTS, 0, 1);
}

void DU04::onKeyPress(SDL_Keycode key, Uint16 mod) {
	switch (key) {
	case SDLK_ESCAPE:
		quit();
	case SDLK_a:
		animationEnabled = !animationEnabled;
		if (animationEnabled) time = 0;
		break;
	}
}

void DU04::updateMatrix() {
	camera.update();
	camera.view = glm::lookAt(glm::vec3(0),-camera.eye, camera.up);
	ivp = glm::inverse(camera.projection*camera.view);
}


int main(int /*argc*/, char ** /*argv*/) {
	DU04 app;
	return app.run();
}