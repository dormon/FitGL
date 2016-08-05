#pragma once

#include "SimpleApp.h"
#include "glm/glm.hpp"

class APP_NAME: public SimpleApp{
public:
	APP_NAME();
protected:
	virtual void init();
	virtual void draw();      
	virtual void onMouseMove(int dx, int dy, int x, int y);
	virtual void onMousePress(Uint8 button, int x, int y);
	virtual void onMouseRelease(Uint8 button, int x, int y);
	virtual void onMouseWheel(int delta);
	virtual void onKeyPress(SDL_Keycode key, Uint16 mod);

	void updateMatrix();     

	/* Mys */
	float zoom;
	bool rmbPressed;
	bool lmbPressed;
	int posx, posy;
	int rotx, roty;

	bool animationEnabled;
	Uint32 time;
	Uint32 lastFrameTics;

	bool wireframe;
	bool debug;
	

	/* Matice */
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;


	/* Opengl objekty */

	/* Buffery */
	GLuint vbo;
	GLuint ebo;
	GLuint vao;

	/* Shadery */
	GLuint program;

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