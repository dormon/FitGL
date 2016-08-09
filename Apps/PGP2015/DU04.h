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

	virtual void onKeyPress(SDL_Keycode key, Uint16 mod);

	void updateMatrix();
	void updateScene(Uint32 time);
	
	/* Pocet kostek */
	static const int sphereCount = 17;

	/* animace */
	bool animationEnabled = true;
	Uint32 time=0;
	Uint32 lastFrameTics=0;


	/* Matice */
	glm::mat4 ivp;

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