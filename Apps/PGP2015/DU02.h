#pragma once

#include "SimpleApp.h"
#include "glm/glm.hpp"

#include <vector>

class DU02: public SimpleApp{
public:
	DU02();
protected:
	/**
	* Init funkce, vola se jednou pri startu aplikace
	*/
	virtual void init();

	/**
	* Draw funkc, vola se v kazdem snimku
	*/
	virtual void draw();

	virtual void onMousePress(Uint8 button, int x, int y);
	virtual void onKeyPress(SDL_Keycode key, Uint16 mod);
	void insertCube(glm::vec3 pos);

	/* Kostky */
	static const int MAX_CUBES = 1024;
	static const size_t cubeSize = sizeof(float) * 3;
	int cubeCount=0;
	std::vector<glm::vec3> cubes;
	Uint32 faceId;
	Uint32 cubeId;
	bool selected;

	/* Opengl objekty */

	/* Textura kostky*/
	GLuint textureDirt;

	/* GBuffer */
	GLuint texturePosition;
	GLuint textureNormal;
	GLuint textureColor;
	GLuint textureId;
	GLuint renderBufferDepth;
	GLuint framebuffer;

	/* Buffery */
	GLuint vao; // vertex array
	GLuint vao1;
	GLuint vbo; // vertex buffer

	/* Shadery */
	GLuint vs, gs, fs, programCubes;
	GLuint vs1, gs1, fs1, programView;

	/* Atributy */
	GLuint positionAttrib;

	/* Uniformy */
	GLuint mUniform;
	GLuint vUniform; 
	GLuint pUniform;	
	GLuint selectedFaceUniform;
	GLuint selectedCubeUniform;
};