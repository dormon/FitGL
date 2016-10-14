#include "DU05.h"

#include <algorithm>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

#define PI 3.1415926535897932384626433832795

DU05::DU05() {
	//kamera
	camera.rotx = 170;
	camera.roty = 25;
	camera.zoom = 80;
	camera.center = glm::vec3(10, 40, 10);
	multisample = 4;

	//svelto
	lightPosition = glm::vec3(-300, 300,150);

	// velikost shadowmapy
	smSize = 4096;
}

void DU05::init() {
	/*
	* Vytvoreni shader programu
	*/

	string shaderPrefix = RESOURCE_DIR"Shaders/PGP2015/";
	texturePrefix = RESOURCE_DIR"Textures/PGP2015/";
	/*
	* Kompilace shaderu
	*/
	GLuint vert = compileShader(GL_VERTEX_SHADER, loadFile(shaderPrefix + "du05.vert"));
	GLuint frag = compileShader(GL_FRAGMENT_SHADER, loadFile(shaderPrefix + "du05.frag"));
	drawProgram = linkShader(2, vert, frag);

	vert = compileShader(GL_VERTEX_SHADER, loadFile(shaderPrefix + "du05_sm.vert"));
	frag = compileShader(GL_FRAGMENT_SHADER, loadFile(shaderPrefix + "du05_sm.frag"));
	smProgram = linkShader(2, vert, frag);

	GLuint comp = compileShader(GL_COMPUTE_SHADER, loadFile(shaderPrefix + "du05.comp"));
	computeProgram = linkShader(1, comp);
	/*
	* Kopirovani dat na GPU
	*/
	createGround();
	createPole();
	createFlag();
	createSM();
}


void DU05::createGround(){
	glGenVertexArrays(1, &vaoGround);
	glBindVertexArray(vaoGround);
	glGenBuffers(1, &vboGround);
	glBindBuffer(GL_ARRAY_BUFFER, vboGround);

	float s = 500;
	float t = 10;
	Vertex data[] = {
		{glm::vec3(-s,0,-s),glm::vec3(0,1,0),glm::vec2(0,0)},
		{glm::vec3(s,0,-s),glm::vec3(0,1,0),glm::vec2(t,0)},
		{glm::vec3(-s,0,s),glm::vec3(0,1,0),glm::vec2(0,t)},
		{glm::vec3(s,0,s),glm::vec3(0,1,0),glm::vec2(t,t)}
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tc));

	
	texGround = textureFromBMP(texturePrefix+"du05a.bmp");
	glBindVertexArray(0);
}

void sphere(glm::vec3 center,float radius, int hdiv, int vdiv, int &vertexCount, Vertex *&vertexData, int &elementCount, unsigned int*&elementData){
	vertexCount = (hdiv + 1) * (vdiv + 1);
	Vertex* data = new Vertex[vertexCount];
	int vertCount = 0;
	for (int h = 0; h < hdiv + 1; h++) {
		for (int v = 0; v < vdiv + 1; v++) {
			float x, y, z;
			float radH = float(h) / hdiv * PI * 2;
			float radV = float(v) / vdiv *PI;

			x = sin(radV) * cos(radH);
			y = -cos(radV);
			z = -sin(radV) * sin(radH);
			data[vertCount].position = radius*glm::vec3(x, y, z) + center;
			data[vertCount].normal = glm::vec3(x, y, z);
			data[vertCount].tc = glm::vec2(float(h)/hdiv,float(v)/vdiv);
			vertCount++;
		}
	}
	vertexData = data;

	elementCount = hdiv * ((vdiv - 1) * 6);
	unsigned int* elements = new unsigned int [elementCount];
	int eptr = 0;
	for (int h = 0; h < hdiv; h++) {
		elements[eptr++] = h * (vdiv + 1);
		elements[eptr++] = (h + 1) % (vdiv + 1) * (vdiv + 1) + 1;
		elements[eptr++] = h * (vdiv + 1) + 1;
		//side
		for (int v = 0; v < vdiv - 2; v++) {
			elements[eptr++] = h * (vdiv + 1) + 1 + v;
			elements[eptr++] = (h + 1) % (hdiv + 1) * (vdiv + 1) + 1 + v + 1;
			elements[eptr++] = h * (vdiv + 1) + 1 + v + 1;

			elements[eptr++] = h * (vdiv + 1) + 1 + v;
			elements[eptr++] = (h + 1) % (hdiv + 1) * (vdiv + 1) + 1 + v;
			elements[eptr++] = (h + 1) % (hdiv + 1) * (vdiv + 1) + 1 + v + 1;

		}
		//bottom
		elements[eptr++] = h * (vdiv + 1) + vdiv - 1;
		elements[eptr++] = (h + 1) % (hdiv + 1) * (vdiv + 1) + vdiv - 1;
		elements[eptr++] = h * (vdiv + 1) + vdiv;
	}
	elementData = elements;
}
void tube(glm::vec3 center, float radius, int height, int hdiv, int vdiv, int &vertexCount, Vertex *&vertexData, int &elementCount, unsigned int*&elementData) {
	vertexCount = (hdiv + 1) * (vdiv + 1);
	Vertex* data = new Vertex[vertexCount];
	int vertCount = 0;
	for (int v = 0; v < vdiv + 1; v++) {
		for (int h = 0; h < hdiv + 1; h++) {
			float x, y, z;
			float radH = float(h) / hdiv * PI * 2;			
			x = radius*sin(radH);
			y = (float(v) / vdiv)*height;
			z = radius*cos(radH);
			data[vertCount].position = glm::vec3(x, y, z) + center;
			data[vertCount].normal = glm::vec3(x, 0, z);
			data[vertCount].tc = glm::vec2(float(h) / hdiv, float(v) / vdiv);
			vertCount++;
		}
	}
	vertexData = data;

	elementCount = hdiv * (vdiv) * 6;
	unsigned int* elements = new unsigned int[elementCount];
	int eptr = 0;
	for (int v = 0; v < vdiv; v++) {
		for (int h = 0; h < hdiv; h++) {
			elements[eptr++] = v*(hdiv + 1) + h;
			elements[eptr++] = v*(hdiv + 1) + h+1;
			elements[eptr++] = (v + 1)*(hdiv + 1) + h+1;

			elements[eptr++] = v*(hdiv + 1) + h;
			elements[eptr++] = (v + 1)*(hdiv + 1) + h + 1;
			elements[eptr++] = (v + 1)*(hdiv + 1) + h;
		}
	}
	elementData = elements;
}
void DU05::createPole(){
	unsigned int* elementData;
	Vertex* vertexData;
	int vertexCount;
	int elementCount;
	sphere(glm::vec3(0,50,0),1,30,30,vertexCount,vertexData,elementCount,elementData);	

	unsigned int* elementData1;
	Vertex* vertexData1;
	int vertexCount1;
	int elementCount1;
	tube(glm::vec3(0, 0, 0), 0.5, 50, 10, 10, vertexCount1, vertexData1, elementCount1, elementData1);
	for (int i = 0; i < elementCount1;i++)elementData1[i]+=vertexCount;

	glGenVertexArrays(1, &vaoPole);
	glBindVertexArray(vaoPole);

	glGenBuffers(1, &vboPole);
	glBindBuffer(GL_ARRAY_BUFFER, vboPole);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*(vertexCount+vertexCount1), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*vertexCount, vertexData);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexCount, sizeof(Vertex)*vertexCount1, vertexData1);

	glGenBuffers(1, &eboPole);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboPole);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*(elementCount + elementCount1), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(int)*elementCount, elementData);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*elementCount, sizeof(int)*elementCount1, elementData1);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tc));

	countPole = elementCount+elementCount1;
	glBindVertexArray(0);
}

void DU05::createFlag(){
	const glm::vec2 connections[] = {
		glm::vec2(-1,0),
		glm::vec2(0,-1),
		glm::vec2(1,0),
		glm::vec2(0,1),
		glm::vec2(-1,-1),
		glm::vec2(-1,1),
		glm::vec2(1,-1),
		glm::vec2(1,1),
		glm::vec2(2,0),
		glm::vec2(-2,0),
		glm::vec2(0,2),
		glm::vec2(0,-2),
		glm::vec2(-2,-2),
		glm::vec2(-2,2),
		glm::vec2(2,-2),
		glm::vec2(2,2),
	};

	int vdiv = 50;
	int hdiv = 100;
	int flagWidth = 30;
	int flagHeight = 15;
	glm::vec3 center(0.5,49-flagHeight,0);

	int vertexCount = (hdiv + 1) * (vdiv + 1);
	SimVertex* data = new SimVertex[vertexCount];
	int vertCount = 0;
	for (int v = 0; v < vdiv + 1; v++) {
		for (int h = 0; h < hdiv + 1; h++) {
			float x, y, z;
			float radH = float(h) / hdiv * PI * 2;
			x = (float(h) / hdiv)*flagWidth;
			y = (float(v) / vdiv)*flagHeight;
			z = 0;
			data[vertCount].position = glm::vec3(x, y, z) + center;
			data[vertCount].normal = glm::vec3(0, 0, 1);
			data[vertCount].tc = glm::vec2(float(h) / hdiv, 1-float(v) / vdiv);
			data[vertCount].velocity = glm::vec3(0, 0, 0);
			vertCount++;
		}
	}
	for (int v = 0; v < vdiv + 1; v++) {
		for (int h = 0; h < hdiv + 1; h++) {
			for (int i = 0; i < 16; i++) {
				
				int hp = h + connections[i].x;
				int vp = v + connections[i].y;
				int id = v*(hdiv + 1) + h;
				int idc = vp*(hdiv + 1) + hp;

				if (hp>=0 && vp >=0 && hp<=hdiv && vp<=vdiv) {
					float l = glm::length(data[id].position - data[idc].position);
					data[id].connections[i] = idc;
					data[id].lengths[i] = l;
				}else {
					data[id].connections[i] = -1;
				}
			}
		}
	}


	flagVertexCount = vertexCount;

	int elementCount = hdiv * (vdiv)* 6;
	unsigned int* elements = new unsigned int[elementCount];
	int eptr = 0;
	for (int v = 0; v < vdiv; v++) {
		for (int h = 0; h < hdiv; h++) {
			elements[eptr++] = v*(hdiv + 1) + h;
			elements[eptr++] = v*(hdiv + 1) + h + 1;
			elements[eptr++] = (v + 1)*(hdiv + 1) + h + 1;

			elements[eptr++] = v*(hdiv + 1) + h;
			elements[eptr++] = (v + 1)*(hdiv + 1) + h + 1;
			elements[eptr++] = (v + 1)*(hdiv + 1) + h;
		}
	}
	countFlag = elementCount;
	
	glGenBuffers(1, &eboFlag);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboFlag);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*elementCount, elements, GL_STATIC_DRAW);
	
	glGenVertexArrays(2, vaoFlag);
	glGenBuffers(2, vboFlag);
	for (int i = 0; i < 2; i++) {
		glBindVertexArray(vaoFlag[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vboFlag[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SimVertex)*vertexCount, data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboFlag);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(SimVertex), (GLvoid*)offsetof(SimVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(SimVertex), (GLvoid*)offsetof(SimVertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(SimVertex), (GLvoid*)offsetof(SimVertex, tc));
	}

	texFlag = textureFromBMP(texturePrefix+"du05b.bmp");
	glBindVertexArray(0);
}

void DU05::createSM(){
	glGenTextures(1, &texSM);
	glBindTexture(GL_TEXTURE_2D, texSM);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, smSize, smSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texSM, 0);
	glDrawBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glm::mat4 proj = glm::perspective(glm::radians(20.f), 1.f, 300.f, 1000.f);
	glm::mat4 view = glm::lookAt(lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 bias = glm::mat4(0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0); 

	shadowMat = proj*view;
	shadowMatBias = bias*proj*view;
	 
}

void DU05::updateScene(int dt){
	/* Na pomalem stroji bude dt moc velke a simulace se rozkmita */
	dt = min(50,dt);

	/* Pocet kroku simulace*/
	int steps = 100;
	glUseProgram(computeProgram); 
	glUniform1f(glGetUniformLocation(computeProgram, "dt"), dt*0.001 / steps);
	glUniform1f(glGetUniformLocation(computeProgram, "time"), time*0.001);
	glUniform1i(glGetUniformLocation(computeProgram, "vertexCount"), flagVertexCount);
	glUniform1i(glGetUniformLocation(computeProgram, "gravityEnabled"), gravityEnabled);
	glUniform1i(glGetUniformLocation(computeProgram, "windEnabled"), windEnabled);
	for (int i = 0; i < steps; i++) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vboFlag[activeBuffer]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, vboFlag[1 - activeBuffer]);
		glDispatchCompute(ceil(flagVertexCount / 64.0), 1, 1);
		activeBuffer = 1 - activeBuffer;
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
}

void DU05::drawScene(GLuint program){
	glUniform3f(glGetUniformLocation(program, "Ld"), 0.8, 0.8, 0.8);
	glUniform3f(glGetUniformLocation(program, "La"), 0.2, 0.2, 0.2);
	glUniform3f(glGetUniformLocation(program, "Kd"), 0, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texGround);
	glBindVertexArray(vaoGround);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(vaoPole);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform3f(glGetUniformLocation(program, "Kd"), 1, 1, 1);
	glDrawElements(GL_TRIANGLES, countPole, GL_UNSIGNED_INT, 0);


	glUniform3f(glGetUniformLocation(program, "Ld"), 0.4, 0.4, 0.4);
	glUniform3f(glGetUniformLocation(program, "La"), 0.6, 0.6, 0.6);
	glBindVertexArray(vaoFlag[activeBuffer]);
	glBindTexture(GL_TEXTURE_2D, texFlag);
	glUniform3f(glGetUniformLocation(program, "Kd"), 0, 0, 0);
	glDrawElements(GL_TRIANGLES, countFlag, GL_UNSIGNED_INT, 0);
}

void DU05::draw() {
	/* Aktualizace casu animace - klavesa A*/
	Uint32 tics = SDL_GetTicks();
	Uint32 dt = tics - lastFrameTics; //ms
	lastFrameTics = tics;
	if (animationEnabled) {
		time += dt;
	}

	/* Matice */
	camera.update();

	// simulace
	updateScene(dt); 
	
	// vykreleni shadow mapy
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
	glViewport(0, 0, smSize, smSize);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(smProgram);
	glUniformMatrix4fv(glGetUniformLocation(smProgram, "vp"), 1, GL_FALSE, glm::value_ptr(shadowMat));

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.5, 10);
	drawScene(smProgram);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);

	// vykresleni sceny
	glClearColor(0.68, 0.88, 0.93, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(drawProgram);
	glUniformMatrix4fv(glGetUniformLocation(drawProgram, "v"), 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glGetUniformLocation(drawProgram, "p"), 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUniformMatrix4fv(glGetUniformLocation(drawProgram, "smb"), 1, GL_FALSE, glm::value_ptr(shadowMatBias));
	glUniform3fv(glGetUniformLocation(drawProgram, "lightPosition"), 1, glm::value_ptr(lightPosition));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texSM);
	glActiveTexture(GL_TEXTURE0);

	/* Prepinani na dratovy model - klavesa W*/
	if (wireframe) {
		glLineWidth(2.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	drawScene(drawProgram);
}

void DU05::onKeyPress(SDL_Keycode key, Uint16 mod) {
	switch (key) {
	case SDLK_ESCAPE:
		quit();
	case SDLK_a:
		animationEnabled = !animationEnabled;
		if (animationEnabled) time = 0;
		break;
	case SDLK_d:
		debug = !debug;
		break;
	case SDLK_w:
		wireframe = !wireframe;
		break;
	case SDLK_g:
		gravityEnabled = !gravityEnabled;
		break;
	case SDLK_v:
		windEnabled = !windEnabled;
		break;
	}
}

int main(int /*argc*/, char ** /*argv*/) {
	DU05 app;
	return app.run();
}