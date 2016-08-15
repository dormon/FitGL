#pragma once

#include "SimpleApp.h"

class DU01: public SimpleApp{
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
	
	/* Opengl objekty */

	/* Buffery */
	GLuint vao; // vertex array
	GLuint vbo; // vertex buffer
	GLuint ebo; // elementa buffer
	GLuint dibo; // draw indirect buffer

	/* Shadery */
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;

	/* Atributy */
	GLuint positionAttrib;

};