#pragma once

#include "SimpleApp.h"
#include "glm/glm.hpp"

class DU03: public SimpleApp{
public:
	DU03();
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

	/* Mys */
	bool animationEnabled=true;
	Uint32 time=0;
	Uint32 lastFrameTics=0;

	bool wireframe=false;
	bool debug=false;

	/* Opengl objekty */

	/* Buffery */
	GLuint vaoTri;
	GLuint vboTri;
	GLuint vaoBunny;
	GLuint vboBunny;
	GLuint eboBunny;

	/* Shadery */
	GLuint vs, tes,tcs, fs, program;

	/* Atributy */
	GLuint positionAttrib;
	GLuint normalAttrib;

	/* Uniformy */
	GLuint mUniform;
	GLuint vUniform; 
	GLuint pUniform;	
	GLuint timeUniform;
	GLuint animationEnabledUniform;
};