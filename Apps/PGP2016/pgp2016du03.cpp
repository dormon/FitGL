#include "pgp2016.h"
using namespace glm;

void PGP2016::du03_initFramebuffer() {
	/***** DU03 - DOPLNIT *****/
	// zmenit na true
	usePicking = false;
	if (!usePicking)return;

	int w = mainWindow->getWidth();
	int h = mainWindow->getHeight();
	
	/***** DU03 - DOPLNIT *****/
	/*
	 - vytvoreni textury pro zapis id - 1 kanal, int32
	 - vytvoreni renderbufferu pro hloubku
	 - vytvoreni framebufferu
	 - pripojeni textury a renderbuffer do framebufferu
	 - kontrola statusu framebufferu
	*/
}

void PGP2016::du03_resizeFramebuffer(int w, int h) {
	if (!usePicking)return ;
	/***** DU03 - DOPLNIT *****/
	/*
	- zmena velikosti renderbufferu a textury
	*/
}

int PGP2016::du03_pick() {
	if (!usePicking)return -1;
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	programID->use();
	programID->setMatrix4fv("v", value_ptr(camera->getView()));
	programID->setMatrix4fv("p", value_ptr(camera->getProjection()));

	glBindVertexArray(vertexArray);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
	glMultiDrawArraysIndirect(GL_TRIANGLES, 0, 1, 0);

	int pick = -1;
	if (mouseX >= 0 && mouseX < width &&
		mouseY >= 0 && mouseY < height) {
		glReadPixels(mouseX, height - mouseY - 1,1,1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pick);
		//glGetTextureSubImage(textureID, 0, mouseX, height - mouseY - 1, 0, 1, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, 4, &pick);
	}
	if (pick >= BALL_COUNT) pick = -1;
	return pick;
}

void PGP2016::du03_onClick() {
	if (pick >= 0) {
		vec3 center = camera->getCenter();
		vec3 eye = camera->getEye();
		vec3 up = camera->getUp();
		vec3 dir = eye - center;
		vec3 a = cross(dir, up);
		vec3 f = normalize(cross(a, up));
		vec2 ballDir = vec2(f.x, f.z);
		vec2 newSpeed = ballDir*30.0f;
		//vec2 newSpeed = vec2(0,0);
		ballData[pick].speed = newSpeed;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ballBuffer[activeBuffer]);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(Ball)*pick + offsetof(Ball, speed), sizeof(vec2), &(ballData[pick].speed));
		//glNamedBufferSubData(ballBuffer[activeBuffer], sizeof(Ball)*pick + offsetof(Ball, speed), sizeof(vec2), &(ballData[pick].speed));
	}
}

void PGP2016::du03_update(float dt) {
	// limity pro pozici kouli
	float w = TABLE_WIDTH / 2 - TABLE_GREEN_SHIFT - BALL_RADIUS;
	float h = TABLE_DEPTH / 2 - TABLE_GREEN_SHIFT - BALL_RADIUS;

	for (int i = 0; i < BALL_COUNT; i++) {

		// treni
		ballData[i].speed *= 0.99f;
		// delta posun
		vec2 dpos = dt*ballData[i].speed;
		ballData[i].pos += dpos;

		// rotace
		mat4 rot(1);
		if (length(dpos)>0)
			rot = rotate(length(dpos) / BALL_RADIUS, cross(vec3(0, 1, 0), vec3(dpos.x, 0, dpos.y)));
		ballData[i].m[3] = vec4(0, 0, 0, 1);
		ballData[i].m = rot*ballData[i].m;
		// posun
		ballData[i].m[3] = vec4(ballData[i].pos.x, 0, ballData[i].pos.y, 1);

		// mantinely
		if (ballData[i].pos.x > w ||
			ballData[i].pos.x < -w) {
			ballData[i].speed.x *= -1;
		}
		if (ballData[i].pos.y > h ||
			ballData[i].pos.y < -h) {
			ballData[i].speed.y *= -1;
		}
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ballBuffer[activeBuffer]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Ball)*BALL_COUNT, ballData);
	//glNamedBufferSubData(ballBuffer[activeBuffer], 0, sizeof(Ball)*BALL_COUNT, ballData);
}

