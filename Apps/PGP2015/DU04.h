#pragma once

#include "SimpleApp.h"
#include "glm/glm.hpp"

class DU04 : public SimpleApp {
public:
	DU04();

	struct Sphere {
		glm::vec3 center;
		float radius;
		glm::vec3 diffuse;
		float reflectivity;
	};

protected:

	/**
	* Init funkce, vola se jednou pri startu aplikace
	*/
	virtual void init();

	/**
	* Draw funkc, vola se v kazdem snimku
	*/
	virtual void draw();

	virtual void onMouseMove(int dx, int dy, int x, int y);
	virtual void onMousePress(Uint8 button, int x, int y);
	virtual void onMouseRelease(Uint8 button, int x, int y);
	virtual void onMouseWheel(int delta);
	virtual void onKeyPress(SDL_Keycode key, Uint16 mod);

	void updateMatrix();
	void updateScene(Uint32 time);
	
	/* Pocet kostek */
	static const int sphereCount = 17;

	/* Mys */
	float zoom;
	bool rmbPressed;
	bool lmbPressed;
	int posx, posy;
	int rotx, roty;

	bool animationEnabled;
	Uint32 time;
	Uint32 lastFrameTics;


	/* Matice */
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 ivp;
	glm::vec3 cameraPosition;


	/* Opengl objekty */

	/* Textura*/
	GLuint texSkybox;

	/* Buffery */
	GLuint vao;
	GLuint sphereBuffer;

	/* Shadery */
	GLuint vs, gs, fs, program;

	/* Atributy */
	GLuint positionAttrib;
	GLuint normalAttrib;

	/* Uniformy */
	GLuint ivpUniform;
	GLuint cameraPositionUniform;
	GLuint timeUniform;
	GLuint animationEnabledUniform;
	GLuint sphereCountUniform;
};