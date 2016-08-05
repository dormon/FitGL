#pragma once

#include "SimpleApp.h"
#include "glm/glm.hpp"

class TextureCompression: public SimpleApp{
public:
	TextureCompression();
protected:
	virtual void init();
	virtual void draw();      

	void compute();

	GLuint queries[2];

	GLuint program;
	GLuint programRead;
	GLuint programWrite;
	GLuint tex;
	GLuint vao;
};