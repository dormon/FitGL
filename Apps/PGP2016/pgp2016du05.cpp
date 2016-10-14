#include "pgp2016.h"

void PGP2016::du05_init(){
	/*****  DOPLNIT *****/
	// zmenit na true
	useComputeShader = false;
}

void PGP2016::du05_update(){
	float w = TABLE_WIDTH / 2 - TABLE_GREEN_SHIFT - BALL_RADIUS;
	float h = TABLE_DEPTH / 2 - TABLE_GREEN_SHIFT - BALL_RADIUS;

	programCompute.use();
	programCompute.set1f("width", w);
	programCompute.set1f("height", h);
	programCompute.set1i("ballCount", BALL_COUNT);
	programCompute.set1f("ballRadius", BALL_RADIUS);
	programCompute.set1f("dt", dt);

	// double buffering
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ballBuffer[activeBuffer]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ballBuffer[1 - activeBuffer]);
	
	// spusteni compute shaderu
	int groups = int(ceilf(float(BALL_COUNT) / 64));
	glDispatchCompute(groups, 1, 1);

	// prohozeni bufferu
	activeBuffer = 1 - activeBuffer;
}